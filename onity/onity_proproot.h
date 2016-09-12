#ifndef __onity_proproot_h__
#define __onity_proproot_h__

#include "xui_proproot.h"

class onity_proproot : public xui_proproot
{
public:
	/*
	//constructor
	*/
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
	xui_treedata*		get_linkdata	( void );
	void				set_linkdata	( xui_treedata* linkdata );

protected:
	/*
	//member
	*/
	xui_treedata*		m_linkdata;
};

#endif//__onity_proproot_h__