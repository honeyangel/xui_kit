#ifndef __xui_syswnd_h__
#define __xui_syswnd_h__

#include "xui_header.h"
#include "xui_rect2d.h"

class xui_syswnd
{
public:
	/*
	//constructor
	*/
	xui_syswnd( HWND hwnd, xui_window* popupctrl );

	/*
	//destructor
	*/
	virtual ~xui_syswnd( void );

	/*
	//method
	*/
	xui_window*			get_popupctrl	( void );
	xui_render_window*	get_renderwnd	( void );

	/*
	//virtual
	*/
	virtual void		update			( f32 delta );
	virtual void		render			( void );

protected:
	/*
	//member
	*/
	xui_render_window*	m_renderwnd;
	xui_window*			m_popupctrl;
};

#endif//__xui_syswnd_h__