#ifndef __cocos_nodedata_h__
#define __cocos_nodedata_h__

#include "cocos_treedata.h"

enum 
{
	NODEFLAG_SHOW = 0,
	NODEFLAG_LOCK,
};

class cocos_nodedata : public cocos_treedata
{
public:
	/*
	//constructor
	*/
	cocos_nodedata( xui_proproot* prop );

	/*
	//virtual
	*/
	virtual std::wstring	get_text( u32 index );
	virtual void			set_text( u32 index, const std::wstring& text );
	virtual bool			get_flag( u32 index );
	virtual void			set_flag( u32 index, bool flag );
};

#endif//__cocos_nodedata_h__