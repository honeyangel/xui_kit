#ifndef __cocos_framedata_h__
#define __cocos_framedata_h__

#include "cocos_treedata.h"

class cocos_framedata : public cocos_treedata
{
public:
	/*
	//constructor
	*/
	cocos_framedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring	get_text( u32 index );
	//virtual void			set_text( u32 index, const std::wstring& text );
};

#endif//__cocos_framedata_h__