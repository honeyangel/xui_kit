#include "xui_native_window.h"
#include "xui_global.h"
#include "glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#include "glfw3native.h"

#define WM_USER_FWATCHNOTIFY WM_USER + 0x1000

HWND xui_native_window::get_win32_handler( void )
{
    if (m_glfw_window)
    {
        return glfwGetWin32Window(m_glfw_window);
    }

    return NULL;
}

bool xui_native_window::was_inside_minimized( const xui_vector<s32>& pt )
{
    if (m_glfw_window)
    {
        HWND handle = get_win32_handler();

        WINDOWPLACEMENT wp = {};
        wp.length = sizeof(WINDOWPLACEMENT);
        ::GetWindowPlacement(handle, &wp);

        return xui_abs(wp.ptMinPosition.x - pt.x) <= 2 && xui_abs(wp.ptMinPosition.y - pt.y) <= 2;
    }

	return false;
}

void xui_native_window::capture( void )
{
    if (m_glfw_window)
    {
        ::SetCapture(get_win32_handler());
    }
}

void xui_native_window::close( void )
{
    if (m_glfw_window)
    {
        SendMessage(get_win32_handler(), WM_CLOSE, 0, 0);
    }
}

int xui_native_window::on_window_proc( GLFWwindow* glfw_window, u32 msg, void* wparam, void* lparam )
{
    switch (msg)
    {
    case WM_CAPTURECHANGED:
        {
            HWND other_hwnd = (HWND)lparam;
            if (other_hwnd)
            {
                //extern HWND gHWND;
                //if (xui_global::get_syswnd(other_hwnd) == NULL || other_hwnd != gHWND)
                //{
                //    if (xui_desktop::get_ins()->get_catchctrl() != NULL)
                //    {
                //        POINT pt;
                //        GetCursorPos(&pt);
                //        ScreenToClient(hwnd, &pt);

                //        xui_method_mouse args;
                //        args.wparam = hwnd;
                //        args.mouse = MB_L;
                //        args.point = xui_vector<s32>((s32)pt.x, (s32)pt.y);
                //        args.ctrl = false;
                //        args.shift = false;
                //        args.alt = false;
                //        xui_desktop::get_ins()->os_mouserise(args);
                //    }
                //}
            }
        }
        break;
    case WM_USER_FWATCHNOTIFY:
        {
            long eventid;
            PIDLIST_ABSOLUTE *rgpidl;
            HANDLE notify_lock = SHChangeNotification_Lock((HANDLE)wparam, (DWORD)lparam, &rgpidl, &eventid);
            if (notify_lock)
            {
                std::wstring srcpath;
                std::wstring dstpath;

                wchar_t buffer[MAX_PATH];
                if (rgpidl[0])
                {
                    SHGetPathFromIDList(rgpidl[0], buffer);
                    srcpath = buffer;
                }
                if (rgpidl[1])
                {
                    SHGetPathFromIDList(rgpidl[1], buffer);
                    dstpath = buffer;
                }

                if (eventid == SHCNE_UPDATEITEM && srcpath.length() > 0)
                {
                    xui_global::add_fwatch(eventid, srcpath, dstpath);
                }
            }
            SHChangeNotification_Unlock(notify_lock);
        }
        break;
    }

    return 0;
}