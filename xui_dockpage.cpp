#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_window.h"
#include "xui_drawer.h"
#include "xui_desktop.h"
#include "xui_dockview.h"
#include "xui_dockpage.h"

xui_implement_rtti(xui_dockpage, xui_control)

xui_dockpage* xui_dockpage::create( xui_bitmap* icon, const std::wstring& text, s32 size, u32 dockarea, s32 minlimit, u08 initdock )
{
	xui_dockpage* dockpage = new xui_dockpage(xui_vector<s32>(size), dockarea, minlimit, initdock);
	dockpage->ini_namectrl(icon, text);
	return dockpage;
}

xui_dockpage::xui_dockpage( const xui_vector<s32>& size, u32 dockarea, s32 minlimit, u08 initdock )
: xui_control(size)
{
	m_border	= xui_rect2d<s32>(2);
	m_inborder	= xui_rect2d<s32>(0);
	m_dockarea	= dockarea;
	m_initdock	= initdock;
	m_minlimit  = minlimit;
	m_autofree	= false;
	m_namectrl	= xui_drawer::create(L"");
	m_namectrl->set_parent(this);
	m_namectrl->xm_topdraw	 += new xui_method_member<xui_method_args,  xui_dockpage>(this, &xui_dockpage::on_namectrltopdraw  );
	m_namectrl->xm_mousedown += new xui_method_member<xui_method_mouse, xui_dockpage>(this, &xui_dockpage::on_namectrlmousedown);
	m_namectrl->xm_mouserise += new xui_method_member<xui_method_mouse, xui_dockpage>(this, &xui_dockpage::on_namectrlmouserise);
	m_widgetvec.push_back(m_namectrl);
}

bool xui_dockpage::was_autofree( void ) const
{
	return m_autofree;
}

void xui_dockpage::set_autofree( bool flag )
{
	m_autofree = flag;
}

const xui_rect2d<s32>& xui_dockpage::get_inborder( void ) const
{
	return m_inborder;
}

void xui_dockpage::set_inborder( const xui_rect2d<s32>& border )
{
	if (m_inborder != border)
	{
		m_inborder  = border;
		perform();
	}
}

bool xui_dockpage::has_dockarea( u08 dockstyle )
{
	switch (dockstyle)
	{
	case k_dockstyle_l: return (m_dockarea & k_arealimit_l) != 0;
	case k_dockstyle_r: return (m_dockarea & k_arealimit_r) != 0;
	case k_dockstyle_t: return (m_dockarea & k_arealimit_t) != 0;
	case k_dockstyle_b: return (m_dockarea & k_arealimit_b) != 0;
	}

	return true;
}

u08 xui_dockpage::get_initdock( void ) const
{
	return m_initdock;
}

s32 xui_dockpage::get_minlimit( void ) const
{
	return m_minlimit;
}

s32 xui_dockpage::get_namesize( void ) const
{
	s32 size = 0;
	size += m_namectrl->get_borderrt().ax;
	size += m_namectrl->get_borderrt().bx;
	size += m_namectrl->get_iconsize().w + m_namectrl->get_textoffset().x;
	size += xui_canvas::get_ins()->calc_size(m_namectrl->get_text(), m_namectrl->get_textfont(), 0, true).w;

	return size;
}

const std::wstring& xui_dockpage::get_pagename( void ) const
{
	return m_namectrl->get_text();
}

void xui_dockpage::ini_namectrl( xui_bitmap* icon, const std::wstring& text )
{
	m_namectrl->ini_drawer(icon);
	m_namectrl->ini_drawer(text);
	if (icon)
	{
		m_namectrl->set_iconalign(k_image_front_text);
		m_namectrl->set_textalign(k_textalign_lc);
		m_namectrl->set_textoffset(xui_vector<s32>(2, 0));
	}
	else
	{
		m_namectrl->set_iconsize(xui_vector<s32>(0));
		m_namectrl->set_textalign(k_textalign_cc);
		m_namectrl->set_textoffset(xui_vector<s32>(0));
	}

	if (m_parent)
		m_parent->invalid();
}

