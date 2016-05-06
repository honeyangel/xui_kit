#ifndef __xui_propctrl_stdvec_h__
#define __xui_propctrl_stdvec_h__

#include "xui_plusctrl.h"
#include "xui_propctrl.h"
#include "xui_propedit_stdvec.h"

class xui_propctrl_stdvec : public xui_propctrl
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
	xui_propctrl_stdvec( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_stdvec( void );

	/*
	//method
	*/
	bool					was_expanded			( void ) const;
	void					set_expanded			( bool flag );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( void );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_invalid				( xui_method_args& args );
	virtual void			on_perform				( xui_method_args& args );

	/*
	//method
	*/
	void					add_propctrl			( xui_propdata* propdata );
	void					del_propctrl			( void );
	xui_propdata_vec		get_propdataall			( u32 index );

	/*
	//event
	*/
	void					on_propctrlperform		( xui_component* sender, xui_method_args&  args );
	void					on_sortctrlupdateself	( xui_component* sender, xui_method_args&  args );
	void					on_sortctrlrenderself	( xui_component* sender, xui_method_args&  args );
	void					on_sortctrltopdraw		( xui_component* sender, xui_method_args&  args );
	void					on_sortctrlmousedown	( xui_component* sender, xui_method_mouse& args );
	void					on_sortctrlmousemove	( xui_component* sender, xui_method_mouse& args );
	void					on_sortctrlmouserise	( xui_component* sender, xui_method_mouse& args );
	void					on_propexpand			( xui_component* sender, xui_method_args&  args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_propedit_stdvec*	m_propedit;
	xui_plusctrl*			m_propplus;
	u32						m_dragelem;
	u32						m_dropelem;
	xui_propctrl_vec		m_ctrlpool;
	xui_propctrl_vec		m_propctrlvec;
};

class xui_propctrl_stdvec_root : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_stdvec_root( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( void );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback			( xui_method_args& args );
	virtual void			on_invalid				( xui_method_args& args );
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_middleinvalid		( xui_component* sender, xui_method_args& args );
	void					on_middleselection		( xui_component* sender, xui_method_args& args );
	void					on_deleteclick			( xui_component* sender, xui_method_args& args );
	void					on_deleterenderself		( xui_component* sender, xui_method_args& args );
	void					on_insertclick			( xui_component* sender, xui_method_args& args );
	void					on_insertrenderself		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_listview*			m_middle;
	xui_drawer*				m_nontip;
	xui_button*				m_insert;
	xui_button*				m_delete;
};

#endif//__xui_propctrl_stdvec_h__