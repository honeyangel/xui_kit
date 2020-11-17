#include "xui_native_device.h"
#include "xui_native_window.h"
#include "xui_bitmap_buffer.h"
#include "xui_global.h"
#include "xui_component.h"
#include "xui_syswnd.h"
#include "xui_render_window.h"
#include "glfw3.h"

std::wstring    xui_native_device::s_work_path;
xui_notify_vec	xui_native_device::s_notify_vec;
xui_cursor_map  xui_native_device::s_cursor_map;
u08*            xui_native_device::s_screen_buf = NULL;

std::wstring xui_native_device::pathstyle_replace( const std::wstring& path, wchar_t src, wchar_t dst )
{
    std::wstring result = path;
    for (u32 i = 0; i < result.length(); ++i)
    {
        if (result[i] == src)
            result[i]  = dst;
    }

    return result;
}

xui_vector<s32> xui_native_device::get_cursor_absolute( void )
{
    return xui_native_window::get_global_window()->get_cursor_absolute();
}

void xui_native_device::add_cursor( u32 cursor, const xui_bitmap_buffer* buffer )
{
    if (buffer == nullptr || buffer->get_format() != xui_bitmap_format::k_r8g8b8a8)
        return;

    xui_cursor_map::iterator itor = s_cursor_map.find(cursor);
    if (itor != s_cursor_map.end())
    {
        glfwDestroyCursor((*itor).second);
        s_cursor_map.erase(itor);
    }

    GLFWimage img;
    img.width  = buffer->get_size().w;
    img.height = buffer->get_size().h;
    img.pixels = (u08*)buffer->get_data();

    s_cursor_map[cursor] = glfwCreateCursor(&img, 0, 0);
}

void xui_native_device::set_cursor( u32 cursor )
{
    add_cursor_standard(cursor);

    xui_cursor_map::iterator itor = s_cursor_map.find(cursor);
    if (itor != s_cursor_map.end())
    {
        set_cursor_internal((*itor).second);
    }
}

const char* xui_native_device::get_clipboard_string( void )
{
    return glfwGetClipboardString(NULL);
}

void xui_native_device::set_clipboard_string( const char* text )
{
    glfwSetClipboardString(NULL, text);
}

void xui_native_device::add_cursor_standard( u32 cursor )
{
    xui_cursor_map::iterator itor = s_cursor_map.find(cursor);
    if (itor == s_cursor_map.end())
    {
        int glfwCursor = GLFW_ARROW_CURSOR;
        switch (cursor)
        {
        case k_cursor_ns:		glfwCursor = GLFW_VRESIZE_CURSOR;	    break;
        case k_cursor_we:		glfwCursor = GLFW_HRESIZE_CURSOR;	    break;
        case k_cursor_nwse:		glfwCursor = GLFW_NWSE_RESIZE_CURSOR;	break;
        case k_cursor_nesw:		glfwCursor = GLFW_NESW_RESIZE_CURSOR;   break;
        case k_cursor_text:		glfwCursor = GLFW_IBEAM_CURSOR;	        break;
        case k_cursor_hand:		glfwCursor = GLFW_HAND_CURSOR;	        break;
        case k_cursor_drag:		glfwCursor = GLFW_HAND_CURSOR;	        break;
        case k_cursor_forbid:	glfwCursor = GLFW_ARROW_CURSOR;	        break;
        default:
            break;
        }

        s_cursor_map[cursor] = glfwCreateStandardCursor(glfwCursor);
    }
}

void xui_native_device::set_cursor_internal( GLFWcursor* glfw_cursor )
{
    glfwSetCursor(xui_native_window::get_global_window()->get_glfw_window(), glfw_cursor);

    std::vector<xui_syswnd*> all_syswnd = xui_global::get_syswndall();
    for (u32 i = 0; i < all_syswnd.size(); ++i)
    {
        xui_syswnd* syswnd = all_syswnd[i];
        if (syswnd && 
            syswnd->get_renderwnd() && 
            syswnd->get_renderwnd()->get_hwnd())
            glfwSetCursor(syswnd->get_renderwnd()->get_hwnd()->get_glfw_window(), glfw_cursor);
    }
}

void xui_native_device::del_screen_shot( void )
{
    if (s_screen_buf != NULL)
    {
        delete[] s_screen_buf;
        s_screen_buf = NULL;
    }
}

bool xui_native_device::has_screen_shot( void )
{
    return s_screen_buf != NULL;
}

xui_vector<f32> xui_native_device::get_window_scale( void )
{
    return xui_native_window::get_global_window()->get_scale();
}

xui_colour xui_native_device::get_screen_color( const xui_vector<s32>& pt )
{
    xui_vector<s32> screen_size = get_screen_size();
    s32 x = pt.x;
    s32 y = pt.y;
    if (x < 0)              x = 0;
    if (x >= screen_size.w) x = screen_size.w - 1;

    if (y < 0)              y = 0;
    if (y >= screen_size.h) y = screen_size.h - 1;

    u32 index = (y * screen_size.w + x) * 3;
    u08 b = s_screen_buf[index];
    u08 g = s_screen_buf[index + 1];
    u08 r = s_screen_buf[index + 2];
    return xui_colour(1.0f, r / 255.0f, g / 255.0f, b / 255.0f);
}

void xui_native_device::add_fwatch( u32 eventid, const std::wstring& srcpath, const std::wstring& dstpath )
{
    xui_notify_info info;
    info.eventid = eventid;
    info.srcpath = pathstyle_replace(srcpath, L'\\', L'/');
    info.dstpath = pathstyle_replace(dstpath, L'\\', L'/');
    if (info.srcpath.length() > 0) info.srcpath.erase(0, s_work_path.length());
    if (info.dstpath.length() > 0) info.dstpath.erase(0, s_work_path.length());

    s_notify_vec.push_back(info);
}

void xui_native_device::del_fwatch( void )
{
    s_notify_vec.clear();
}

const xui_notify_vec& xui_native_device::get_fwatch( void )
{
    return s_notify_vec;
}
