#include "xui_canvas.h"
#include "xui_caretdrawer.h"

xui_caretdrawer::xui_caretdrawer( void )
{
	m_caretrect = xui_rect2d<s32>(0);
	m_caretshow = false;
	m_carettime = 0.0f;
}

const xui_rect2d<s32>& xui_caretdrawer::get_caretrect( void ) const
{
	return m_caretrect;
}

void xui_caretdrawer::set_caretrect( const xui_rect2d<s32>& rect )
{
	if (m_caretrect != rect)
	{
		m_caretrect  = rect;
		reset();
	}
}

void xui_caretdrawer::update( f32 delta )
{
	m_carettime += delta;

	if (m_carettime > 0.5f)
	{
		m_caretshow = !m_caretshow;
		m_carettime = 0.0f;
	}
}

void xui_caretdrawer::render( void )
{
	if (m_caretshow)
	{
		xui_canvas::get_ins()->fill_rectangle(m_caretrect, xui_colour::k_white);
	}
}

void xui_caretdrawer::reset( void )
{
	m_caretshow = true;
	m_carettime = 0.0f;
}