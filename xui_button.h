#ifndef __xui_button_h__
#define __xui_button_h__

#include "xui_drawer.h"

class xui_button : public xui_drawer
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const xui_colour default_backcolor;
	static const xui_colour default_sidecolor;
	static const xui_colour default_movecolor;
	static const xui_colour default_downcolor;

	static xui_button*			create			( xui_bitmap* icon );
	static xui_button*			create			( xui_bitmap* icon, const std::wstring& text, s32 width );

	/*
	//constructor
	*/
	xui_button( const xui_vector<s32>& size, xui_component* parent = NULL );

	/*
	//color
	*/
	const xui_colour&			get_movecolor	( void ) const;
	void						set_movecolor	( const xui_colour& color );
	const xui_colour&			get_downcolor	( void ) const;
	void						set_downcolor	( const xui_colour& color );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_click;

protected:
	/*
	//callback
	*/
	virtual void				on_mouseclick	( xui_method_mouse& args );

	/*
	//virtual
	*/
	virtual xui_colour			get_rendercolor	( void ) const;
	virtual xui_vector<s32>		get_rendericonpt( void ) const;

	/*
	//member
	*/
	xui_colour					m_movecolor;
	xui_colour					m_downcolor;
};

#endif//__xui_button_h__