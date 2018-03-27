#ifndef __cocos_propctrl_asset_h__
#define __cocos_propctrl_asset_h__

#include "xui_propdata.h"
#include "xui_propctrl_object.h"

class cocos_propctrl_asset : public xui_propctrl_object
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_asset( xui_propdata* propdata );

protected:
	/*
	//event
	*/
	void						on_editctrlmouseenter	( xui_component* sender, xui_method_mouse& args );
	void						on_editctrlmouseleave	( xui_component* sender, xui_method_mouse& args );
};

#endif//__cocos_propctrl_asset_h__