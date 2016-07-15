#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_scroll.h"

xui_implement_rtti(xui_scroll, xui_control);

/*
//static
*/
xui_method_explain(xui_scroll, create,			xui_scroll*		)( u08 style )
{
	xui_vector<s32> size(default_size);
	switch (style)
	{
	case FLOWSTYLE_H:
		size.w = 100;
		break;
	case FLOWSTYLE_V:
		size.h = 100;
		break;
	}

	xui_scroll* scroll = new xui_scroll(size, style);
	xui_method_ptrcall(scroll, set_backcolor)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(scroll, set_drawcolor)(true);
	xui_method_ptrcall(scroll, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(scroll, ini_scroll	)(100, 0);

	return scroll;
}

/*
//constructor
*/
xui_create_explain(xui_scroll)( const xui_vector<s32>& size, u08 style )
: xui_control(size)
{
	m_smallchange   = 1;
	m_largechange	= 10;
	m_thumbresize	= true;
	m_style			= style;
	m_range			= 0;
	m_value			= 0;
	m_valueaction	= new xui_action_ctrl_impl<s32>(this);
	m_valueaction->set_soft(true);
	m_valueaction->xm_tick += new xui_method_member<xui_method_args, xui_scroll>(this, &xui_scroll::on_actiontick);

	//thumb
	m_thumb	= new xui_scrollthumb(xui_vector<s32>(12), style);
	xui_method_ptrcall(m_thumb, set_parent		)(this);
	xui_method_ptrcall(m_thumb, set_drawcolor	)(true);
	xui_method_ptrcall(m_thumb, set_corner		)(5);
	m_widgetvec.push_back(m_thumb);

	//arrow
	m_arrow[ARROW_INC] = new xui_scrollarrow(xui_vector<s32>(12), style,  1, ARROWDRAW_TRIANGLE);
	m_arrow[ARROW_DEC] = new xui_scrollarrow(xui_vector<s32>(12), style, -1, ARROWDRAW_TRIANGLE);
	m_arrow[ARROW_INC]->set_parent(this);
	m_arrow[ARROW_DEC]->set_parent(this);
	m_widgetvec.push_back(m_arrow[ARROW_DEC]);
	m_widgetvec.push_back(m_arrow[ARROW_INC]);
	switch(m_style)
	{
	case FLOWSTYLE_V:
		m_arrow[ARROW_DEC]->ini_component(0, 0, DOCKSTYLE_T);
		m_arrow[ARROW_INC]->ini_component(0, 0, DOCKSTYLE_B);
		break;
	case FLOWSTYLE_H:
		m_arrow[ARROW_DEC]->ini_component(0, 0, DOCKSTYLE_L);
		m_arrow[ARROW_INC]->ini_component(0, 0, DOCKSTYLE_R);
		break;
	}

	refresh();
}

/*
//destructor
*/
xui_delete_explain(xui_scroll)( void )
{
	delete m_valueaction;
}

/*
//init
*/
xui_method_explain(xui_scroll, ini_scroll,		void					)( s32 range, s32 value )
{
	m_range = range;
	m_value = value;
	m_value = xui_max(m_value, 0);
	m_value = xui_min(m_value, range);
	resize_thumb();
	update_thumb();
}

/*
//arrow&thumb
*/
xui_method_explain(xui_scroll, get_arrow,		xui_scrollarrow*		)( u08 arrow ) const
{
	return m_arrow[arrow];
}
xui_method_explain(xui_scroll, get_thumb,		xui_scrollthumb*		)( void ) const
{
	return m_thumb;
}

/*
//method
*/
xui_method_explain(xui_scroll, get_valueaction,	xui_action_ctrl*		)( void )
{
	return m_valueaction;
}
xui_method_explain(xui_scroll, get_smallchange, s32						)( void ) const
{
	return m_smallchange;
}
xui_method_explain(xui_scroll, set_smallchange, void					)( s32 value )
{
	m_smallchange = value;
}

xui_method_explain(xui_scroll, get_largechange,	s32						)( void ) const
{
	return m_largechange;
}
xui_method_explain(xui_scroll, set_largechange,	void					)( s32 value )
{
	m_largechange = value;
}

xui_method_explain(xui_scroll, was_thumbresize, bool					)( void ) const
{
	return m_thumbresize;
}
xui_method_explain(xui_scroll, set_thumbresize, void					)( bool flag )
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
xui_method_explain(xui_scroll, get_range,		s32						)( void ) const
{
	return m_range;
}
xui_method_explain(xui_scroll, set_range,		void					)( s32 range )
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
xui_method_explain(xui_scroll, get_value,		s32						)( void ) const
{
	return m_value;
}
xui_method_explain(xui_scroll, set_value,		void					)( s32 value, bool stop_action )
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

	if (stop_action)
		m_valueaction->clear();
}

