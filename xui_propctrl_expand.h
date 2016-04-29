#ifndef __xui_propctrl_expand_h__
#define __xui_propctrl_expand_h__

#include "xui_plusctrl.h"
#include "xui_propctrl.h"

class xui_propctrl_expand : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create	( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_expand( xui_propdata* propdata );

	/*
	//method
	*/
	bool					was_expanded	( void ) const;
	void					set_expanded	( bool flag );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( void );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void					on_propexpand	( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	xui_propdata_vec		get_propdataall	( u32 index );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_plusctrl*			m_propplus;
	xui_propctrl_vec		m_propctrlvec;
};

class xui_propctrl_expand_plus : public xui_propctrl_expand
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
	xui_propctrl_expand_plus( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata	( void );
};

class xui_propctrl_expand_string : public xui_propctrl_expand
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
	xui_propctrl_expand_string( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_expand_string( void );

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
	xui_propedit_string*	m_propedit;
};

class xui_propctrl_expand_bool : public xui_propctrl_expand
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
	xui_propctrl_expand_bool( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_expand_bool( void );

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
	xui_propedit_bool*		m_propedit;
};

class xui_propctrl_expand_enum : public xui_propctrl_expand
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
	xui_propctrl_expand_enum( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_expand_enum( void );

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
	xui_propedit_enum*		m_propedit;
};

#endif//__xui_propctrl_expand_h__