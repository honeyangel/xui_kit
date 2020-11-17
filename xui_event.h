#ifndef __xui_event_h__
#define __xui_event_h__

#include "xui_header.h"
#include "xui_vector.h"

class xui_event
{
public:
    xui_event(){}
    virtual ~xui_event(){}

    virtual void        on_mouse_move       ( xui_native_window* hwnd, const xui_vector<s32>& pt );
    virtual void        on_mouse_button     ( xui_native_window* hwnd, s32 button, s32 action, s32 mods );
    virtual void        on_mouse_scroll     ( xui_native_window* hwnd, s32 value );
    virtual void        on_key              ( xui_native_window* hwnd, s32 key,    s32 action, s32 mods );
    virtual void        on_char             ( xui_native_window* hwnd, u16 c );
    virtual void        on_dropfile         ( xui_native_window* hwnd, const std::vector<std::string>& files );
    virtual void        on_window_pt        ( xui_native_window* hwnd, const xui_vector<s32>& pt );
    virtual void        on_window_sz        ( xui_native_window* hwnd, const xui_vector<s32>& sz );
    virtual void        on_window_focus     ( xui_native_window* hwnd, s32 focus );
    virtual bool        on_window_close     ( xui_native_window* hwnd );

protected:
    static  u08         convert_key         ( s32 key );
    static  u08         convert_button      ( s32 button );
};

#endif//__xui_event_h__