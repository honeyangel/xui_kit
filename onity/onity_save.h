#ifndef __onity_save_h__
#define __onity_save_h__

#include "xui_window.h"

class onity_save : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_save( void );

	/*
	//method
	*/
	void				load_unsavedfiles		( void );

protected:
	/*
	//callback
	*/
	virtual void		on_accept				( xui_component* sender, xui_method_args& args );

	/*
	//event
	*/
	void				on_recentdoubleclick	( xui_component* sender, xui_method_mouse& args );
	void				on_buttonclick			( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void				restore					( void );
	
	/*
	//member
	*/
	xui_drawer*						m_drawer;
	xui_treeview*					m_save;
	xui_panel*						m_bottom;
	xui_button*						m_accept;
	xui_button*						m_cancel;
};

#endif//__onity_save_h__