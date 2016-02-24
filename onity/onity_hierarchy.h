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
	//member
	*/
	xui_menuitem*	m_entity;
	xui_textbox*	m_search;
	xui_button*		m_clear;
	xui_panel*		m_head;
	xui_treeview*	m_tree;
};

#endif//__onity_hierarchy_h__