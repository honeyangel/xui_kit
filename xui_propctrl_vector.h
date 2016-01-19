#ifndef __xui_propctrl_vector_h__
#define __xui_propctrl_vector_h__

#include "xui_propctrl.h"
#include "xui_propdata.h"
#include "xui_propedit.h"

class xui_propctrl_vector : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_vector( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_vector( void );

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
	xui_propedit_number*	m_subxedit;
	xui_propedit_number*	m_subyedit;
};

class xui_propctrl_vector_button : public xui_propctrl_vector
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_vector_button( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( void );

protected:
	/*
	//override
	*/
	virtual void			on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void					on_zeroctrlclick( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*				m_zeroctrl;
};

#endif//__xui_propctrl_vector_h__