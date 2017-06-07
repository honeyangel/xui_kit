#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSAssetFile.h"
#include "NP2DSTransRef.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NP2DSSceneFile.h"
#include "NP2DSSceneLayer.h"
#include "Entity/Component/GUI/GUIComponent.h"
#include "Entity/Entity.h"
#include "Game/Scene/Star/StarAIComp.h"

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
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_level.h"
#include "onity_inspector.h"
#include "onity_scene.h"
#include "onity_boundbox.h"
#include "onity_filterdata.h"
#include "onity_coursedata.h"
#include "onity_entitydata.h"
#include "onity_propcourse.h"
#include "onity_propcoursenode.h"
#include "onity_propentitynode.h"
#include "onity_propentitystar.h"
#include "onity_propscenelayer.h"
#include "onity_hierarchy.h"

xui_implement_rtti(onity_hierarchy, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_hierarchy)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_L)
, m_editprop(NULL)
{
	ini_namectrl(onity_resource::icon_hierarchy, L"Hierarchy");

	xui_menu* menu1 = xui_menu::create(80);
	m_layer		= menu1->add_item(NULL, L"Layer");
	m_instance	= menu1->add_item(NULL, L"Instance");
	xui_method_ptrcall(m_layer,		xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_menuclick);
	xui_method_ptrcall(m_instance,	xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_menuclick);

	m_create	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	xm_toggleclick		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_toggleclick);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu1);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer			)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(onity_resource::icon_clear);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_headperform);
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
	xui_method_ptrcall(m_copy,		xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_move,		xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_paste,		xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_delete,	xm_click			) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"name", NULL, 0));
	m_tree		= new xui_treeview(xui_vector<s32>(100), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_tree,		xm_selectedchange	) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treeselection);
	xui_method_ptrcall(m_tree,		xm_keybddown		) += new xui_method_member<xui_method_keybd,	onity_hierarchy>(this, &onity_hierarchy::on_treekeybddown);
	xui_method_ptrcall(m_tree,		xm_mousedown		) += new xui_method_member<xui_method_mouse,	onity_hierarchy>(this, &onity_hierarchy::on_treemousedown);
	xui_method_ptrcall(m_tree,		xm_mouseclick		) += new xui_method_member<xui_method_mouse,	onity_hierarchy>(this, &onity_hierarchy::on_treemouseclick);
	xui_method_ptrcall(m_tree,		xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse,	onity_hierarchy>(this, &onity_hierarchy::on_treemousedoubleclick);
	xui_method_ptrcall(m_tree,		xm_mousedragover	) += new xui_method_member<xui_method_dragdrop, onity_hierarchy>(this, &onity_hierarchy::on_treemousedragover);
	xui_method_ptrcall(m_tree,		xm_mousedragdrop	) += new xui_method_member<xui_method_dragdrop, onity_hierarchy>(this, &onity_hierarchy::on_treemousedragdrop);
	xui_method_ptrcall(m_tree,		ini_component		)(0, 0, DOCKSTYLE_F);
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
xui_method_explain(onity_hierarchy, reset,					void				)( bool forcedel )
{
	bool gamerun = onity_mainform::get_ptr()->was_gamerun();
	if (gamerun == false || forcedel)
	{
		m_tree->del_upmostnodeall();
		m_entitymap.clear();
		if (gamerun == false && m_editprop)
		{
			NP2DSSceneFile* scenefile = m_editprop->get_scenefile();
			for (npu16 i = 0; i <scenefile->GetSceneLayerCount(); ++i)
			{
				NP2DSSceneLayer* scenelayer = scenefile->GetSceneLayer(i);
				xui_treenode* root = m_tree->add_upmostnode(i, new onity_filterdata(onity_resource::icon_filter, new onity_propscenelayer(scenelayer), false));
				std::list<NPRenderObject*> children = scenelayer->GetChildren();
				for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
				{
					NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, (*itor));
					u32 index = root->get_leafnodecount();
					root->add_leafnode(index, new onity_coursedata(onity_resource::icon_entity, new onity_propcoursenode(transref)));
				}
			}
		}
	}
}

