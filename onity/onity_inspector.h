#ifndef __onity_inspector_h__
#define __onity_inspector_h__

#include "xui_dockpage.h"

class onity_inspector : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_inspector( void );

protected:
	/*
	//callback
	*/
	virtual void	on_load( xui_method_args& args );

	/*
	//member
	*/
	xui_propview*	m_view;
};

#endif//__onity_inspector_h__