#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"

xui_implement_rtti(xui_menuitem, xui_drawer);

/*
//constructor
*/
xui_create_explain(xui_menuitem)( void )
: xui_drawer(xui_vector<s32>(0, 20))
{
	m_flag		= false;
	m_submenu	= NULL;
}

/*
//destructor
*/
xui_delete_explain(xui_menuitem)( void )
{
	xui_desktop::get_ins()->move_recycle(m_submenu);
}

/*
//method
*/
xui_method_explain(xui_menuitem, get_maxwidth,		s32						)( void )
{
	s32 width = 72;
	width += m_border.ax;
	width += m_border.bx;
	width += m_iconoffset.x;
	width += m_iconsize.w;
	width += m_textoffset.x;
	if (m_text.length() > 0)
	{
		width += xui_convas::get_ins()->calc_size(m_text, m_textfont, 0, true).w;
	}

	return width;
}
xui_method_explain(xui_menuitem, get_submenu,		xui_menu*				)( void )
{
	return m_submenu;
}
xui_method_explain(xui_menuitem, set_submenu,		void					)( xui_menu* submenu )
{
	m_submenu = submenu;
}

/*
//flag
*/
xui_method_explain(xui_menuitem, get_flag,			bool					)( void ) const
{
	return m_flag;
}
xui_method_explain(xui_menuitem, set_flag,			void					)( bool flag )
{
	m_flag = flag;
}

/*
//hottext
*/
xui_method_explain(xui_menuitem, get_hint,			const std::wstring&		)( void ) const
{
	return m_hint;
}
xui_method_explain(xui_menuitem, set_hint,			void					)( const std::wstring& hint )
{
	m_hint = hint;
}
xui_method_explain(xui_menuitem, get_draw,			const xui_family_render&)( void ) const
{
	return m_draw;
}
xui_method_explain(xui_menuitem, set_draw,			void					)( const xui_family_render& draw )
{
	m_draw = draw;
}

/*
//override
*/
xui_method_explain(xui_menuitem, get_rendercolor,	xui_colour				)( void ) const
{
	xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);
	if (was_hover() || (m_submenu && menu->get_showsubmenu() == m_submenu))
		return xui_colour(0.5f, 127.0f/255.0f);

	return m_backcolor;
}

/*
//callback
*/
xui_method_explain(xui_menuitem, on_nonfocus,		void					)( xui_method_args&  args )
{
	xui_drawer::on_nonfocus(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, xui_desktop::get_ins()->get_floatctrl());
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || menu == NULL || menu->was_series(focusctrl) == false)
	{
		xui_toggle* toggle = menu->get_ownertoggle();
		if (toggle && focusctrl != toggle)
			toggle->ini_toggle(false);

		xui_desktop::get_ins()->set_floatctrl(NULL, NULL);
	}
}
xui_method_explain(xui_menuitem, on_renderself,		void					)( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);
	if (m_icon == NULL && m_flag)
	{
		xui_vector<s32> pt = get_rendericonpt()+get_screenpt();
		pt.x += m_iconsize.w/2;
		pt.y += m_iconsize.h/2;
		xui_convas::get_ins()->draw_tick(pt, 8, xui_colour::white);
	}

	if (m_hint.length() > 0)
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		rt.ax = rt.bx-72;
		rt    = xui_convas::get_ins()->calc_draw(m_hint, m_textfont, rt, TEXTALIGN_LC, true);
		xui_convas::get_ins()->draw_text(m_hint, m_textfont, rt.get_pt(), m_draw);
	}

	if (m_submenu)
	{
		xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);

		xui_colour      color  = (was_hover() || menu->get_showsubmenu() == m_submenu) ? xui_button::default_downcolor : xui_button::default_backcolor;
		xui_rect2d<s32> rt     = get_renderrtins() + get_screenpt();
		xui_vector<s32> center = xui_vector<s32>(rt.bx-6, rt.ay+rt.get_h()/2);
		xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, get_vertexcolor()*color);
	}
}
xui_method_explain(xui_menuitem, on_mouseclick,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mouseclick(args);
	if (args.mouse == MB_L && m_submenu == NULL)
	{
		xui_method_args other_args;
		xm_click(this,  other_args);

		xui_desktop::get_ins()->set_focusctrl(NULL);
	}
}
xui_method_explain(xui_menuitem, on_mouseenter,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mouseenter(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);
	menu->set_showsubmenu(m_submenu);
	if (m_submenu)
	{
		xui_rect2d<s32> rt = get_renderrtabs();
		xui_vector<s32> pt = xui_vector<s32>(rt.bx, rt.ay);
		if (pt.x + m_submenu->get_renderw() > xui_desktop::get_ins()->get_renderw())
			pt.x = rt.ax - m_submenu->get_renderw();
		if (pt.y + m_submenu->get_renderh() > xui_desktop::get_ins()->get_renderh())
			pt.y = xui_desktop::get_ins()->get_renderh() - m_submenu->get_renderh();

		m_submenu->set_renderpt(pt);
	}
}