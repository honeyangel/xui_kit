#ifndef __xui_timermgr_h__
#define __xui_timermgr_h__

#include "xui_timer.h"

class xui_timermgr
{
	xui_declare_instance(xui_timermgr)

public:
	/*
	//constructor
	*/
	xui_timermgr( void );

	/*
	//destructor
	*/
	virtual ~xui_timermgr( void );

	/*
	//method
	*/
	xui_timer*				add_timer	( xui_component* owner, f32 interval, void* data );
	void					del_timer	( xui_timer* timer );

	/*
	//update
	*/
	virtual void			update		( f32 delta );

protected:
	/*
	//member
	*/
	std::list<xui_timer*>	m_timerlst;
};

#endif//__xui_timermgr_h__