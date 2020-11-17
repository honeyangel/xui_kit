#ifndef __xui_native_device_h__
#define __xui_native_device_h__

#include "xui_header.h"
#include "xui_vector.h"
#include "xui_method.h"

struct GLFWcursor;
struct xui_notify_info
{
    u32				eventid;
    std::wstring	srcpath;
    std::wstring	dstpath;
};
typedef std::vector<xui_notify_info> xui_notify_vec;
typedef std::map<u32, GLFWcursor*>   xui_cursor_map;

class xui_bitmap_buffer;
class xui_native_device
{
public:
    static std::wstring                 pathstyle_replace       ( const std::wstring& path, wchar_t src, wchar_t dst );

    static xui_vector<s32>              get_cursor_absolute     ( void );
    static void				            set_cursor              ( u32 cursor );
    static void				            add_cursor              ( u32 cursor, const xui_bitmap_buffer* buffer );

    static const char*                  get_clipboard_string    ( void );
    static void                         set_clipboard_string    ( const char* text );

    static void                         del_screen_shot         ( void );
    static bool                         has_screen_shot         ( void );
    static xui_colour                   get_screen_color        ( const xui_vector<s32>& pt );
    static xui_vector<s32>              get_screen_size         ( void );
    static xui_vector<f32>              get_window_scale        ( void );

    static void							set_fwatchstart         ( const std::wstring& path );
    static void							set_fwatchclose         ( void );
    static void							add_fwatch              ( u32 eventid, const std::wstring& srcpath, const std::wstring& dstpath );
    static void							del_fwatch              ( void );
    static const xui_notify_vec&		get_fwatch              ( void );

    static void                         set_openpath_dialog     ( xui_method<const std::wstring>* callback );
    static std::string                  get_temppath            ( void );
    static const std::wstring&			get_workpath            ( void );
    static void							set_workpath            ( const std::wstring& path );
    static void							set_showfind            ( const std::wstring& path );
    static std::vector<std::wstring>    get_path                ( const std::wstring& path );
    static std::vector<std::wstring>	get_file                ( const std::wstring& path, const std::wstring& filter = L"*.*" );
    static bool							has_path                ( const std::wstring& path );
    static bool							add_path                ( const std::wstring& path );
    static bool							del_file                ( const std::wstring& file );
    static bool							mov_file                ( const std::wstring& src, const std::wstring& dst );
    static bool							cpy_file                ( const std::wstring& src, const std::wstring& dst );
    static bool							rna_file                ( const std::wstring& src, const std::wstring& dst );

    static void                         sleep                   ( f32 seconds );
    static void                         release_capture         ( void );
    static void                         capture_screenshot      ( void );

protected:
    static void                         add_cursor_standard     ( u32 cursor );
    static void                         set_cursor_internal     ( GLFWcursor* glfw_cursor );

    static std::wstring                 s_work_path;
    static xui_notify_vec	            s_notify_vec;
    static xui_cursor_map               s_cursor_map;
    static u08*                         s_screen_buf;
};

#endif//__xui_native_device_h__
