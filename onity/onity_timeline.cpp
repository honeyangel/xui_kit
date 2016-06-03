#include "xui_bitmap.h"
#include "xui_dockview.h"
#include "xui_timeview.h"
#include "xui_timedata.h"
#include "xui_timeline.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_lineview.h"
#include "onity_preview.h"
#include "onity_propactor.h"
#include "onity_proplayer.h"
#include "onity_layerdata.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
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
	xui_method_ptrcall(m_lineview,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_lineview,	ini_component	)(true, false);

	m_linectrl = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_linectrl,	xm_toggleclick	) += new xui_method_member<xui_method_args,			onity_timeline>(this, &onity_timeline::on_linectrlclick);
	xui_method_ptrcall(m_linectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_linectrl,	ini_drawer		)(onity_resource::icon_layerview);
	xui_method_ptrcall(m_linectrl,	set_corner		)(3);
	xui_method_ptrcall(m_linectrl,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_linectrl,	set_drawcolor	)(true);

	m_drawview = new onity_preview;
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_drawview,	set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_toolshow	)(false);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name(total)",	NULL,							0, true));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"",				onity_resource::icon_lead,		0, false, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"",				onity_resource::icon_visible,	0, false, TOGGLE_NORMAL));
	m_timeview = new xui_timeview(xui_vector<s32>(200), columninfo, 24);
	xui_method_ptrcall(m_timeview,	xm_addlayer		) += new xui_method_member<xui_method_args,			onity_timeline>(this, &onity_timeline::on_timeviewaddlayer);
	xui_method_ptrcall(m_timeview,	xm_dellayer		) += new xui_method_member<xui_method_args,			onity_timeline>(this, &onity_timeline::on_timeviewdellayer);
	xui_method_ptrcall(m_timeview,	ini_component	)(0, 0, DOCKSTYLE_F);

	xui_treeview* timetree = m_timeview->get_timetree();
	xui_method_ptrcall(timetree,	xm_mouseclick	) += new xui_method_member<xui_method_mouse,		onity_timeline>(this, &onity_timeline::on_timetreemouseclick);
	xui_method_ptrcall(timetree,	xm_treedragover	) += new xui_method_member<xui_method_treedragdrop,	onity_timeline>(this, &onity_timeline::on_timetreedragover);
	xui_method_ptrcall(timetree,	xm_treedragdrop	) += new xui_method_member<xui_method_treedragdrop,	onity_timeline>(this, &onity_timeline::on_timetreedragdrop);

	m_sizectrl = new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse,		onity_timeline>(this, &onity_timeline::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor		)(CURSOR_WE);
	add_pagectrl(m_lineview);
	add_pagectrl(m_drawview);
	add_pagectrl(m_timeview);
	add_pagectrl(m_sizectrl);
	add_pagectrl(m_linectrl);
}

/*
//method
*/
xui_method_explain(onity_timeline, get_drawview,			onity_preview*	)( void )
{
	return m_drawview;
}
xui_method_explain(onity_timeline, get_timeview,			xui_timeview*	)( void )
{
	return m_timeview;
}
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
		m_drawview->set_viewprop(m_editprop, false);
		refresh_timeview();
	}
}

/*
//override
*/
xui_method_explain(onity_timeline, render_else,				void			)( void )
{
	xui_dockpage::render_else();

	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	if (dockview->get_showpage() == this)
	{
		xui_rect2d<s32> rt;
		rt = m_drawview->get_renderrtabs();
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.ax, rt.by), xui_control::default_sidecolor);
		if (m_lineview->was_visible() == false)
			return;
		rt = m_lineview->get_renderrtabs();
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx, rt.by), xui_control::default_sidecolor);
	}
}

/*
//callback
*/
xui_method_explain(onity_timeline, on_perform,				void			)( xui_method_args& args )
{
	xui_dockpage::on_perform(args);

	xui_rect2d<s32> rt = m_drawview->get_renderrt()+m_drawview->get_renderpt();
	m_sizectrl->on_perform_h(rt.get_h());
	m_sizectrl->on_perform_x(rt.ax-m_sizectrl->get_renderw());
	m_sizectrl->on_perform_y(rt.ay);
	m_linectrl->on_perform_x(rt.bx-m_linectrl->get_renderw()-8);
	m_linectrl->on_perform_y(rt.ay+8);
}

/*
//event
*/
xui_method_explain(onity_timeline, on_linectrlclick,		void			)( xui_component* sender, xui_method_args&			args )
{
	if (m_linectrl->was_push())
	{
		xui_method_ptrcall(m_lineview,	set_visible)(true);
		xui_method_ptrcall(m_parent,	set_renderh)(m_parent->get_renderh() + m_lineview->get_renderh());
	}
	else
	{
		xui_method_ptrcall(m_lineview,	set_visible)(false);
		xui_method_ptrcall(m_parent,	set_renderh)(m_parent->get_renderh() - m_lineview->get_renderh());
	}

	xui_dockview* dockview = xui_dynamic_cast(xui_dockview, m_parent);
	dockview->cal_portions();
}
xui_method_explain(onity_timeline, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse&			args )
{
	if (m_sizectrl->has_catch())
	{
		s32 delta = xui_desktop::get_ins()->get_mousemove().x;
		s32 width = m_drawview->get_renderw() - delta;
		width = xui_max(width, 200);
		width = xui_min(width, 500);
		m_drawview->set_renderw(width);		
	}
}
xui_method_explain(onity_timeline, on_timeviewaddlayer,		void			)( xui_component* sender, xui_method_args&			args )
{
	if (m_editprop)
	{
		xui_proproot* prop = m_editprop->add_layer();
		xui_timeline* line = m_timeview->add_timeline(0, new onity_layerdata(prop));
		xui_timedata* data = line->get_linkdata();
		xui_treenode* node = data->get_node();
		m_timeview->get_timetree()->refresh();
		node->set_edittext(0);
	}
}
xui_method_explain(onity_timeline, on_timeviewdellayer,		void			)( xui_component* sender, xui_method_args&			args )
{
	xui_treeview* timetree = m_timeview->get_timetree();
	std::vector<xui_treenode*> vec = timetree->get_selectednode();
	if (vec.size())
	{
		xui_treenode*   node = vec.front();
		onity_timedata* data = (onity_timedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();
		m_timeview->del_timeline(data->get_line());
		m_editprop->del_layer(data->get_prop());
	}
}
xui_method_explain(onity_timeline, on_timetreedragover,		void			)( xui_component* sender, xui_method_treedragdrop&	args )
{

}
xui_method_explain(onity_timeline, on_timetreedragdrop,		void			)( xui_component* sender, xui_method_treedragdrop&	args )
{

}
xui_method_explain(onity_timeline, on_timetreemouseclick,	void			)( xui_component* sender, xui_method_mouse&			args )
{
	xui_treeview* timetree = xui_dynamic_cast(xui_treeview, sender);
	std::vector<xui_treenode*> vec = timetree->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*   node = vec.front();
		onity_timedata* data = (onity_timedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(prop);
	}
}

/*
//method
*/
xui_method_explain(onity_timeline, refresh_timeview,		void			)( void )
{
	m_timeview->del_timelineall();

	if (m_editprop)
	{
		const xui_proproot_vec& proplayers = m_editprop->get_layers();
		for (u32 i = 0; i < proplayers.size(); ++i)
		{
			m_timeview->add_timeline(i, new onity_layerdata(proplayers[i]));
		}
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