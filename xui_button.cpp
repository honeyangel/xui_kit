#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_button.h"

xui_implement_rtti(xui_button, xui_drawer);

/*
//static
*/
const xui_colour xui_button::default_backcolor = xui_colour(1.0f,  90.0f/255.0f);
const xui_colour xui_button::default_movecolor = xui_colour(1.0f, 127.0f/255.0f);
const xui_colour xui_button::default_downcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

xui_method_explain(xui_button, create, xui_button*)( xui_bitmap* icon, s32 width )
{
	xui_button* button = new xui_button(xui_vector<s32>(width, 24));
	xui_method_ptrcall(button, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(button, set_corner		)(3);
	xui_method_ptrcall(button, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(button, set_drawcolor	)(true);
	xui_method_ptrcall(button, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(button, ini_drawer		)(icon, xui_vector<s32>(16));

	return button;
}
xui_method_explain(xui_button, create, xui_button*)( xui_bitmap* icon, const std::wstring& text, s32 width )
{
	xui_button* button = new xui_button(xui_vector<s32>(width, 24));
	xui_method_ptrcall(button, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(button, set_corner		)(3);
	xui_method_ptrcall(button, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(button, set_drawcolor	)(true);
	xui_method_ptrcall(button, set_textoffset	)(xui_vector<s32>(6, 0));
	xui_method_ptrcall(button, ini_drawer		)(icon, xui_vector<s32>(16));
	xui_method_ptrcall(button, ini_drawer		)(text);

	return button;
}

/*
//constructor
*/
xui_create_explain(xui_button)( const xui_vector<s32>& size )
: xui_drawer(size)
{
	m_backcolor = default_backcolor;
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
	if (args.mouse == MB_L)
	{
		xui_method_args		 other_args;
		xm_buttonclick(this, other_args);
	}
}
xui_method_explain(xui_button, on_renderself,	void				)( xui_method_args&  args )
{
	xui_control::on_renderself(args);
	xui_bitmap*  icon = get_rendericon();
	std::wstring text = get_rendertext();
	if (icon == NULL && text.empty())
		return;

	xui_colour color  = get_vertexcolor();
	// draw text
	xui_family_render textdraw = m_textdraw;
	textdraw.normalcolor *= color;
	textdraw.strokecolor *= color;
	if (text.length() > 0)
	{
		xui_rect2d<s32> rt = get_rendertextrt();
		if (has_catch())
			rt += xui_vector<s32>(0, 1);

		xui_convas::get_ins()->draw_text(
			text, 
			m_textfont, 
			rt+get_screenpt(), 
			textdraw,
			m_singleline);
	}

	// draw icon
	if (icon)
	{
		xui_vector<s32> pt = get_rendericonpt();
		if (has_catch())
			pt += xui_vector<s32>(0, 1);

		xui_convas::get_ins()->draw_image(
			icon, 
			xui_rect2d<s32>(pt+get_screenpt(), m_iconsize), 
			color);
	}
}

/*
//virtual
*/
xui_method_explain(xui_button, get_rendercolor, xui_colour			)( void ) const
{
	if		(has_catch())	return m_downcolor;
	else if (was_hover())	return m_movecolor;
	else					return m_backcolor;
}

