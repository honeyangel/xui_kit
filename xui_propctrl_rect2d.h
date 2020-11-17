#ifndef __xui_propctrl_rect2d_h__
#define __xui_propctrl_rect2d_h__

#include "xui_propctrl.h"
#include "xui_propdata.h"
#include "xui_propedit.h"

class xui_propctrl_rect2d : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_rect2d( xui_propdata* propdata );
	virtual ~xui_propctrl_rect2d( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_drawer*				m_namectrl;
	xui_propedit_number*	m_subxedit;
	xui_propedit_number*	m_subyedit;
	xui_propedit_number*	m_subwedit;
	xui_propedit_number*	m_subhedit;
};

#endif//__xui_propctrl_rect2d_h__