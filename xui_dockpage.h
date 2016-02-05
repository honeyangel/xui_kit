#ifndef __xui_dockpage_h__
#define __xui_dockpage_h__

#include "xui_control.h"

class xui_dockpage : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_dockpage( const xui_vector<s32>& size );

	/*
	//method
	*/
	const std::wstring&		get_pagename	( void ) const;
	void					set_pagename	( const std::wstring& name );

	/*
	//override
	*/
	virtual xui_rect2d<s32>	get_renderrtins	( void ) const;

	/*
	//virtual
	*/
	virtual xui_component*	choose			( const xui_vector<s32>& pt );
	virtual void			update			( f32 delta );
	virtual void			render			( void );
	virtual xui_component*	choose_else		( const xui_vector<s32>& pt );
	virtual void			render_else		( void );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback		( xui_method_args& args );
	virtual void			on_topdraw			( xui_method_args& args );

	/*
	//event
	*/
	void					on_namemousedown	( xui_component* sender, xui_method_mouse& args );
	void					on_namemousemove	( xui_component* sender, xui_method_mouse& args );
	void					on_namemouserise	( xui_component* sender, xui_method_mouse& args );

	/*
	//member
	*/
	xui_drawer*				m_name;
};

#endif//__xui_dockpage_h__