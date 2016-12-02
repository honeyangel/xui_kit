#ifndef __onity_courseconfigdata_h__
#define __onity_courseconfigdata_h__

#include "onity_treedata.h"

class onity_courseconfigdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_courseconfigdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_courseconfigdata( void );

	/*
	//virtual
	*/
	virtual std::wstring	get_text( u32 index );
	virtual bool			get_flag( u32 index );
	virtual void			set_flag( u32 index, bool flag );

protected:
	/*
	//member
	*/
	bool					m_selected;
};

#endif//__onity_courseconfigdata_h__