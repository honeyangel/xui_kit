#ifndef __xui_border_h__
#define __xui_border_h__

#include "xui_component.h"

enum 
{
	k_resizeflag_l,
	k_resizeflag_t,
	k_resizeflag_r,
	k_resizeflag_b,
};

class xui_border : public xui_component
{
	xui_declare_rtti

public:
	xui_border( const xui_vector<s32>& size, u08 resize );

	u08				get_resize		( void ) const;

protected:
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_mouserise	( xui_method_mouse& args );
	virtual void	on_topdraw		( xui_method_args&  args );

	u08				m_resize;
	xui_vector<s32>	m_downpt;
};

#endif//__xui_border_h__