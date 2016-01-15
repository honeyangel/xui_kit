#ifndef __xui_border_h__
#define __xui_border_h__

#include "xui_component.h"

enum 
{
	RESIZEFLAG_L,
	RESIZEFLAG_T,
	RESIZEFLAG_R,
	RESIZEFLAG_B,
};

class xui_border : public xui_component
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_border( const xui_vector<s32>& size, xui_component* parent, u08 flag );

	/*
	//flag
	*/
	u08				get_flag		( void ) const;

protected:
	/*
	//callback
	*/
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_mouserise	( xui_method_mouse& args );
	virtual void	on_topdraw		( xui_method_args&  args );

	/*
	//member
	*/
	u08				m_flag;
	xui_vector<s32>	m_down;
};

#endif//__xui_border_h__