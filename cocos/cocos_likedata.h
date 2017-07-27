#ifndef __cocos_likedata_h__
#define __cocos_likedata_h__

#include "cocos_pathdata.h"

class cocos_likedata : public cocos_pathdata
{
public:
	/*
	//constructor
	*/
	cocos_likedata( xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring		get_text	( u32 index );
};

#endif//__cocos_likedata_h__