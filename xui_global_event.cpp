#include "xui_global.h"
#include "xui_syswnd.h"
#include "xui_desktop.h"
#include "xui_syswnd.h"
#include "xui_window.h"
#include "xui_render_window.h"
#include "xui_native_window.h"
#include "xui_global_event.h"

xui_vector<s32> xui_global_event::s_global_pt = xui_vector<s32>( 100, 100);
xui_vector<s32>	xui_global_event::s_global_sz = xui_vector<s32>(1600, 900);

void xui_global_event::on_window_pt( xui_native_window* hwnd, const xui_vector<s32>& pt )
{
    if (hwnd->was_inside_minimized(pt))
        return;

#if xui_target_platform == xui_platform_win32
    xui_vector<s32> delta = pt - s_global_pt;
    std::vector<xui_syswnd*> vec = xui_global::get_syswndall();
    for (u32 i = 0; i < vec.size(); ++i)
    {
        xui_syswnd* syswnd = vec[i];

        xui_native_window* syshwnd = syswnd->get_renderwnd()->get_hwnd();
        xui_window* window = syswnd->get_popupctrl();
        if (syshwnd && window)
        {
            window->set_owner(NULL);
            window->set_renderpt(window->get_renderpt() - delta);
            window->set_owner(syswnd);
        }
    }
#endif

    s_global_pt = pt;
}

void xui_global_event::on_window_sz( xui_native_window* hwnd, const xui_vector<s32>& sz )
{
    if (xui_desktop::get_ins() && sz.w > 0 && sz.h > 0)
        xui_desktop::get_ins()->set_rendersz(sz);

    s_global_sz = sz;
}

bool xui_global_event::on_window_close( xui_native_window* hwnd )
{
    exit(0);
    return true;
}
