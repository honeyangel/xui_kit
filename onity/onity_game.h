#ifndef __onity_game_h__
#define __onity_game_h__

#include "xui_dockpage.h"
#include "onity_renderview.h"

class onity_game : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_game( void );

	/*
	//method
	*/
	void				ini_game			( void );

protected:
	/*
	//callback
	*/
	virtual void		on_perform			( xui_method_args& args );
	virtual void		on_renderself		( xui_method_args& args );

	/*
	//event
	*/
	void				on_menuclick		( xui_component* sender, xui_method_args&	args );
	void				on_viewupdateself	( xui_component* sender, xui_method_update& args );
	void				on_viewrenderself	( xui_component* sender, xui_method_args&	args );
	void				on_viewrenderelse	( xui_component* sender, xui_method_args&   args );
	void				on_viewsetrendersz	( xui_component* sender, xui_method_args&	args );
	void				on_viewmousedown	( xui_component* sender, xui_method_mouse&	args );
	void				on_viewmousemove	( xui_component* sender, xui_method_mouse&	args );
	void				on_viewmouserise	( xui_component* sender, xui_method_mouse&	args );
	void				on_viewkeybddown	( xui_component* sender, xui_method_keybd&	args );
	void				on_viewkeybdrise	( xui_component* sender, xui_method_keybd&	args );
	void				on_viewkeybdchar	( xui_component* sender, xui_method_keybd&	args );

	/*
	//member
	*/
	xui_panel*			m_head;
	onity_renderview*	m_view;
	xui_toggle*			m_aspect;
	xui_menuitem*		m_fill;
	xui_menuitem*		m_iphone4;
	xui_menuitem*		m_iphone5;
	xui_menuitem*		m_iphone6;
	xui_menuitem*		m_iphone6p;
	xui_menuitem*		m_ipad;
};

#endif//__onity_game_h__