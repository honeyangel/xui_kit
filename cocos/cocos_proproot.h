#ifndef __cocos_proproot_h__
#define __cocos_proproot_h__

#include "xui_proproot.h"

class cocos_proproot : public xui_proproot
{
public:
	/*
	//constructor
	*/
	cocos_proproot( void );

	/*
	//destructor
	*/
	virtual ~cocos_proproot( void );

	/*
	//method
	*/
	xui_treedata*				get_linkdata	( xui_treeview* treeview );
	void						add_linkdata	( xui_treedata* linkdata );
	void						del_linkdata	( xui_treedata* linkdata );

protected:
	/*
	//member
	*/
	std::vector<xui_treedata*>	m_linkdatavec;
};

#endif//__cocos_proproot_h__