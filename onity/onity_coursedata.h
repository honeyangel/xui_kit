#ifndef __onity_coursedata_h__
#define __onity_coursedata_h__

#include "onity_treedata.h"

class onity_coursedata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_coursedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_coursedata( void );

	/*
	//virtual
	*/
	virtual std::wstring get_text( u32 index );
};

#endif//__onity_coursedata_h__