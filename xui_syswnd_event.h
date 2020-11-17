#ifndef __xui_syswnd_event_h__
#define __xui_syswnd_event_h__

#include "xui_event.h"

class xui_syswnd_event : public xui_event
{
public:
    virtual void on_window_sz   ( xui_native_window* hwnd, const xui_vector<s32>& sz );
    virtual bool on_window_close( xui_native_window* hwnd );
};

#endif//__xui_syswnd_event_h__