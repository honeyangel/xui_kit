#include "xui_convas.h"
#include "xui_drawer.h"
#include "xui_desktop.h"
#include "xui_dockview.h"
#include "xui_dockpage.h"

xui_implement_rtti(xui_dockpage, xui_control);

/*
//static
*/
xui_method_explain(xui_dockpage, create,				xui_dockpage*			)( xui_bitmap* icon, const std::wstring& text, s32 size, u32 dockarea, s32 minlimit, u08 initdock )
{
	xui_dockpage* dockpage = new xui_dockpage(xui_vector<s32>(size), dockarea, minlimit, initdock);
	dockpage->ini_namectrl(icon, text);
	return dockpage;
}

/*
//constructor
*/
xui_create_explain(xui_dockpage)( const xui_vector<s32>& size, u32 dockarea, s32 minlimit, u08 initdock )
: xui_control(size)
{
	m_border	= xui_rect2d<s32>(2);
	m_inborder	= xui_rect2d<s32>(0);
	m_dockarea	= dockarea;
	m_initdock	= initdock;
	m_minlimit  = minlimit;
	m_namectrl	= xui_drawer::create(L"");
	xui_method_ptrcall(m_namectrl, set_parent)(this);
	m_namectrl->xm_topdraw	 += new xui_method_member<xui_method_args,  xui_dockpage>(this, &xui_dockpage::on_namectrltopdraw  );
	m_namectrl->xm_mousedown += new xui_method_member<xui_method_mouse, xui_dockpage>(this, &xui_dockpage::on_namectrlmousedown);
	m_namectrl->xm_mouserise += new xui_method_member<xui_method_mouse, xui_dockpage>(this, &xui_dockpage::on_namectrlmouserise);
	m_widgetvec.push_back(m_namectrl);
}

/*
//method
*/
xui_method_explain(xui_dockpage, get_inborder,			const xui_rect2d<s32>&	)( void ) const
{
	return m_inborder;
}
xui_method_explain(xui_dockpage, set_inborder,			void					)( const xui_rect2d<s32>& border )
{
	if (m_inborder != border)
	{
		m_inborder  = border;
		perform();
	}
}
xui_method_explain(xui_dockpage, has_dockarea,			bool					)( u08 dockstyle )
{
	switch (dockstyle)
	{
	case DOCKSTYLE_L: return (m_dockarea & AREALIMIT_L) != 0;
	case DOCKSTYLE_R: return (m_dockarea & AREALIMIT_R) != 0;
	case DOCKSTYLE_T: return (m_dockarea & AREALIMIT_T) != 0;
	case DOCKSTYLE_B: return (m_dockarea & AREALIMIT_B) != 0;
	case DOCKSTYLE_F: return (m_dockarea & AREALIMIT_F) != 0;
	}

	return false;
}
xui_method_explain(xui_dockpage, get_initdock,			u08						)( void ) const
{
	return m_initdock;
}
xui_method_explain(xui_dockpage, get_minlimit,			s32						)( void ) const
{
	return m_minlimit;
}
xui_method_explain(xui_dockpage, get_namesize,			s32						)( void ) const
{
	s32 size = 0;
	size += m_namectrl->get_borderrt().ax;
	size += m_namectrl->get_borderrt().bx;
	size += m_namectrl->get_iconsize().w + m_namectrl->get_textoffset().x;
	size += xui_convas::get_ins()->calc_size(m_namectrl->get_text(), m_namectrl->get_textfont(), 0, true).w;

	return size;
}
xui_method_explain(xui_dockpage, get_pagename,			const std::wstring&		)( void ) const
{
	return m_namectrl->get_text();
}
xui_method_explain(xui_dockpage, ini_namectrl,			void					)( xui_bitmap* icon, const std::wstring& text )
{
	m_namectrl->ini_drawer(icon);
	m_namectrl->ini_drawer(text);
	if (icon)
	{
		xui_method_ptrcall(m_namectrl, set_iconalign )(IMAGE_FRONT_TEXT);
		xui_method_ptrcall(m_namectrl, set_textalign )(TA_LC);
		xui_method_ptrcall(m_namectrl, set_textoffset)(xui_vector<s32>(2, 0));
	}
	else
	{
		xui_method_ptrcall(m_namectrl, set_iconsize	 )(xui_vector<s32>(0));
		xui_method_ptrcall(m_namectrl, set_textalign )(TA_CC);
		xui_method_ptrcall(m_namectrl, set_textoffset)(xui_vector<s32>(0));
	}

	if (m_parent)
		m_parent->invalid();
}
xui_method_explain(xui_dockpage, add_pagectrl,			void					)( xui_component* ctrl )
{
	if (ctrl->get_parent())
		return;

	xui_method_ptrcall(ctrl, set_rendery)(ctrl->get_rendery()+m_namectrl->get_renderh());
	xui_method_ptrcall(ctrl, set_parent	)(this);
	m_widgetvec.push_back(ctrl);
	invalid();
}
xui_method_explain(xui_dockpage, del_pagectrl,			void					)( xui_component* ctrl )
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
xui_method_explain(xui_dockpage, mov_namectrl,			void					)( s32 x, s32 y, s32 w )
{
	m_namectrl->on_perform_x(x);
	m_namectrl->on_perform_y(y);
	m_namectrl->on_perform_w(w);
}

