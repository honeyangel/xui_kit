#ifndef  __xui_render_window_h__
#define  __xui_render_window_h__

#include "xui_header.h"

class xui_render_window
{
public:
	xui_render_window( xui_native_window* hwnd );

    xui_native_window*  get_hwnd	( void );
	void			    current		( void );
	void			    present		( void );

protected:
    xui_native_window*	m_hwnd;
};

#endif//__xui_render_window_h__