#ifndef __onity_propnodemodule_h__
#define __onity_propnodemodule_h__

#include "onity_propnode2dsref.h"

class onity_propnodemodule : public onity_propnode2dsref
{
public:
	/*
	//constructor
	*/
	onity_propnodemodule( NP2DSTransRef* transref );

protected:
	/*
	//static
	*/
	static void*			get_image		( void* userptr );
	static void				set_image		( void* userptr, void* value );

	/*
	//member
	*/
	xui_propkind*			m_reskind;
};

#endif//__onity_propnodemodule_h__