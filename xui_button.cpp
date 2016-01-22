#include "xui_desktop.h"
#include "xui_button.h"

xui_implement_rtti(xui_button, xui_drawer);

/*
//static
*/
const xui_colour xui_button::default_backcolor = xui_colour(1.0f,  90.0f/255.0f);
const xui_colour xui_button::default_sidecolor = xui_colour(1.0f, 160.0f/255.0f);
const xui_colour xui_button::default_movecolor = xui_colour(1.0f, 127.0f/255.0f);
const xui_colour xui_button::default_downcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

xui_method_explain(xui_button, create, xui_button*)( xui_bitmap* icon )
{
	xui_button* button = new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(button, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(button, set_corner	)(5);
	xui_method_ptrcall(button, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(button, set_drawcolor)(true);
	xui_method_ptrcall(button, ini_drawer	)(icon, xui_vector<s32>(16));

	return button;
}
xui_method_explain(xui_button, create, xui_button*)( xui_bitmap* icon, const std::wstring& text, s32 width )
{
	xui_button* button = new xui_button(xui_vector<s32>(width, 24));
	xui_method_ptrcall(button, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(button, set_corner	)(5);
	xui_method_ptrcall(button, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(button, set_drawcolor)(true);
	xui_method_ptrcall(button, ini_drawer	)(icon, xui_vector<s32>(16));
	xui_method_ptrcall(button, ini_drawer	)(text);

	return button;
}

/*
//constructor
*/
xui_create_explain(xui_button)( const xui_vector<s32>& size, xui_component* parent )
: xui_drawer(size, parent)
{
	m_backcolor = default_backcolor;
	m_sidecolor = default_sidecolor;
	m_movecolor = default_movecolor;
	m_downcolor = default_downcolor;
}

/*
//color
*/
xui_method_explain(xui_button, get_movecolor,	const xui_colour&	)( void ) const
{
	return m_movecolor;
}
xui_method_explain(xui_button, set_movecolor,	void				)( const xui_colour& color )
{
	m_movecolor = color;
}
xui_method_explain(xui_button, get_downcolor,	const xui_colour&	)( void ) const
{
	return m_downcolor;
}
xui_method_explain(xui_button, set_downcolor,	void				)( const xui_colour& color )
{
	m_downcolor = color;
}

/*
//callback
*/
xui_method_explain(xui_button, on_mouseclick,	void				)( xui_method_mouse& args )
{
	xui_drawer::on_mouseclick(args);
	xui_method_args other_args;
	xm_click(this,  other_args);
}

/*
//virtual
*/
xui_method_explain(xui_button, get_rendercolor, xui_colour			)( void ) const
{
	if		(g_desktop->get_catchctrl() == this) return m_downcolor;
	else if (g_desktop->get_hoverctrl() == this) return m_movecolor;
	else										 return m_backcolor;
}
xui_method_explain(xui_button, get_rendericonpt,xui_vector<s32>		)( void ) const
{
	xui_vector<s32> pt = xui_drawer::get_rendericonpt();
	if (g_desktop->get_catchctrl() == this)
		pt.y += 1;

	return pt;
}

