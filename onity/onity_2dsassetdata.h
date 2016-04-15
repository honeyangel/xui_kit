#ifndef __onity_2dsassetdata_h__
#define __onity_2dsassetdata_h__

#include "onity_treedata.h"

class onity_2dsassetdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_2dsassetdata( xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring	get_text( u32 index );
	virtual void			set_text( u32 index, const std::wstring& text );
};

#endif//__onity_assetdata_h__