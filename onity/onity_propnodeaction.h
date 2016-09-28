#ifndef __onity_propnodeaction_h__
#define __onity_propnodeaction_h__

#include "onity_propnode2dsref.h"

class onity_propnodeaction : public onity_propnode2dsref
{
public:
	/*
	//constructor
	*/
	onity_propnodeaction( NP2DSTransRef* transref );

protected:
	/*
	//static
	*/
	static bool					was_play				( void* userptr );
	static void					set_play				( void* userptr, bool  value );
	static f64					get_time				( void* userptr );
	static void					set_time				( void* userptr, f64   value );
	static void*				get_actor				( void* userptr );
	static void					set_actor				( void* userptr, void* value );
	static void*				get_statectrl			( void* userptr );
	static void					set_statectrl			( void* userptr, void* value );

	/*
	//member
	*/
	xui_propkind*				m_reskind;
};

#endif//__onity_propnodeaction_h__