#ifndef __onity_propctrl_condition_h__
#define __onity_propctrl_condition_h__

#include "xui_propctrl.h"

class NP2DSTransition;
class onity_propdata_condition : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_condition( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, NP2DSTransition* transition )
	: xui_propdata(kind, name, func)
	, m_transition(transition)
	{}

	/*
	//method
	*/
	NP2DSTransition*		get_transition		( void )
	{
		return m_transition;
	}

protected:
	/*
	//member
	*/
	NP2DSTransition*		m_transition;
};

class NP2DSParam;
class onity_propctrl_condition : public xui_propctrl
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
	onity_propctrl_condition( void );

	/*
	//override
	*/
	virtual void			on_linkpropdata		( void );
	virtual void			on_editvalue		( xui_propedit* sender );

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
	void					on_middleinvalid	( xui_component* sender, xui_method_args& args );
	void					on_middleperform	( xui_component* sender, xui_method_args& args );
	void					on_deleteclick		( xui_component* sender, xui_method_args& args );
	void					on_deleterenderself	( xui_component* sender, xui_method_args& args );
	void					on_insertclick		( xui_component* sender, xui_method_args& args );
	void					on_insertrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_panel*				m_middle;
	xui_drawer*				m_nontip;
	xui_button*				m_insert;
	xui_button*				m_delete;
};

#endif//__onity_propctrl_condition_h__