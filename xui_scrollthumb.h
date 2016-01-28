#ifndef __xui_scrollthumb_h__
#define __xui_scrollthumb_h__

#include "xui_button.h"

class xui_scrollthumb : public xui_button
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_scrollthumb( const xui_vector<s32>& size, u08 flowstyle );

protected:
	/*
	//callback
	*/
	virtual void	on_mousemove	( xui_method_mouse& args );
	virtual void	on_mouserise	( xui_method_mouse& args );

	/*
	//member
	*/
	u08				m_flowstyle;
};

#endif//__xui_scrollthumb_h__