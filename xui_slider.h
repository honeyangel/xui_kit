#ifndef __xui_slider_h__
#define __xui_slider_h__

#include "xui_scroll.h"

class xui_slider : public xui_scroll
{
public:
	/*
	//constructor
	*/
	xui_slider( const std::string& name, const xui_rect2d<s32>& rect, u08 style, bool anglerender = false, xui_bitmap** arrowbitmap = NULL );

protected:
	/*
	//callback
	*/
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_renderback	( xui_method_args&  args );

protected:
	/*
	//member
	*/
	bool			m_anglerender;
};

#endif//__xui_slider_h__