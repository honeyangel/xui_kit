#ifndef __xui_slider_h__
#define __xui_slider_h__

#include "xui_scrollarrow.h"
#include "xui_scroll.h"

class xui_slider : public xui_scroll
{
	xui_declare_rtti

public:
    xui_slider( const xui_vector<s32>& size, u08 style, u08 arrowdraw );

	static xui_slider*	create			( u08 style, u08 arrowdraw = k_arrowdraw_none );

protected:
	virtual void		on_mousedown	( xui_method_mouse& args );
	virtual void		on_renderback	( xui_method_args&  args );
};

#endif//__xui_slider_h__