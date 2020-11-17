#ifndef __xui_native_window_h__
#define __xui_native_window_h__

#include "xui_header.h"
#include "xui_vector.h"

class xui_event;
class xui_global_event;
class xui_native_window;
struct GLFWwindow;

typedef std::map<GLFWwindow*, xui_native_window*> xui_native_window_map;

class xui_native_window
{
public:
    struct create_info
    {
        xui_native_window*  parent      = NULL;
        xui_vector<s32>     pt          = xui_vector<s32>(0,   0  );
        xui_vector<s32>     sz          = xui_vector<s32>(400, 400);
        std::string         name        = "";
        bool                resizable   = true;
        bool                decorated   = false;
        xui_native_window*  shared      = NULL;
    };

    xui_native_window( const create_info& info, xui_event* event );
	virtual ~xui_native_window( void );

    static void                     new_global_window           ( const std::string& name, xui_global_event* event );
    static xui_native_window*       get_global_window           ( void );
    static xui_native_window*       get_native_window           ( GLFWwindow* glfw_window );
    static xui_native_window*       get_current                 ( void );

    xui_event*                      get_event                   ( void );
    GLFWwindow*                     get_glfw_window             ( void );

#if   xui_target_platform == xui_platform_win32
    HWND                            get_win32_handler           ( void );
#endif

    void                            set_floating                ( bool flag );
    void                            req_focus                   ( void );
    xui_vector<f32>                 get_scale                   ( void ) const;
    bool                            was_enable                  ( void ) const;
    xui_vector<s32>                 get_pt                      ( void ) const;
    xui_vector<s32>                 get_sz                      ( void ) const;
    void                            set_pt                      ( const xui_vector<s32>& pt );
    void                            set_sz                      ( const xui_vector<s32>& sz );

    xui_vector<s32>                 get_cursor_position         ( void ) const;
    xui_vector<s32>                 get_cursor_absolute         ( void ) const;
    bool                            was_inside                  ( void ) const;
    s32                             get_key_state               ( s32 key ) const;

    void                            current                     ( void );
    void                            present                     ( void );
    bool                            should_close                ( void );

    void                            set_enable                  ( bool flag );
    bool                            was_inside_minimized        ( const xui_vector<s32>& pt );
    void                            capture                     ( void );
    void                            close                       ( void );

protected:
    static void                     on_mouse_move               ( GLFWwindow* glfw_window, f64 x, f64 y );
    static void                     on_mouse_button             ( GLFWwindow* glfw_window, s32 button, s32 action, s32 mods );
    static void                     on_mouse_scroll             ( GLFWwindow* glfw_window, f64 x, f64 y );
    static void                     on_key                      ( GLFWwindow* glfw_window, s32 key, s32 scancode, s32 action, s32 mods );
    static void                     on_char                     ( GLFWwindow* glfw_window, u32 charCode );
    static void                     on_dropfile                 ( GLFWwindow* glfw_window, s32 count, const char** files );
    static void                     on_window_pt                ( GLFWwindow* glfw_window, s32 x, s32 y );
    static void                     on_window_sz                ( GLFWwindow* glfw_window, s32 w, s32 h );
    static void                     on_window_focus             ( GLFWwindow* glfw_window, s32 focus );
    static void                     on_window_close             ( GLFWwindow* glfw_window );

#if xui_target_platform == xui_platform_win32
    static int                      on_window_proc              ( GLFWwindow* glfw_window, u32 msg, void* wparam, void* lparam );
#endif

    GLFWwindow*                     m_glfw_window;
    xui_event*                      m_event;
    bool                            m_enabled;

    static xui_native_window_map    s_native_window_map;
    static xui_native_window*       s_global_window;
};

#endif//__xui_native_window_h__
