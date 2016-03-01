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

protected:
	/*
	//event
	*/
	void				on_viewload			( xui_component* sender, xui_method_args&  args );
	void				on_viewupdateself	( xui_component* sender, xui_method_args&  args );
	void				on_viewrenderself	( xui_component* sender, xui_method_args&  args );
	void				on_viewsetrendersz	( xui_component* sender, xui_method_args&  args );
	void				on_viewmousedown	( xui_component* sender, xui_method_mouse& args );
	void				on_viewmousemove	( xui_component* sender, xui_method_mouse& args );
	void				on_viewmouserise	( xui_component* sender, xui_method_mouse& args );
	void				on_viewkeybddown	( xui_component* sender, xui_method_keybd& args );
	void				on_viewkeybdrise	( xui_component* sender, xui_method_keybd& args );
	void				on_viewkeybdchar	( xui_component* sender, xui_method_keybd& args );

	/*
	//member
	*/
	onity_renderview*	m_view;
};

#endif//__onity_game_h__