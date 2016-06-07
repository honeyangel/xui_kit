#ifndef __onity_treedata_h__
#define __onity_treedata_h__

#include "xui_treedata.h"

class onity_treedata : public xui_treedata
{
public:
	/*
	//constructor
	*/
	onity_treedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_treedata( void );

	/*
	//method
	*/
	xui_proproot*		get_prop( void );
	void				set_null( void );

protected:
	/*
	//member
	*/
	xui_proproot*		m_prop;
};

#endif//__onity_treedata_h__