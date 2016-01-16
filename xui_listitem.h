#ifndef __xui_listitem_h__
#define __xui_listitem_h__

#include "xui_drawer.h"

class xui_listitem : public xui_drawer
{
	friend class xui_listview;
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_listitem( xui_component* parent, const xui_rect2d<s32>& border, bool drawtick );

	/*
	//method
	*/
	bool					was_selected	( void ) const;
	s32						get_maxwidth	( void );

	/*
	//override
	*/
	virtual xui_colour		get_rendercolor	( void ) const;
	virtual xui_rect2d<s32>	get_renderrtins	( void ) const;

protected:
	/*
	//callback
	*/
	virtual void			on_setborderrt	( xui_method_args&  args );
	virtual void			on_textchanged	( xui_method_args&  args );
	virtual void			on_fontchanged	( xui_method_args&  args );
	virtual void			on_iconchanged	( xui_method_args&  args );
	virtual void			on_renderself	( xui_method_args&  args );
	virtual void			on_mousedown	( xui_method_mouse& args );
	virtual void			on_keybddown	( xui_method_keybd& args );

	/*
	//method
	*/
	void					set_selected	( bool flag );

	/*
	//member
	*/
	bool					m_selected;
	bool					m_drawtick;
};

#endif//__xui_listitem_h__