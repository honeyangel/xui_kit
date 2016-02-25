#ifndef __onity_console_h__
#define __onity_console_h__

#include "xui_dockpage.h"

class onity_console : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_console( void );

protected:
	/*
	//event
	*/
	void				on_toggletextchanged	( xui_component* sender, xui_method_args& args );
	void				on_toggleclick			( xui_component* sender, xui_method_args& args );
	void				on_clearclick			( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*			m_clear;
	xui_toggle*			m_error;
	xui_toggle*			m_warning;
	xui_toggle*			m_message;
	xui_panel*			m_head;
	xui_treeview*		m_tree;
};

#endif//__onity_console_h__