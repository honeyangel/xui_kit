#include "xui_component.h"
#include "xui_timermgr.h"
#include "xui_timer.h"

xui_timer::xui_timer( xui_component* owner, f32 interval, void* data )
{
	m_owner		= owner;
	m_enable	= true;
	m_interval	= interval;
	m_data		= data;
	m_time		= 0.0f;
}

bool xui_timer::was_enable( void ) const
{
	return m_enable;
}

void xui_timer::set_enable( bool flag )
{
	if (m_enable != flag)
	{
		m_enable  = flag;
		reset();
	}
}

f32 xui_timer::get_interval( void ) const
{
	return m_interval;
}

void xui_timer::set_interval( f32 interval )
{
	m_interval = interval;
}

void* xui_timer::get_data( void ) const
{
	return m_data;
}

void xui_timer::set_data( void* data )
{
	m_data = data;
}

void xui_timer::update( f32 delta )
{
	if (m_enable)
	{
		m_time += delta;
		if (m_time >= m_interval)
		{
			m_time -= m_interval;

			xui_method_args  args;
			xm_tick(m_owner, args);
		}
	}
}

void xui_timer::reset( void )
{
	m_time = 0.0f;
}