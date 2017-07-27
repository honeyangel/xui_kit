#ifndef __cocos_save_h__
#define __cocos_save_h__

#include "xui_window.h"
#include "cocos_propfile.h"

class cocos_save : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_save( void );

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
	void							load					( void );
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

#endif//__cocos_save_h__