/*
//rectangle
*/
xui_method_explain(xui_scroll, get_hallowrt,	xui_rect2d<s32>			)( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		rt.ax += (m_arrow[ARROW_DEC]->was_visible()) ? m_arrow[ARROW_DEC]->get_renderw() : 0;
		rt.bx -= (m_arrow[ARROW_INC]->was_visible()) ? m_arrow[ARROW_INC]->get_renderw() : 0;
		break;
	case FLOWSTYLE_V:
		rt.ay += (m_arrow[ARROW_DEC]->was_visible()) ? m_arrow[ARROW_DEC]->get_renderh() : 0;
		rt.by -= (m_arrow[ARROW_INC]->was_visible()) ? m_arrow[ARROW_INC]->get_renderh() : 0;
		break;
	}

	return rt;
}

/*
//callback
*/
xui_method_explain(xui_scroll, on_lock,			void					)( xui_method_args&   args )
{
	xui_control::on_lock(args);
	set_maskcolor(xui_colour::white);
	m_thumb->set_visible(false);
}
xui_method_explain(xui_scroll, on_free,			void					)( xui_method_args&   args )
{
	xui_control::on_free(args);
	set_maskcolor(xui_colour::white);
	m_thumb->set_visible(true);
}
xui_method_explain(xui_scroll, on_perform,		void					)( xui_method_args&   args )
{
	xui_control::on_perform(args);

	//thumb
	xui_rect2d<s32> rt = get_hallowrt();
	switch (m_style)
	{
	case FLOWSTYLE_V:
		m_thumb->on_perform_x(rt.ax);
		m_thumb->on_perform_w(rt.get_w());
		break;
	case FLOWSTYLE_H:
		m_thumb->on_perform_y(rt.ay);
		m_thumb->on_perform_h(rt.get_h());
		break;
	}

	//更新位置
	resize_thumb();
	update_thumb();
}
xui_method_explain(xui_scroll, on_mousedown,	void					)( xui_method_mouse&  args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		m_valueaction->clear();

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
}
xui_method_explain(xui_scroll, on_updateself,	void					)( xui_method_update& args )
{
	xui_control::on_updateself(args);
	m_valueaction->update(args.delta);
}

/*
//method
*/
xui_method_explain(xui_scroll, resize_thumb,	void					)( void )
{
	if (m_thumbresize)
	{
		xui_rect2d<s32> rt = get_hallowrt();
		switch (m_style)
		{
		case FLOWSTYLE_H:
			{
				s32 w = rt.get_w() - m_range;
				m_thumb->on_perform_w(xui_max(w, 12));
			}
			break;
		case FLOWSTYLE_V:
			{
				s32 h = rt.get_h() - m_range;
				m_thumb->on_perform_h(xui_max(h, 12));
			}
			break;
		}
	}
}
xui_method_explain(xui_scroll, update_thumb,	void					)( void )
{
	//比例
	f32 ratio = (m_range == 0) ? 0.0f : (f32)m_value / (f32)m_range;

	//更新位置
	xui_rect2d<s32> rt = get_hallowrt();
	switch (m_style)
	{
	case FLOWSTYLE_H:
		{
			s32 x = rt.ax + (s32)((rt.get_w() - m_thumb->get_renderw()) * ratio);
			m_thumb->on_perform_x(x);
		}
		break;
	case FLOWSTYLE_V:
		{
			s32 y = rt.ay + (s32)((rt.get_h() - m_thumb->get_renderh()) * ratio);
			m_thumb->on_perform_y(y);
		}
		break;
	}
}
xui_method_explain(xui_scroll, update_value,	void					)( void )
{
	xui_rect2d<s32> rt = get_hallowrt();

	//比例
	f32 ratio = 0.0f;
	switch (m_style)
	{
	case FLOWSTYLE_H:
		ratio = (f32)(m_thumb->get_renderx() - rt.ax) / (f32)(rt.get_w() - m_thumb->get_renderw());
		break;
	case FLOWSTYLE_V:
		ratio = (f32)(m_thumb->get_rendery() - rt.ay) / (f32)(rt.get_h() - m_thumb->get_renderh());
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

/*
//event
*/
xui_method_explain(xui_scroll, on_actiontick,	void					)( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<s32>* action = (xui_action_ctrl_impl<s32>*)m_valueaction;
	set_value(action->sample(), false);
	if (m_value == 0 || m_value == m_range)
		m_valueaction->clear();
}