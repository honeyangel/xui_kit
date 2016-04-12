#ifndef __onity_param_h__
#define __onity_param_h__

#include "xui_control.h"

class NP2DSParam;
class onity_param : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32	default_width;
	static const s32	default_height;

	/*
	//constructor
	*/
	onity_param( NP2DSParam* param, bool reqfocus );

	/*
	//method
	*/
	NP2DSParam*		get_param			( void );

protected:
	/*
	//callback
	*/
	virtual void	on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void			on_editctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void			on_editctrlnonfocus		( xui_component* sender, xui_method_args& args );
	void			on_namectrltextchanged	( xui_component* sender, xui_method_args& args );
	void			on_numbctrltextchanged	( xui_component* sender, xui_method_args& args );
	void			on_boolctrlclick		( xui_component* sender, xui_method_args& args );
	void			on_deleteclick			( xui_component* sender, xui_method_args& args );
	void			on_deleterenderself		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_textbox*	m_namectrl;
	xui_toggle*		m_boolctrl;
	xui_textbox*	m_numbctrl;
	xui_button*		m_delete;
	NP2DSParam*		m_param;
};

#endif//__onity_param_h__