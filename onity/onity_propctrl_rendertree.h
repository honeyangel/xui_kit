#ifndef __onity_propctrl_rendertree_h__
#define __onity_propctrl_rendertree_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"

class NPNode;
class onity_propdata_rendertree : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_rendertree( xui_propkind* kind, const std::wstring& name, NPNode* root );

	/*
	//method
	*/
	NPNode*						get_root			( void );

protected:
	/*
	//member
	*/
	NPNode*						m_root;
};

class onity_propctrl_rendertree : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create				( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_rendertree( void );

	/*
	//override
	*/
	virtual void				on_linkpropdata		( void );
	virtual void				on_editvalue		( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void				on_renderback		( xui_method_args& args );
	virtual void				on_invalid			( xui_method_args& args );
	virtual void				on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void						on_middlenodeleave	( xui_component* sender, xui_method_args& args );
	void						on_middlenodeenter	( xui_component* sender, xui_method_args& args );
	void						on_middleinvalid	( xui_component* sender, xui_method_args& args );
	void						on_middleselection	( xui_component* sender, xui_method_args& args );
	void						on_deleteclick		( xui_component* sender, xui_method_args& args );
	void						on_deleterenderself	( xui_component* sender, xui_method_args& args );
	void						on_insertrenderself	( xui_component* sender, xui_method_args& args );
	void						on_menuclick		( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void						add_rendernode		( xui_treenode* root, NPNode* rendernode );
	xui_proproot_vec			get_renderprop		( void );

	/*
	//member
	*/
	xui_drawer*					m_header;
	xui_treeview*				m_middle;
	xui_drawer*					m_multip;
	xui_toggle*					m_insert;
	xui_button*					m_delete;

	xui_menuitem*				m_module;
	xui_menuitem*				m_sprite;
	xui_menuitem*				m_action;
	xui_menuitem*				m_particle;
	xui_menuitem*				m_container;
};

#endif//__onity_propctrl_rendertree_h__