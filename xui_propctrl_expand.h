#ifndef __xui_propctrl_expand_h__
#define __xui_propctrl_expand_h__

#include "xui_plusctrl.h"
#include "xui_propctrl.h"

class xui_propctrl_expand : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create	( xui_propdata* propdata );

	xui_propctrl_expand( xui_propdata* propdata );

	bool					was_expanded	( void ) const;
	void					set_expanded	( bool flag );
	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );
	void					on_propexpand	( xui_component* sender, xui_method_args& args );
	xui_propdata_vec		get_propdataall	( u32 index );

	xui_drawer*				m_namectrl;
	xui_plusctrl*			m_propplus;
	xui_propctrl_vec		m_propctrlvec;
};

class xui_propctrl_expand_plus : public xui_propctrl_expand
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_expand_plus( void );
	virtual void			on_linkpropdata	( bool selfupdate = false );
};

class xui_propctrl_expand_number : public xui_propctrl_expand
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_expand_number( xui_propdata* propdata );
	virtual ~xui_propctrl_expand_number( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_propedit_number*	m_propedit;
};

class xui_propctrl_expand_bool : public xui_propctrl_expand
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_expand_bool( xui_propdata* propdata );
	virtual ~xui_propctrl_expand_bool( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_propedit_bool*		m_propedit;
};

class xui_propctrl_expand_enum : public xui_propctrl_expand
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_expand_enum( xui_propdata* propdata );
	virtual ~xui_propctrl_expand_enum( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_propedit_enum*		m_propedit;
};

#endif//__xui_propctrl_expand_h__