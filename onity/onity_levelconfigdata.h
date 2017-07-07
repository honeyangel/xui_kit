#ifndef __onity_levelconfigdata_h__
#define __onity_levelconfigdata_h__

#include "onity_treedata.h"

class onity_levelconfigdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_levelconfigdata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_levelconfigdata( void );

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

#endif//__onity_levelconfigdata_h__