#include "xui_bitmap.h"
#include "xui_timeview.h"
#include "xui_timedata.h"
#include "xui_timeline.h"
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

/*
//callback
*/
//DEBUG
xui_method_explain(onity_timeline, on_load, void)( xui_method_args& args )
{
	xui_dockpage::on_load(args);
	for (int i = 0; i < 4; ++i)
	{
		std::map<s32, u08> keyframe;
		keyframe[rand()%5+ 0] = 0;
		keyframe[rand()%5+ 5] = 0;
		keyframe[rand()%5+10] = 1;
		keyframe[rand()%5+20] = 1;
		keyframe[rand()%5+30] = 2;
		keyframe[rand()%5+50] = 1;
		keyframe[rand()%5+70] = 1;
		keyframe[rand()%5+90] = 2;

		std::wstringstream text;
		text << L"layer";
		text << i;
		xui_timedata* data = new xui_timedata(text.str(), xui_bitmap::create("icon/edit.png"), keyframe);
		xui_timeline* line = m_view->add_timeline(i, data);

		if (i % 2 == 0)
		{
			std::map<s32, u08> subkeyframe;
			subkeyframe[rand()%5+ 0] = 0;
			subkeyframe[rand()%5+ 5] = 0;
			subkeyframe[rand()%5+10] = 1;
			subkeyframe[rand()%5+20] = 1;
			subkeyframe[rand()%5+30] = 2;
			subkeyframe[rand()%5+50] = 1;
			subkeyframe[rand()%5+70] = 1;
			subkeyframe[rand()%5+90] = 2;
			xui_timedata* childdata = new xui_timedata(L"sublayer", xui_bitmap::create("icon/edit.png"), subkeyframe);
			xui_timeline* childline = line->add_timeline(0, childdata);
		}
	}
}