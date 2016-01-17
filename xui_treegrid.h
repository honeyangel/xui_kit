#ifndef __xui_treegrid_h__
#define __xui_treegrid_h__

#include "xui_control.h"

class xui_treegrid : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_treegrid( xui_component* parent, u32 index );

protected:
	/*
	//callback
	*/
	virtual void	on_mousemove( xui_method_mouse& args );

	/*
	//member
	*/
	u32				m_index;
};

#endif//__xui_treegrid_h__