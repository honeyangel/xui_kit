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
	//static
	*/
	static xui_toggle*			circle			( void );
	static xui_toggle*			create			( void );
	static xui_toggle*			create			( xui_bitmap* icon, s32 width );
	static xui_toggle*			create			( xui_bitmap* icon, const std::wstring& text, s32 width );

	/*
	//constructor
	*/
	xui_toggle( const xui_vector<s32>& size, u08 drawstyle = TOGGLE_CIRCLE );

	/*
	//destructor
	*/
	virtual ~xui_toggle( void );

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
	//menu
	*/
	xui_menu*					get_menu		( void );
	void						set_menu		( xui_menu* menu );

	/*
	//virtual
	*/
	virtual xui_colour			get_rendercolor	( void ) const;

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
	//member
	*/
	xui_menu*					m_menu;
	bool						m_push;
	u08							m_drawstyle;
};

#endif//__xui_toggle_h__