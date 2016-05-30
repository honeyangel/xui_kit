#ifndef __xui_propedit_colour_h__
#define __xui_propedit_colour_h__

#include "xui_propedit.h"

class xui_propedit_colour : public xui_propedit_base
{
public:
	/*
	//member
	*/
	static xui_bitmap*			icon_pick;

	/*
	//constructor
	*/
	xui_propedit_colour( xui_propctrl* propctrl );

	/*
	//method
	*/
	xui_drawer*					get_pickctrl			( void ) const;
	const xui_colour&			get_value				( void ) const;
	void						set_value				( const xui_colour& value );

	/*
	//override
	*/
	virtual void				reset					( void );

protected:
	/*
	//event
	*/
	void						on_editctrlrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_pickctrlnonfocus		( xui_component* sender, xui_method_args&  args );
	void						on_editctrlclick		( xui_component* sender, xui_method_mouse& args );
	void						on_pickctrlclick		( xui_component* sender, xui_method_mouse& args );
	void						on_pickctrlkeybddown	( xui_component* sender, xui_method_keybd& args );

	/*
	//member
	*/
	xui_drawer*					m_pickctrl;
};

#include "xui_window.h"

class xui_colour_pickwnd : public xui_window
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_colour_pickwnd*	get_ptr					( void );

	/*
	//constructor
	*/
	xui_colour_pickwnd( void );

	/*
	//method
	*/
	const xui_colour&			get_value				( void ) const;
	void						set_value				( const xui_colour& value );
	xui_propctrl*				get_propctrl			( void );
	void						set_propctrl			( xui_propctrl* propctrl );

protected:
	/*
	//callback
	*/
	virtual void				on_invalid				( xui_method_args& args );

	/*
	//event
	*/
	void						on_windownonfocus		( xui_component* sender, xui_method_args&  args );
	void						on_plusctrlexpand		( xui_component* sender, xui_method_args&  args );
	void						on_pickctrlclick		( xui_component* sender, xui_method_mouse& args );
	void						on_pickctrlkeybddown	( xui_component* sender, xui_method_keybd& args );
	void						on_mainviewrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_compctrlrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_compctrlclick		( xui_component* sender, xui_method_mouse& args );
	void						on_viewctrlrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_viewctrldowndrag		( xui_component* sender, xui_method_mouse& args );
	void						on_viewpanerenderself	( xui_component* sender, xui_method_args&  args );
	void						on_mainrollrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_mainrolldowndrag		( xui_component* sender, xui_method_mouse& args );
	void						on_comppanerenderself	( xui_component* sender, xui_method_args&  args );
	void						on_comprollrenderself	( xui_component* sender, xui_method_args&  args );
	void						on_comprolldowndrag		( xui_component* sender, xui_method_mouse& args );
	void						on_compedittextchanged	( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void						set_colordata			( void );
	void						set_colortext			( void );

	/*
	//member
	*/
	xui_propctrl*				m_propctrl;
	xui_panel*					m_mainpane;
	xui_drawer*					m_pickctrl;
	xui_drawer*					m_mainview;
	xui_panel*					m_viewhead;
	xui_plusctrl*				m_viewplus;
	xui_drawer*					m_viewname;
	xui_drawer*					m_compctrl;
	xui_panel*					m_viewpane;
	xui_drawer*					m_viewctrl;
	xui_drawer*					m_mainroll;
	xui_panel*					m_comphead;
	xui_drawer*					m_compname;
	xui_plusctrl*				m_compplus;
	xui_panel*					m_comppane;
	xui_drawer*					m_comptext[4];
	xui_drawer*					m_comproll[4];
	xui_numbbox*				m_compedit[4];
};

#endif//__xui_propedit_stdvec_h__