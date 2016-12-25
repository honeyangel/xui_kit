#ifndef __onity_restore_h__
#define __onity_restore_h__

#include "xui_window.h"

class onity_restore : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_restore( void );

	/*
	//method
	*/
	std::wstring		get_selectpath			( void );

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
	xui_drawer*			m_drawer;
	xui_treeview*		m_restore;
	xui_panel*			m_bottom;
	xui_button*			m_accept;
};

#endif//__onity_restore_h__