#ifndef __cocos_propctrl_fontsize_h__
#define __cocos_propctrl_fontsize_h__

#include "xui_propctrl.h"
#include "xui_propedit.h"

class cocos_propctrl_fontsize : public xui_propctrl_base
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_propctrl_fontsize( xui_propdata* propdata );

	/*
	//create
	*/
	static xui_propctrl*	create				( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata		( bool selfupdate = false );
	virtual void			on_editvalue		( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_perform			( xui_method_args& args );
};

#endif//__cocos_propctrl_fontsize_h__