#ifndef __xui_dockpage_h__
#define __xui_dockpage_h__

#include "xui_control.h"

class xui_dockpage : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_dockpage*	create				( xui_bitmap* icon, const std::wstring& text );

	/*
	//constructor
	*/
	xui_dockpage( void );

	/*
	//method
	*/
	const std::wstring&		get_pagename		( void ) const;
	void					ini_namectrl		( xui_bitmap* icon, const std::wstring& text );
	void					mov_namectrl		( s32 x, s32 y, s32 w );
	void					add_pagectrl		( xui_component* ctrl );
	void					del_pagectrl		( xui_component* ctrl );

	/*
	//override
	*/
	virtual xui_rect2d<s32>	get_renderrtins		( void ) const;

	/*
	//virtual
	*/
	virtual xui_component*	choose				( const xui_vector<s32>& pt );
	virtual xui_component*	choose_else			( const xui_vector<s32>& pt );
	virtual void			update_else			( f32 delta );
	virtual void			render_else			( void );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback		( xui_method_args& args );

	/*
	//event
	*/
	void					on_namectrlmousedown( xui_component* sender, xui_method_mouse& args );
	void					on_namectrlmouserise( xui_component* sender, xui_method_mouse& args );
	void					on_namectrltopdraw	( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	xui_dockview*			get_dockview		( xui_component* comp );
	u08						cal_dockinfo		( xui_dockview* dockview, const xui_vector<s32>& pt );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
};

#endif//__xui_dockpage_h__