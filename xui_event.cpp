#include "xui_event.h"
#include "xui_desktop.h"
#include "xui_native_window.h"
#include "xui_global.h"
#include "glfw3.h"

#if xui_target_platform == xui_platform_macos
#define CTRL_MOD_KEY GLFW_MOD_SUPER
#define LEFT_CONTROL_KEY GLFW_KEY_LEFT_SUPER
#else
#define CTRL_MOD_KEY GLFW_MOD_CONTROL
#define LEFT_CONTROL_KEY GLFW_KEY_LEFT_CONTROL
#endif

u08 xui_event::convert_key( s32 key )
{
    switch (key)
    {
    case GLFW_KEY_A:				return k_key_a;
    case GLFW_KEY_B:				return k_key_b;
    case GLFW_KEY_C:				return k_key_c;
    case GLFW_KEY_D:				return k_key_d;
    case GLFW_KEY_E:				return k_key_e;
    case GLFW_KEY_F:				return k_key_f;
    case GLFW_KEY_G:				return k_key_g;
    case GLFW_KEY_H:				return k_key_h;
    case GLFW_KEY_I:				return k_key_i;
    case GLFW_KEY_J:				return k_key_j;
    case GLFW_KEY_K:				return k_key_k;
    case GLFW_KEY_L:				return k_key_l;
    case GLFW_KEY_M:				return k_key_m;
    case GLFW_KEY_N:				return k_key_n;
    case GLFW_KEY_O:				return k_key_o;
    case GLFW_KEY_P:				return k_key_p;
    case GLFW_KEY_Q:				return k_key_q;
    case GLFW_KEY_R:				return k_key_r;
    case GLFW_KEY_S:				return k_key_s;
    case GLFW_KEY_T:				return k_key_t;
    case GLFW_KEY_U:				return k_key_u;
    case GLFW_KEY_V:				return k_key_v;
    case GLFW_KEY_W:				return k_key_w;
    case GLFW_KEY_X:				return k_key_x;
    case GLFW_KEY_Y:				return k_key_y;
    case GLFW_KEY_Z:				return k_key_z;
    case GLFW_KEY_1:                return k_key_1;
    case GLFW_KEY_2:                return k_key_2;
    case GLFW_KEY_3:                return k_key_3;
    case GLFW_KEY_4:                return k_key_4;
    case GLFW_KEY_5:                return k_key_5;
    case GLFW_KEY_6:                return k_key_6;
    case GLFW_KEY_7:                return k_key_7;
    case GLFW_KEY_8:                return k_key_8;
    case GLFW_KEY_9:                return k_key_9;
    case GLFW_KEY_0:                return k_key_0;
    case GLFW_KEY_ESCAPE:			return k_key_esc;
    case GLFW_KEY_ENTER:			return k_key_enter;
    case GLFW_KEY_KP_ENTER:         return k_key_enter;
    case GLFW_KEY_TAB:  			return k_key_tab;
    case GLFW_KEY_BACKSPACE:		return k_key_back;
    case GLFW_KEY_LEFT_SHIFT:   	return k_key_shift;
    case GLFW_KEY_RIGHT_SHIFT:   	return k_key_shift;
    case GLFW_KEY_UP:				return k_key_uarrow;
    case GLFW_KEY_DOWN:			    return k_key_darrow;
    case GLFW_KEY_LEFT:			    return k_key_larrow;
    case GLFW_KEY_RIGHT:			return k_key_rarrow;
    case GLFW_KEY_HOME:	   		    return k_key_home;
    case GLFW_KEY_END:   			return k_key_end;
    case GLFW_KEY_DELETE:			return k_key_delete;
    case GLFW_KEY_F1:				return k_key_f1;
    case GLFW_KEY_F2:				return k_key_f2;
    case GLFW_KEY_F3:				return k_key_f3;
    case GLFW_KEY_F4:				return k_key_f4;
    case GLFW_KEY_F5:				return k_key_f5;
    case GLFW_KEY_F6:				return k_key_f6;
    case GLFW_KEY_F7:				return k_key_f7;
    case GLFW_KEY_F8:				return k_key_f8;
    case GLFW_KEY_F9:				return k_key_f9;
    default:		        		return k_key_none;
    }
}

