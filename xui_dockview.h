#ifndef __xui_dockview_h__
#define __xui_dockview_h__

#include "xui_control.h"

class xui_dockview : public xui_control
{
	xui_declare_rtti

public:
	static xui_dockview*		create		( void );

	/*
	//constructor
	*/
	xui_dockview( const xui_vector<s32>& size );

	/*
	//method
	*/
	xui_dockpage*				get_showpage( void );
	void						set_showpage( xui_dockpage* page );
	xui_rect2d<s32>				get_freerect( void ) const;
	xui_rect2d<s32>				get_namerect( void ) const;

	/*
	//page
	*/
	void						add_dockpage( xui_dockpage* page, u08 dockstyle );
	void						del_dockpage( xui_dockpage* page );
	void						del_dockview( xui_dockview* view );

protected:
	/*
	//callback
	*/
	virtual void				on_invalid	( xui_method_args& args );
	virtual void				on_perform	( xui_method_args& args );

	/*
	//method
	*/
	void						add_dockctrl( xui_component* comp );
	void						del_dockctrl( xui_component* comp );

	/*
	//member
	*/
	xui_dockpage*				m_showpage;
	std::vector<xui_dockpage*>	m_pagelist;
	std::vector<xui_dockview*>	m_viewlist;
};

#endif//__xui_dockview_h__