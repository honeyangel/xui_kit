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
	void				on_viewrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	onity_renderview*	m_view;
};

#endif//__onity_game_h__