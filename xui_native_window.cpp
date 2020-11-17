#include "xui_native_window.h"
#include "xui_global_event.h"
#include "xui_event.h"
#include "glfw3.h"

xui_native_window*      xui_native_window::s_global_window = NULL;
xui_native_window_map   xui_native_window::s_native_window_map;

void xui_native_window::new_global_window( const std::string& name, xui_global_event* event )
{
    xui_native_window::create_info info;
    info.pt         = xui_global_event::s_global_pt;
    info.sz         = xui_global_event::s_global_sz;
    info.decorated  = true;
    info.resizable  = true;
    info.name       = name;

    s_global_window = new xui_native_window(info, event);
}

xui_native_window* xui_native_window::get_global_window( void )
{
    return s_global_window;
}

xui_native_window* xui_native_window::get_native_window( GLFWwindow* glfw_window )
{
    xui_native_window_map::iterator itor = s_native_window_map.find(glfw_window);
    if (itor != s_native_window_map.end())
    {
        return (*itor).second;
    }

    return NULL;
}

xui_native_window* xui_native_window::get_current( void )
{
    GLFWwindow* glfw_window = glfwGetCurrentContext();
    if (glfw_window == NULL)
        return NULL;

    return get_native_window(glfw_window);
}

xui_native_window::xui_native_window( const create_info& info, xui_event* event )
: m_glfw_window(nullptr)
, m_event(event)
, m_enabled(true)
{
    glfwWindowHint(GLFW_DECORATED,  info.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE,    GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE,  info.resizable ? GLFW_TRUE : GLFW_FALSE);

    s32 w = info.sz.w <= 0 ? 4 : info.sz.w;
    s32 h = info.sz.h <= 0 ? 4 : info.sz.h;
    
    m_glfw_window = glfwCreateWindow(
        w,
        h,
        info.name.c_str(),
        NULL,
        (info.shared ? info.shared->get_glfw_window() : NULL),
        (info.parent ? info.parent->get_glfw_window() : NULL));

    set_sz(xui_vector<s32>(w, h));
    set_pt(info.pt);

    s_native_window_map[m_glfw_window] = this;

#define GLFW_FUNCTION_SET(x, func)  x(m_glfw_window, func);
    GLFW_FUNCTION_SET(glfwSetCursorPosCallback,     on_mouse_move)
    GLFW_FUNCTION_SET(glfwSetMouseButtonCallback,   on_mouse_button);
    GLFW_FUNCTION_SET(glfwSetScrollCallback,        on_mouse_scroll)
    GLFW_FUNCTION_SET(glfwSetKeyCallback,           on_key)
    GLFW_FUNCTION_SET(glfwSetCharCallback,          on_char)
    GLFW_FUNCTION_SET(glfwSetDropCallback,          on_dropfile)
    GLFW_FUNCTION_SET(glfwSetWindowPosCallback,     on_window_pt)
    GLFW_FUNCTION_SET(glfwSetWindowSizeCallback,    on_window_sz)
    GLFW_FUNCTION_SET(glfwSetWindowFocusCallback,   on_window_focus)
    GLFW_FUNCTION_SET(glfwSetWindowCloseCallback,   on_window_close)
#if xui_target_platform == xui_platform_win32
    GLFW_FUNCTION_SET(glfwSetWindowProcCallback,    on_window_proc)
#endif
#undef  GLFW_FUNCTION_SET

    glfwShowWindow(m_glfw_window);
}

xui_native_window::~xui_native_window( void )
{
    if (m_glfw_window)
    {
        xui_native_window_map::iterator itor = s_native_window_map.find(m_glfw_window);
        if (itor != s_native_window_map.end())
        {
            s_native_window_map.erase(itor);
        }

        glfwDestroyWindow(m_glfw_window);
        m_glfw_window = NULL;
    }

    if (m_event)
    {
        delete m_event;
        m_event = NULL;
    }
}

xui_event* xui_native_window::get_event( void )
{
    return m_event;
}

GLFWwindow* xui_native_window::get_glfw_window( void )
{
    return m_glfw_window;
}

xui_vector<s32> xui_native_window::get_pt( void ) const
{
    xui_vector<s32> result;
    if (m_glfw_window)
    {
        glfwGetWindowPos(m_glfw_window, &result.x, &result.y);
    }

    return result;
}

xui_vector<s32> xui_native_window::get_sz( void ) const
{
    xui_vector<s32> result;
    if (m_glfw_window)
    {
        glfwGetWindowSize(m_glfw_window, &result.w, &result.h);
    }

    return result;
}

bool xui_native_window::was_enable( void ) const
{
    return m_event != NULL && m_enabled;
}