u08 xui_event::convert_button( s32 button )
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:	return k_mb_left;
    case GLFW_MOUSE_BUTTON_RIGHT:	return k_mb_right;
    case GLFW_MOUSE_BUTTON_MIDDLE:	return k_mb_middle;
    default:                        return k_mb_none;
    }
}

void xui_event::on_mouse_move( xui_native_window* hwnd, const xui_vector<s32>& pt )
{
    xui_method_mouse args;
    args.wparam = hwnd;
    args.point  = hwnd->get_cursor_position();
    args.ctrl   = hwnd->get_key_state(LEFT_CONTROL_KEY)    == GLFW_PRESS;
    args.shift  = hwnd->get_key_state(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    args.alt    = hwnd->get_key_state(GLFW_KEY_LEFT_ALT)   == GLFW_PRESS;
    xui_desktop::get_ins()->os_mousemove(args);
}

void xui_event::on_mouse_button( xui_native_window* hwnd, s32 button, s32 action, s32 mods )
{
    if (hwnd == NULL)
        return;

    xui_method_mouse args;
    args.wparam = hwnd;
    args.point  = hwnd->get_cursor_position();
    //args.ctrl   = window->get_key_state(LEFT_CONTROL_KEY) == GLFW_PRESS;
    //args.shift  = window->get_key_state(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    //args.alt    = window->get_key_state(GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
    args.ctrl   = (mods & CTRL_MOD_KEY)     != 0;
    args.shift  = (mods & GLFW_MOD_SHIFT)   != 0;
    args.alt    = (mods & GLFW_MOD_ALT)     != 0;
    args.mouse  = convert_button(button);

    switch (action)
    {
    case GLFW_PRESS:
        xui_desktop::get_ins()->os_mousedown(args);
        if (xui_desktop::get_ins()->get_catchctrl())
            hwnd->capture();
        break;
    case GLFW_RELEASE:
        xui_desktop::get_ins()->os_mouserise(args);
        if (xui_desktop::get_ins()->get_catchctrl())
            xui_native_device::release_capture();
        break;
    }
}

void xui_event::on_mouse_scroll( xui_native_window* hwnd, s32 value )
{
    if (value != 0)
    {
        xui_method_mouse args;
        args.wparam = hwnd;
        args.wheel  = value * 40;
        args.ctrl   = hwnd->get_key_state(LEFT_CONTROL_KEY)    == GLFW_PRESS;
        args.shift  = hwnd->get_key_state(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        args.alt    = hwnd->get_key_state(GLFW_KEY_LEFT_ALT)   == GLFW_PRESS;
        xui_desktop::get_ins()->os_mousewheel(args);
    }
}

void xui_event::on_key( xui_native_window* hwnd, s32 key, s32 action, s32 mods )
{
    xui_method_keybd args;
    args.ctrl   = (mods & CTRL_MOD_KEY)     != 0;
    args.shift  = (mods & GLFW_MOD_SHIFT)   != 0;
    args.alt    = (mods & GLFW_MOD_ALT)     != 0;
    args.kcode  = convert_key(key);
    switch (action)
    {
    case GLFW_PRESS:
    case GLFW_REPEAT:
        xui_desktop::get_ins()->os_keybddown(args);
        break;
    case GLFW_RELEASE:
        xui_desktop::get_ins()->os_keybdrise(args);
        break;
    }
}

void xui_event::on_char( xui_native_window* hwnd, u16 c )
{
    xui_desktop::get_ins()->os_keybdchar(c);
}

void xui_event::on_dropfile( xui_native_window* hwnd, const std::vector<std::string>& files )
{}

void xui_event::on_window_pt( xui_native_window* hwnd, const xui_vector<s32>& pt )
{}

void xui_event::on_window_sz( xui_native_window* hwnd, const xui_vector<s32>& sz )
{}

void xui_event::on_window_focus( xui_native_window* hwnd, s32 focus )
{
    if (focus == GLFW_FALSE)
    {
        if (hwnd == xui_global::s_hwnd_scolor)
        {
            xui_desktop::get_ins()->set_focusctrl(NULL);
            hwnd->req_focus();
        }
    }
}

bool xui_event::on_window_close( xui_native_window* hwnd )
{
    return true;
}

