#ifndef __xui_toggle_h__
#define __xui_toggle_h__

#include "xui_button.h"

enum
{
	TOGGLE_BUTTON,
	TOGGLE_NORMAL,
	TOGGLE_CIRCLE,
};

class xui_toggle : public xui_button
{
public:
	/*
	//constructor
	*/
	xui_toggle( const std::string& name, const xui_rect2d<s32>& rect, u08 style = TOGGLE_BUTTON );

	/*
	//init
	*/
	void						ini_toggle		( bool flag );

	/*
	//check
	*/
	bool						was_check		( void ) const;
	void						set_check		( bool flag );

	/*
	//style
	*/
	u08							get_style		( void ) const;
	void						set_style		( u08 style );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_click;

protected:
	/*
	//callback
	*/
	virtual void				on_mousedown	( xui_method_mouse& args);
	virtual void				on_renderself	( xui_method_args&  args);

	/*
	//virtual
	*/
	virtual xui_colour			get_rendercolor	( void ) const;
	virtual xui_bitmap*			get_rendericon	( void ) const;
	virtual xui_vector<s32>		get_rendericonpt( void ) const;
	virtual xui_rect2d<s32>		get_rendertextrt( void ) const;

	/*
	//member
	*/
	bool						m_check;
	u08							m_style;
};

#endif//__xui_toggle_h__