/*
//method
*/
xui_method_explain(onity_hierarchy, add_entitynode,			xui_treenode*		)( Omiga::Entity* ent )
{
	std::string				filtername = ent->GetTemplateName();
	xui_treenode*			filternode = get_filternode(filtername, ent->GetLinkRef() != NULL);
	onity_propentitynode*	entityprop = new onity_propentitynode(ent);
	xui_treenode*			entitynode = filternode->add_leafnode(filternode->get_leafnodecount(), new onity_entitydata(onity_resource::icon_entity, entityprop));

	if (entityprop->get_entity()->GetTemplateName() == "Star")
	{
		BreezeGame::StarAIComp* starAI = entityprop->get_entity()->GetComponent<BreezeGame::StarAIComp>();
		u32 count = starAI->GetStarVec().size();
		for (u32 i = 0; i < count; ++i)
		{
			entitynode->add_leafnode(i, new onity_entitydata(onity_resource::icon_entity, new onity_propentitystar(entityprop, i)));
		}
	}

	m_entitymap[ent] = entitynode;
	return entitynode;
}
xui_method_explain(onity_hierarchy, get_entitynode,			xui_treenode*		)( Omiga::Entity* ent )
{
	onity_entity_map::iterator itor = m_entitymap.find(ent);
	if (itor != m_entitymap.end())
		return (*itor).second;

	return NULL;
}
xui_method_explain(onity_hierarchy, del_entitynode,			void				)( Omiga::Entity* ent )
{
	onity_entity_map::iterator itor = m_entitymap.find(ent);
	if (itor != m_entitymap.end())
	{
		xui_treenode* entitynode = (*itor).second;
		xui_treenode* filternode = entitynode->get_rootnode();
		filternode->del_leafnode(entitynode);
		m_entitymap.erase(itor);

		if (filternode->get_leafnodecount() == 0)
			m_tree->del_upmostnode(filternode);
	}
}
xui_method_explain(onity_hierarchy, add_coursenode,			void				)( const xui_vector<s32>& pos, NP2DSAsset* asset )
{
	xui_treenode* root = NULL;
	std::vector<xui_treenode*> nodevec = m_tree->get_selectednode();
	if (nodevec.size() > 0)
	{
		root = nodevec.front();
		if (root->get_rootnode())
			root = root->get_rootnode();
	}
	else
	{
		root = m_tree->get_upmostnode(0);
	}

	xui_treenode*		  node = add_coursenode(root, asset, NULL);
	onity_treedata*		  data = (onity_treedata*)node->get_linkdata();
	onity_propcoursenode* prop = dynamic_cast<onity_propcoursenode*>(data->get_prop());
	prop->get_boundbox()->set_position(pos);

	m_tree->non_selectednode(false);
	m_tree->set_selectednode(node, true);
}
xui_method_explain(onity_hierarchy, del_coursenode,			void				)( void )
{
	if (onity_mainform::get_ptr()->was_gamerun())
		return;

	xui_treenode* next = NULL;
	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	for (s32 i = nodes.size()-1; i >= 0; --i)
	{
		xui_treenode* root = nodes[i]->get_rootnode();
		if (root)
		{
			std::vector<xui_treenode*>::iterator itor = std::find(
				nodes.begin(),
				nodes.end(),
				root);
			if (itor != nodes.end())
				nodes.erase(nodes.begin()+i);
			else
			{
				if (next == NULL)
				{
					for (u32 ileaf = 0; ileaf < root->get_leafnodecount(); ++ileaf)
					{
						xui_treenode* leaf = root->get_leafnode(ileaf);
						if (leaf->was_selected() == false)
						{
							next = leaf;
							break;
						}
					}
				}
			}
		}
	}

	del_coursenode(nodes);
	if (next)
	{
		m_tree->set_selectednode(next, true);
	}
}
xui_method_explain(onity_hierarchy, get_editprop,			onity_propcourse*	)( void )
{
	return m_editprop;
}
xui_method_explain(onity_hierarchy, set_editprop,			void				)( onity_propcourse* editprop )
{
	if (m_editprop != editprop)
	{
		if (m_editprop)
		{
			m_editprop->ntf_detach(editprop);
		}

		m_editprop  = editprop;

		if (m_editprop)
		{
			onity_level* wnd = onity_mainform::get_ptr()->get_level();
			wnd->set_firstlevel(m_editprop);
		}

		reset();
	}
}
xui_method_explain(onity_hierarchy, get_treeview,			xui_treeview*		)( void )
{
	return m_tree;
}

/*
//static
*/
xui_method_explain(onity_hierarchy, on_entityadd,			void				)( Omiga::Entity* ent )
{
	if (onity_mainform::get_ptr()->was_gamerun() == false)
		return;

	if (ent->GetComponent<Omiga::GUIComponent>())
		return;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	hierarchy->add_entitynode(ent);
}
xui_method_explain(onity_hierarchy, on_entitydel,			void				)( Omiga::Entity* ent )
{
	if (onity_mainform::get_ptr()->was_gamerun() == false)
		return;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	hierarchy->del_entitynode(ent);
}

