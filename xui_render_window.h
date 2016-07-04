#ifndef  __xui_render_window_h__
#define  __xui_render_window_h__

#include "xui_header.h"

class xui_render_window
{
public:
	/*
	//constructor
	*/
	xui_render_window( HWND hwnd );

	/*
	//static
	*/
	static void		init		( HWND hwnd );

	/*
	//method
	*/
	HWND			get_hwnd	( void );
	void			current		( void );
	void			present		( void );

protected:
	/*
	//member
	*/
	HWND			m_hwnd;
	HDC				m_hdc;
};

#endif//__xui_render_window_h__