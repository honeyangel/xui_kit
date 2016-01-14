#include "xui_convas.h"
#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_slider.h"

/*
//constructor
*/
xui_create_explain(xui_slider)( const std::string& name, const xui_rect2d<s32>& rect, u08 style, bool anglerender, xui_bitmap** arrowbitmap )
: xui_scroll(name, rect, style)
{
	m_type       += "slider";
	m_thumbresize = false;
	m_anglerender = anglerender;

	for (u08 i = 0; i < 2; ++i)
	{
		if (arrowbitmap)
		{
			m_arrow[i]->set_arrowdraw(false);
			m_arrow[i]->set_icon(arrowbitmap[i]);
		}
		else
		{
			std::vector<xui_component*>::iterator itor = std::find(
				m_widgetvec.begin(),
				m_widgetvec.end(),
				m_arrow[i]);

			if (itor != m_widgetvec.end())
			{
				m_widgetvec.erase(itor);

				delete m_arrow[i];
				m_arrow[i] = NULL;
			}
		}
	}

	m_thumb->set_rendersz(xui_vector<s32>(20, 20));
	m_thumb->set_corner(10);
	perform();
}

/*
//callback
*/
xui_method_explain(xui_slider, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);

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

			m_thumb->set_renderpt(xui_vector<s32>(x, m_thumb->get_rendery()), false);
		}
		break;
	case FLOWSTYLE_V:
		{
			s32 y = args.point.y - get_screenpt().y - m_thumb->get_renderh()/2;
			if (y < rt.ay)
				y = rt.ay;
			if (y > rt.by-m_thumb->get_renderh())
				y = rt.by-m_thumb->get_renderh();

			m_thumb->set_renderpt(xui_vector<s32>(m_thumb->get_renderx(), y), false);
		}
		break;
	}

	update_value();
	m_thumb->req_catch();
}
xui_method_explain(xui_slider, on_renderback, void)( xui_method_args& args )
{
	xui_scroll::on_renderback(args);
	if (m_anglerender == false)
		return;

	xui_rect2d<s32> rt = get_hallowrt() + get_screenpt();
	xui_vector<s32> pt[3];
	switch (m_style)
	{
	case FLOWSTYLE_H:
		pt[0] = xui_vector<s32>(rt.ax, rt.by);
		pt[1] = xui_vector<s32>(rt.bx, rt.ay);
		pt[2] = xui_vector<s32>(rt.bx, rt.by);
		break;
	case FLOWSTYLE_V:
		pt[0] = xui_vector<s32>(rt.ax, rt.ay);
		pt[1] = xui_vector<s32>(rt.ax, rt.by);
		pt[2] = xui_vector<s32>(rt.bx, rt.by);
		break;
	}

	xui_colour color = m_backcolor;
	color.r = xui_max(0.0f, color.r - 0.1f); 
	color.g = xui_max(0.0f, color.g - 0.1f);
	color.b = xui_max(0.0f, color.b - 0.1f);
	g_convas->fill_poly(pt, 3, get_vertexcolor()*color);
}
