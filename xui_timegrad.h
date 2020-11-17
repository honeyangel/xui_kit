#ifndef __xui_timegrad_h__
#define __xui_timegrad_h__

#include "xui_drawer.h"

class xui_timegrad : public xui_drawer
{
	xui_declare_rtti

public:
	xui_timegrad( xui_timeview* timeview );

protected:
	virtual void on_mousedown	( xui_method_mouse& args );
	virtual void on_mousemove	( xui_method_mouse& args );
	virtual void on_renderself	( xui_method_args&  args );

	void		set_curframe	( s32 x );
};

#endif//__xui_timegrad_h__