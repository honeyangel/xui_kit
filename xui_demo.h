#ifndef __xui_demo_h__
#define __xui_demo_h__

#include "xui_bitmap.h"
#include "xui_window.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_linebox.h"
#include "xui_gridbox.h"
#include "xui_toolbar.h"

class xui_demo
{
public:
	static void test_button ( xui_window* window );
	static void test_toggle ( xui_window* window );
	static void test_textbox( xui_window* window );
	static void test_linebox( xui_window* window );
	static void test_gridbox( xui_window* window );
	static void test_toolbar( xui_window* window );
};

#endif//__xui_demo_h__