void xui_dockpage::add_pagectrl( xui_component* ctrl )
{
	if (ctrl->get_parent())
		return;

	ctrl->set_rendery(ctrl->get_rendery()+m_namectrl->get_renderh());
	ctrl->set_parent(this);
	m_widgetvec.push_back(ctrl);
	invalid();
}

void xui_dockpage::del_pagectrl( xui_component* ctrl )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		ctrl);

	if (itor == m_widgetvec.end())
		return;

	ctrl->set_parent(NULL);
	m_widgetvec.erase(itor);
	xui_desktop::get_ins()->move_recycle(ctrl);
	invalid();
}

void xui_dockpage::mov_namectrl( s32 x, s32 y, s32 w )
{
	m_namectrl->on_perform_x(x);
	m_namectrl->on_perform_y(y);
	m_namectrl->on_perform_w(w);
}

xui_rect2d<s32> xui_dockpage::get_renderrtins( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	rt.ay += m_namectrl->get_renderh();
	return rt;
}

xui_component* xui_dockpage::choose( const xui_vector<s32>& pt )
{
	if (m_enable)
	{
		return choose_else(pt);
	}

	return NULL;
}

xui_component* xui_dockpage::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	if (m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();

		xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
		if (dockview->get_showpage() == this)
		{
			for (s32 i = (s32)m_widgetvec.size()-1; i >= 0; --i)
			{
				if (component = m_widgetvec[i]->choose(relative))
					return component;
			}
		}
		else
		{
			return m_namectrl->choose(relative);
		}
	}

	return NULL;
}

void xui_dockpage::update_else( f32 delta )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		for (u32 i = 0; i < m_widgetvec.size(); ++i)
		{
			if (m_widgetvec[i]->was_visible())
				m_widgetvec[i]->update(delta);
		}
	}
	else
	{
		m_namectrl->update(delta);
	}
}

void xui_dockpage::render_else( void )
{
	xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
	xui_canvas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtabs()));
	m_namectrl->render();
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
		xui_canvas::get_ins()->set_cliprect(currrect);
		for (u32 i = 0; i < m_widgetvec.size(); ++i)
		{
			if (m_widgetvec[i] == m_namectrl)
				continue;

			if (m_widgetvec[i]->was_visible() && currrect.get_inter(m_widgetvec[i]->get_renderrtabs()).was_valid())
				m_widgetvec[i]->render();
		}
		xui_canvas::get_ins()->set_cliprect(cliprect);
	}

	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> pagert = get_renderrtins() + get_screenpt();
		xui_rect2d<s32> namert = m_namectrl->get_renderrtabs();
		xui_colour      color  = get_vertexcolor() * m_sidecolor;

		pagert.bx -= 1;
		pagert.by -= 1;
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.by  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.by  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.ay  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.ay  ), xui_vector<s32>(namert.ax,   pagert.ay  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.ay  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.ax+3, namert.ay	 ), xui_vector<s32>(namert.bx-3, namert.ay  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.bx,	  namert.ay+3), xui_vector<s32>(namert.bx,	 namert.by  ), color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.ax,	  namert.by  ), xui_vector<s32>(namert.ax,   namert.ay+3), color);
		xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.ax,			 
			namert.ay,
			namert.ax+6, 
			namert.ay+6), color, 180, 90, 1);
		xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.bx-6, 
			namert.ay,			
			namert.bx,		   
			namert.ay+6), color, 270, 90, 1);
	}
}

void xui_dockpage::on_perform( xui_method_args&  args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		rt.ax += m_inborder.ax;
		rt.ay += m_inborder.ay;
		rt.bx -= m_inborder.bx;
		rt.by -= m_inborder.by;

		perform_alignhorz(rt, m_widgetvec);
		perform_alignvert(rt, m_widgetvec);
		perform_dockstyle(rt, m_widgetvec);
	}
}

void xui_dockpage::on_renderback( xui_method_args&  args )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> rt;
		xui_colour		color = get_vertexcolor() * xui_colour(1.0f, 0.25f);
		rt = get_renderrtins() + get_screenpt();
		xui_canvas::get_ins()->fill_rectangle(rt, color);
		rt = m_namectrl->get_renderrtabs();
		xui_canvas::get_ins()->fill_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	}
}

