#include "xui_window.h"
#include "xui_render_window.h"
#include "xui_syswnd.h"

/*
//constrctor
*/
xui_create_explain(xui_syswnd)( HWND hwnd, xui_window* popupctrl )
: m_popupctrl(popupctrl)
{
	m_renderwnd = new xui_render_window(hwnd);
	m_popupctrl->set_owner(this);
}

/*
//destructor
*/
xui_delete_explain(xui_syswnd)( void )
{
	m_popupctrl->set_owner(NULL);
	delete m_renderwnd;
}

/*
//method
*/
xui_method_explain(xui_syswnd, get_popupctrl,	xui_window*		)( void )
{
	return m_popupctrl;
}
xui_method_explain(xui_syswnd, get_renderwnd,	xui_render_window*	)( void )
{
	return m_renderwnd;
}

/*
//virtual
*/
xui_method_explain(xui_syswnd, update,			void				)( f32 delta )
{
	xui_method_ptrcall(m_popupctrl, update	)(delta);
}
xui_method_explain(xui_syswnd, render,			void				)( void )
{
	xui_method_ptrcall(m_renderwnd, current	)();
	xui_method_ptrcall(m_popupctrl, render	)();
	xui_method_ptrcall(m_renderwnd, present	)();
}