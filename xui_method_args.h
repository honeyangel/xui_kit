#ifndef __xui_method_args_h__
#define __xui_method_args_h__

#include "xui_vector.h"
#include "xui_rect2d.h"

class xui_method_args
{
public:
	bool	handle;
	void*	wparam;
	void*	lparam;

	xui_method_args( void )
	{
		handle = false;
		wparam = NULL;
		lparam = NULL;
	}
};

#define xui_declare_args(name) \
class xui_method_##name : public xui_method_args

enum
{
    k_mb_none,
	k_mb_left,
	k_mb_middle,
	k_mb_right,
};

xui_declare_args(mouse)
{
public:
	u08				mouse;
	xui_vector<s32>	point;
	s32				wheel;

	bool			alt;
	bool			ctrl;
	bool			shift;

	xui_method_mouse( void )
	{
		mouse = 0;
		wheel = 0;

		alt   = false;
		ctrl  = false;
		shift = false;
	}
};

enum
{
	k_key_none,
	k_key_a,
	k_key_b,
	k_key_c,
	k_key_d,
	k_key_e,
	k_key_f,
	k_key_g,
	k_key_h,
	k_key_i,
	k_key_j,
	k_key_k,
	k_key_l,
	k_key_m,
	k_key_n,
	k_key_o,
	k_key_p,
	k_key_q,
	k_key_r,
	k_key_s,
	k_key_t,
	k_key_u,
	k_key_v,
	k_key_w,
	k_key_x,
	k_key_y,
	k_key_z,
	k_key_1,
	k_key_2,
	k_key_3,
	k_key_4,
	k_key_5,
	k_key_6,
	k_key_7,
	k_key_8,
	k_key_9,
    k_key_0,
	k_key_esc,
	k_key_enter,
	k_key_back,
	k_key_tab,
	k_key_home,
	k_key_end,
	k_key_delete,
	k_key_shift,
	k_key_larrow,
	k_key_rarrow,
	k_key_uarrow,
	k_key_darrow,
	k_key_f1,
	k_key_f2,
	k_key_f3,
	k_key_f4,
	k_key_f5,
	k_key_f6,
	k_key_f7,
	k_key_f8,
	k_key_f9,
};

xui_declare_args(keybd)
{
public:
	u08				kcode;
	u16				wchar;

	bool			alt;
	bool			ctrl;
	bool			shift;

	xui_method_keybd( void )
	{
		kcode = 0;
		wchar = 0;

		alt   = false;
		ctrl  = false;
		shift = false;
	}
};

xui_declare_args(dragdrop)
{
public:
	xui_component*	drag;
	void*			data;
	std::string		type;
	u08				allow;

	xui_method_dragdrop( void )
	{
		drag  = NULL;
		data  = NULL;
		allow = false;
	}
};

xui_declare_args(update)
{
public:
	f32				delta;

	xui_method_update( void )
	{
		delta = 0.0f;
	}
};

xui_declare_args(propdata)
{
public:
	xui_propdata*	propdata;

	xui_method_propdata( void )
	{
		propdata = NULL;
	}
};

#endif//__xui_method_args_h__