#include "xui_convas.h"
#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_slider.h"

xui_implement_rtti(xui_slider, xui_scroll);

/*
//constructor
*/
xui_create_explain(xui_slider)( const xui_vector<s32>& size, xui_component* parent, u08 style, u08 arrowdraw )
: xui_scroll(size, parent, style)
{
	m_thumbresize = false;
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

	m_thumb->set_corner(10);
	m_thumb->on_perform_sz(xui_vector<s32>(20, 20));

	refresh();
}

/*
//callback
*/
xui_method_explain(xui_slider, on_mousedown,	void)( xui_method_mouse& args )
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
xui_method_explain(xui_slider, on_renderback,	void)( xui_method_args&  args )
{
	xui_scroll::on_renderback(args);

	//xui_rect2d<s32> rt = get_hallowrt() + get_screenpt();
	//xui_vector<s32> pt[3];
	//switch (m_style)
	//{
	//case FLOWSTYLE_H:
	//	pt[0] = xui_vector<s32>(rt.ax, rt.by);
	//	pt[1] = xui_vector<s32>(rt.bx, rt.ay);
	//	pt[2] = xui_vector<s32>(rt.bx, rt.by);
	//	break;
	//case FLOWSTYLE_V:
	//	pt[0] = xui_vector<s32>(rt.ax, rt.ay);
	//	pt[1] = xui_vector<s32>(rt.ax, rt.by);
	//	pt[2] = xui_vector<s32>(rt.bx, rt.by);
	//	break;
	//}

	//xui_colour color = m_backcolor;
	//color.r = xui_max(0.0f, color.r - 0.1f); 
	//color.g = xui_max(0.0f, color.g - 0.1f);
	//color.b = xui_max(0.0f, color.b - 0.1f);
	//g_convas->fill_poly(pt, 3, get_vertexcolor()*color);
}
