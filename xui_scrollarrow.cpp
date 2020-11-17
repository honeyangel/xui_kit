#include "xui_canvas.h"
#include "xui_scroll.h"
#include "xui_scrollarrow.h"

xui_implement_rtti(xui_scrollarrow, xui_button)

xui_scrollarrow::xui_scrollarrow( const xui_vector<s32>& size, u08 flowstyle, s32 direction, u08 arrowdraw )
: xui_button(size)
{
	m_flowstyle	= flowstyle;
	m_arrowdraw = arrowdraw;
	m_direction = direction;
	m_deltahold = 0.0f;
}

u08 xui_scrollarrow::get_arrowdraw( void ) const
{
	return m_arrowdraw;
}

void xui_scrollarrow::set_arrowdraw( u08 arrowdraw )
{
	m_arrowdraw = arrowdraw;
}

void xui_scrollarrow::update( f32 delta )
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

void xui_scrollarrow::on_mousedown( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == k_mb_left)
	{
		xui_scroll* scroll = xui_dynamic_cast(xui_scroll, m_parent);
		scroll->get_valueaction()->clear();
		m_deltahold = 0.0f;
	}
}

void xui_scrollarrow::on_mouseclick( xui_method_mouse& args )
{
	xui_button::on_mouseclick(args);
	xui_scroll* scroll = xui_dynamic_cast(xui_scroll, m_parent);
	scroll->set_value(scroll->get_value()+m_direction*scroll->get_smallchange());
}

void xui_scrollarrow::on_renderself( xui_method_args& args )
{
	xui_button::on_renderself(args);

	xui_rect2d<s32> rt		= get_renderrtins() + get_screenpt();
	xui_colour		color   = get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (m_arrowdraw == k_arrowdraw_plusandminus)
	{
		xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-4,
			center.y-1,
			center.x+4,
			center.y+1), color);

		if (m_direction > 0)
			xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
				center.x-1,
				center.y-4,
				center.x+1,
				center.y+4), color);
	}
	else
	{
		if (m_flowstyle == k_flowstyle_v)
		{
			if (m_direction < 0) xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_up,    color);
			if (m_direction > 0) xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_down,  color);
		}
		else
		{
			if (m_direction < 0) xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_left,  color);
			if (m_direction > 0) xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_right, color);
		}
	}
}
