#ifndef __onity_inspector_h__
#define __onity_inspector_h__

#include "xui_dockpage.h"
#include "xui_propview.h"

class onity_inspector : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_inspector( void );

	/*
	//method
	*/
	void				set_proproot	( xui_proproot* proproot );
	void				set_proproot	( const xui_proproot_vec& proproot );

protected:
	/*
	//member
	*/
	xui_propview*		m_view;
};

#endif//__onity_inspector_h__