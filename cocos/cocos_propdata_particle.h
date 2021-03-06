#ifndef __cocos_propdata_particle_h__
#define __cocos_propdata_particle_h__

#include "xui_propdata.h"

class cocos_propdata_particle : public xui_propdata_object_func
{
public:
	/*
	//constructor
	*/
	cocos_propdata_particle( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		get_func			userget, 
		set_func			userset, 
		void*				userptr );

	/*
	//static
	*/
	static xui_bitmap*		get_icon		( xui_propdata* propdata );
	static std::wstring		get_name		( xui_propdata* propdata );

protected:
	/*
	//event
	*/
	void					on_doubleclick	( xui_component* sender, xui_method_args& args );
};

#endif//__cocos_propdata_particle_h__