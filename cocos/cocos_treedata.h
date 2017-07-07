#ifndef __cocos_treedata_h__
#define __cocos_treedata_h__

#include "xui_treedata.h"

class cocos_treedata : public xui_treedata
{
public:
	/*
	//constructor
	*/
	cocos_treedata( xui_bitmap* icon, const std::wstring& text, bool flag, xui_proproot* prop );
	cocos_treedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~cocos_treedata( void );

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

#endif//__cocos_treedata_h__