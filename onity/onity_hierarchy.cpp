#include "xui_bitmap.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_treeview.h"
#include "onity_hierarchy.h"

xui_implement_rtti(onity_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
{
	ini_namectrl(xui_bitmap::create("icon/hierarchy.png"), L"Hierarchy");

	xui_menu* menu = xui_menu::create(80);
	xui_menuitem* create = menu->add_item(NULL, L"Create");
	create->set_submenu(xui_menu::create(80));
	m_entity = create->get_submenu()->add_item(NULL, L"Entity");

	m_search = xui_textbox::create(100);
	xui_method_ptrcall(m_search,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter	) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_search,	ini_drawer		)(xui_bitmap::create("icon/search.png"));
	xui_method_ptrcall(m_search,	set_sidestyle	)(SIDESTYLE_N);
	xui_method_ptrcall(m_search,	set_textalign	)(TA_LC);
	xui_method_ptrcall(m_search,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner		)(8);
	xui_method_ptrcall(m_search,	set_hinttext	)(L"All");

	m_clear = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_click		) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component	)(true, false);
	xui_method_ptrcall(m_clear,		ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner		)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor	)(true);
	xui_method_ptrcall(m_clear,		ini_drawer		)(xui_bitmap::create("icon/clear.png"));

	m_head  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform		) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_headperform);
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		add_child		)(m_search);
	xui_method_ptrcall(m_head,		add_child		)(m_clear);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"name", NULL, 0));
	m_tree  = new xui_treeview(xui_vector<s32>(100), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_tree,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tree,		set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_tree,		set_contextmenu	)(menu);
	xui_method_ptrcall(m_tree,		set_hscrollauto )(false);
	add_pagectrl(m_head);
	add_pagectrl(m_tree);
}

/*
//event
*/
xui_method_explain(onity_hierarchy, on_clearclick,			void)( xui_component* sender, xui_method_args& args )
{
	m_search->set_text(L"");
}
xui_method_explain(onity_hierarchy, on_searchtextchanged,	void)( xui_component* sender, xui_method_args& args )
{
	m_clear->ini_component(true, m_search->get_text().length() > 0);
	if (m_search->get_text().empty())
	{
		m_tree->set_searchtext(L"");
	}
}
xui_method_explain(onity_hierarchy, on_searchtextenter,		void)( xui_component* sender, xui_method_args& args )
{
	m_tree->set_searchtext(m_search->get_text());
}
xui_method_explain(onity_hierarchy, on_headperform,			void)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	m_clear->on_perform_x(rt.bx-m_clear->get_renderw()-2);
}