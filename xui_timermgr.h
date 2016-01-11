#ifndef __xui_timermgr_h__
#define __xui_timermgr_h__

#include "xui_timer.h"

class xui_timermgr
{
public:
	/*
	//constructor
	*/
	xui_timermgr( void );

	/*
	//method
	*/
	void					add_timer	( xui_timer* timer );
	void					del_timer	( xui_timer* timer );

	/*
	//update
	*/
	void					update		( f32 delta );

protected:
	/*
	//member
	*/
	std::list<xui_timer*>	m_timerlst;
};

extern xui_timermgr* g_timermgr;

#endif//__xui_timermgr_h__