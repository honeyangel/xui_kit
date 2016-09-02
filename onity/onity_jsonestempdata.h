#ifndef __onity_jsonestempdata_h__
#define __onity_jsonestempdata_h__

#include "onity_treedata.h"

class onity_jsonestempdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_jsonestempdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring	get_text( u32 index );
	virtual void			set_text( u32 index, const std::wstring& text );
};

#endif//__onity_jsonestempdata_h__