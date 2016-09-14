#ifndef __onity_propctrl_sceneparam_h__
#define __onity_propctrl_sceneparam_h__

#include "NP2DSTransRef.h"
#include "xui_propdata.h"
#include "xui_propedit.h"
#include "xui_propctrl.h"

class onity_propdata_sceneparam : public xui_propdata
{
public:
	/*
	//constructor
	*/
	onity_propdata_sceneparam( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, NP2DSTransRef::SParam* param )
	: xui_propdata(kind, name, func)
	, m_param(param)
	{}

	/*
	//method
	*/
	NP2DSTransRef::SParam*	get_param		( void )
	{
		return m_param;
	}

protected:
	/*
	//member
	*/
	NP2DSTransRef::SParam*	m_param;
};

class onity_propctrl_sceneparam : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_sceneparam( void );

	/*
	//destructor
	*/
	virtual ~onity_propctrl_sceneparam( void );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( void );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_perform		( xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_propedit_string*	m_nameedit;
	xui_propedit_string*	m_textedit;
};

#endif//__onity_propctrl_sceneparam_h__