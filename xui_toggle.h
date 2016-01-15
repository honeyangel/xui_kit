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
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_toggle( const xui_vector<s32>& size, xui_component* parent, u08 drawstyle = TOGGLE_CIRCLE );

	/*
	//init
	*/
	void						ini_toggle		( bool push );

	/*
	//check
	*/
	bool						was_push		( void ) const;
	void						set_push		( bool push );

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
	virtual xui_vector<s32>		get_rendericonpt( void ) const;

	/*
	//member
	*/
	bool						m_push;
	u08							m_drawstyle;
};

#endif//__xui_toggle_h__