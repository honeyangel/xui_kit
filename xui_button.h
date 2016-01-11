#ifndef __xui_button_h__
#define __xui_button_h__

#include "xui_drawer.h"

class xui_button : public xui_drawer
{
public:
	/*
	//constructor
	*/
	xui_button( const std::string& name, const xui_rect2d<s32>& rect );

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
	virtual xui_rect2d<s32>		get_rendertextrt( void ) const;

	/*
	//member
	*/
	xui_colour					m_movecolor;
	xui_colour					m_downcolor;
};

#endif//__xui_button_h__