/*
//event
*/
xui_method_explain(onity_hierarchy, on_toggleclick,			void				)( xui_component* sender, xui_method_args&		args )
{
	if (m_create->was_push())
	{
		bool enable = (m_editprop && onity_mainform::get_ptr()->was_gamerun() == false);
		xui_method_ptrcall(m_layer,		set_enable)(enable);
		xui_method_ptrcall(m_instance,	set_enable)(enable);
	}
}
xui_method_explain(onity_hierarchy, on_clearclick,			void				)( xui_component* sender, xui_method_args&		args )
{
	m_search->set_text(L"");
}
xui_method_explain(onity_hierarchy, on_searchtextchanged,	void				)( xui_component* sender, xui_method_args&		args )
{
	m_clear->ini_component(true, m_search->get_text().length() > 0);
	if (m_search->get_text().empty())
	{
		m_tree->set_searchtext(L"");
	}
}
xui_method_explain(onity_hierarchy, on_searchtextenter,		void				)( xui_component* sender, xui_method_args&		args )
{
	m_tree->set_searchtext(m_search->get_text());
}
xui_method_explain(onity_hierarchy, on_headperform,			void				)( xui_component* sender, xui_method_args&		args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search, on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search, on_perform_x)(rt.ax+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,  on_perform_x)(rt.ax+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
}
xui_method_explain(onity_hierarchy, on_menuclick,			void				)( xui_component* sender, xui_method_args&		args )
{
	if (sender == m_layer)
	{
		add_scenelayer("New Layer");
	}
	else
	if (sender == m_instance)
	{
		std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
		if (nodes.size() > 0)
		{
			xui_treenode* root = nodes.front();
			if (root->get_rootnode())
				root = root->get_rootnode();

			add_coursenode(root, NULL, NULL);
		}
	}
}
xui_method_explain(onity_hierarchy, on_treemenuclick,		void				)( xui_component* sender, xui_method_args&		args )
{
	xui_treenode* next = NULL;
	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	for (s32 i = nodes.size()-1; i >= 0; --i)
	{
		xui_treenode* root = nodes[i]->get_rootnode();
		if (root)
		{
			std::vector<xui_treenode*>::iterator itor = std::find(
				nodes.begin(),
				nodes.end(),
				root);
			if (itor != nodes.end())
				nodes.erase(nodes.begin()+i);
			else
			{
				if (next == NULL)
				{
					for (u32 ileaf = 0; ileaf < root->get_leafnodecount(); ++ileaf)
					{
						xui_treenode* leaf = root->get_leafnode(ileaf);
						if (leaf->was_selected() == false)
						{
							next = leaf;
							break;
						}
					}
				}
			}
		}
	}

	std::vector<NP2DSTransRef*> vec;
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		onity_treedata*			data	= (onity_treedata*)nodes[i]->get_linkdata();
		onity_propscenelayer*	propsl	= dynamic_cast<onity_propscenelayer*>(data->get_prop());
		onity_propcoursenode*	propcn	= dynamic_cast<onity_propcoursenode*>(data->get_prop());
		if (propsl) vec.push_back(propsl->get_scenelayer());
		if (propcn) vec.push_back(propcn->get_2dsref());
	}

	if (sender == m_delete)
	{
		del_coursenode(nodes);
		if (next)
		{
			m_tree->set_selectednode(next, true);
		}
	}
	else
	if (sender == m_copy)
	{
		m_menuvec = vec;
	}
	else
	if (sender == m_move)
	{
		m_menuvec = vec;
		del_coursenode(nodes);
	}
	else
	if (sender == m_paste)
	{
		pst_coursenode();
	}
}
xui_method_explain(onity_hierarchy, on_treeselection,		void				)( xui_component* sender, xui_method_args&		args )
{
	std::vector<xui_proproot*> props;
	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		onity_treedata* data = (onity_treedata*)nodes[i]->get_linkdata();
		props.push_back(data->get_prop());
	}
	if (props.size() > 0)
	{
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		if (inspector->get_propview()->get_proproot() != props)
			inspector->get_propview()->set_proproot(props);
	}

	if (nodes.size() > 0)
	{
		m_tree->set_nodevisible(nodes.front());
	}

	onity_scene* scene = onity_mainform::get_ptr()->get_scene();
	scene->set_toolupdate();
}
xui_method_explain(onity_hierarchy, on_treekeybddown,		void				)( xui_component* sender, xui_method_keybd&     args )
{
	if (args.kcode == KEY_DELETE)
	{
		del_coursenode();
	}
}
xui_method_explain(onity_hierarchy, on_treemousedown,		void				)( xui_component* sender, xui_method_mouse&		args )
{
	if (args.mouse == MB_R)
	{
		bool enable = (m_editprop && onity_mainform::get_ptr()->was_gamerun() == false);
		std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
		xui_method_ptrcall(m_delete,	set_enable)(enable && nodes.size() > 0);
		xui_method_ptrcall(m_copy,		set_enable)(enable && nodes.size() > 0);
		xui_method_ptrcall(m_move,		set_enable)(enable && nodes.size() > 0);
		xui_method_ptrcall(m_paste,		set_enable)(enable && m_menuvec.size() > 0);
	}
}
xui_method_explain(onity_hierarchy, on_treemouseclick,		void				)( xui_component* sender, xui_method_mouse&		args )
{
	if (args.mouse == MB_L)
	{
		xui_method_args other_args;
		on_treeselection(sender, other_args);
	}
}
xui_method_explain(onity_hierarchy, on_treemousedragover,	void				)( xui_component* sender, xui_method_dragdrop&	args )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return;

	if (args.type == "EntityTemplate" ||
		args.type == "NP2DSImage" ||
		args.type == "NP2DSFrame" ||
		args.type == "NP2DSActor")
	{
		args.allow = true;
	}
}
xui_method_explain(onity_hierarchy, on_treemousedragdrop,	void				)( xui_component* sender, xui_method_dragdrop&	args )
{
	if (args.type == "EntityTemplate")
	{
		Omiga::EntityTemplate* temp = (Omiga::EntityTemplate*)args.data;
		add_scenelayer(temp->GetName());
	}
	else
	{
		xui_vector<s32> pt = m_tree->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		xui_treenode* root = m_tree->choose_node(pt);
		if (root == NULL)
		{
			std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
			if (nodes.size() > 0)
				root = nodes.front();
		}

		if (root)
		{
			if (root->get_rootnode())
				root = root->get_rootnode();

			add_coursenode(root, (NP2DSAsset*)args.data, NULL);
		}
	}
}
xui_method_explain(onity_hierarchy, on_treemousedoubleclick,void				)( xui_component* sender, xui_method_mouse&     args )
{
	if (args.mouse == MB_L)
	{
		xui_vector<s32> pt = m_tree->get_renderpt(args.point);
		xui_treenode* node = m_tree->choose_node(pt);
		if (node->get_rootnode())
		{
			onity_scene*	 	scene = onity_mainform::get_ptr()->get_scene();
			onity_treedata*	 	data  = (onity_treedata*)node->get_linkdata();
			onity_propeditnode*	prop  = dynamic_cast<onity_propeditnode*>(data->get_prop());
			onity_boundbox*		bbox  = prop->get_boundbox();
			scene->set_nodevisible(bbox);
		}
	}
}

