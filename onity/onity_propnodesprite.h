#ifndef __onity_propnodesprite_h__
#define __onity_propnodesprite_h__

#include "onity_propnode2dsref.h"

class onity_propnodesprite : public onity_propnode2dsref
{
public:
	/*
	//constructor
	*/
	onity_propnodesprite( NP2DSTransRef* transref );

protected:
	/*
	//static
	*/
	static void*			get_frame		( void* userptr );
	static void				set_frame		( void* userptr, void* value );

	/*
	//member
	*/
	xui_propkind*			m_reskind;
};

#endif//__onity_propnodesprite_h__

