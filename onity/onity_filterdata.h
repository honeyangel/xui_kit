#ifndef __onity_filterdata_h__
#define __onity_filterdata_h__

#include "onity_treedata.h"

class onity_filterdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_filterdata( xui_bitmap* icon, const std::wstring& text, xui_proproot* prop );

	/*
	//virtual
	*/
	//virtual std::wstring get_text( u32 index );
};

#endif//__onity_filterdata_h__