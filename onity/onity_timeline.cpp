#include "xui_bitmap.h"
#include "xui_timeview.h"
#include "onity_timeline.h"

xui_implement_rtti(onity_timeline, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_timeline)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(xui_bitmap::create("icon/timeline.png"), L"Timeline");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0));
	m_view = new xui_timeview(xui_vector<s32>(200), columninfo, 24);
	xui_method_ptrcall(m_view, ini_component)(0, 0, DOCKSTYLE_F);
	add_pagectrl(m_view);
}