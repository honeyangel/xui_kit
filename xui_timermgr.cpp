#include "xui_timermgr.h"

xui_implement_instance_member(xui_timermgr);
xui_implement_instance_method(xui_timermgr);

/*
//constructor
*/
xui_create_explain(xui_timermgr)( void )
{}

/*
//destructor
*/
xui_delete_explain(xui_timermgr)( void )
{
	for (std::list<xui_timer*>::iterator itor = m_timerlst.begin(); itor != m_timerlst.end(); ++itor)
		delete (*itor);
}

/*
//method
*/
xui_method_explain(xui_timermgr, add_timer,	xui_timer*	)( xui_component* owner, f32 interval, void* data )
{
	xui_timer* timer = new xui_timer(owner, interval, data);
	m_timerlst.push_back(timer);

	return timer;
}
xui_method_explain(xui_timermgr, del_timer,	void		)( xui_timer* timer )
{
	m_timerlst.remove(timer);
	delete timer;
}

/*
//update
*/
xui_method_explain(xui_timermgr, update,	void		)( f32 delta )
{
	for (std::list<xui_timer*>::iterator itor = m_timerlst.begin(); itor != m_timerlst.end(); ++itor)
	{
		(*itor)->update(delta);
	}
}