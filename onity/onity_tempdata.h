#ifndef __onity_tempdata_h__
#define __onity_tempdata_h__

#include "onity_treedata.h"

class onity_tempdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_tempdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring	get_text( u32 index );
};

#endif//__onity_tempdata_h__