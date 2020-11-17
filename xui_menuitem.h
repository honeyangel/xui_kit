#ifndef __xui_menuitem_h__
#define __xui_menuitem_h__

#include "xui_drawer.h"

class xui_menuitem : public xui_drawer
{
	xui_declare_rtti

public:
	xui_menuitem( void );
	virtual ~xui_menuitem( void );

	s32							get_maxwidth	( void );
	xui_menu*					get_submenu		( void );
	void						set_submenu		( xui_menu* submenu );
	bool						get_flag		( void ) const;
	void						set_flag		( bool flag );
	const std::wstring&			get_hint		( void ) const;
	void						set_hint		( const std::wstring&      hint );
	const xui_family_render&	get_draw		( void ) const;
	void						set_draw		( const xui_family_render& draw );

	virtual xui_colour			get_rendercolor	( void ) const;

	xui_method<xui_method_args>	xm_click;

protected:
	virtual void				on_nonfocus		( xui_method_args&  args );
	virtual void				on_renderself	( xui_method_args&  args );
	virtual void				on_mouseenter	( xui_method_mouse& args );
	virtual void				on_mouseclick	( xui_method_mouse& args );

	bool						m_flag;
	std::wstring				m_hint;
	xui_family_render			m_draw;
	xui_menu*					m_submenu;
};

#endif//__xui_menuitem_h__