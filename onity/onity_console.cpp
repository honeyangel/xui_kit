#include "xui_bitmap.h"
#include "onity_console.h"

xui_implement_rtti(onity_console, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_console)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(xui_bitmap::create("icon/console.png"), L"Console");
}