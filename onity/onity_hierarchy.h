#ifndef __onity_hierarchy_h__
#define __onity_hierarchy_h__

#include "Entity/Entity.h"
#include "xui_dockpage.h"

class onity_propcourse;
class onity_hierarchy : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_hierarchy( void );

	/*
	//static
	*/
	static void			on_entityadd		( Omiga::Entity* ent );
	static void			on_entitydel		( Omiga::Entity* ent );

	/*
	//reset
	*/
	void				reset				( bool forcedel = false );

	/*
	//method
	*/
	xui_treenode*		add_entitynode		( Omiga::Entity* ent );
	xui_treenode*		get_entitynode		( Omiga::Entity* ent );
	void				del_entitynode		( Omiga::Entity* ent );
	onity_propcourse*	get_editprop		( void );
	void				set_editprop		( onity_propcourse* editprop );

protected:
	/*
	//event
	*/
	void				on_clearclick		( xui_component* sender, xui_method_args&  args );
	void				on_searchtextchanged( xui_component* sender, xui_method_args&  args );
	void				on_searchtextenter	( xui_component* sender, xui_method_args&  args );
	void				on_headperform		( xui_component* sender, xui_method_args&  args );
	void				on_treemouseclick	( xui_component* sender, xui_method_mouse& args );

	/*
	//method
	*/
	xui_treenode*		add_filternode		( const std::string& name );
	xui_treenode*		get_filternode		( const std::string& name );

	/*
	//member
	*/
	xui_toggle*			m_create;
	xui_menuitem*		m_entity;
	xui_textbox*		m_search;
	xui_button*			m_clear;
	xui_panel*			m_head;
	xui_treeview*		m_tree;
	onity_propcourse*	m_editprop;
};

#endif//__onity_hierarchy_h__