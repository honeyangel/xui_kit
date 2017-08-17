#ifndef __cocos_toolpane_h__
#define __cocos_toolpane_h__

#include "xui_control.h"

class cocos_toolpane : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_toolpane( const std::wstring& name );

	/*
	//method
	*/
	void				add_toolctrl		( xui_bitmap* icon, const std::wstring& name );

protected:
	/*
	//override
	*/
	virtual void		on_invalid			( xui_method_args& args );

	/*
	//event
	*/
	void				on_plusctrlexpand	( xui_component* sender, xui_method_args&		args );
	void				on_toollineinvalid	( xui_component* sender, xui_method_args&		args );
	void				on_toollineperform	( xui_component* sender, xui_method_args&		args );
	void				on_toolctrlmousedrag( xui_component* sender, xui_method_dragdrop&	args );

	/*
	//member
	*/
	xui_panel*			m_nameline;
	xui_drawer*			m_namectrl;
	xui_plusctrl*		m_plusctrl;
	xui_panel*			m_toolline;
};

#endif//__cocos_toolpane_h__