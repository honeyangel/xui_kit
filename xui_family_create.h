#ifndef __xui_family_create_h__
#define __xui_family_create_h__

#include "xui_vector.h"
#include "xui_family.h"

class xui_family_create
{
public:
	/*
	//virtual
	*/
	virtual void			add_font	( const xui_family& family )			= 0;
	virtual xui_vector<s32>	get_size	( const xui_family& family )			= 0;
	virtual void*			get_bits	( const xui_family& family, u16 wc )	= 0;
};

extern xui_family_create* g_family_create;

#endif//__xui_family_create_h__