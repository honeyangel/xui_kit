#include "xui_convas.h"
#include "xui_caret.h"

/*
//constructor
*/
xui_create_explain(xui_caret)( void )
{
	m_caretrect = xui_rect2d<s32>(0);
	m_caretshow = false;
	m_carettime = 0.0f;
}

/*
//rectangle
*/
xui_method_explain(xui_caret, get_caretrect,	const xui_rect2d<s32>&	)( void ) const
{
	return m_caretrect;
}
xui_method_explain(xui_caret, set_caretrect,	void					)( const xui_rect2d<s32>& rect )
{
	if (m_caretrect != rect)
	{
		m_caretrect  = rect;
		reset();
	}
}

/*
//method
*/
xui_method_explain(xui_caret, update,			void					)( f32 delta )
{
	m_carettime += delta;

	if (m_carettime > 0.5f)
	{
		m_caretshow = !m_caretshow;
		m_carettime = 0.0f;
	}
}
xui_method_explain(xui_caret, render,			void					)( void )
{
	if (m_caretshow)
	{
		g_convas->fill_rectangle(m_caretrect, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
	}
}
xui_method_explain(xui_caret, reset,			void					)( void )
{
	m_caretshow = true;
	m_carettime = 0.0f;
}