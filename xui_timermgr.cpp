#include "xui_timermgr.h"

xui_implement_instance_member(xui_timermgr)
xui_implement_instance_method(xui_timermgr)

xui_timermgr::xui_timermgr( void )
{}

xui_timermgr::~xui_timermgr( void )
{
	for (std::list<xui_timer*>::iterator itor = m_timerlst.begin(); itor != m_timerlst.end(); ++itor)
		delete (*itor);
}

xui_timer* xui_timermgr::add_timer( xui_component* owner, f32 interval, void* data )
{
	xui_timer* timer = new xui_timer(owner, interval, data);
	m_timerlst.push_back(timer);

	return timer;
}

void xui_timermgr::del_timer( xui_timer* timer )
{
	m_timerlst.remove(timer);
	delete timer;
}

void xui_timermgr::update( f32 delta )
{
	for (std::list<xui_timer*>::iterator itor = m_timerlst.begin(); itor != m_timerlst.end(); ++itor)
	{
		(*itor)->update(delta);
	}
}