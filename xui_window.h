#ifndef __xui_window_h__
#define __xui_window_h__

#include "xui_panel.h"

class xui_window : public xui_panel
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_window( const xui_vector<s32>& size, bool modal );

	/*
	//modal
	*/
	bool						was_modal		( void ) const;
	xui_syswnd*					get_owner		( void );
	void						set_owner		( xui_syswnd* owner );

	/*
	//virtual
	*/
	virtual xui_vector<s32>		get_screenpt	( void ) const;
	virtual xui_component*		choose			( const xui_vector<s32>& pt );

	/*
	//event
	*/
	xui_method<xui_method_args>	xm_accept;
	xui_method<xui_method_args>	xm_cancel;

protected:
	/*
	//callback
	*/
	virtual void				on_keybddown	( xui_method_keybd&	args );
	virtual void				on_show			( xui_method_args&	args );
	virtual void				on_hide			( xui_method_args&	args );
	virtual void				on_setrenderpt	( xui_method_args&  args );
	virtual void				on_invalid		( xui_method_args&  args );

	virtual void				on_accept		( xui_component* sender, xui_method_args& args );
	virtual void				on_cancel		( xui_component* sender, xui_method_args& args );

protected:
	/*
	//member
	*/
	bool						m_modal;
	xui_syswnd*					m_owner;
};

#endif//__xui_window_h__