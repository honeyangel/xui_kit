#ifndef __onity_maprefdata_h__
#define __onity_maprefdata_h__

#include "onity_treedata.h"

class onity_maprefdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_maprefdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_maprefdata( void );

	/*
	//virtual
	*/
	virtual std::wstring get_text( u32 index );
};

#endif//__onity_maprefdata_h__