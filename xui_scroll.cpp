#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_scroll.h"

/*
//constructor
*/
xui_create_explain(xui_scroll)( const std::string& name, const xui_rect2d<s32>& rect, u08 style )
: xui_control(name, rect)
{
	m_type		   += "scroll";
	m_smallchange   = 1;
	m_largechange	= 100;
	m_thumbresize	= true;
	m_style			= style;
	m_range			= 0;
	m_value			= 0;

	m_thumb	= new xui_scrollthumb("", xui_rect2d<s32>(0, 0, 20, 20), style, this);
	m_thumb->set_corner(5);
	m_widgetvec.push_back(m_thumb);

	xui_rect2d<s32> borderrt;
	switch (m_style)
	{
	case FLOWSTYLE_H: borderrt = xui_rect2d<s32>(4, 0, 4, 0); break;
	case FLOWSTYLE_V: borderrt = xui_rect2d<s32>(0, 2, 0, 2); break;
	}
	for (u08 i = 0; i < 2; ++i)
	{
		m_arrow[i] = new xui_scrollarrow("", xui_rect2d<s32>(0, 0, 16, 16), style, (i == 0) ? -1 : 1, this);
		m_arrow[i]->set_borderrt(borderrt);

		m_widgetvec.push_back(m_arrow[i]);
	}

	perform();
}

/*
//init
*/
xui_method_explain(xui_scroll, ini_scroll,		void			)( s32 range, s32 value )
{
	m_range = range;
	m_value = xui_max(value, 0);
	m_value = xui_min(value, range);
	resize_thumb();
	update_thumb();
}

/*
//arrow&thumb
*/
xui_method_explain(xui_scroll, get_startarrow,	xui_scrollarrow*)( void ) const
{
	return m_arrow[0];
}
xui_method_explain(xui_scroll, get_finalarrow,	xui_scrollarrow*)( void ) const
{
	return m_arrow[1];
}
xui_method_explain(xui_scroll, get_thumb,		xui_scrollthumb*)( void ) const
{
	return m_thumb;
}

/*
//method
*/
xui_method_explain(xui_scroll, get_smallchange, s32				)( void ) const
{
	return m_smallchange;
}
xui_method_explain(xui_scroll, set_smallchange, void			)( s32 value )
{
	m_smallchange = value;
}

xui_method_explain(xui_scroll, get_largechange,	s32				)( void ) const
{
	return m_largechange;
}
xui_method_explain(xui_scroll, set_largechange,	void			)( s32 value )
{
	m_largechange = value;
}

xui_method_explain(xui_scroll, was_thumbresize, bool			)( void ) const
{
	return m_thumbresize;
}
xui_method_explain(xui_scroll, set_thumbresize, void			)( bool flag )
{
	if (m_thumbresize != flag)
	{
		m_thumbresize  = flag;
		if (m_thumbresize)
		{
			resize_thumb();
			update_thumb();
		}
	}
}

/*
//range
*/
xui_method_explain(xui_scroll, get_range,		s32				)( void ) const
{
	return m_range;
}
xui_method_explain(xui_scroll, set_range,		void			)( s32 range )
{
	if (m_range != range)
	{
		m_range  = range;
		resize_thumb();
		update_thumb();

		if (m_value > m_range)
			set_value(m_range);
	}
}

/*
//value
*/
xui_method_explain(xui_scroll, get_value,		s32				)( void ) const
{
	return m_value;
}
xui_method_explain(xui_scroll, set_value,		void			)( s32 value )
{
	value = xui_min(value, m_range);
	value = xui_max(value, 0);
	if (m_value != value)
	{
		m_value  = value;
		update_thumb();

		xui_method_args args;
		xm_scroll(this, args);
	}
}

/*
//rectangle
*/
xui_method_explain(xui_scroll, get_hallowrt,	xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		rt.ax += (m_arrow[0] != NULL) ? m_arrow[0]->get_renderw() : 0;
		rt.bx -= (m_arrow[1] != NULL) ? m_arrow[1]->get_renderw() : 0;
		break;
	case FLOWSTYLE_V:
		rt.ay += (m_arrow[0] != NULL) ? m_arrow[0]->get_renderh() : 0;
		rt.by -= (m_arrow[1] != NULL) ? m_arrow[1]->get_renderh() : 0;
		break;
	}

	return rt;
}

