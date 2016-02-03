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
#include "xui_scroll.h"
#include "xui_slider.h"
#include "xui_listview.h"
#include "xui_dropbox.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "xui_timedata.h"
#include "xui_timeline.h"
#include "xui_timeview.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_slider.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "xui_propctrl_rect2d.h"
#include "xui_propctrl_object.h"
#include "xui_propctrl_simple.h"
#include "xui_propview.h"
#include "xui_kindctrl.h"
#include "xui_menu.h"
#include "xui_menuitem.h"

class xui_demo
{
public:
	static void test_button		( xui_window* window );
	static void test_toggle		( xui_window* window );
	static void test_textbox	( xui_window* window );
	static void test_linebox	( xui_window* window );
	static void test_gridbox	( xui_window* window );
	static void test_toolbar	( xui_window* window );
	static void test_scroll		( xui_window* window );
	static void test_slider		( xui_window* window );
	static void test_listview	( xui_window* window );
	static void test_dropbox	( xui_window* window );
	static void test_treeview	( xui_window* window );
	static void test_timeview	( xui_window* window );
	static void test_propview	( xui_window* window );
	static void test_menu		( xui_window* window );
};

#endif//__xui_demo_h__