#include "xui_scrollarrow.h"
#include "xui_scrollthumb.h"
#include "xui_scroll.h"

xui_implement_rtti(xui_scroll, xui_control)

xui_scroll* xui_scroll::create( u08 style )
{
	xui_vector<s32> size(k_default_size);
	switch (style)
	{
	case k_flowstyle_h:
		size.w = 100;
		break;
	case k_flowstyle_v:
		size.h = 100;
		break;
	}

	xui_scroll* scroll = new xui_scroll(size, style);
	scroll->set_backcolor(xui_colour(1.0f, 0.2f));
	scroll->set_drawcolor(true);
	scroll->set_borderrt(xui_rect2d<s32>(2));
	scroll->ini_scroll(100, 0);

	return scroll;
}

xui_scroll::xui_scroll( const xui_vector<s32>& size, u08 style )
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
	m_thumb->set_parent(this);
	m_thumb->set_drawcolor(true);
	m_thumb->set_corner(5);
	m_widgetvec.push_back(m_thumb);

	//arrow
	m_arrow[k_arrow_inc] = new xui_scrollarrow(xui_vector<s32>(12), style,  1, k_arrowdraw_triangle);
	m_arrow[k_arrow_dec] = new xui_scrollarrow(xui_vector<s32>(12), style, -1, k_arrowdraw_triangle);
	m_arrow[k_arrow_inc]->set_parent(this);
	m_arrow[k_arrow_dec]->set_parent(this);
	m_widgetvec.push_back(m_arrow[k_arrow_dec]);
	m_widgetvec.push_back(m_arrow[k_arrow_inc]);
	switch(m_style)
	{
	case k_flowstyle_v:
		m_arrow[k_arrow_dec]->ini_component(0, 0, k_dockstyle_t);
		m_arrow[k_arrow_inc]->ini_component(0, 0, k_dockstyle_b);
		break;
	case k_flowstyle_h:
		m_arrow[k_arrow_dec]->ini_component(0, 0, k_dockstyle_l);
		m_arrow[k_arrow_inc]->ini_component(0, 0, k_dockstyle_r);
		break;
	}

	refresh();
}

xui_scroll::~xui_scroll( void )
{
	delete m_valueaction;
}

void xui_scroll::ini_scroll( s32 range, s32 value )
{
	m_range = range;
	m_value = value;
	m_value = xui_max(m_value, 0);
	m_value = xui_min(m_value, range);
	resize_thumb();
	update_thumb();
}

xui_scrollarrow* xui_scroll::get_arrow( u08 arrow ) const
{
	return m_arrow[arrow];
}

xui_scrollthumb* xui_scroll::get_thumb( void ) const
{
	return m_thumb;
}

xui_action_ctrl* xui_scroll::get_valueaction( void )
{
	return m_valueaction;
}

s32 xui_scroll::get_smallchange( void ) const
{
	return m_smallchange;
}

void xui_scroll::set_smallchange( s32 value )
{
	m_smallchange = value;
}

s32 xui_scroll::get_largechange( void ) const
{
	return m_largechange;
}

void xui_scroll::set_largechange( s32 value )
{
	m_largechange = value;
}

bool xui_scroll::was_thumbresize( void ) const
{
	return m_thumbresize;
}

void xui_scroll::set_thumbresize( bool flag )
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

s32 xui_scroll::get_range( void ) const
{
	return m_range;
}

void xui_scroll::set_range( s32 range )
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

s32 xui_scroll::get_value( void ) const
{
	return m_value;
}

void xui_scroll::set_value( s32 value, bool stop_action )
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

xui_rect2d<s32> xui_scroll::get_hallowrt( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	switch (m_style)
	{
	case k_flowstyle_h:
		rt.ax += (m_arrow[k_arrow_dec]->was_visible()) ? m_arrow[k_arrow_dec]->get_renderw() : 0;
		rt.bx -= (m_arrow[k_arrow_inc]->was_visible()) ? m_arrow[k_arrow_inc]->get_renderw() : 0;
		break;
	case k_flowstyle_v:
		rt.ay += (m_arrow[k_arrow_dec]->was_visible()) ? m_arrow[k_arrow_dec]->get_renderh() : 0;
		rt.by -= (m_arrow[k_arrow_inc]->was_visible()) ? m_arrow[k_arrow_inc]->get_renderh() : 0;
		break;
	}

	return rt;
}

