#ifndef __xui_slider_h__
#define __xui_slider_h__

#include "xui_scrollarrow.h"
#include "xui_scroll.h"

class xui_slider : public xui_scroll
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_slider( const xui_vector<s32>& size, xui_component* parent, u08 style, u08 arrowdraw );

protected:
	/*
	//callback
	*/
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_renderback	( xui_method_args&  args );
};

#endif//__xui_slider_h__