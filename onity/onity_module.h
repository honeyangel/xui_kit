#ifndef __onity_module_h__
#define __onity_module_h__

#include "onity_asset.h"

class onity_bounding;
class onity_propmodule;
class onity_module : public onity_asset
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_module( void );

	/*
	//destructor
	*/
	virtual ~onity_module( void );

	/*
	//method
	*/
	onity_propmodule*		get_editprop				( void );
	void					set_editprop				( onity_propmodule* prop );
	virtual onity_proproot*	hit_propvisible				( const xui_vector<s32>& pt );

protected:
	/*
	//event
	*/
	virtual void			on_buttonclick				( xui_component* sender, xui_method_args&		args );
	virtual void			on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	virtual void			on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	void					on_timertick				( xui_component* sender, xui_method_args&		args );

	/*
	//override
	*/
	virtual void			on_keybdmoveimpl			( const xui_vector<s32>& delta );
	virtual void			on_mousepickimpl			( onity_proproot* pick, bool alt, bool ctrl, bool shift );
	virtual void			on_mousedragimpl			( const xui_vector<s32>& delta );
	virtual void			on_mulselectimpl			( const xui_rect2d<s32>& rt, bool ctrl );

	/*
	//member
	*/
	onity_propmodule*		m_editprop;
	xui_button*				m_automode;
	xui_timer*				m_addtimer;	
};

#endif//__onity_module_h__