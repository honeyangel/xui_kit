#ifndef __onity_filterdata_h__
#define __onity_filterdata_h__

#include "onity_treedata.h"

class onity_filterdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_filterdata( xui_bitmap* icon, xui_proproot* prop, bool top );

	/*
	//destructor
	*/
	virtual ~onity_filterdata( void );

	/*
	//virtual
	*/
	virtual std::wstring		get_text		( u32 index );
	virtual void				set_text		( u32 index, const std::wstring& text );
	virtual xui_family_render	get_textdraw	( u32 index );

protected:
	/*
	//member
	*/
	bool						m_top;
};

#endif//__onity_filterdata_h__