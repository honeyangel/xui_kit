#ifndef __xui_propctrl_object_h__
#define __xui_propctrl_object_h__

#include "xui_propctrl.h"
#include "xui_propedit_object.h"

class xui_propctrl_object : public xui_propctrl
{
public:
	/*
	//create
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_object( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_object( void );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata			( void );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_textctrlclick		( xui_componet* sender, xui_method_mouse&	 args );
	void					on_textctrldragenter	( xui_componet* sender, xui_method_dragdrop& args );
	void					on_textctrldragleave	( xui_componet* sender, xui_method_dragdrop& args );
	void					on_textctrldragover		( xui_componet* sender, xui_method_dragdrop& args );
	void					on_textctrldragdrop		( xui_componet* sender, xui_method_dragdrop& args );

	/*
	//member
	*/
	xui_propedit_object*	m_propedit;
};

#include "xui_window.h"

class xui_pickwnd : public xui_window
{
public:
	/*
	//constructor
	*/
	xui_pickwnd( xui_propctrl* propctrl );

	/*
	//virtual
	*/
	virtual void			set_value				( void* value ) = 0;
	virtual void*			get_value				( void )		= 0;

protected:
	/*
	//method
	*/
	void					on_accept				( void );
	void					on_cancel				( void );

	/*
	//member
	*/
	xui_propctrl*			m_propctrl;
};

#endif//__xui_propctrl_object_h__