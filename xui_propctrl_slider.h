#ifndef __xui_propctrl_slider_h__
#define __xui_propctrl_slider_h__

#include "xui_propctrl.h"
#include "xui_propedit_slider.h"

class xui_propctrl_slider : public xui_propctrl
{
	xui_declare_rtti

public:
	static xui_propctrl*	create			( xui_propdata* propdata );

	xui_propctrl_slider( xui_propdata* propdata );
	virtual ~xui_propctrl_slider( void );

	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	virtual void			on_perform		( xui_method_args& args );

	xui_propedit_slider*	m_propedit;
};

#endif//__xui_propctrl_slider_h__