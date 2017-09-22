#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCParticleSystemQuad.h"

#include "xui_desktop.h"
#include "xui_bitmap.h"
#include "xui_global.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_treeview.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_propview.h"
#include "cocos_resource.h"
#include "cocos_propcsd.h"
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_treedata.h"
#include "cocos_nodedata.h"
#include "cocos_propnodelayer.h"
#include "cocos_propnodeparticle.h"
#include "cocos_propnodesprite.h"
#include "cocos_propparticle.h"
#include "cocos_propnodeimageview.h"
#include "cocos_scene.h"
#include "cocos_game.h"
#include "cocos_boundbox.h"
#include "cocos_hierarchy.h"

xui_implement_rtti(cocos_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
{
	ini_namectrl(cocos_resource::icon_hierarchy, L"Hierarchy");

	xui_menu* menu1 = xui_menu::create(80);
	m_node		= menu1->add_item(NULL, L"Node");
	m_sprite	= menu1->add_item(NULL, L"Sprite");
	m_particle	= menu1->add_item(NULL, L"Particle");
	xui_method_ptrcall(m_node,		xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_menuclick);
	xui_method_ptrcall(m_sprite,	xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_menuclick);
	xui_method_ptrcall(m_particle,	xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_menuclick);

	m_create	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	xm_toggleclick		) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_toggleclick);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu1);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_searchtextchanged);
	xui_method_ptrcall(m_search,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer			)(cocos_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(cocos_resource::icon_clear);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_headperform);
	xui_method_ptrcall(m_head,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor		)(false);
	xui_method_ptrcall(m_head,		set_borderrt		)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_head,		add_child			)(m_create);
	xui_method_ptrcall(m_head,		add_child			)(m_search);
	xui_method_ptrcall(m_head,		add_child			)(m_clear );

	xui_menu* menu2 = xui_menu::create(80);
	m_copy		= menu2->add_item(NULL, L"Copy");
	m_move		= menu2->add_item(NULL, L"Cut");
	m_paste		= menu2->add_item(NULL, L"Paste");
	m_delete	= menu2->add_item(NULL, L"Delete");
	xui_method_ptrcall(m_copy,		xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_move,		xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_paste,		xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_delete,	xm_click			) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_treemenuclick);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"", cocos_resource::icon_visible, 0, false, TOGGLE_CIRCLE));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"", cocos_resource::icon_lead,    0, false, TOGGLE_CIRCLE));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"Name", NULL, 0));
	m_tree		= new xui_treeview(xui_vector<s32>(100), columninfo, 20, PLUSRENDER_NORMAL, true, true);
	xui_method_ptrcall(m_tree,		xm_selectedchange	) += new xui_method_member<xui_method_args,			cocos_hierarchy>(this, &cocos_hierarchy::on_treeselection);
	xui_method_ptrcall(m_tree,		xm_keybddown		) += new xui_method_member<xui_method_keybd,		cocos_hierarchy>(this, &cocos_hierarchy::on_treekeybddown);
	xui_method_ptrcall(m_tree,		xm_mousedown		) += new xui_method_member<xui_method_mouse,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemousedown);
	xui_method_ptrcall(m_tree,		xm_mouseclick		) += new xui_method_member<xui_method_mouse,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemouseclick);
	xui_method_ptrcall(m_tree,		xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemousedoubleclick);
	xui_method_ptrcall(m_tree,		xm_mousedragleave	) += new xui_method_member<xui_method_dragdrop,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemousedragleave);
	xui_method_ptrcall(m_tree,		xm_mousedragover	) += new xui_method_member<xui_method_dragdrop,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemousedragover);
	xui_method_ptrcall(m_tree,		xm_mousedragdrop	) += new xui_method_member<xui_method_dragdrop,		cocos_hierarchy>(this, &cocos_hierarchy::on_treemousedragdrop);
	xui_method_ptrcall(m_tree,		xm_treedragover		) += new xui_method_member<xui_method_treedragdrop, cocos_hierarchy>(this, &cocos_hierarchy::on_treeinnerdragover);
	xui_method_ptrcall(m_tree,		xm_treedragdrop		) += new xui_method_member<xui_method_treedragdrop, cocos_hierarchy>(this, &cocos_hierarchy::on_treeinnerdragdrop);
	xui_method_ptrcall(m_tree,		ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tree,		set_borderrt		)(xui_rect2d<s32>(0, 0, 1, 0));
	xui_method_ptrcall(m_tree,		set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_tree,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_tree,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_tree,		set_allowmulti		)(true);
	xui_method_ptrcall(m_tree,		set_contextmenu		)(menu2);
	add_pagectrl(m_head);
	add_pagectrl(m_tree);
}

