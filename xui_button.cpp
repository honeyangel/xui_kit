#include "xui_desktop.h"
#include "xui_button.h"

/*
//constructor
*/
xui_create_explain(xui_button)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_drawer(name, rect)
{
	m_type	   += "button";
	m_movecolor = xui_colour(1.0f, 0.61f, 0.61f, 0.61f);
	m_downcolor = xui_colour(1.0f, 0.16f, 0.52f, 0.74f);
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
	if (g_desktop->get_catchctrl() == this && m_text.length() == 0)
		pt.y  += 1;

	return pt;
}

xui_method_explain(xui_button, get_rendertextrt,xui_rect2d<s32>		)( void ) const
{
	xui_rect2d<s32> rt = xui_drawer::get_rendertextrt();
	if (g_desktop->get_catchctrl() == this)
		rt.oft_y(1);

	return rt;
}

