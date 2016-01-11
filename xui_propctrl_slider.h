#ifndef __xui_propctrl_slider_h__
#define __xui_propctrl_slider_h__

#include "xui_propctrl.h"
#include "xui_propedit_slider.h"

class xui_propctrl_slider : public xui_propctrl
{
public:
	/*
	//create
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	xui_propctrl_slider( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_slider( void );

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
	xui_propedit_slider*	m_propedit;
};

#endif//__xui_propctrl_slider_h__