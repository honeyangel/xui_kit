#ifndef __xui_syswnd_h__
#define __xui_syswnd_h__

#include "xui_header.h"
#include "xui_rect2d.h"

class xui_syswnd
{
public:
	xui_syswnd(xui_native_window* hwnd, xui_window* popupctrl );
	virtual ~xui_syswnd( void );

	xui_window*			get_popupctrl	( void );
	xui_render_window*	get_renderwnd	( void );

protected:
	xui_render_window*	m_renderwnd;
	xui_window*			m_popupctrl;
};

#endif//__xui_syswnd_h__