#ifndef __cocos_recent_h__
#define __cocos_recent_h__

#include "xui_window.h"

class cocos_recent : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_recent( void );

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
	void				load_config				( void );
	void				save_config				( void );

	/*
	//member
	*/
	xui_drawer*			m_drawer;
	xui_treeview*		m_recent;
	xui_panel*			m_bottom;
	xui_button*			m_accept;
	xui_button*			m_browse;
};

#endif//__cocos_recent_h__