void xui_dockpage::on_namectrltopdraw( xui_component* sender, xui_method_args&  args )
{
	xui_dockview* dockview = get_dockview(xui_desktop::get_ins()->get_hoverctrl());
	if (dockview == NULL)
		return;

	xui_window* window = dockview->get_window();
	if (window != args.wparam)
		return;

	xui_vector<s32> pt = xui_global::get_syswndmouse(window->get_owner());
	xui_colour fill_color = xui_colour(0.5f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	xui_colour side_color = xui_colour(1.0f,          0.0f,          0.9f,          0.9f);
	u08 dockstyle = cal_dockinfo(dockview, pt);
	if (dockstyle == k_dockstyle_f)
	{
		xui_rect2d<s32> rt = dockview->get_namerect() + dockview->get_screenpt() + dockview->get_freerect().get_pt();
		rt.ax = rt.bx+ 6;
		rt.bx = rt.ax+ 2;
		rt.by = rt.ay+20;
		xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_white);
		xui_canvas::get_ins()->fill_triangle (xui_vector<s32>(rt.ax+1, rt.by), 3, k_triangle_down, xui_colour::k_white);
	}
	else
	if (dockstyle == k_dockstyle_u)
	{
		xui_vector<s32> sz = get_rendersz();
		xui_rect2d<s32> rt;
		rt.ax = pt.x-sz.w/4;
		rt.bx = pt.x+sz.w/4;
		rt.ay = pt.y-sz.h/4;
		rt.by = pt.y+sz.h/4;

		xui_rect2d<s32> pagert = rt;
		pagert.ax += m_border.ax;
		pagert.ay += m_border.ay;
		pagert.ay += m_namectrl->get_renderh();
		pagert.bx -= m_border.bx;
		pagert.by -= m_border.by;
		xui_rect2d<s32> namert = rt;
		namert.ax += m_border.ax;
		namert.ay += m_border.ay;
		namert.set_sz(m_namectrl->get_rendersz());

		xui_canvas::get_ins()->fill_rectangle(pagert, fill_color);
		xui_canvas::get_ins()->fill_round(namert, fill_color, xui_rect2d<s32>(3, 3, 0, 0));

		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.by  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.by  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.ay  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.ay  ), xui_vector<s32>(namert.ax,   pagert.ay  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.ay  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.ax+3, namert.ay	 ), xui_vector<s32>(namert.bx-3, namert.ay  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.bx,	  namert.ay+3), xui_vector<s32>(namert.bx,	 namert.by  ), side_color);
		xui_canvas::get_ins()->draw_line(xui_vector<s32>(namert.ax,	  namert.by  ), xui_vector<s32>(namert.ax,   namert.ay+3), side_color);
		xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.ax,			 
			namert.ay,
			namert.ax+6, 
			namert.ay+6), side_color, 180, 90, 1);
		xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.bx-6, 
			namert.ay,			
			namert.bx,		   
			namert.ay+6), side_color, 270, 90, 1);

		xui_bitmap*  icon = m_namectrl->get_rendericon();
		std::wstring text = m_namectrl->get_rendertext();
		if (text.length() > 0)
		{
			xui_canvas::get_ins()->draw_text(
				text, 
				m_namectrl->get_textfont(), 
				m_namectrl->get_rendertextrt()+namert.get_pt(), 
				m_namectrl->get_textdraw(),
				true);
		}
		if (icon)
		{
			xui_canvas::get_ins()->draw_image(
				icon, 
				xui_rect2d<s32>(m_namectrl->get_rendericonpt()+namert.get_pt(), m_namectrl->get_iconsize()), 
				xui_colour::k_white);
		}
	}
	else 
	if (dockstyle != k_dockstyle_n)
	{
		xui_rect2d<s32> rt = dockview->get_freerect() + dockview->get_screenpt();
		rt.ax += m_border.ax;
		rt.ay += m_border.ay;
		rt.bx -= m_border.bx;
		rt.by -= m_border.by;
		rt.ay += 24;

		switch (dockstyle)
		{
		case k_dockstyle_l: rt.bx  = rt.ax + rt.get_w()/3; break;
		case k_dockstyle_r: rt.ax  = rt.bx - rt.get_w()/3; break;
		case k_dockstyle_t: rt.by  = rt.ay + rt.get_h()/3; break;
		case k_dockstyle_b: rt.ay  = rt.by - rt.get_h()/3; break;
		}

		rt.bx -= 1;
		rt.by -= 1;
		xui_canvas::get_ins()->fill_rectangle(rt, fill_color);
		xui_canvas::get_ins()->draw_rectangle(rt, side_color);
	}
}

