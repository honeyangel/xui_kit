#include "xui_bitmap.h"
#include "onity_project.h"

xui_implement_rtti(onity_project, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_project)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(xui_bitmap::create("icon/project.png"), L"Project");
}