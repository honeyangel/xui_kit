#include "xui_bitmap.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_treeview.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "onity_resource.h"
#include "onity_hierarchy.h"

xui_implement_rtti(onity_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
{
	ini_namectrl(onity_resource::icon_hierarchy, L"Hierarchy");

	xui_menu*  menu = xui_menu::create(80);
	m_entity = menu->add_item(NULL, L"Entity");

	m_create = new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner		)(3);
	xui_method_ptrcall(m_create,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor	)(true);
	xui_method_ptrcall(m_create,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer		)(L"Create");
	xui_method_ptrcall(m_create,	set_menu		)(menu);

	m_search = new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter	) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer		)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor	)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor	)(true);
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner		)(10);
	xui_method_ptrcall(m_search,	set_hintdraw	)(xui_family_render(xui_colour::gray));
	xui_method_ptrcall(m_search,	set_hinttext	)(L"All");

	m_clear = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component	)(true, false);
	xui_method_ptrcall(m_clear,		ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner		)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor	)(true);
	xui_method_ptrcall(m_clear,		ini_drawer		)(onity_resource::icon_clear);

	m_head  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform		) += new xui_method_member<xui_method_args, onity_hierarchy>(this, &onity_hierarchy::on_headperform);
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto	)(false);
	xui_method_ptrcall(m_head,		add_child		)(m_create);
	xui_method_ptrcall(m_head,		add_child		)(m_search);
	xui_method_ptrcall(m_head,		add_child		)(m_clear );

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"name", NULL, 0));
	m_tree  = new xui_treeview(xui_vector<s32>(100), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_tree,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tree,		set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_tree,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_tree,		set_hscrollauto )(false);
	add_pagectrl(m_head);
	add_pagectrl(m_tree);
}

/*
//callback
*/
//DEBUG
class onity_entity_data : public xui_treedata
{
public:
	/*
	//constructor
	*/
	onity_entity_data( const std::wstring& text )
	: xui_treedata(text)
	{}

	/*
	//override
	*/
	virtual xui_family_render	get_textdraw	( u32 index )
	{
		if (m_node && m_node->was_selected() == false)
		{
			s32 colorindex = rand()%8;
			switch (colorindex)
			{
			case 4:
				return xui_family_render(xui_colour::gray);
			case 5:
			case 6:
				return xui_family_render(xui_colour(1.0f, 76.0f/255.0f, 128.0f/255.0f, 217.0f/255.0f));
			case 7:
				return xui_family_render(xui_colour::red);
			}
		}

		return xui_family_render();
	}
};

xui_method_explain(onity_hierarchy, on_load,				void)( xui_method_args& args )
{
	xui_dockpage::on_load(args);

	xui_treenode* node = NULL;
	m_tree->add_upmostnode(0, new onity_entity_data(L"Camera"));
	node = m_tree->add_upmostnode(1, new onity_entity_data(L"SceneSpawnMgr"));
	node->add_leafnode(0, new onity_entity_data(L"Monster"));
	node->add_leafnode(1, new onity_entity_data(L"Star"));
	node->add_leafnode(2, new onity_entity_data(L"LargeMonster"));
	node->add_leafnode(3, new onity_entity_data(L"Ground"));
	node = m_tree->add_upmostnode(2, new onity_entity_data(L"UI"));
	node->add_leafnode(0, new onity_entity_data(L"GameHUD"));
	m_tree->add_upmostnode(3, new onity_entity_data(L"Player"));
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
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search, on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search, on_perform_x)(rt.ax+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,  on_perform_x)(rt.ax+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
}