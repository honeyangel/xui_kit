#ifndef __onity_pathdata_h__
#define __onity_pathdata_h__

#include "xui_propview.h"
#include "onity_filedata.h"

class onity_pathdata : public onity_filedata
{
public:
	/*
	//constructor
	*/
	onity_pathdata( xui_bitmap* icon, const std::wstring& full, xui_proproot* prop );

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

#endif//__onity_pathdata_h__