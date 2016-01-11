#ifndef __xui_timehead_h__
#define __xui_timehead_h__

#include "xui_control.h"

class xui_timehead : public xui_control
{
public:
	/*
	//constructor
	*/
	xui_timehead( void );

	/*
	//selected frame
	*/
	//bool						was_selafter		( s32 frame ) const;
	//bool						has_selframe		( void ) const;
	//const std::vector<s32>&		get_selframe		( void ) const;
	//void						set_selframe		( const std::vector<s32>& selframe );

protected:
	/*
	//override
	*/
	virtual void				on_mousedown		( xui_method_mouse& args );
	virtual void				on_mousemove		( xui_method_mouse& args );
	virtual void				on_mouserise		( xui_method_mouse& args );
	virtual void				on_renderself		( xui_method_args&  args );

	/*
	//method
	*/
	s32							hit_frame			( s32 x );

	/*
	//member
	*/
	xui_vector<s32>				m_downrela;
	xui_vector<s32>				m_currrela;
};

#endif//__xui_timehead_h__