void xui_scroll::on_lock( xui_method_args& args )
{
	xui_control::on_lock(args);
	set_maskcolor(xui_colour::k_white);
	m_thumb->set_visible(false);
}

void xui_scroll::on_free( xui_method_args& args )
{
	xui_control::on_free(args);
	set_maskcolor(xui_colour::k_white);
	m_thumb->set_visible(true);
}

void xui_scroll::on_perform( xui_method_args& args )
{
	xui_control::on_perform(args);

	//thumb
	xui_rect2d<s32> rt = get_hallowrt();
	switch (m_style)
	{
	case k_flowstyle_v:
		m_thumb->on_perform_x(rt.ax);
		m_thumb->on_perform_w(rt.get_w());
		break;
	case k_flowstyle_h:
		m_thumb->on_perform_y(rt.ay);
		m_thumb->on_perform_h(rt.get_h());
		break;
	}

	resize_thumb();
	update_thumb();
}

void xui_scroll::on_mousedown( xui_method_mouse&  args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == k_mb_left)
	{
		m_valueaction->clear();

		xui_rect2d<s32> rt = m_thumb->get_renderrtabs();
		switch (m_style)
		{
		case k_flowstyle_h:
			if (args.point.x < rt.ax)
				set_value(m_value-m_largechange);
			if (args.point.x > rt.bx)
				set_value(m_value+m_largechange);
			break;
		case k_flowstyle_v:
			if (args.point.y < rt.ay)
				set_value(m_value-m_largechange);
			if (args.point.y > rt.by)
				set_value(m_value+m_largechange);
			break;
		}
	}
}

void xui_scroll::on_updateself( xui_method_update& args )
{
	xui_control::on_updateself(args);
	m_valueaction->update(args.delta);
}

void xui_scroll::resize_thumb( void )
{
	if (m_thumbresize)
	{
		xui_rect2d<s32> rt = get_hallowrt();
		switch (m_style)
		{
		case k_flowstyle_h:
			{
				s32 w = rt.get_w() - m_range;
				m_thumb->on_perform_w(xui_max(w, 12));
			}
			break;
		case k_flowstyle_v:
			{
				s32 h = rt.get_h() - m_range;
				m_thumb->on_perform_h(xui_max(h, 12));
			}
			break;
		}
	}
}

void xui_scroll::update_thumb( void )
{
	f32 ratio = (m_range == 0) ? 0.0f : (f32)m_value / (f32)m_range;

	xui_rect2d<s32> rt = get_hallowrt();
	switch (m_style)
	{
	case k_flowstyle_h:
		{
			s32 x = rt.ax + (s32)((rt.get_w() - m_thumb->get_renderw()) * ratio);
			m_thumb->on_perform_x(x);
		}
		break;
	case k_flowstyle_v:
		{
			s32 y = rt.ay + (s32)((rt.get_h() - m_thumb->get_renderh()) * ratio);
			m_thumb->on_perform_y(y);
		}
		break;
	}
}

void xui_scroll::update_value( void )
{
	xui_rect2d<s32> rt = get_hallowrt();

	f32 ratio = 0.0f;
	switch (m_style)
	{
	case k_flowstyle_h:
		ratio = (f32)(m_thumb->get_renderx() - rt.ax) / (f32)(rt.get_w() - m_thumb->get_renderw());
		break;
	case k_flowstyle_v:
		ratio = (f32)(m_thumb->get_rendery() - rt.ay) / (f32)(rt.get_h() - m_thumb->get_renderh());
		break;
	}

	s32 value = (s32)(m_range * ratio);
	if (m_value != value)
	{
		m_value  = value;

		xui_method_args args;
		xm_scroll(this, args);
	}
}

void xui_scroll::on_actiontick( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<s32>* action = (xui_action_ctrl_impl<s32>*)m_valueaction;
	set_value(action->sample(), false);
	if (m_value == 0 || m_value == m_range)
		m_valueaction->clear();
}