#ifndef __cocos_hierarchy_h__
#define __cocos_hierarchy_h__

#include "xui_dockpage.h"
#include "xui_propview.h"
#include "xui_treeview.h"

class cocos_propfile;
class cocos_propnodebase;
class cocos_hierarchy : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_hierarchy( void );

	/*
	//reset
	*/
	void						reset					( void );

	/*
	//method
	*/
	xui_treeview*				get_treeview			( void );
	std::vector<xui_treenode*>	get_realnode			( void );
	cocos_propnodebase*			new_propnode			( cocos_propnodebase* proproot, const std::wstring& nodertti, u32 index = -1 );
	cocos_propnodebase*			new_propnode			( cocos_propnodebase* proproot, cocos_propfile*		propfile, u32 index = -1 );
	cocos_propnodebase*			new_propnode			( cocos_propnodebase* proproot, cocos2d::Ref*		data,	  u32 index = -1 );
	void						add_propnode			( cocos_propnodebase* proproot, cocos_propnodebase* propleaf, u32 index = -1 );
	void						del_propnode			( cocos_propnodebase* proproot, cocos_propnodebase* propleaf, bool destroy = false );
	void						del_propnode			( void );

protected:
	/*
	//event
	*/
	void						on_toggleclick			( xui_component* sender, xui_method_args&			args );
	void						on_clearclick			( xui_component* sender, xui_method_args&			args );
	void						on_searchtextchanged	( xui_component* sender, xui_method_args&			args );
	void						on_headperform			( xui_component* sender, xui_method_args&			args );
	void						on_menuclick			( xui_component* sender, xui_method_args&			args );
	void						on_treeselection		( xui_component* sender, xui_method_args&			args );
	void						on_treemenuclick		( xui_component* sender, xui_method_args&			args );
	void						on_treekeybddown		( xui_component* sender, xui_method_keybd&			args );
	void						on_treemousedown		( xui_component* sender, xui_method_mouse&			args );
	void						on_treemouseclick		( xui_component* sender, xui_method_mouse&			args );
	void						on_treemousedragleave	( xui_component* sender, xui_method_dragdrop&		args );
	void						on_treemousedragover	( xui_component* sender, xui_method_dragdrop&		args );
	void						on_treemousedragdrop	( xui_component* sender, xui_method_dragdrop&		args );
	void						on_treeinnerdragover	( xui_component* sender, xui_method_treedragdrop&	args );
	void						on_treeinnerdragdrop	( xui_component* sender, xui_method_treedragdrop&	args );
	void						on_treemousedoubleclick	( xui_component* sender, xui_method_mouse&			args );

	/*
	//method
	*/
	void						add_propnode			( xui_treenode* root, xui_proproot* prop, u32 index );
	bool						has_ancestor			( xui_treenode* node, const std::vector<xui_treenode*>& vec );
	bool						was_ancestor			( xui_treenode* leaf, xui_treenode* root );

	/*
	//member
	*/
	xui_toggle*					m_create;
	xui_menuitem*				m_node;
	xui_menuitem*				m_sprite;
	xui_menuitem*				m_particle;
	xui_menuitem*				m_delete;
	xui_menuitem*				m_copy;
	xui_menuitem*				m_move;
	xui_menuitem*				m_paste;
	xui_proproot_vec			m_menuvec;
	xui_textbox*				m_search;
	xui_button*					m_clear;
	xui_panel*					m_head;
	xui_treeview*				m_tree;
};

#endif//__cocos_hierarchy_h__