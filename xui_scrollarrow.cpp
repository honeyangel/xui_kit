#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_scrollarrow.h"

xui_implement_rtti(xui_scrollarrow, xui_button);

/*
//constructor
*/
xui_create_explain(xui_scrollarrow)( const xui_vector<s32>& size, u08 flowstyle, s32 direction, u08 arrowdraw )
: xui_button(size)
{
	m_flowstyle	= flowstyle;
	m_arrowdraw = arrowdraw;
	m_direction = direction;
	m_deltahold = 0.0f;
}

/*
//method
*/
xui_method_explain(xui_scrollarrow, get_arrowdraw,	u08	)( void ) const
{
	return m_arrowdraw;
}
xui_method_explain(xui_scrollarrow, set_arrowdraw,	void)( u08 arrowdraw )
{
	m_arrowdraw = arrowdraw;
}

/*
//virtual
*/
xui_method_explain(xui_scrollarrow, update,			void)( f32 delta )
{
	xui_button::update(delta);

	if (has_catch())
	{
		m_deltahold += delta;
		if (m_deltahold > 0.5f)
		{
			xui_scroll* scroll = xui_dynamic_cast(xui_scroll, m_parent);
			scroll->set_value(scroll->get_value()+m_direction*scroll->get_smallchange());
		}
	}
}

/*
//callback
*/
xui_method_explain(xui_scrollarrow, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		m_deltahold = 0.0f;
	}
}
xui_method_explain(xui_scrollarrow, on_mouseclick,	void)( xui_method_mouse& args )
{
	xui_button::on_mouseclick(args);
	xui_scroll* scroll = xui_dynamic_cast(xui_scroll, m_parent);
	scroll->set_value(scroll->get_value()+m_direction*scroll->get_smallchange());
}
xui_method_explain(xui_scrollarrow, on_renderself,	void)( xui_method_args&  args )
{
	xui_button::on_renderself(args);

	xui_rect2d<s32> rt		= get_renderrtins() + get_screenpt();
	xui_colour		color   = get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (m_arrowdraw == ARROWDRAW_PLUSANDMINUS)
	{
		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-4,
			center.y-1,
			center.x+4,
			center.y+1), color);

		if (m_direction > 0)
			xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
				center.x-1,
				center.y-4,
				center.x+1,
				center.y+4), color);
	}
	else
	{
		if (m_flowstyle == FLOWSTYLE_V)
		{
			if (m_direction < 0) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_UP,    color);
			if (m_direction > 0) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_DOWN,  color);
		}
		else
		{
			if (m_direction < 0) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_LEFT,  color);
			if (m_direction > 0) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
		}
	}
}
