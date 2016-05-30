#ifndef __onity_propctrl_texture_h__
#define __onity_propctrl_texture_h__

#include "xui_propctrl_object.h"

class onity_renderview;
class onity_propctrl_texture : public xui_propctrl_object
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
	onity_propctrl_texture( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( void );

protected:
	/*
	//callback
	*/
	virtual void			on_invalid				( xui_method_args& args );
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_viewctrlrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	onity_renderview*		m_viewctrl;
};

#endif//__onity_propctrl_texture_h__