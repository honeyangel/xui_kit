#ifndef __xui_propctrl_simple_h__
#define __xui_propctrl_simple_h__

#include "xui_propctrl.h"

class xui_propctrl_simple : public xui_propctrl
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
	xui_propctrl_simple( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_simple( void );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	/*
	//method
	*/
	xui_propedit_base*		new_propedit	( xui_propdata* propdata );
	void					set_samedata	( xui_propdata* propdata, const xui_propdata_vec& vec, xui_propedit* propedit );
	void					set_editdata	( xui_propedit* propedit, const xui_propdata_vec& vec );
	xui_propdata_vec		get_propdataall ( u32 index );

	/*
	//def
	*/
	typedef std::vector<xui_propedit_base*>
		xui_editbase_vec;

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_editbase_vec		m_propeditvec;
};

#endif//__xui_propctrl_simple_h__