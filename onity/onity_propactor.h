#ifndef __onity_propactor_h__
#define __onity_propactor_h__

#include "onity_prop2dsasset.h"

class onity_propactor : public onity_prop2dsasset
{
public:
	/*
	//constructor
	*/
	onity_propactor( onity_propfile* propfile, u32 id );

protected:
	/*
	//static
	*/
	static bool			was_loop		( void* userptr );
	static void			set_loop		( void* userptr, bool value );
	static f64			get_loopstart	( void* userptr );
	static void			set_loopstart	( void* userptr, f64  value );

	/*
	//member
	*/
	xui_propkind*		m_actorkind;
};

#endif//__onity_propactor_h__