#ifndef __xui_timerect_h__
#define __xui_timerect_h__

#include "xui_control.h"

class xui_timerect : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_timerect( xui_component* parent );

	/*
	//method
	*/
	xui_rect2d<s32>	get_range		( void ) const;
	void			set_range		( s32 startlayer, s32 finallayer, s32 startframe, s32 finalframe );
	void			set_rangemove	( s32 delta );

protected:
	/*
	//callback
	*/
	virtual void	on_nonfocus		( xui_method_args&  args );
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_mousemove	( xui_method_mouse& args );
	virtual void	on_mouserise	( xui_method_mouse& args );

	/*
	//method
	*/
	s32				hit_frame		(s32 x );

	/*
	//member
	*/
	xui_vector<s32> m_downrela;
	xui_vector<s32> m_currrela;
	s32				m_startlayer;
	s32				m_finallayer;
	s32				m_startframe;
	s32				m_finalframe;
};

#endif//__xui_timerect_h__