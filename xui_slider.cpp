#include "xui_convas.h"
#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_slider.h"

xui_implement_rtti(xui_slider, xui_scroll);

/*
//static
*/
xui_method_explain(xui_slider, create,			xui_slider*	)( u08 style, u08 arrowdraw )
{
	xui_vector<s32> size(16);
	switch (style)
	{
	case FLOWSTYLE_H:
		size.w = 100;
		break;
	case FLOWSTYLE_V:
		size.h = 100;
		break;
	}

	xui_slider* scroll = new xui_slider(size, style, arrowdraw);
	xui_method_ptrcall(scroll, set_backcolor)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(scroll, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(scroll, ini_scroll	)(100, 0);

	return scroll;
}

/*
//constructor
*/
xui_create_explain(xui_slider)( const xui_vector<s32>& size, u08 style, u08 arrowdraw )
: xui_scroll(size, style)
{
	if (arrowdraw == ARROWDRAW_NONE)
	{
		m_arrow[ARROW_DEC]->set_visible(false);
		m_arrow[ARROW_INC]->set_visible(false);
	}
	else
	{
		m_arrow[ARROW_DEC]->set_arrowdraw(arrowdraw);
		m_arrow[ARROW_INC]->set_arrowdraw(arrowdraw);
	}

	m_thumbresize = false;
	m_thumb->on_perform_sz(xui_vector<s32>(12));
	m_thumb->set_corner(6);

	refresh();
}

/*
//callback
*/
xui_method_explain(xui_slider, on_mousedown,	void		)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		xui_rect2d<s32> rt = get_hallowrt();
		switch (m_style)
		{
		case FLOWSTYLE_H:
			{
				s32 x = args.point.x - get_screenpt().x - m_thumb->get_renderw()/2;
				if (x < rt.ax)
					x = rt.ax;
				if (x > rt.bx-m_thumb->get_renderw())
					x = rt.bx-m_thumb->get_renderw();

				m_thumb->on_perform_x(x);
			}
			break;
		case FLOWSTYLE_V:
			{
				s32 y = args.point.y - get_screenpt().y - m_thumb->get_renderh()/2;
				if (y < rt.ay)
					y = rt.ay;
				if (y > rt.by-m_thumb->get_renderh())
					y = rt.by-m_thumb->get_renderh();

				m_thumb->on_perform_y(y);
			}
			break;
		}

		update_value();
		m_thumb->req_catch();
	}
}
xui_method_explain(xui_slider, on_renderback,	void		)( xui_method_args&  args )
{
	xui_scroll::on_renderback(args);

	xui_colour		color = get_rendercolor() * get_vertexcolor();
	xui_rect2d<s32> rt    = get_hallowrt() + get_screenpt();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		rt.oft_y(rt.get_h()/4);
		rt.set_h(rt.get_h()/2);
		xui_convas::get_ins()->fill_round(rt, color, rt.get_h()/2);
		break;
	case FLOWSTYLE_V:
		rt.oft_x(rt.get_w()/4);
		rt.set_w(rt.get_w()/2);
		xui_convas::get_ins()->fill_round(rt, color, rt.get_w()/2);
		break;
	}
}
