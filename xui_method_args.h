#ifndef __xui_method_args_h__
#define __xui_method_args_h__

#include "xui_vector.h"
#include "xui_rect2d.h"

class xui_method_args
{
public:
	/*
	//member
	*/
	bool	handle;
	void*	wparam;
	void*	lparam;

	/*
	//constructor
	*/
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
	MB_L,
	MB_M,
	MB_R,
};

xui_declare_args(mouse)
{
public:
	/*
	//member
	*/
	u08				mouse;
	xui_vector<s32>	point;
	s32				wheel;

	bool			alt;
	bool			ctrl;
	bool			shift;

	/*
	//constructor
	*/
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
	KEY_NONE,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_ESC,
	KEY_ENTER,
	KEY_BACK,
	KEY_TAB,
	KEY_HOME,
	KEY_END,
	KEY_DELETE,
	KEY_SHIFT,
	KEY_LARROW,
	KEY_RARROW,
	KEY_UARROW,
	KEY_DARROW,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
};

xui_declare_args(keybd)
{
public:
	/*
	//member
	*/
	u08				kcode;
	u16				wchar;

	bool			alt;
	bool			ctrl;
	bool			shift;

	/*
	//constructor
	*/
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
	/*
	//member
	*/
	xui_component*	drag;
	void*			data;
	std::string		type;
	u08				allow;

	/*
	//constructor
	*/
	xui_method_dragdrop( void )
	{
		drag  = NULL;
		data  = NULL;
		allow = false;
	}
};

xui_declare_args(propdata)
{
public:
	/*
	//member
	*/
	xui_propdata*	propdata;

	/*
	//constructor
	*/
	xui_method_propdata( void )
	{
		propdata = NULL;
	}
};

//xui_declare_args(action)
//{
//public:
//	/*
//	//member
//	*/
//	u32				event;
//
//	/*
//	//constructor
//	*/
//	xui_method_action( void )
//	{
//		event = 0;
//	}
//};

//xui_declare_args(update)
//{
//public:
//	/*
//	//member
//	*/
//	f32				delta;
//
//	/*
//	//constructor
//	*/
//	xui_method_update( f32 other_delta )
//	{
//		delta = other_delta;
//	}
//};
//
//xui_declare_args(render)
//{
//public:
//	/*
//	//member
//	*/
//	xui_convas*		paint;
//
//	/*
//	//constructor
//	*/
//	xui_method_render( xui_convas* other_paint )
//	{
//		paint = other_paint;
//	}
//};

#endif//__xui_method_args_h__