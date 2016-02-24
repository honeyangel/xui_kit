#include "xui_bitmap.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "onity_hierarchy.h"

xui_implement_rtti(onity_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
{
	ini_namectrl(xui_bitmap::create("icon/hierarchy.png"), L"Hierarchy");

	xui_menu* menu = xui_menu::create(128);
	m_entity = menu->add_item(NULL, L"Entity");

	//m_create = new xui_toggle(xui_vector<s32>(80, 24), TOGGLE_BUTTON);
	//xui_method_ptrcall(m_create, ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	//xui_method_ptrcall(m_create, set_borderrt	)(xui_rect2d<s32>(4));
	//xui_method_ptrcall(m_create, set_drawcolor	)(true);
	//xui_method_ptrcall(m_create, set_iconsize	)(xui_vector<s32>(0));
	//xui_method_ptrcall(m_create, ini_drawer		)(L"Create");
	//xui_method_ptrcall(m_create, set_menu		)(menu);

	m_search = xui_textbox::create(100);
	xui_method_ptrcall(m_search,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_search,	ini_drawer		)(xui_bitmap::create("icon/search.png"));
	xui_method_ptrcall(m_search,	set_sidestyle	)(SIDESTYLE_N);
	xui_method_ptrcall(m_search,	set_textalign	)(TA_LC);
	xui_method_ptrcall(m_search,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner		)(8);
	xui_method_ptrcall(m_search,	set_hinttext	)(L"All");

	m_head = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		add_child		)(m_search);
	add_pagectrl(m_head);
}