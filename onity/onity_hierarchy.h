#ifndef __onity_hierarchy_h__
#define __onity_hierarchy_h__

#include "Entity/Entity.h"
#include "xui_dockpage.h"

class NP2DSAsset;
class NP2DSTransRef;
class NP2DSSceneLayer;
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
	static void			on_entityadd			( Omiga::Entity* ent );
	static void			on_entitydel			( Omiga::Entity* ent );

	/*
	//reset
	*/
	void				reset					( bool forcedel = false );

	/*
	//method
	*/
	xui_treenode*		add_entitynode			( Omiga::Entity* ent );
	xui_treenode*		get_entitynode			( Omiga::Entity* ent );
	void				del_entitynode			( Omiga::Entity* ent );
	void				add_maprefnode			( const xui_vector<s32>& pos, NP2DSAsset* asset );
	void				del_coursenode			( void );
	onity_propcourse*	get_editprop			( void );
	void				set_editprop			( onity_propcourse* editprop );
	xui_treeview*		get_treeview			( void );

protected:
	/*
	//event
	*/
	void				on_toggleclick			( xui_component* sender, xui_method_args&     args );
	void				on_clearclick			( xui_component* sender, xui_method_args&	  args );
	void				on_searchtextchanged	( xui_component* sender, xui_method_args&	  args );
	void				on_searchtextenter		( xui_component* sender, xui_method_args&	  args );
	void				on_headperform			( xui_component* sender, xui_method_args&	  args );
	void				on_menuclick			( xui_component* sender, xui_method_args&	  args );
	void				on_treeselection		( xui_component* sender, xui_method_args&     args );
	void				on_treemenuclick		( xui_component* sender, xui_method_args&	  args );
	void				on_treekeybddown		( xui_component* sender, xui_method_keybd&	  args );
	void				on_treemousedown		( xui_component* sender, xui_method_mouse&	  args );
	void				on_treemouseclick		( xui_component* sender, xui_method_mouse&	  args );
	void				on_treemousedragover	( xui_component* sender, xui_method_dragdrop& args );
	void				on_treemousedragdrop	( xui_component* sender, xui_method_dragdrop& args );
	void				on_treemousedoubleclick	( xui_component* sender, xui_method_mouse&    args );

	/*
	//method
	*/
	xui_treenode*		add_maprefnode			( xui_treenode* root, NP2DSAsset* asset, NP2DSTransRef* src );
	xui_treenode*		add_scenelayer			( NP2DSSceneLayer* src );
	xui_treenode*		add_scenelayer			( const std::string& name );
	xui_treenode*		add_filternode			( const std::string& name );
	xui_treenode*		get_filternode			( const std::string& name );
	void				del_coursenode			( const std::vector<xui_treenode*>& nodes );
	void				del_scenelayer			( xui_treenode* root );
	void				del_maprefnode			( xui_treenode* node );
	void				pst_coursenode			( void );

	/*
	//typedef
	*/
	typedef std::map<Omiga::Entity*, xui_treenode*>
		onity_entity_map;

	typedef std::vector<NP2DSTransRef*>
		onity_transref_vec;

	/*
	//member
	*/
	xui_toggle*			m_create;
	xui_menuitem*		m_layer;
	xui_menuitem*		m_instance;
	xui_menuitem*		m_delete;
	xui_menuitem*		m_copy;
	xui_menuitem*		m_move;
	xui_menuitem*		m_paste;
	onity_transref_vec	m_menuvec;
	xui_textbox*		m_search;
	xui_button*			m_clear;
	xui_panel*			m_head;
	xui_treeview*		m_tree;
	onity_propcourse*	m_editprop;
	onity_entity_map	m_entitymap;
};

#endif//__onity_hierarchy_h__