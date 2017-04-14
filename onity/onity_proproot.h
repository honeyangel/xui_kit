#ifndef __onity_proproot_h__
#define __onity_proproot_h__

#include "xui_proproot.h"

class onity_proproot : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_proproot( void );

	/*
	//destructor
	*/
	virtual ~onity_proproot( void );

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

#endif//__onity_proproot_h__