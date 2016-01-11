#include "xui_timermgr.h"

xui_timermgr* g_timermgr = NULL;

/*
//constructor
*/
xui_create_explain(xui_timermgr)( void )
{

}

/*
//method
*/
xui_method_explain(xui_timermgr, add_timer,	void)( xui_timer* timer )
{
	m_timerlst.push_back(timer);
}
xui_method_explain(xui_timermgr, del_timer,	void)( xui_timer* timer )
{
	m_timerlst.remove(timer);
}

/*
//update
*/
xui_method_explain(xui_timermgr, update,		void)( f32 delta )
{
	for (std::list<xui_timer*>::iterator itor = m_timerlst.begin(); itor != m_timerlst.end(); ++itor)
	{
		(*itor)->update(delta);
	}
}