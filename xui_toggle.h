#ifndef __xui_toggle_h__
#define __xui_toggle_h__

#include "xui_button.h"

enum
{
	k_toggle_button,
	k_toggle_normal,
	k_toggle_circle,
};

class xui_toggle : public xui_button
{
	xui_declare_rtti

public:
	static xui_toggle*			circle			( void );
	static xui_toggle*			create			( void );
	static xui_toggle*			create			( xui_bitmap* icon, s32 width );
	static xui_toggle*			create			( xui_bitmap* icon, const std::wstring& text, s32 width );

	xui_toggle( const xui_vector<s32>& size, u08 drawstyle = k_toggle_circle );
	virtual ~xui_toggle( void );

	void						ini_toggle		( bool push );
	bool						was_push		( void ) const;
	void						set_push		( bool push );
	xui_menu*					get_menu		( void );
	void						set_menu		( xui_menu* menu );
	virtual xui_colour			get_rendercolor	( void ) const;

	xui_method<xui_method_args>	xm_toggleclick;

protected:
	virtual void				on_mousedown	( xui_method_mouse& args);
	virtual void				on_renderself	( xui_method_args&  args);

	xui_menu*					m_menu;
	bool						m_push;
	u08							m_drawstyle;
};

#endif//__xui_toggle_h__