#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_desktop.h"
#include "xui_toggle.h"

xui_implement_rtti(xui_toggle, xui_button);

/*
//constructor
*/
xui_create_explain(xui_toggle)( const xui_vector<s32>& size, xui_component* parent, u08 drawstyle )
: xui_button(size, parent)
{
	m_push		= false;
	m_drawstyle = drawstyle;
	set_drawcolor(m_drawstyle != TOGGLE_CIRCLE);
}

/*
//init
*/
xui_method_explain(xui_toggle, ini_toggle,			void			)( bool push )
{
	m_push = push;
}

/*
//check
*/
xui_method_explain(xui_toggle, was_push,			bool			)( void ) const
{
	return m_push;
}
xui_method_explain(xui_toggle, set_push,			void			)( bool push )
{
	if (m_push != push)
	{
		m_push  = push;

		xui_method_args args;
		xm_click(this,  args);
	}
}

/*
//callback
*/
xui_method_explain(xui_toggle, on_mousedown,		void			)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == MB_L)
		set_push(!m_push);
}
xui_method_explain(xui_toggle, on_renderself,		void			)( xui_method_args&  args )
{
	switch (m_drawstyle)
	{
	case TOGGLE_CIRCLE:
		{
			xui_colour      color  = get_vertexcolor();
			xui_rect2d<s32> rt     = get_renderrtabs();
			xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
			xui_convas::get_ins()->fill_circle(center, 4, color*get_rendercolor(),	0, 360);
			xui_convas::get_ins()->draw_circle(center, 4, color*m_sidecolor,		0, 360);
		}
		break;
	case TOGGLE_NORMAL:
		{
			xui_colour      color  = get_vertexcolor();
			xui_rect2d<s32> rt     = get_renderrtabs();
			xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

			xui_colour final_color = color*get_rendercolor();
			xui_vector<s32> p1;
			xui_vector<s32> p2;
			p1 = xui_vector<s32>(center.x-6, center.y  );
			p2 = xui_vector<s32>(center.x,   center.y+6);
			xui_convas::get_ins()->draw_line(p1,						 p2,						 final_color);
			xui_convas::get_ins()->draw_line(p1+xui_vector<s32>(-1,  1), p2+xui_vector<s32>(-1,  1), final_color);
			xui_convas::get_ins()->draw_line(p1+xui_vector<s32>( 1, -1), p2+xui_vector<s32>( 1, -1), final_color);
			p1 = xui_vector<s32>(center.x,   center.y+6);
			p2 = xui_vector<s32>(center.x+6, center.y-6);
			xui_convas::get_ins()->draw_line(p1,						 p2,						 final_color);
			xui_convas::get_ins()->draw_line(p1+xui_vector<s32>(-1, -1), p2+xui_vector<s32>(-1, -1), final_color);
			xui_convas::get_ins()->draw_line(p1+xui_vector<s32>( 1,  1), p2+xui_vector<s32>( 1,  1), final_color);
		}
		break;
	case TOGGLE_BUTTON:
		{
			xui_button::on_renderself(args);
		}
		break;
	}
}

/*
//virtual
*/
xui_method_explain(xui_toggle, get_rendercolor,		xui_colour		)( void ) const
{
	if (m_push)
		return m_downcolor;

	return xui_button::get_rendercolor();
}
xui_method_explain(xui_toggle, get_rendericonpt,	xui_vector<s32>	)( void ) const
{
	xui_vector<s32> pt = xui_drawer::get_rendericonpt();
	if (m_push)
		pt.y += 1;

	return pt;
}