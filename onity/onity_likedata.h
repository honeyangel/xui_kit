#ifndef __onity_likedata_h__
#define __onity_likedata_h__

#include "onity_pathdata.h"

class onity_likedata : public onity_pathdata
{
public:
	/*
	//constructor
	*/
	onity_likedata( xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring		get_text	( u32 index );
	//virtual xui_family_render	get_textdraw( u32 index );
};

#endif//__onity_likedata_h__