#include "xui_syswnd_event.h"
#include "xui_global.h"
#include "xui_window.h"
#include "xui_desktop.h"
#include "xui_syswnd.h"
#include "xui_render_window.h"
#include "xui_native_device.h"
#include "xui_native_window.h"

void xui_syswnd_event::on_window_sz( xui_native_window* hwnd, const xui_vector<s32>& size )
{
    xui_syswnd* syswnd = xui_global::get_syswnd(hwnd);
    if (syswnd)
    {
        xui_vector<s32> parent_pt = xui_native_window::get_global_window()->get_pt();
        xui_vector<s32> pt = hwnd->get_pt();

        s32 x = pt.x - parent_pt.x;
        s32 y = pt.y - parent_pt.y;

        xui_window* window = syswnd->get_popupctrl();
        window->set_owner(NULL);
        window->set_renderpt(xui_vector<s32>(x, y));
        window->set_rendersz(hwnd->get_sz());
        window->set_owner(syswnd);
    }
}

bool xui_syswnd_event::on_window_close( xui_native_window* hwnd )
{
    xui_syswnd* syswnd = xui_global::get_syswnd(hwnd);
    if (syswnd)
    {
        xui_global::set_syswndclose(syswnd);
    }

    return true;
}