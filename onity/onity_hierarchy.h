#ifndef __onity_hierarchy_h__
#define __onity_hierarchy_h__

#include "xui_dockpage.h"

class onity_hierarchy : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_hierarchy( void );

protected:
	/*
	//event
	*/
	void			on_clearclick		( xui_component* sender, xui_method_args& args );
	void			on_searchtextchanged( xui_component* sender, xui_method_args& args );
	void			on_searchtextenter	( xui_component* sender, xui_method_args& args );
	void			on_headperform		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_menuitem*	m_entity;
	xui_textbox*	m_search;
	xui_button*		m_clear;
	xui_panel*		m_head;
	xui_treeview*	m_tree;
};

#endif//__onity_hierarchy_h__