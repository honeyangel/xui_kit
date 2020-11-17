#ifndef __xui_propctrl_object_h__
#define __xui_propctrl_object_h__

#include "xui_propctrl.h"
#include "xui_propedit_object.h"

class xui_propctrl_object : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_object( xui_propdata* propdata );
	virtual ~xui_propctrl_object( void );

	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	virtual void			on_perform				( xui_method_args& args );
	void					on_textctrlkeybddown	( xui_component* sender, xui_method_keybd&	  args );
	void					on_textctrldoubleclick	( xui_component* sender, xui_method_mouse&	  args );
	void					on_textctrldragenter	( xui_component* sender, xui_method_dragdrop& args );
	void					on_textctrldragleave	( xui_component* sender, xui_method_dragdrop& args );
	void					on_textctrldragover		( xui_component* sender, xui_method_dragdrop& args );
	void					on_textctrldragdrop		( xui_component* sender, xui_method_dragdrop& args );

	xui_propedit_object*	m_propedit;
};

#include "xui_window.h"

class xui_object_pickwnd : public xui_window
{
	xui_declare_rtti

public:
	xui_object_pickwnd( void );

	xui_propctrl*			get_propctrl			( void );
	void					set_propctrl			( xui_propctrl* propctrl );
	virtual void			set_value				( void* value ) = 0;
	virtual void*			get_value				( void )		= 0;

protected:
	virtual void			on_accept				( xui_component* sender, xui_method_args& args );
	virtual void			on_cancel				( xui_component* sender, xui_method_args& args );

	xui_propctrl*			m_propctrl;
};

#endif//__xui_propctrl_object_h__