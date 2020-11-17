#ifndef __xui_propctrl_vector_h__
#define __xui_propctrl_vector_h__

#include "xui_propctrl.h"
#include "xui_propdata.h"
#include "xui_propedit.h"

class xui_propctrl_vector : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_vector( xui_propdata* propdata );
	virtual ~xui_propctrl_vector( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_drawer*				m_namectrl;
	xui_propedit_number*	m_subxedit;
	xui_propedit_number*	m_subyedit;
};

class xui_propctrl_vector_button : public xui_propctrl_vector
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_vector_button( xui_propdata* propdata );
	virtual void			on_linkpropdata	( bool selfupdate = false );

protected:
	virtual void			on_perform		( xui_method_args& args );
	void					on_zeroctrlclick( xui_component* sender, xui_method_args& args );

	xui_button*				m_zeroctrl;
};

#endif//__xui_propctrl_vector_h__