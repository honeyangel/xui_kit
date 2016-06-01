#include "xui_bitmap.h"
#include "xui_timeview.h"
#include "xui_timedata.h"
#include "xui_timeline.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_lineview.h"
#include "onity_preview.h"
#include "onity_propactor.h"
#include "onity_timeline.h"

xui_implement_rtti(onity_timeline, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_timeline)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
, m_editprop(NULL)
{
	ini_namectrl(onity_resource::icon_timeline, L"Timeline");

	m_lineview = new onity_lineview;
	xui_method_ptrcall(m_lineview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_lineview,	set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_lineview,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_lineview,	ini_component	)(true, false);

	m_linectrl = new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_linectrl,	xm_toggleclick	) += new xui_method_member<xui_method_args,		onity_timeline>(this, &onity_timeline::on_linectrlclick);
	xui_method_ptrcall(m_linectrl,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_linectrl,	set_corner		)(3);
	xui_method_ptrcall(m_linectrl,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_linectrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_linectrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_linectrl,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_linectrl,	ini_drawer		)(L"Layer View");
	m_toolpane = new xui_panel(xui_vector<s32>(40));
	xui_method_ptrcall(m_toolpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_toolpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_toolpane,	add_child		)(m_linectrl);
	m_drawview = new onity_preview;
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_drawview,	set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_toolshow	)(false);
	m_leftpane = new xui_panel(xui_vector<s32>(240));
	xui_method_ptrcall(m_leftpane,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_leftpane,	set_borderrt	)(xui_rect2d<s32>(8, 0, 0, 16));
	xui_method_ptrcall(m_leftpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_leftpane,	add_child		)(m_toolpane);
	xui_method_ptrcall(m_leftpane,	add_child		)(m_drawview);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0));
	m_timeview = new xui_timeview(xui_vector<s32>(200), columninfo, 24);
	xui_method_ptrcall(m_timeview,	ini_component	)(0, 0, DOCKSTYLE_F);

	m_sizectrl = new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_timeline>(this, &onity_timeline::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor		)(CURSOR_WE);
	add_pagectrl(m_lineview);
	add_pagectrl(m_leftpane);
	add_pagectrl(m_timeview);
	add_pagectrl(m_sizectrl);
}

/*
//method
*/
xui_method_explain(onity_timeline, get_editprop,			onity_propactor*)( void )
{
	return m_editprop;
}
xui_method_explain(onity_timeline, set_editprop,			void			)( onity_propactor* editprop )
{
	if (m_editprop != editprop)
	{
		m_editprop  = editprop;
		m_lineview->set_viewprop(m_editprop);
		m_drawview->set_viewprop(m_editprop);
	}
}

/*
//callback
*/
xui_method_explain(onity_timeline, on_perform,				void			)( xui_method_args& args )
{
	xui_dockpage::on_perform(args);

	xui_rect2d<s32> rt = m_leftpane->get_renderrt()+m_leftpane->get_renderpt();
	m_sizectrl->on_perform_h(rt.get_h());
	m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
	m_sizectrl->on_perform_y(rt.ay);
}

/*
//event
*/
xui_method_explain(onity_timeline, on_linectrlclick,		void			)( xui_component* sender, xui_method_args& args )
{
	if (m_linectrl->was_push())
	{
		m_lineview->set_visible(true);
		set_renderh(get_renderh() + m_lineview->get_renderh());
	}
	else
	{
		m_lineview->set_visible(false);
		set_renderh(get_renderh() - m_lineview->get_renderh());
	}

	refresh();
}
xui_method_explain(onity_timeline, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch())
	{
		s32 delta = xui_desktop::get_ins()->get_mousemove().x;
		s32 width = m_leftpane->get_renderw() + delta;
		width = xui_max(width, 300);
		width = xui_min(width, 500);
		m_leftpane->set_renderw(width);		
	}
}


////DEBUG
//xui_method_explain(onity_timeline, on_load, void)( xui_method_args& args )
//{
//	xui_dockpage::on_load(args);
//	for (int i = 0; i < 4; ++i)
//	{
//		std::map<s32, u08> keyframe;
//		keyframe[rand()%5+ 0] = 0;
//		keyframe[rand()%5+ 5] = 0;
//		keyframe[rand()%5+10] = 1;
//		keyframe[rand()%5+20] = 1;
//		keyframe[rand()%5+30] = 2;
//		keyframe[rand()%5+50] = 1;
//		keyframe[rand()%5+70] = 1;
//		keyframe[rand()%5+90] = 2;
//
//		std::wstringstream text;
//		text << L"layer";
//		text << i;
//		xui_timedata* data = new xui_timedata(text.str(), xui_bitmap::create("icon/edit.png"), keyframe);
//		xui_timeline* line = m_view->add_timeline(i, data);
//
//		if (i % 2 == 0)
//		{
//			std::map<s32, u08> subkeyframe;
//			subkeyframe[rand()%5+ 0] = 0;
//			subkeyframe[rand()%5+ 5] = 0;
//			subkeyframe[rand()%5+10] = 1;
//			subkeyframe[rand()%5+20] = 1;
//			subkeyframe[rand()%5+30] = 2;
//			subkeyframe[rand()%5+50] = 1;
//			subkeyframe[rand()%5+70] = 1;
//			subkeyframe[rand()%5+90] = 2;
//			xui_timedata* childdata = new xui_timedata(L"sublayer", xui_bitmap::create("icon/edit.png"), subkeyframe);
//			xui_timeline* childline = line->add_timeline(0, childdata);
//		}
//	}
//}