#ifndef __xui_caretdrawer_h__
#define __xui_caretdrawer_h__

#include "xui_rect2d.h"

class xui_caretdrawer
{
public:
	xui_caretdrawer( void );

	const xui_rect2d<s32>&	get_caretrect	( void ) const;
	void					set_caretrect	( const xui_rect2d<s32>& rect );
	void					update			( f32 delta );
	void					render			( void );
	void					reset			( void );

protected:
	xui_rect2d<s32>			m_caretrect;
	bool					m_caretshow;
	f32						m_carettime;
};

#endif//__xui_caretdrawer_h__