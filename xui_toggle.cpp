#include "xui_canvas.h"
#include "xui_bitmap.h"
#include "xui_desktop.h"
#include "xui_window.h"
#include "xui_menu.h"
#include "xui_toggle.h"

xui_implement_rtti(xui_toggle, xui_button)

xui_toggle* xui_toggle::circle( void )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16), k_toggle_circle);
	toggle->set_drawcolor(false);
	return toggle;
}

xui_toggle* xui_toggle::create( void )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16), k_toggle_normal);
	toggle->set_corner(3);
	toggle->set_drawcolor(true);
	toggle->set_backcolor(xui_colour::k_darkgray);

	return toggle;
}

xui_toggle* xui_toggle::create( xui_bitmap* icon, s32 width )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(width, 24), k_toggle_button);
	toggle->set_sidestyle(k_sidestyle_s);
	toggle->set_corner(3);
	toggle->set_borderrt(xui_rect2d<s32>(4));
	toggle->set_drawcolor(true);
	toggle->set_iconalign(k_image_c);
	toggle->ini_drawer(icon, xui_vector<s32>(16));

	return toggle;
}

xui_toggle* xui_toggle::create( xui_bitmap* icon, const std::wstring& text, s32 width )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(width, 24), k_toggle_button);
	toggle->set_sidestyle(k_sidestyle_s);
	toggle->set_corner(3);
	toggle->set_borderrt(xui_rect2d<s32>(4));
	toggle->set_drawcolor(true);
	toggle->set_textoffset(xui_vector<s32>(6, 0));
	toggle->ini_drawer(icon, xui_vector<s32>(16));
	toggle->ini_drawer(text);

	return toggle;
}

xui_toggle::xui_toggle( const xui_vector<s32>& size, u08 drawstyle )
: xui_button(size)
{
	m_menu		= NULL;
	m_push		= false;
	m_drawstyle = drawstyle;
}

xui_toggle::~xui_toggle( void )
{
	xui_desktop::get_ins()->move_recycle(m_menu);
}

void xui_toggle::ini_toggle( bool push )
{
	m_push = push;
}

bool xui_toggle::was_push( void ) const
{
	return m_push;
}

void xui_toggle::set_push( bool push )
{
	if (m_push != push)
	{
		m_push  = push;

		xui_method_args		 args;
		xm_toggleclick(this, args);

		if (m_menu && m_push)
		{
			m_menu->refresh();

			xui_window* window = get_window();
			xui_vector<s32> pt = get_screenpt() + xui_vector<s32>(0, get_renderh());
			xui_rect2d<s32> rt = window->get_renderrtabs();
			if (pt.x + m_menu->get_renderw() > rt.bx)
				pt.x = rt.bx - m_menu->get_renderw();
			if (pt.y + m_menu->get_renderh() > rt.by)
				pt.y = rt.by - m_menu->get_renderh();

			m_menu->set_renderpt(pt);
			m_menu->set_showsubmenu(NULL);
			m_menu->req_focus();
			xui_desktop::get_ins()->set_floatctrl(window, m_menu);
		}
	}
}

xui_menu* xui_toggle::get_menu( void )
{
	return m_menu;
}

void xui_toggle::set_menu( xui_menu* menu )
{
	m_menu = menu;

	if (m_menu)
		m_menu->set_ownertoggle(this);
}

void xui_toggle::on_mousedown( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == k_mb_left)
		set_push(!m_push);
}

void xui_toggle::on_renderself( xui_method_args&  args )
{
	if (m_drawstyle == k_toggle_normal && m_push == false)
		return;

	xui_colour      color  = get_vertexcolor();
	xui_rect2d<s32> rt     = get_renderrtins() + get_screenpt();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	switch (m_drawstyle)
	{
	case k_toggle_circle:
		{
			if (was_hover() || m_push)
				xui_canvas::get_ins()->fill_circle(center, 5,	 color*get_rendercolor(),	0, 360);

			xui_canvas::get_ins()->draw_circle(center, 5,	 color*m_sidecolor,			0, 360);
		}
		break;
	case k_toggle_normal:
		{
			s32	half = rt.get_w()/2;
			xui_canvas::get_ins()->draw_tick  (center, half, color);
		}
		break;
	case k_toggle_button:
		{
			xui_button::on_renderself(args);
			if (m_menu && m_text.length() > 0)
			{
				if (m_drawcolor == false)
				{
					if (was_hover() || m_push)
						color *= xui_button::k_default_downcolor;
				}

				xui_canvas::get_ins()->fill_triangle(xui_vector<s32>(rt.bx-6, center.y), 3, k_triangle_down, color);
			}
		}
		break;
	}
}

xui_colour xui_toggle::get_rendercolor( void ) const
{
	if (m_drawstyle == k_toggle_normal)
	{
		return m_backcolor;
	}
	else
	if (m_drawstyle == k_toggle_circle)
	{
		if (m_push)
		{
			if (was_hover())return m_movecolor;
			else			return m_backcolor;
		}
		else
		{
			return xui_colour::k_zero;
		}
	}
	else
	{
		if (m_push)
			return m_downcolor;
		else
			return xui_button::get_rendercolor();
	}
}