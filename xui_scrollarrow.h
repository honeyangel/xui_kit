#ifndef __xui_scrollarrow_h__
#define __xui_scrollarrow_h__

#include "xui_button.h"

class xui_scrollarrow : public xui_button
{
public:
	/*
	//constructor
	*/
	xui_scrollarrow( const std::string& name, const xui_rect2d<s32>& rect, u08 style, s32 direction, xui_component* parent );

	/*
	//method
	*/
	bool			was_arrowdraw	( void ) const;
	void			set_arrowdraw	( bool flag );

	/*
	//virtual
	*/
	virtual void	update			( f32 delta );

protected:
	/*
	//callback
	*/
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_mouseclick	( xui_method_mouse& args );
	virtual void	on_renderback	( xui_method_args&  args );

	/*
	//member
	*/
	u08				m_style;
	s32				m_direction;
	f32				m_deltahold;
	bool			m_arrowdraw;
};

#endif//__xui_scrollarrow_h__