#ifndef __xui_scrollthumb_h__
#define __xui_scrollthumb_h__

#include "xui_button.h"

class xui_scrollthumb : public xui_button
{
public:
	/*
	//constructor
	*/
	xui_scrollthumb( const std::string& name, const xui_rect2d<s32>& rect, u08 style, xui_component* parent );

protected:
	/*
	//callback
	*/
	virtual void	on_mousemove	( xui_method_mouse& args );
	virtual void	on_mouserise	( xui_method_mouse& args );

	/*
	//member
	*/
	u08				m_style;
};

#endif//__xui_scrollthumb_h__