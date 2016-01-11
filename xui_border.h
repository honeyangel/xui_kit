#ifndef __xui_border_h__
#define __xui_border_h__

#include "xui_componet.h"

enum 
{
	RESIZEFLAG_L,
	RESIZEFLAG_T,
	RESIZEFLAG_R,
	RESIZEFLAG_B,
};

class xui_border : public xui_componet
{
public:
	/*
	//constructor
	*/
	xui_border( const std::string& name, const xui_rect2d<s32>& rect, u08 flag, xui_control* parent );

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