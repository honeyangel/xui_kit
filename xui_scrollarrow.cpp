#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_scrollarrow.h"

/*
//constructor
*/
xui_create_explain(xui_scrollarrow)( const std::string& name, const xui_rect2d<s32>& rect, u08 style, s32 direction, xui_component* parent )
: xui_button(name, rect)
{
	m_style		= style;
	m_direction = direction;
	m_deltahold = 0.0f;
	m_arrowdraw = true;
	m_parent	= parent;
}

/*
//method
*/
xui_method_explain(xui_scrollarrow, was_arrowdraw,	bool)( void ) const
{
	return m_arrowdraw;
}
xui_method_explain(xui_scrollarrow, set_arrowdraw,	void)( bool flag )
{
	m_arrowdraw = flag;
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
			xui_scroll* scroll = (xui_scroll*)m_parent;
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
	xui_scroll* scroll = (xui_scroll*)m_parent;
	scroll->set_value(scroll->get_value()+m_direction*scroll->get_smallchange());
}

xui_method_explain(xui_scrollarrow, on_renderback,	void)( xui_method_args& args )
{
	//xui_button::on_renderback(args);
	if (m_arrowdraw == false)
		return;

	xui_vector<s32> pt[3];
	xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		{
			if (m_direction < 0)
			{
				pt[0] = xui_vector<s32>(rt.ax, rt.ay+rt.get_sz().h/2);
				pt[1] = xui_vector<s32>(rt.bx, rt.ay);
				pt[2] = xui_vector<s32>(rt.bx, rt.by);
			}
			else
			{
				pt[0] = xui_vector<s32>(rt.bx,  rt.ay+rt.get_sz().h/2);
				pt[1] = xui_vector<s32>(rt.ax,  rt.ay);
				pt[2] = xui_vector<s32>(rt.ax,  rt.by);
			}
		}
		break;
	case FLOWSTYLE_V:
		{
			if (m_direction < 0)
			{
				pt[0] = xui_vector<s32>(rt.ax+rt.get_sz().w/2,	rt.ay);
				pt[1] = xui_vector<s32>(rt.ax,					rt.by);
				pt[2] = xui_vector<s32>(rt.bx,					rt.by);
			}
			else
			{
				pt[0] = xui_vector<s32>(rt.ax+rt.get_sz().w/2,	rt.by);
				pt[1] = xui_vector<s32>(rt.ax,					rt.ay);
				pt[2] = xui_vector<s32>(rt.bx,					rt.ay);
			}
		}
		break;
	}

	g_convas->fill_poly(pt, 3, get_vertexcolor()*get_rendercolor());
}
