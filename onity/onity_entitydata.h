#ifndef __onity_entitydata_h__
#define __onity_entitydata_h__

#include "onity_treedata.h"

class onity_entitydata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_entitydata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_entitydata( void );

	/*
	//virtual
	*/
	virtual std::wstring get_text( u32 index );
};

#endif//__onity_entitydata_h__