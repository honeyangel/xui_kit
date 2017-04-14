#ifndef __onity_save_h__
#define __onity_save_h__

#include "xui_window.h"
#include "onity_propfile.h"

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
	void							load_unsavedfiles		( void );

protected:
	/*
	//callback
	*/
	virtual void					on_updateself			( xui_method_update& args );
	virtual void					on_accept				( xui_component* sender, xui_method_args&  args );
	virtual void					on_cancel				( xui_component* sender, xui_method_args&  args );

	/*
	//event
	*/
	void							on_toggleclick			( xui_component* sender, xui_method_args&  args );
	void							on_buttonclick			( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void							save					( void );
	void							quit					( void );
	
	/*
	//member
	*/
	xui_drawer*						m_header;
	xui_treeview*					m_save;
	xui_panel*						m_bottom;
	xui_toggle*						m_selectall;
	xui_drawer*						m_selecttxt;
	xui_toolbar*					m_tool;
	xui_button*						m_yes;
	xui_button*						m_no;
	xui_button*						m_cancel;
};

#endif//__onity_save_h__