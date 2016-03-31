#ifndef __onity_filedata_h__
#define __onity_filedata_h__

#include "onity_pathdata.h"

class onity_filedata : public onity_pathdata
{
public:
	/*
	//constructor
	*/
	onity_filedata( const std::wstring& full, xui_proproot* prop );

	/*
	//method
	*/
	virtual std::wstring	get_text	( u32 index );

	/*
	//static
	*/
	static xui_proproot*	create_prop	( const std::wstring& full );
};

#endif//__onity_filedata_h__