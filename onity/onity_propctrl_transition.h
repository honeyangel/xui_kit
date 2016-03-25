#ifndef __onity_propctrl_transition_h__
#define __onity_propctrl_transition_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"

class onity_state;
class onity_propdata_transition : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_transition( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, onity_state* statectrl )
	: xui_propdata(kind, name, func)
	, m_statectrl(statectrl)
	{}

	/*
	//method
	*/
	onity_state*			get_statectrl		( void )
	{
		return m_statectrl;
	}

protected:
	/*
	//member
	*/
	onity_state*			m_statectrl;
};

class NP2DSState;
class onity_propctrl_transition : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create				( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_transition( void );

	/*
	//override
	*/
	virtual void			on_linkpropdata		( void );
	virtual void			on_editvalue		( xui_propedit* sender );

	/*
	//method
	*/
	void					del_statelink		( NP2DSState* state );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback		( xui_method_args& args );
	virtual void			on_invalid			( xui_method_args& args );
	virtual void			on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void					on_sublstinvalid	( xui_component* sender, xui_method_args& args );
	void					on_sublstselection	( xui_component* sender, xui_method_args& args );
	void					on_deleteclick		( xui_component* sender, xui_method_args& args );
	void					on_deleterenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_button*				m_delete;
	xui_listview*			m_sublst;
	xui_drawer*				m_nontip;
};

#endif//__onity_propctrl_transition_h__