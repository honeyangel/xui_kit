#ifndef __onity_entitystardata_h__
#define __onity_entitystardata_h__

#include "onity_treedata.h"

class onity_entitystardata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_entitystardata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_entitystardata( void );

	/*
	//virtual
	*/
	virtual std::wstring		get_text		( u32 index );
	virtual xui_family_render	get_textdraw	( u32 index );
};

#endif//__onity_entitystardata_h__