/*
//override
*/
xui_method_explain(xui_dockpage, get_renderrtins,		xui_rect2d<s32>			)( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	rt.ay += m_namectrl->get_renderh();
	return rt;
}

/*
//virtual
*/
xui_method_explain(xui_dockpage, choose,				xui_component*			)( const xui_vector<s32>& pt )
{
	if (m_enable)
	{
		return choose_else(pt);
	}

	return NULL;
}
xui_method_explain(xui_dockpage, choose_else,			xui_component*			)( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	if (m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();

		xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
		if (dockview->get_showpage() == this)
		{
			xui_vecptr_delloop(m_widgetvec)
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
xui_method_explain(xui_dockpage, update_else,			void					)( f32 delta )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_vecptr_addloop(m_widgetvec)
		{
			if (m_widgetvec[i]->was_enable() && m_widgetvec[i]->was_visible())
				m_widgetvec[i]->update(delta);
		}
	}
	else
	{
		m_namectrl->update(delta);
	}
}
xui_method_explain(xui_dockpage, render_else,			void					)( void )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtabs()));
	m_namectrl->render();
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
		xui_convas::get_ins()->set_cliprect(currrect);
		xui_vecptr_addloop(m_widgetvec)
		{
			if (m_widgetvec[i] == m_namectrl)
				continue;

			if (m_widgetvec[i]->was_visible() && currrect.get_inter(m_widgetvec[i]->get_renderrtabs()).was_valid())
				m_widgetvec[i]->render();
		}
		xui_convas::get_ins()->set_cliprect(cliprect);
	}

	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> pagert = get_renderrtins() + get_screenpt();
		xui_rect2d<s32> namert = m_namectrl->get_renderrtabs();
		xui_colour      color  = get_vertexcolor() * m_sidecolor;

		//pagert.bx -= 1;
		//pagert.by -= 1;
		xui_convas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.by  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(pagert.bx,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.by  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.by  ), xui_vector<s32>(pagert.ax,   pagert.ay  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(pagert.ax,   pagert.ay  ), xui_vector<s32>(namert.ax,   pagert.ay  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(namert.bx,   pagert.ay  ), xui_vector<s32>(pagert.bx,   pagert.ay  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(namert.ax+3, namert.ay	 ), xui_vector<s32>(namert.bx-3, namert.ay  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(namert.bx,	  namert.ay+3), xui_vector<s32>(namert.bx,	 namert.by  ), color);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(namert.ax,	  namert.by  ), xui_vector<s32>(namert.ax,   namert.ay+3), color);
		xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.ax,			 
			namert.ay,
			namert.ax+6, 
			namert.ay+6), color, 180, 90, 1);
		xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
			namert.bx-6, 
			namert.ay,			
			namert.bx,		   
			namert.ay+6), color, 270, 90, 1);
	}
}

/*
//callback
*/
xui_method_explain(xui_dockpage, on_perform,			void					)( xui_method_args& args )
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
xui_method_explain(xui_dockpage, on_renderback,			void					)( xui_method_args& args )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> rt;
		xui_colour		color = get_vertexcolor() * xui_colour(1.0f, 0.25f);
		rt = get_renderrtins() + get_screenpt();
		xui_convas::get_ins()->fill_rectangle(rt, color);
		rt = m_namectrl->get_renderrtabs();
		xui_convas::get_ins()->fill_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	}
}