/*
//method
*/
xui_method_explain(onity_hierarchy, add_filternode,			xui_treenode*		)( const std::string& name, bool top )
{
	u32 index = top ? 0 : m_tree->get_upmostnodecount();
	return m_tree->add_upmostnode(index, new onity_filterdata(onity_resource::icon_filter, new onity_propentitytemp(name), top));
}
xui_method_explain(onity_hierarchy, get_filternode,			xui_treenode*		)( const std::string& name, bool top )
{
	std::wstring  filtername = xui_global::ascii_to_unicode(name);
	xui_treenode* filternode = NULL;
	for (u32 i = 0; i < m_tree->get_upmostnodecount(); ++i)
	{
		xui_treenode* node = m_tree->get_upmostnode(i);
		xui_treedata* data = node->get_linkdata();
		if (data->get_text(0) == filtername)
		{
			filternode = node;
			break;
		}
	}

	if (filternode == NULL)
	{
		filternode = add_filternode(name, top);
	}

	return filternode;
}
xui_method_explain(onity_hierarchy, add_scenelayer,			xui_treenode*		)( const std::string& name )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return NULL;

	NP2DSSceneLayer* scenelayer = m_editprop->get_scenefile()->AddSceneLayer(NP2DSSceneLayer::LT_SCENE, 1, 1, 8000, 640);
	scenelayer->SetName(name);
	u32 index = m_tree->get_upmostnodecount();
	return m_tree->add_upmostnode(index, new onity_filterdata(onity_resource::icon_filter, new onity_propscenelayer(scenelayer), false));
}
xui_method_explain(onity_hierarchy, add_scenelayer,			xui_treenode*		)( NP2DSSceneLayer* src )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return NULL;

	NP2DSSceneLayer* scenelayer = m_editprop->get_scenefile()->AddSceneLayer(NP2DSSceneLayer::LT_SCENE, 1, 1, 8000, 640);
	scenelayer->Clone(src);
	u32 index = m_tree->get_upmostnodecount();
	xui_treenode* root = m_tree->add_upmostnode(index, new onity_filterdata(onity_resource::icon_filter, new onity_propscenelayer(scenelayer), false));

	std::list<NPRenderObject*> children = scenelayer->GetChildren();
	for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
	{
		NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, (*itor));
		u32 index = root->get_leafnodecount();
		root->add_leafnode(index, new onity_coursedata(onity_resource::icon_entity, new onity_propcoursenode(transref)));
	}

	return root;
}
xui_method_explain(onity_hierarchy, add_coursenode,			xui_treenode*		)( xui_treenode* root, NP2DSAsset* asset, NP2DSTransRef* src )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun() || root == NULL)
		return NULL;

	onity_filterdata*	  data = (onity_filterdata*)root->get_linkdata();
	onity_propscenelayer* prop = dynamic_cast<onity_propscenelayer*>(data->get_prop());
	npu32 resfile = (asset == NULL) ? -1 : asset->GetOwnedFile()->GetKey();
	npu32 assetid = (asset == NULL) ? -1 : asset->GetKey();
	
	NP2DSTransRef* transref = NULL;
	if ((asset && NPIsExaKindOf(NP2DSImage, asset)) || (src && NPIsSubKindOf(NP2DSImageRef, src)))
	{
		transref = prop->get_scenelayer()->AddImageRef(resfile, assetid);
	}
	else 
	if ((asset && NPIsExaKindOf(NP2DSActor, asset)) || (src && NPIsSubKindOf(NP2DSActorRef, src)))
	{
		transref = prop->get_scenelayer()->AddActorRef(resfile, assetid);
	}
	else
	{
		transref = prop->get_scenelayer()->AddFrameRef(resfile, assetid);
	}

	if (src)
	{
		transref->Clone(src);
	}

	u32 index = root->get_leafnodecount();
	return root->add_leafnode(index, new onity_coursedata(onity_resource::icon_entity, new onity_propcoursenode(transref)));
}
xui_method_explain(onity_hierarchy, del_coursenode,			void				)( const std::vector<xui_treenode*>& nodes )
{
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode* node = nodes[i];
		if (node->get_rootnode())
			del_coursenode(node);
		else
			del_scenelayer(node);
	}
}
xui_method_explain(onity_hierarchy, del_scenelayer,			void				)( xui_treenode* root )
{
	onity_filterdata*	   rootdata = (onity_filterdata*)root->get_linkdata();
	onity_propscenelayer*  rootprop = dynamic_cast<onity_propscenelayer*>(rootdata->get_prop());
	m_editprop->get_scenefile()->DelSceneLayer(rootprop->get_scenelayer());
	m_tree->del_upmostnode(root);
}
xui_method_explain(onity_hierarchy, del_coursenode,			void				)( xui_treenode* node )
{
	xui_treenode*		   root     = node->get_rootnode();
	onity_filterdata*	   rootdata = (onity_filterdata*)root->get_linkdata();
	onity_propscenelayer*  rootprop = dynamic_cast<onity_propscenelayer*>(rootdata->get_prop());
	onity_coursedata*	   nodedata = (onity_coursedata*)node->get_linkdata();
	onity_propcoursenode*  nodeprop = dynamic_cast<onity_propcoursenode*>(nodedata->get_prop());
	rootprop->get_scenelayer()->DelSceneRef(nodeprop->get_2dsref());
	root->del_leafnode(node);
}
xui_method_explain(onity_hierarchy, pst_coursenode,			void				)( void )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return;

	xui_treenode* root = NULL;
	std::vector<xui_treenode*> nodes = m_tree->get_selectednode();
	if (nodes.size())
	{
		root = nodes.front();
		if (root->get_rootnode())
			root = root->get_rootnode();
	}

	for (u32 i = 0; i < m_menuvec.size(); ++i)
	{
		NP2DSSceneLayer* scenelayer = NPDynamicCast(NP2DSSceneLayer, m_menuvec[i]);
		if (scenelayer)
			add_scenelayer(scenelayer);
		else
			add_coursenode(root, NULL, m_menuvec[i]);
	}

	m_menuvec.clear();
}