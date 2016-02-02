#ifndef  __xui_render_window_h__
#define  __xui_render_window_h__

#include "xui_header.h"

class xui_render_window
{
public:
	/*
	//constructor
	*/
	xui_render_window( void* hwnd );

	/*
	//destructor
	*/
	virtual ~xui_render_window( void );

	/*
	//method
	*/
	void*			get_hwnd	( void );
	void			current		( void );
	void			present		( void );

protected:
	/*
	//member
	*/
	void*			m_hwnd;
	void*			m_hdc;
	void*			m_context;
};

#endif//__xui_render_window_h__