xui_vector<f32> xui_native_window::get_scale( void ) const
{
    xui_vector<f32> scale = xui_vector<f32>(1.0f);
#if xui_target_platform == xui_platform_macos
    if (m_glfw_window)
    {
        glfwGetWindowContentScale(m_glfw_window, &scale.x, &scale.y);
    }
#endif

    return scale;
}

void xui_native_window::set_floating( bool flag )
{
    if (m_glfw_window)
    {
        glfwSetWindowAttrib(m_glfw_window, GLFW_FLOATING, flag ? GLFW_TRUE : GLFW_FALSE);
    }
}

void xui_native_window::req_focus( void )
{
    if (m_glfw_window)
    {
        glfwFocusWindow(m_glfw_window);
    }
}

void xui_native_window::set_pt( const xui_vector<s32>& pt )
{
    if (m_glfw_window)
    {
        glfwSetWindowPos(m_glfw_window, pt.x, pt.y);
    }
}

void xui_native_window::set_sz( const xui_vector<s32>& sz )
{
    if (m_glfw_window)
    {
        glfwSetWindowSize(m_glfw_window, sz.w, sz.h);
    }
}

void xui_native_window::set_enable( bool flag )
{
    if (m_glfw_window)
    {
        m_enabled = flag;
    }
}

xui_vector<s32> xui_native_window::get_cursor_position( void ) const
{
    xui_vector<f64> result;
    if (m_glfw_window)
    {
        glfwGetCursorPos(m_glfw_window, &result.x, &result.y);
    }

    return result.to<s32>();
}

xui_vector<s32> xui_native_window::get_cursor_absolute( void ) const
{
    return get_pt() + get_cursor_position();
}

bool xui_native_window::was_inside( void ) const
{
    if (m_glfw_window)
    {
        xui_vector<s32> pt = get_cursor_position();
        xui_vector<s32> sz = get_sz();

        return pt.x >= 0 && pt.x < sz.w && pt.y >= 0 && pt.y < sz.h;
    }

    return false;
}

s32 xui_native_window::get_key_state( s32 key ) const
{
    if (m_glfw_window)
    {
        return glfwGetKey(m_glfw_window, key);
    }

    return -1;
}

bool xui_native_window::should_close( void )
{
    if (m_glfw_window)
    {
        return glfwWindowShouldClose(m_glfw_window) != 0;
    }

    return true;
}

void xui_native_window::current( void )
{
    if (m_glfw_window)
    {
        glfwMakeContextCurrent(m_glfw_window);
    }
}

void xui_native_window::present( void )
{
    if (m_glfw_window)
    {
        glfwSwapBuffers(m_glfw_window);
    }
}

void xui_native_window::on_mouse_move( GLFWwindow* glfw_window, f64 x, f64 y )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable())
        hwnd->get_event()->on_mouse_move(hwnd, xui_vector<s32>((s32)x, (s32)y));
}

void xui_native_window::on_mouse_button( GLFWwindow* glfw_window, s32 button, s32 action, s32 mods )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable())
        hwnd->get_event()->on_mouse_button(hwnd, button, action, mods);
}

void xui_native_window::on_mouse_scroll( GLFWwindow* glfw_window, f64 x, f64 y )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable() && hwnd->was_inside())
        hwnd->get_event()->on_mouse_scroll(hwnd, (s32)y);
}

void xui_native_window::on_key( GLFWwindow* glfw_window, s32 key, s32 scancode, s32 action, s32 mods )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable())
        hwnd->get_event()->on_key(hwnd, key, action, mods);
}

void xui_native_window::on_char( GLFWwindow* glfw_window, u32 c )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable())
        hwnd->get_event()->on_char(hwnd, (u16)c);
}

void xui_native_window::on_dropfile( GLFWwindow* glfw_window, s32 count, const char** files )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->was_enable())
    {
        std::vector<std::string> vec;
        for (s32 i = 0; i < count; ++i)
            vec.push_back(std::string(files[i]));

        hwnd->get_event()->on_dropfile(hwnd, vec);
    }
}

void xui_native_window::on_window_pt( GLFWwindow* glfw_window, s32 x, s32 y )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->get_event())
        hwnd->get_event()->on_window_pt(hwnd, xui_vector<s32>(x, y));
}

void xui_native_window::on_window_sz( GLFWwindow* glfw_window, s32 w, s32 h )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->get_event())
        hwnd->get_event()->on_window_sz(hwnd, xui_vector<s32>(w, h));
}

void xui_native_window::on_window_focus( GLFWwindow* glfw_window, s32 focus )
{
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->get_event())
        hwnd->get_event()->on_window_focus(hwnd, focus);
}

void xui_native_window::on_window_close( GLFWwindow* glfw_window )
{
    bool destroy = true;
    xui_native_window* hwnd = get_native_window(glfw_window);
    if (hwnd && hwnd->get_event())
        destroy = hwnd->get_event()->on_window_close(hwnd);

    if (destroy)
    {
        delete hwnd;
    }
}
