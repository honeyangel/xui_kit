#ifndef __xui_timer_h__
#define __xui_timer_h__

#include "xui_method.h"

class xui_timer
{
public:
	/*
	//constructor
	*/
	xui_timer( xui_componet* owner, f32 interval );

	/*
	//destructor
	*/
	virtual ~xui_timer( void );

	/*
	//method
	*/
	bool								was_enable	( void ) const;
	void								set_enable	( bool flag );
	f32									get_interval( void ) const;
	void								set_interval( f32 interval );

	/*
	//virtual
	*/
	virtual void						update		( f32 delta );
	virtual void						reset		( void );
	
	/*
	//method
	*/
	xui_method<xui_method_args>			xm_tick;

protected:
	/*
	//member
	*/
	xui_componet*						m_owner;
	bool								m_enable;
	f32									m_interval;
	f32									m_time;
};

#endif//__xui_timer_h__