/*
//event
*/
xui_method_explain(xui_dockpage, on_namectrltopdraw,	void					)( xui_component* sender, xui_method_args&  args )
{
	if (m_namectrl->has_catch())
	{
		xui_dockview* dockview = get_dockview(xui_desktop::get_ins()->get_hoverctrl());
		if (dockview)
		{
			u08 dockstyle = cal_dockinfo(dockview, xui_desktop::get_ins()->get_mousecurr());
			if (dockstyle == DOCKSTYLE_F)
			{
				xui_rect2d<s32> rt = dockview->get_namerect() + dockview->get_screenpt() + dockview->get_freerect().get_pt();
				rt.ax = rt.bx+ 6;
				rt.bx = rt.ax+ 2;
				rt.by = rt.ay+20;
				xui_convas::get_ins()->fill_rectangle(rt, xui_colour::white);
				xui_convas::get_ins()->fill_triangle (xui_vector<s32>(rt.ax+1, rt.by), 3, TRIANGLE_DOWN, xui_colour::white);
			}

			if (dockstyle != DOCKSTYLE_N)
			{
				xui_rect2d<s32> rt = dockview->get_freerect() + dockview->get_screenpt();
				rt.ax += m_border.ax;
				rt.ay += m_border.ay;
				rt.bx -= m_border.bx;
				rt.by -= m_border.by;
				rt.ay += 24;

				xui_colour fill_color = xui_colour(0.5f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
				xui_colour side_color = xui_colour(1.0f,          0.0f,          0.9f,          0.9f);
				switch (dockstyle)
				{
				case DOCKSTYLE_L: rt.bx  = rt.ax + rt.get_w()/3; break;
				case DOCKSTYLE_R: rt.ax  = rt.bx - rt.get_w()/3; break;
				case DOCKSTYLE_T: rt.by  = rt.ay + rt.get_h()/3; break;
				case DOCKSTYLE_B: rt.ay  = rt.by - rt.get_h()/3; break;
				}

				//rt.bx -= 1;
				//rt.by -= 1;
				xui_convas::get_ins()->fill_rectangle(rt, fill_color);
				xui_convas::get_ins()->draw_rectangle(rt, side_color);
			}
		}
	}
}
xui_method_explain(xui_dockpage, on_namectrlmousedown,	void					)( xui_component* sender, xui_method_mouse& args )
{
	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	dockview->set_showpage(this);
}
xui_method_explain(xui_dockpage, on_namectrlmouserise,	void					)( xui_component* sender, xui_method_mouse& args )
{
	if (m_namectrl->has_catch())
	{
		xui_dockview* dockview = get_dockview(xui_desktop::get_ins()->get_hoverctrl());
		if (dockview)
		{
			u08 dockstyle = cal_dockinfo(dockview, xui_desktop::get_ins()->get_mousecurr());
			if (dockstyle != DOCKSTYLE_N)
			{
				xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
				rootview->del_dockpage(this);
				dockview->add_dockpage(this, dockstyle);
			}
		}
	}
}

/*
//method
*/
xui_method_explain(xui_dockpage, get_dockview,			xui_dockview*			)( xui_component* comp )
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
xui_method_explain(xui_dockpage, cal_dockinfo,			u08						)( xui_dockview* dockview, const xui_vector<s32>& pt )
{
	u08 dockstyle = DOCKSTYLE_N;

	xui_rect2d<s32> rt = dockview->get_freerect() + dockview->get_screenpt();
	rt.ax += m_border.ax;
	rt.ay += m_border.ay;
	rt.bx -= m_border.bx;
	rt.by -= m_border.by;

	if (pt.y < rt.ay+24)
	{
		if (has_dockarea(dockview->get_dockstyle()) && dockview != m_parent)
			dockstyle = DOCKSTYLE_F;
	}
	else
	{
		if (has_dockarea(dockview->get_dockstyle()) || dockview->get_dockstyle() == DOCKSTYLE_F)
		{
			rt.ay += 24;
			if		 (pt.x < (rt.ax+rt.get_w()/3) && has_dockarea(DOCKSTYLE_L))	dockstyle = DOCKSTYLE_L;
			else if  (pt.x > (rt.bx-rt.get_w()/3) && has_dockarea(DOCKSTYLE_R))	dockstyle = DOCKSTYLE_R;
			else if	 (pt.y < (rt.ay+rt.get_h()/3) && has_dockarea(DOCKSTYLE_T))	dockstyle = DOCKSTYLE_T;
			else if  (pt.y > (rt.by-rt.get_h()/3) && has_dockarea(DOCKSTYLE_B))	dockstyle = DOCKSTYLE_B;
			else
			{}
		}
	}

	return dockstyle;
}