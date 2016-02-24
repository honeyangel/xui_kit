#include "xui_bitmap.h"
#include "onity_inspector.h"

xui_implement_rtti(onity_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_inspector)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_R)
{
	ini_namectrl(xui_bitmap::create("icon/inspector.png"), L"Inspector");
}