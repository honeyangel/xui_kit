#ifndef __onity_scene_h__
#define __onity_scene_h__

#include "xui_dockpage.h"
#include "onity_renderview.h"

class onity_scene : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_scene( void );

protected:
	/*
	//event
	*/
	void				on_viewupdateself	( xui_component* sender, xui_method_update& args );
	void				on_viewrenderself	( xui_component* sender, xui_method_args&	args );
	void				on_viewsetrendersz	( xui_component* sender, xui_method_args&	args );
	void				on_viewmousedown	( xui_component* sender, xui_method_mouse&	args );
	void				on_viewmousemove	( xui_component* sender, xui_method_mouse&	args );
	void				on_viewmouserise	( xui_component* sender, xui_method_mouse&	args );

	/*
	//member
	*/
	xui_panel*			m_head;
	onity_renderview*	m_view;
};

#endif//__onity_scene_h__