/*
//reset
*/
xui_method_explain(cocos_hierarchy, reset,					void				)( void )
{
	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	if (scene)
	{
		m_tree->del_upmostnodeall();

		cocos_propcsd*		file = scene->get_editprop();
		cocos_propnodebase* prop = file->get_prop();
		xui_treenode*		root = m_tree->add_upmostnode(0, new cocos_nodedata(prop));
		root->set_expanded(true);

		xui_proproot_vec vec = prop->get_leafprop();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			add_propnode(root, vec[i], -1);
		}

		cocos_mainform::get_ptr()->get_game()->set_viewprop(file);
	}
}

/*
//method
*/
//xui_method_explain(cocos_hierarchy, del_coursenode,			void				)( void )
//{
//	if (cocos_mainform::get_ptr()->was_gamerun())
//		return;
//
//	xui_treenode* next = NULL;
//	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
//	for (s32 i = nodes.size()-1; i >= 0; --i)
//	{
//		xui_treenode* root = nodes[i]->get_rootnode();
//		if (root)
//		{
//			std::vector<xui_treenode*>::iterator itor = std::find(
//				nodes.begin(),
//				nodes.end(),
//				root);
//			if (itor != nodes.end())
//				nodes.erase(nodes.begin()+i);
//			else
//			{
//				if (next == NULL)
//				{
//					for (u32 ileaf = 0; ileaf < root->get_leafnodecount(); ++ileaf)
//					{
//						xui_treenode* leaf = root->get_leafnode(ileaf);
//						if (leaf->was_selected() == false)
//						{
//							next = leaf;
//							break;
//						}
//					}
//				}
//			}
//		}
//	}
//
//	del_coursenode(nodes);
//	if (next)
//	{
//		m_tree->set_selectednode(next, true);
//	}
//}
xui_method_explain(cocos_hierarchy, get_treeview,			xui_treeview*				)( void )
{
	return m_tree;
}
xui_method_explain(cocos_hierarchy, get_realnode,			std::vector<xui_treenode*>	)( void )
{
	std::vector<xui_treenode*> vec = m_tree->get_selectednode();
	for (s32 i = (s32)vec.size()-1; i >= 0; --i)
	{
		if (has_ancestor(vec[i], vec))
			vec.erase(vec.begin()+i);
	}

	return vec;
}
xui_method_explain(cocos_hierarchy, new_propnode,			cocos_propnodebase*			)( cocos_propnodebase* proproot, const std::wstring& nodertti, u32 index )
{
	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	cocos_propnodebase* newprop = cocos_propnodebase::new_prop(scene->get_editprop(), nodertti);
	if (proproot == NULL)
	{
		xui_treenode*   rootnode = m_tree->get_upmostnode(0);
		cocos_treedata* rootdata = (cocos_treedata*)rootnode->get_linkdata();
		proproot = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
	}
	add_propnode(proproot, newprop, index);

	m_tree->non_selectednode(false);
	xui_treedata* newdata = newprop->get_linkdata(m_tree);
	xui_treenode* newnode = newdata->get_node();
	m_tree->ini_selectednode(newnode, true);

	cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
	inspector->set_proproot(newprop);

	return newprop;
}
xui_method_explain(cocos_hierarchy, new_propnode,			cocos_propnodebase*			)( cocos_propnodebase* proproot, cocos_propfile*	 propfile, u32 index )
{
	cocos_propnodebase* newprop = NULL;
	if (propfile->get_dragtype() == "ParticleSystem")
	{
		newprop = new_propnode(proproot, L"Particle");
		cocos_propparticle*		propparticle = dynamic_cast<cocos_propparticle*>(propfile);
		cocos_propnodeparticle* nodeparticle = dynamic_cast<cocos_propnodeparticle*>(newprop);
		nodeparticle->set_particlefile(propparticle);
	}

	return newprop;
}
xui_method_explain(cocos_hierarchy, new_propnode,			cocos_propnodebase*			)( cocos_propnodebase* proproot, cocos2d::Ref*		data,	   u32 index )
{
	cocos_propnodeimageview* newprop = dynamic_cast<cocos_propnodeimageview*>(new_propnode(proproot, L"ImageView"));
	cocos_propnodeimageview::set_background(newprop, data);

	return newprop;
}
xui_method_explain(cocos_hierarchy, add_propnode,			void						)( cocos_propnodebase* proproot, cocos_propnodebase* propleaf, u32 index )
{
	proproot->get_node()->addChild(propleaf->get_node());
	proproot->add_propnode(propleaf, index);
	if (index != -1)
		proproot->get_node()->setChildIndex(propleaf->get_node(), index);

	xui_treedata* rootdata = proproot->get_linkdata(m_tree);
	if (rootdata)
	{
		add_propnode(rootdata->get_node(), propleaf, index);
	}

	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	cocos_propcsd* propfile = scene->get_editprop();
	propfile->set_modify(true);
}
xui_method_explain(cocos_hierarchy, del_propnode,			void						)( cocos_propnodebase* proproot, cocos_propnodebase* propleaf, bool destroy )
{
	proproot->get_node()->removeChild(propleaf->get_node());
	proproot->del_propnode(propleaf);

	xui_treedata* rootdata = proproot->get_linkdata(m_tree);
	xui_treedata* leafdata = propleaf->get_linkdata(m_tree);
	if (rootdata && leafdata)
	{
		xui_treenode* rootnode = rootdata->get_node();
		xui_treenode* leafnode = leafdata->get_node();
		rootnode->del_leafnode(leafnode);
	}

	if (destroy)
		delete propleaf;

	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	cocos_propcsd* propfile = scene->get_editprop();
	propfile->set_modify(true);
}
xui_method_explain(cocos_hierarchy, del_propnode,			void						)( void )
{
	std::vector<xui_treenode*> nodevec = get_realnode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		if (nodevec[i]->get_rootnode() == NULL)
			continue;

		xui_treenode*		rootnode = nodevec[i]->get_rootnode();
		cocos_treedata*		rootdata = (cocos_treedata*)rootnode->get_linkdata();
		cocos_propnodebase* rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
		cocos_treedata*		leafdata = (cocos_treedata*)nodevec[i]->get_linkdata();
		cocos_propnodebase* leafprop = dynamic_cast<cocos_propnodebase*>(leafdata->get_prop());
		del_propnode(rootprop, leafprop, true);
	}
}

