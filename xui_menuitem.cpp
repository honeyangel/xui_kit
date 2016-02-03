#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_button.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"

xui_implement_rtti(xui_menuitem, xui_drawer);

/*
//constructor
*/
xui_create_explain(xui_menuitem)( void )
: xui_drawer(xui_vector<s32>(0, 24))
{
	m_submenu = NULL;
}

/*
//destructor
*/
xui_delete_explain(xui_menuitem)( void )
{
	delete m_submenu;
}

/*
//method
*/
xui_method_explain(xui_menuitem, get_maxwidth,		s32					)( void )
{
	s32 width = 12;
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
xui_method_explain(xui_menuitem, get_submenu,		xui_menu*			)( void )
{
	return m_submenu;
}
xui_method_explain(xui_menuitem, set_submenu,		void				)( xui_menu* submenu )
{
	m_submenu = submenu;
}

/*
//override
*/
xui_method_explain(xui_menuitem, get_rendercolor,	xui_colour			)( void ) const
{
	xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);
	if (was_hover() || (m_submenu && menu->get_showsubmenu() == m_submenu))
		return xui_colour(0.5f, 127.0f/255.0f);

	return m_backcolor;
}

/*
//callback
*/
xui_method_explain(xui_menuitem, on_nonfocus,		void				)( xui_method_args&  args )
{
	xui_drawer::on_nonfocus(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, xui_desktop::get_ins()->get_floatctrl());
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || menu == NULL || menu->was_series(focusctrl) == false)
	{
		xui_desktop::get_ins()->set_floatctrl(NULL);
	}
}
xui_method_explain(xui_menuitem, on_renderself,		void				)( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);
	if (m_submenu)
	{
		xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);

		xui_colour      color  = (was_hover() || menu->get_showsubmenu() == m_submenu) ? xui_button::default_downcolor : xui_button::default_backcolor;
		xui_rect2d<s32> rt     = get_renderrtins() + get_screenpt();
		xui_vector<s32> center = xui_vector<s32>(rt.bx-6, rt.ay+rt.get_h()/2);
		xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, get_vertexcolor()*color);
	}
}
xui_method_explain(xui_menuitem, on_mouseclick,		void				)( xui_method_mouse& args )
{
	xui_drawer::on_mouseclick(args);
	if (args.mouse == MB_L && m_submenu == NULL)
	{
		xui_method_args other_args;
		xm_click(this,  other_args);

		xui_desktop::get_ins()->set_focusctrl(NULL);
	}
}
xui_method_explain(xui_menuitem, on_mouseenter,		void				)( xui_method_mouse& args )
{
	xui_drawer::on_mouseenter(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, m_parent);
	menu->set_showsubmenu(m_submenu);
	if (m_submenu)
	{
		xui_rect2d<s32> rt = get_renderrtabs();
		m_submenu->set_renderpt(xui_vector<s32>(rt.bx, rt.ay));
	}
}