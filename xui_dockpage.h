#ifndef __xui_dockpage_h__
#define __xui_dockpage_h__

#include "xui_control.h"

enum
{
	k_arealimit_n = 0,
	k_arealimit_l = 0x00000001,
	k_arealimit_r = 0x00000002,
	k_arealimit_t = 0x00000004,
	k_arealimit_b = 0x00000008,
	k_arealimit_a = 0xFFFFFFFF,
};

class xui_dockpage : public xui_control
{
	xui_declare_rtti

public:
	static xui_dockpage*	create				( xui_bitmap* icon, const std::wstring& text, s32 size, u32 dockarea, s32 minlimit, u08 initdock );

	xui_dockpage( const xui_vector<s32>& size, u32 dockarea, s32 minlimit, u08 initdock );

	bool					was_autofree		( void ) const;
	void					set_autofree		( bool flag );
	const xui_rect2d<s32>&	get_inborder		( void ) const;
	void					set_inborder		( const xui_rect2d<s32>& border );
	bool					has_dockarea		( u08 dockstyle );
	u08						get_initdock		( void ) const;
	s32						get_minlimit		( void ) const;
	s32						get_namesize		( void ) const;
	const std::wstring&		get_pagename		( void ) const;
	void					ini_namectrl		( xui_bitmap* icon, const std::wstring& text );
	void					mov_namectrl		( s32 x, s32 y, s32 w );
	void					add_pagectrl		( xui_component* ctrl );
	void					del_pagectrl		( xui_component* ctrl );

	virtual xui_rect2d<s32>	get_renderrtins		( void ) const;
	virtual xui_component*	choose				( const xui_vector<s32>& pt );
	virtual xui_component*	choose_else			( const xui_vector<s32>& pt );
	virtual void			update_else			( f32 delta );
	virtual void			render_else			( void );

protected:
	virtual void			on_perform			( xui_method_args& args );
	virtual void			on_renderback		( xui_method_args& args );

	void					on_namectrlmousedown( xui_component* sender, xui_method_mouse& args );
	void					on_namectrlmouserise( xui_component* sender, xui_method_mouse& args );
	void					on_namectrltopdraw	( xui_component* sender, xui_method_args&  args );
	xui_dockview*			get_dockview		( xui_component* comp );
	u08						cal_dockinfo		( xui_dockview* dockview, const xui_vector<s32>& pt );

	xui_drawer*				m_namectrl;
	u08						m_initdock;
	u32						m_dockarea;
	s32						m_minlimit;
	xui_rect2d<s32>			m_inborder;
	bool					m_autofree;
};

#endif//__xui_dockpage_h__