#ifndef __onity_2dsrefdata_h__
#define __onity_2dsrefdata_h__

#include "onity_treedata.h"

class onity_2dsrefdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_2dsrefdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_2dsrefdata( void );

	/*
	//virtual
	*/
	virtual std::wstring	get_text( u32 index );
};

#endif//__onity_2dsrefdata_h__