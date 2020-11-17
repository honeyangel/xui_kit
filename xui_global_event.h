#ifndef __xui_global_event_h__
#define __xui_global_event_h__

#include "xui_event.h"

class xui_global_event : public xui_event
{
public:
    virtual void            on_window_pt    ( xui_native_window* hwnd, const xui_vector<s32>& pt );
    virtual void            on_window_sz    ( xui_native_window* hwnd, const xui_vector<s32>& sz );
    virtual bool            on_window_close ( xui_native_window* hwnd );

    static xui_vector<s32>  s_global_pt;
    static xui_vector<s32>  s_global_sz;
};

#endif//__xui_global_event_h__