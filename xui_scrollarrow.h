#ifndef __xui_scrollarrow_h__
#define __xui_scrollarrow_h__

#include "xui_button.h"

enum 
{
	k_arrowdraw_none,
	k_arrowdraw_triangle,
	k_arrowdraw_plusandminus,
};

class xui_scrollarrow : public xui_button
{
	xui_declare_rtti

public:
	xui_scrollarrow( const xui_vector<s32>& size, u08 flowstyle, s32 direction, u08 arrowdraw );

	u08				get_arrowdraw	( void ) const;
	void			set_arrowdraw	( u08 arrowdraw );
	virtual void	update			( f32 delta );

protected:
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_mouseclick	( xui_method_mouse& args );
	virtual void	on_renderself	( xui_method_args&  args );

	u08				m_flowstyle;
	u08				m_arrowdraw;
	s32				m_direction;
	f32				m_deltahold;
};

#endif//__xui_scrollarrow_h__