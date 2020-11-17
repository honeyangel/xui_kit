#include "xui_window.h"
#include "xui_render_window.h"
#include "xui_syswnd.h"

xui_syswnd::xui_syswnd( xui_native_window* hwnd, xui_window* popupctrl )
: m_popupctrl(popupctrl)
{
	m_renderwnd = new xui_render_window(hwnd);
	m_popupctrl->set_owner(this);
}

xui_syswnd::~xui_syswnd( void )
{
	m_popupctrl->set_owner(NULL);
	delete m_renderwnd;
}

xui_window* xui_syswnd::get_popupctrl( void )
{
	return m_popupctrl;
}

xui_render_window* xui_syswnd::get_renderwnd( void )
{
	return m_renderwnd;
}