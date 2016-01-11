#ifndef __xui_caret_h__
#define __xui_caret_h__

#include "xui_rect2d.h"

class xui_caret
{
public:
	/*
	//constructor
	*/
	xui_caret( void );

	/*
	//rectangle
	*/
	const xui_rect2d<s32>&	get_caretrect	( void ) const;
	void					set_caretrect	( const xui_rect2d<s32>& rect );

	/*
	//method
	*/
	void					update			( f32 delta );
	void					render			( void );
	void					reset			( void );

protected:
	/*
	//member
	*/
	xui_rect2d<s32>			m_caretrect;
	bool					m_caretshow;
	f32						m_carettime;
};

#endif//__xui_caret_h__