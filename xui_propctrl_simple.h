#ifndef __xui_propctrl_simple_h__
#define __xui_propctrl_simple_h__

#include "xui_propctrl.h"

class xui_propctrl_simple : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_simple( xui_propdata* propdata );
	virtual ~xui_propctrl_simple( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	xui_propedit_base*		new_propedit	( xui_propdata* propdata );
	void					set_samedata	( xui_propdata* propdata, const xui_propdata_vec& vec, xui_propedit* propedit );
	void					set_editdata	( xui_propedit* propedit, const xui_propdata_vec& vec );
	xui_propdata_vec		get_propdataall ( u32 index );

	typedef std::vector<xui_propedit_base*>
		xui_editbase_vec;

	xui_drawer*				m_namectrl;
	xui_editbase_vec		m_propeditvec;
};

#endif//__xui_propctrl_simple_h__