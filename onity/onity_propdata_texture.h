#ifndef __onity_propdata_texture_h__
#define __onity_propdata_texture_h__

#include "xui_propdata.h"

class onity_propdata_texture : public xui_propdata_object_func
{
public:
	/*
	//constructor
	*/
	onity_propdata_texture( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		get_func			userget, 
		set_func			userset, 
		void*				userptr );

protected:
	/*
	//static
	*/
	static xui_bitmap*		get_texicon	( xui_propdata* propdata );
	static std::wstring		get_texname	( xui_propdata* propdata );
};

#endif//__onity_propdata_texture_h__