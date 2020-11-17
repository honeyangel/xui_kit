#include "xui_canvas.h"
#include "xui_render_window.h"
#include "xui_native_window.h"

xui_render_window::xui_render_window( xui_native_window* hwnd )
: m_hwnd(hwnd)
{}

xui_native_window* xui_render_window::get_hwnd( void )
{
	return m_hwnd;
}

void xui_render_window::current( void )
{
    if (m_hwnd == nullptr)
        return;

    m_hwnd->current();

    xui_vector<s32> size = m_hwnd->get_sz();
	xui_canvas::get_ins()->set_viewport(xui_rect2d<s32>(0, 0, size.w, size.h));
	xui_canvas::get_ins()->clear(xui_colour(1.0f, 0.1f, 0.1f, 0.1f));
	xui_canvas::get_ins()->begin();
}

void xui_render_window::present( void )
{
    if (m_hwnd == nullptr)
        return;

	glFinish();
    m_hwnd->present();
}
