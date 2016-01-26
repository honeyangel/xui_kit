#include "xui_convas.h"
#include "xui_plusctrl.h"

xui_implement_rtti(xui_plusctrl, xui_button);

/*
//constructor
*/
xui_create_explain(xui_plusctrl)( u08 drawmode, bool expanded, xui_component* parent )
: xui_button(xui_vector<s32>(16), parent)
{
	m_movecolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	m_downcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	m_visible	= false;
	m_onlyside  = false;
	m_expanded	= expanded;
}

/*
//method
*/
xui_method_explain(xui_plusctrl, was_expanded,		bool		)( void ) const
{
	return m_expanded;
}
xui_method_explain(xui_plusctrl, set_expanded,		void		)( bool flag )
{
	if (m_expanded != flag)
	{
		m_expanded  = flag;

		xui_method_args args;
		xm_expand(this, args);
	}
}
xui_method_explain(xui_plusctrl, set_onlyside,		void		)( bool flag )
{
	m_onlyside = flag;
}

/*
//virtual
*/
xui_method_explain(xui_plusctrl, get_rendercolor,	xui_colour	)( void ) const
{
	if (m_onlyside)
		return m_backcolor;

	return xui_button::get_rendercolor();
}

/*
//callback
*/
xui_method_explain(xui_plusctrl, on_mousedown,		void		)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == MB_L)
		set_expanded(!m_expanded);
}
xui_method_explain(xui_plusctrl, on_renderself,		void		)( xui_method_args&  args )
{
	xui_button::on_renderself(args);

	xui_rect2d<s32> rt		= get_renderrtins() + get_screenpt();
	xui_colour		color   = get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (m_drawmode == PLUSRENDER_SYMBOL)
	{
		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-4,
			center.y-1,
			center.x+4,
			center.y+1), color);

		if (m_expanded == false)
			xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-1,
			center.y-4,
			center.x+1,
			center.y+4), color);
	}
	else
	{
		if (m_onlyside && was_hover())
		{
			if (m_expanded) xui_convas::get_ins()->draw_triangle(center, 3, TRIANGLE_DOWN,  color);
			else			xui_convas::get_ins()->draw_triangle(center, 3, TRIANGLE_RIGHT, color);
		}
		else
		{
			if (m_expanded) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_DOWN,  color);
			else			xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
		}
	}
}