/*
//event
*/
xui_method_explain(cocos_hierarchy, on_toggleclick,			void						)( xui_component* sender, xui_method_args&			args )
{
	if (m_create->was_push())
	{
		cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
		bool enable = (scene && scene->get_editprop());
		xui_method_ptrcall(m_node,		set_enable)(enable);
		xui_method_ptrcall(m_sprite,	set_enable)(enable);
		xui_method_ptrcall(m_particle,	set_enable)(enable);
	}
}
xui_method_explain(cocos_hierarchy, on_clearclick,			void						)( xui_component* sender, xui_method_args&			args )
{
	m_search->set_text(L"");
}
xui_method_explain(cocos_hierarchy, on_searchtextchanged,	void						)( xui_component* sender, xui_method_args&			args )
{
	std::wstring text = m_search->get_text();
	xui_method_ptrcall(m_clear, ini_component	)(true, text.length() > 0);
	xui_method_ptrcall(m_tree,	set_searchtext	)(text);
}
xui_method_explain(cocos_hierarchy, on_headperform,			void						)( xui_component* sender, xui_method_args&			args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search, on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search, on_perform_x)(rt.ax+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,  on_perform_x)(rt.ax+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
}
xui_method_explain(cocos_hierarchy, on_menuclick,			void						)( xui_component* sender, xui_method_args&			args )
{
	cocos_propnodebase* rootprop = NULL;
	std::vector<xui_treenode*> nodevec = m_tree->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*	rootnode = nodevec.front();
		cocos_treedata* rootdata = (cocos_treedata*)rootnode->get_linkdata();
		rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
	}

	xui_menuitem* menuitem = xui_dynamic_cast(xui_menuitem, sender);
	new_propnode(rootprop, menuitem->get_text());
}
xui_method_explain(cocos_hierarchy, on_treemenuclick,		void						)( xui_component* sender, xui_method_args&			args )
{
	//xui_treenode* next = NULL;
	//std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	//for (s32 i = nodes.size()-1; i >= 0; --i)
	//{
	//	xui_treenode* root = nodes[i]->get_rootnode();
	//	if (root)
	//	{
	//		std::vector<xui_treenode*>::iterator itor = std::find(
	//			nodes.begin(),
	//			nodes.end(),
	//			root);
	//		if (itor != nodes.end())
	//			nodes.erase(nodes.begin()+i);
	//		else
	//		{
	//			if (next == NULL)
	//			{
	//				for (u32 ileaf = 0; ileaf < root->get_leafnodecount(); ++ileaf)
	//				{
	//					xui_treenode* leaf = root->get_leafnode(ileaf);
	//					if (leaf->was_selected() == false)
	//					{
	//						next = leaf;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//std::vector<NP2DSTransRef*> vec;
	//for (u32 i = 0; i < nodes.size(); ++i)
	//{
	//	cocos_treedata*			data	= (cocos_treedata*)nodes[i]->get_linkdata();
	//	cocos_propscenelayer*	propsl	= dynamic_cast<cocos_propscenelayer*>(data->get_prop());
	//	cocos_propcoursenode*	propcn	= dynamic_cast<cocos_propcoursenode*>(data->get_prop());
	//	if (propsl) vec.push_back(propsl->get_scenelayer());
	//	if (propcn) vec.push_back(propcn->get_2dsref());
	//}

	if (sender == m_delete)
	{
		del_propnode();
	}
	//else
	//if (sender == m_copy)
	//{
	//	m_menuvec = vec;
	//}
	//else
	//if (sender == m_move)
	//{
	//	m_menuvec = vec;
	//	del_coursenode(nodes);
	//}
	//else
	//if (sender == m_paste)
	//{
	//	pst_coursenode();
	//}
}
xui_method_explain(cocos_hierarchy, on_treeselection,		void						)( xui_component* sender, xui_method_args&			args )
{
	std::vector<xui_proproot*> props;
	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		cocos_treedata* data = (cocos_treedata*)nodes[i]->get_linkdata();
		props.push_back(data->get_prop());
	}
	if (props.size() > 0)
	{
		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		if (inspector->get_propview()->get_proproot() != props)
			inspector->get_propview()->set_proproot(props);
	}

	if (nodes.size() > 0)
	{
		m_tree->set_nodevisible(nodes.front());
	}

	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
	if (scene)
		scene->set_toolupdate();
}
xui_method_explain(cocos_hierarchy, on_treekeybddown,		void						)( xui_component* sender, xui_method_keybd&			args )
{
	if (args.kcode == KEY_DELETE)
	{
		del_propnode();
	}

	if (args.ctrl)
	{
		switch (args.kcode)
		{
		case KEY_C:
			break;
		case KEY_X:
			break;
		case KEY_V:
			break;
		}
	}
}
xui_method_explain(cocos_hierarchy, on_treemousedown,		void						)( xui_component* sender, xui_method_mouse&			args )
{
	if (args.mouse == MB_R)
	{
		std::vector<xui_treenode*> nodevec = m_tree->get_selectednode();
		xui_method_ptrcall(m_delete,	set_enable)(nodevec.size() > 0);
		xui_method_ptrcall(m_copy,		set_enable)(false);
		xui_method_ptrcall(m_move,		set_enable)(false);
		xui_method_ptrcall(m_paste,		set_enable)(false);
	}
}
xui_method_explain(cocos_hierarchy, on_treemouseclick,		void						)( xui_component* sender, xui_method_mouse&			args )
{
	if (args.mouse == MB_L)
	{
		xui_method_args other_args;
		on_treeselection(sender, other_args);
	}
}
xui_method_explain(cocos_hierarchy, on_treemousedragleave,	void						)( xui_component* sender, xui_method_dragdrop&		args )
{
	m_tree->set_allowplace(TREEDROP_NOTALLOW);
}
xui_method_explain(cocos_hierarchy, on_treemousedragover,	void						)( xui_component* sender, xui_method_dragdrop&		args )
{
	if (args.type == "Toolbox" ||
		args.type == "ParticleSystem" ||
		args.type == "SpriteFrame")
	{
		xui_vector<s32> mouse = xui_desktop::get_ins()->get_mousecurr();
		xui_treenode* hovernode = m_tree->choose_node(m_tree->get_renderpt(mouse));
		if (hovernode)
		{
			args.allow = true;
			m_tree->set_allowplace(TREEDROP_INNER);
			if (hovernode->get_rootnode())
			{
				xui_vector<s32> pt = hovernode->get_renderpt(mouse);
				if (pt.y < m_tree->get_lineheight()/3)	
					m_tree->set_allowplace(TREEDROP_FRONT);
				if (pt.y > m_tree->get_lineheight()*2/3) 
					m_tree->set_allowplace(TREEDROP_AFTER);
			}
		}
	}
}
xui_method_explain(cocos_hierarchy, on_treemousedragdrop,	void						)( xui_component* sender, xui_method_dragdrop&		args )
{
	xui_treenode* dropnode = m_tree->choose_node(m_tree->get_renderpt(xui_desktop::get_ins()->get_mousecurr()));
	if (dropnode)
	{
		u32 index = -1;
		cocos_propnodebase* rootprop = NULL;
		if (m_tree->get_allowplace() == TREEDROP_INNER)
		{
			cocos_treedata*	rootdata = (cocos_treedata*)dropnode->get_linkdata();
			rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
		}
		else
		{
			u32 index = dropnode->get_rootnode()->get_leafnodeindex(dropnode);
			if (m_tree->get_allowplace() == TREEDROP_AFTER)
				++index;

			cocos_treedata*	rootdata = (cocos_treedata*)dropnode->get_rootnode()->get_linkdata();
			rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
		}

		if (args.type == "Toolbox")
		{
			xui_button* toolctrl = (xui_button*)args.data;
			new_propnode(rootprop, toolctrl->get_text(), index);
		}
		else
		if (args.type == "ParticleSystem")
		{
			new_propnode(rootprop, (cocos_propfile*)args.data, index);
		}
		else
		if (args.type == "SpriteFrame" ||
			args.type == "Texture2D")
		{
			new_propnode(rootprop, (cocos2d::SpriteFrame*)args.data, index);
		}
	}

	m_tree->set_allowplace(TREEDROP_NOTALLOW);
}
xui_method_explain(cocos_hierarchy, on_treeinnerdragover,	void						)( xui_component* sender, xui_method_treedragdrop&	args )
{
	std::vector<xui_treenode*> vec = get_realnode();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (was_ancestor(args.dropnode, vec[i]))
		{
			args.allowplace = TREEDROP_NOTALLOW;
			break;
		}
	}

	if (args.dropnode->get_rootnode() == NULL)
	{
		if (args.allowplace == TREEDROP_FRONT ||
			args.allowplace == TREEDROP_AFTER)
			args.allowplace  = TREEDROP_NOTALLOW;
	}
}
xui_method_explain(cocos_hierarchy, on_treeinnerdragdrop,	void						)( xui_component* sender, xui_method_treedragdrop&	args )
{
	std::vector<cocos_propnodebase*> propvec;
	std::vector<xui_treenode*> nodevec = get_realnode();
	if (args.allowplace == TREEDROP_INNER)
	{
		cocos_treedata*		dropdata = (cocos_treedata*)args.dropnode->get_linkdata();
		cocos_propnodebase* dropprop = dynamic_cast<cocos_propnodebase*>(dropdata->get_prop());
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*		rootnode = nodevec[i]->get_rootnode();
			cocos_treedata*		rootdata = (cocos_treedata*)rootnode->get_linkdata();
			cocos_propnodebase* rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
			cocos_treedata*		dragdata = (cocos_treedata*)nodevec[i]->get_linkdata();
			cocos_propnodebase* dragprop = dynamic_cast<cocos_propnodebase*>(dragdata->get_prop());

			propvec.push_back(dragprop);
			if (rootprop != dropprop)
			{
				del_propnode(rootprop, dragprop);
				add_propnode(dropprop, dragprop);
			}
		}
	}
	else
	{
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*		rootnode = nodevec[i]->get_rootnode();
			cocos_treedata*		rootdata = (cocos_treedata*)rootnode->get_linkdata();
			cocos_propnodebase* rootprop = dynamic_cast<cocos_propnodebase*>(rootdata->get_prop());
			cocos_treedata*		dragdata = (cocos_treedata*)nodevec[i]->get_linkdata();
			cocos_propnodebase* dragprop = dynamic_cast<cocos_propnodebase*>(dragdata->get_prop());

			propvec.push_back(dragprop);
			del_propnode(rootprop, dragprop);
		}

		u32 index = args.dropnode->get_rootnode()->get_leafnodeindex(args.dropnode);
		if (args.allowplace == TREEDROP_AFTER)
			++index;

		cocos_treedata*		dropdata = (cocos_treedata*)args.dropnode->get_rootnode()->get_linkdata();
		cocos_propnodebase* dropprop = dynamic_cast<cocos_propnodebase*>(dropdata->get_prop());
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			add_propnode(dropprop, propvec[i], index);
			++index;
		}
	}

	m_tree->non_selectednode(false);
	for (u32 i = 0; i < propvec.size(); ++i)
	{
		xui_treedata* data = propvec[i]->get_linkdata(m_tree);
		xui_treenode* node = data->get_node();
		m_tree->ini_selectednode(node, true);
	}
}
xui_method_explain(cocos_hierarchy, on_treemousedoubleclick,void						)( xui_component* sender, xui_method_mouse&			args )
{
	if (args.mouse == MB_L)
	{
		xui_vector<s32>		pt	 = m_tree->get_renderpt(args.point);
		xui_treenode*		node = m_tree->choose_node(pt);
		cocos_treedata*		data = (cocos_treedata*)node->get_linkdata();
		cocos_propnodebase*	prop = dynamic_cast<cocos_propnodebase*>(data->get_prop());
		cocos_boundbox*		bbox = prop->get_boundbox();

		//cocos_scene* scene = cocos_mainform::get_ptr()->get_scene();
		//scene->set_nodevisible(bbox);
	}
}

/*
//method
*/
xui_method_explain(cocos_hierarchy, add_propnode,			void						)( xui_treenode* root, xui_proproot* prop, u32 index )
{
	u32 nodeindex = (index == -1 ? root->get_leafnodecount() : index);
	xui_treenode* node = root->add_leafnode(nodeindex, new cocos_nodedata(prop));
	node->set_expanded(true);

	xui_proproot_vec vec = dynamic_cast<cocos_propnodebase*>(prop)->get_leafprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		add_propnode(node, vec[i], -1);
	}
}
xui_method_explain(cocos_hierarchy, has_ancestor,			bool						)( xui_treenode* node, const std::vector<xui_treenode*>& vec )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (node == vec[i])
			continue;

		if (was_ancestor(node, vec[i]))
			return true;
	}

	return false;
}
xui_method_explain(cocos_hierarchy, was_ancestor,			bool						)( xui_treenode* leaf, xui_treenode* root )
{
	if (leaf->get_rootnode() == NULL)
		return false;

	return leaf->get_rootnode() == root || was_ancestor(leaf->get_rootnode(), root);
}
