#ifndef __cocos_toolbox_h__
#define __cocos_toolbox_h__

#include "xui_dockpage.h"

class cocos_toolpane;
class cocos_toolbox : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_toolbox( void );

protected:
	/*
	//override
	*/
	void				on_fillperform			( xui_component* sender, xui_method_args& args );
	void				on_fillrenderelse		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_panel*			m_fill;
	cocos_toolpane*		m_object;
	cocos_toolpane*		m_controller;
	cocos_toolpane*		m_container;
	cocos_toolpane*		m_custom;
};

#endif//__cocos_toolbox_h__