#ifndef __xui_timer_h__
#define __xui_timer_h__

#include "xui_method.h"

class xui_timer
{
	friend class xui_timermgr;

public:
	/*
	//property
	*/
	bool								was_enable	( void ) const;
	void								set_enable	( bool flag );
	f32									get_interval( void ) const;
	void								set_interval( f32 interval );
	void*								get_data	( void ) const;
	void								set_data	( void* data );

	/*
	//method
	*/
	void								update		( f32 delta );
	void								reset		( void );
	
	/*
	//method
	*/
	xui_method<xui_method_args>			xm_tick;

protected:
	/*
	//constructor
	*/
	xui_timer( xui_component* owner, f32 interval, void* data );

	/*
	//member
	*/
	xui_component*						m_owner;
	bool								m_enable;
	f32									m_interval;
	void*								m_data;
	f32									m_time;
};

#endif//__xui_timer_h__