/*
//callback
*/
xui_method_explain(xui_scroll, on_lock,			void			)( xui_method_args&  args )
{
	xui_control::on_lock(args);
	m_thumb->set_visible(false);
}
xui_method_explain(xui_scroll, on_free,			void			)( xui_method_args&  args )
{
	xui_control::on_free(args);
	m_thumb->set_visible(true);
}
xui_method_explain(xui_scroll, on_perform,		void			)( xui_method_args& args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;

	//arrow
	if (m_arrow[0])
	{
		pt = m_arrow[0]->get_renderpt();
		sz = m_arrow[0]->get_rendersz();
		switch (m_style)
		{
		case FLOWSTYLE_H:
			pt.x = rt.ax;
			pt.y = rt.ay;
			sz.h = rt.get_sz().h;
			break;
		case FLOWSTYLE_V:
			pt.x = rt.ax;
			pt.y = rt.ay;
			sz.w = rt.get_sz().w;
			break;
		}
		m_arrow[0]->set_renderpt(pt, false);
		m_arrow[0]->set_rendersz(sz, false);
	}

	if (m_arrow[1])
	{
		pt = m_arrow[1]->get_renderpt();
		sz = m_arrow[1]->get_rendersz();
		switch (m_style)
		{
		case FLOWSTYLE_H:
			pt.x = rt.bx - sz.w;
			pt.y = rt.ay;
			sz.h = rt.get_sz().h;
			break;
		case FLOWSTYLE_V:
			pt.x = rt.ax;
			pt.y = rt.by - sz.h;
			sz.w = rt.get_sz().w;
			break;
		}
		m_arrow[1]->set_renderpt(pt, false);
		m_arrow[1]->set_rendersz(sz, false);
	}

	//thumb
	rt = get_hallowrt();
	pt = m_thumb->get_renderpt();
	sz = m_thumb->get_rendersz();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		pt.y = rt.ay;
		sz.h = rt.get_sz().h;
		break;
	case FLOWSTYLE_V:
		pt.x = rt.ax;
		sz.w = rt.get_sz().w;
		break;
	}
	m_thumb->set_renderpt(pt, false);
	m_thumb->set_rendersz(sz, false);

	//更新位置
	resize_thumb();
	update_thumb();
}
xui_method_explain(xui_scroll, on_mousedown,	void			)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);

	//点击其它位置
	xui_rect2d<s32> rt = m_thumb->get_renderrtabs();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		if (args.point.x < rt.ax)
			set_value(m_value-m_largechange);
		if (args.point.x > rt.bx)
			set_value(m_value+m_largechange);
		break;
	case FLOWSTYLE_V:
		if (args.point.y < rt.ay)
			set_value(m_value-m_largechange);
		if (args.point.y > rt.by)
			set_value(m_value+m_largechange);
		break;
	}
}

/*
//method
*/
xui_method_explain(xui_scroll, resize_thumb,	void			)( void )
{
	if (m_thumbresize)
	{
		xui_rect2d<s32> rt = get_hallowrt();
		switch (m_style)
		{
		case FLOWSTYLE_H:
			{
				s32 w = rt.get_sz().w - m_range;
				m_thumb->set_rendersz(xui_vector<s32>(xui_max(w, 20), m_thumb->get_renderh()), false);
			}
			break;
		case FLOWSTYLE_V:
			{
				s32 h = rt.get_sz().h - m_range;
				m_thumb->set_rendersz(xui_vector<s32>(m_thumb->get_renderw(), xui_max(h, 20)), false);
			}
			break;
		}
	}
}
xui_method_explain(xui_scroll, update_thumb,	void			)( void )
{
	//比例
	f32 ratio = (m_range == 0) ? 0.0f : (f32)m_value / (f32)m_range;

	//更新位置
	xui_rect2d<s32> rt = get_hallowrt();
	xui_vector<s32> pt = m_thumb->get_renderpt();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		pt.x = rt.ax + (s32)((rt.get_sz().w - m_thumb->get_renderw()) * ratio);
		break;
	case FLOWSTYLE_V:
		pt.y = rt.ay + (s32)((rt.get_sz().h - m_thumb->get_renderh()) * ratio);
		break;
	}

	m_thumb->set_renderpt(pt, false);
}
xui_method_explain(xui_scroll, update_value,	void			)( void )
{
	xui_rect2d<s32> rt = get_hallowrt();

	//比例
	f32 ratio = 0.0f;
	switch (m_style)
	{
	case FLOWSTYLE_H:
		ratio = (f32)(m_thumb->get_renderx() - rt.ax) / (f32)(rt.get_sz().w - m_thumb->get_renderw());
		break;
	case FLOWSTYLE_V:
		ratio = (f32)(m_thumb->get_rendery() - rt.ay) / (f32)(rt.get_sz().h - m_thumb->get_renderh());
		break;
	}

	//更新滚动位置
	s32 value = (s32)(m_range * ratio);
	if (m_value != value)
	{
		m_value  = value;

		xui_method_args args;
		xm_scroll(this, args);
	}
}