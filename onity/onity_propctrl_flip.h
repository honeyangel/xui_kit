#ifndef __onity_propctrl_flip_h__
#define __onity_propctrl_flip_h__

#include "xui_propctrl.h"

class onity_propctrl_flip : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_flip( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_toggleclick			( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_toggle*				m_horzctrl;
	xui_toggle*				m_vertctrl;
};

#endif//__onity_propctrl_flip_h__