#ifndef __xui_button_h__
#define __xui_button_h__

#include "xui_drawer.h"

class xui_button : public xui_drawer
{
	xui_declare_rtti

public:
	static const xui_colour k_default_backcolor;
	static const xui_colour k_default_movecolor;
	static const xui_colour k_default_downcolor;

	static xui_button*			create			( xui_bitmap* icon, s32 width );
	static xui_button*			create			( xui_bitmap* icon, const std::wstring& text, s32 width );

	xui_button( const xui_vector<s32>& size );

	const xui_colour&			get_movecolor	( void ) const;
	void						set_movecolor	( const xui_colour& color );
	const xui_colour&			get_downcolor	( void ) const;
	void						set_downcolor	( const xui_colour& color );

	xui_method<xui_method_args>	xm_buttonclick;

protected:
	virtual void				on_mouseclick	( xui_method_mouse& args );
	virtual void				on_renderself	( xui_method_args&  args );
	virtual xui_colour			get_rendercolor	( void ) const;

	xui_colour					m_movecolor;
	xui_colour					m_downcolor;
};

#endif//__xui_button_h__