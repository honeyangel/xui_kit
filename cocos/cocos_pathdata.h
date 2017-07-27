#ifndef __cocos_pathdata_h__
#define __cocos_pathdata_h__

#include "xui_propview.h"
#include "cocos_filedata.h"

class cocos_pathdata : public cocos_filedata
{
public:
	/*
	//constructor
	*/
	cocos_pathdata( xui_bitmap* icon, const std::wstring& full, xui_proproot* prop );

	/*
	//override
	*/
	virtual std::wstring	get_text		( u32 index );
	virtual void			set_text		( u32 index, const std::wstring& text );

	/*
	//override
	*/
	virtual void			ntf_rename		( const std::wstring& last, const std::wstring& curr );
};

#endif//__cocos_pathdata_h__