void xui_dockpage::on_namectrlmousedown( xui_component* sender, xui_method_mouse& args )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	dockview->set_showpage(this);
}

void xui_dockpage::on_namectrlmouserise( xui_component* sender, xui_method_mouse& args )
{
	xui_dockview* dockview = get_dockview(xui_desktop::get_ins()->get_hoverctrl());
	if (m_namectrl->has_catch() && dockview)
	{
		xui_window* window = dockview->get_window();
		u08 dockstyle = cal_dockinfo(dockview, xui_global::get_syswndmouse(window->get_owner()));
		if (dockstyle != k_dockstyle_n)
		{
			xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
			rootview->del_dockpage(this, false);
			if (dockstyle == k_dockstyle_u)
			{
				xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
				xui_vector<s32> sz = get_rendersz();
				pt.x -= sz.w/2; 
				pt.y -= sz.h/2;
				pt.x  = xui_max(0, pt.x);
				pt.y  = xui_max(0, pt.y);

				xui_window*   popupwnd = new xui_window(sz, false, true);
				xui_dockview* fillview = new xui_dockview(xui_vector<s32>(0), k_dockstyle_f);
				fillview->add_dockpage(this, k_dockstyle_f);
				popupwnd->add_child(fillview);
				popupwnd->set_renderpt(pt);
				xui_desktop::get_ins()->add_child(popupwnd);
			}
			else
			{
				dockview->add_dockpage(this, dockstyle);
			}
		}
	}
}

xui_dockview* xui_dockpage::get_dockview( xui_component* comp )
{
	xui_component* root      = comp;
	xui_dockview*  dockview  = NULL;
	while (root)
	{
		if (dockview = xui_dynamic_cast(xui_dockview, root))
			break;

		root = root->get_parent();
	}

	return dockview;
}

u08 xui_dockpage::cal_dockinfo( xui_dockview* dockview, const xui_vector<s32>& pt )
{
	u08 dockstyle = k_dockstyle_n;
	if (m_dockarea != k_arealimit_n && dockview)
	{
		xui_rect2d<s32> rt = dockview->get_freerect() + dockview->get_screenpt();
		rt.ax += m_border.ax;
		rt.ay += m_border.ay;
		rt.bx -= m_border.bx;
		rt.by -= m_border.by;

		if (pt.y < rt.ay+24)
		{
			if (has_dockarea(dockview->get_dockstyle()) && dockview != m_parent)
			{
				dockstyle = k_dockstyle_f;
			}
		}
		else
		{
			const std::vector<xui_dockpage*>& pagelist = dockview->get_pagelist();
			if (dockview == m_parent && pagelist.size() == 1 && pagelist.front() == this)
			{
				xui_window* window = get_window();
				if (window->get_owner() == NULL || dockview->get_parent() != window)
					dockstyle = k_dockstyle_u;
			}
			else
			{
				if (has_dockarea(dockview->get_dockstyle()) || dockview->get_dockstyle() == k_dockstyle_f)
				{
					rt.ay += 24;
					if		 (pt.x < (rt.ax+rt.get_w()/3) && has_dockarea(k_dockstyle_l))	dockstyle = k_dockstyle_l;
					else if  (pt.x > (rt.bx-rt.get_w()/3) && has_dockarea(k_dockstyle_r))	dockstyle = k_dockstyle_r;
					else if	 (pt.y < (rt.ay+rt.get_h()/3) && has_dockarea(k_dockstyle_t))	dockstyle = k_dockstyle_t;
					else if  (pt.y > (rt.by-rt.get_h()/3) && has_dockarea(k_dockstyle_b))	dockstyle = k_dockstyle_b;
					else																    dockstyle = k_dockstyle_u;
				}
			}
		}
	}

	return dockstyle;
}