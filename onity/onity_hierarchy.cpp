#include "xui_bitmap.h"
#include "onity_hierarchy.h"

xui_implement_rtti(onity_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
{
	ini_namectrl(xui_bitmap::create("icon/hierarchy.png"), L"Hierarchy");
}