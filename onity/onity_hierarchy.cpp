#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NP2DSSceneFile.h"
#include "NP2DSSceneLayer.h"
#include "Entity/Component/GUI/GUIComponent.h"

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
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_filterdata.h"
#include "onity_maprefdata.h"
#include "onity_entitydata.h"
#include "onity_propcourse.h"
#include "onity_propmapref.h"
#include "onity_propentity.h"
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
	xui_method_ptrcall(m_layer,		xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_menuclick);
	xui_method_ptrcall(m_instance,	xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_menuclick);

	m_create	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	xm_toggleclick	) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_toggleclick);
	xui_method_ptrcall(m_create,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner		)(3);
	xui_method_ptrcall(m_create,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor	)(true);
	xui_method_ptrcall(m_create,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer		)(L"Create");
	xui_method_ptrcall(m_create,	set_menu		)(menu1);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged	) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter	) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer		)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor	)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor	)(true);
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt	)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner		)(10);

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component	)(true, false);
	xui_method_ptrcall(m_clear,		ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner		)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor	)(true);
	xui_method_ptrcall(m_clear,		ini_drawer		)(onity_resource::icon_clear);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_headperform);
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto	)(false);
	xui_method_ptrcall(m_head,		add_child		)(m_create);
	xui_method_ptrcall(m_head,		add_child		)(m_search);
	xui_method_ptrcall(m_head,		add_child		)(m_clear );

	xui_menu* menu2 = xui_menu::create(80);
	m_copy		= menu2->add_item(NULL, L"Copy");
	m_move		= menu2->add_item(NULL, L"Cut");
	m_paste		= menu2->add_item(NULL, L"Paste");
	m_delete	= menu2->add_item(NULL, L"Delete");
	xui_method_ptrcall(m_copy,		xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_move,		xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_paste,		xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);
	xui_method_ptrcall(m_delete,	xm_click		) += new xui_method_member<xui_method_args,		onity_hierarchy>(this, &onity_hierarchy::on_treemenuclick);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"name", NULL, 0));
	m_tree		= new xui_treeview(xui_vector<s32>(100), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_tree,		xm_keybddown	) += new xui_method_member<xui_method_keybd,	onity_hierarchy>(this, &onity_hierarchy::on_treekeybddown);
	xui_method_ptrcall(m_tree,		xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_hierarchy>(this, &onity_hierarchy::on_treemousedown);
	xui_method_ptrcall(m_tree,		xm_mouseclick	) += new xui_method_member<xui_method_mouse,	onity_hierarchy>(this, &onity_hierarchy::on_treemouseclick);
	xui_method_ptrcall(m_tree,		xm_mousedragover) += new xui_method_member<xui_method_dragdrop, onity_hierarchy>(this, &onity_hierarchy::on_treemousedragover);
	xui_method_ptrcall(m_tree,		xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop, onity_hierarchy>(this, &onity_hierarchy::on_treemousedragdrop);
	xui_method_ptrcall(m_tree,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_tree,		set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_tree,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_tree,		set_hscrollauto	)(false);
	xui_method_ptrcall(m_tree,		set_allowmulti	)(true);
	xui_method_ptrcall(m_tree,		set_contextmenu	)(menu2);
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
		if (gamerun == false && m_editprop)
		{
			NP2DSSceneFile* scenefile = m_editprop->get_scenefile();
			for (npu16 i = 0; i <scenefile->GetSceneLayerCount(); ++i)
			{
				NP2DSSceneLayer* scenelayer = scenefile->GetSceneLayer(i);
				xui_treenode* root = m_tree->add_upmostnode(i, new onity_filterdata(NULL, new onity_propscenelayer(scenelayer)));
				std::list<NPRenderObject*> children = scenelayer->GetChildren();
				for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
				{
					NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, (*itor));
					u32 index = root->get_leafnodecount();
					root->add_leafnode(index, new onity_maprefdata(NULL, new onity_propmapref(transref)));
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
	std::string   filtername = ent->GetTemplateName();
	xui_treenode* filternode = get_filternode(filtername);
	xui_treenode* entitynode = filternode->add_leafnode(
		filternode->get_leafnodecount(), 
		new onity_entitydata(NULL, new onity_propentity(ent)));

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

			add_maprefnode(root, NULL, NULL);
		}
	}
}
xui_method_explain(onity_hierarchy, on_treemenuclick,		void				)( xui_component* sender, xui_method_args&		args )
{
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
		}
	}

	std::vector<NP2DSTransRef*> vec;
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		onity_treedata*			data	= (onity_treedata*)nodes[i]->get_linkdata();
		onity_propscenelayer*	propsl	= dynamic_cast<onity_propscenelayer*>(data->get_prop());
		onity_propmapref*		propmr	= dynamic_cast<onity_propmapref*	>(data->get_prop());
		if (propsl) vec.push_back(propsl->get_scenelayer());
		if (propmr) vec.push_back(propmr->get_2dsref());
	}

	if (sender == m_delete)
	{
		del_coursenode(nodes);
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
xui_method_explain(onity_hierarchy, on_treekeybddown,		void				)( xui_component* sender, xui_method_keybd&     args )
{
	if (args.kcode == KEY_DELETE)
	{
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
			}
		}

		del_coursenode(nodes);
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
			inspector->set_proproot(props);
		}
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

			add_maprefnode(root, (NP2DSAsset*)args.data, NULL);
		}
	}
}

/*
//method
*/
xui_method_explain(onity_hierarchy, add_filternode,			xui_treenode*		)( const std::string& name )
{
	u32 index = m_tree->get_upmostnodecount();
	return m_tree->add_upmostnode(index, new onity_filterdata(NULL, new onity_propentitytemp(name)));
}
xui_method_explain(onity_hierarchy, get_filternode,			xui_treenode*		)( const std::string& name )
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
		filternode = add_filternode(name);
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
	return m_tree->add_upmostnode(index, new onity_filterdata(NULL, new onity_propscenelayer(scenelayer)));
}
xui_method_explain(onity_hierarchy, add_scenelayer,			xui_treenode*		)( NP2DSSceneLayer* src )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return NULL;

	NP2DSSceneLayer* scenelayer = m_editprop->get_scenefile()->AddSceneLayer(NP2DSSceneLayer::LT_SCENE, 1, 1, 8000, 640);
	scenelayer->Clone(src);
	u32 index = m_tree->get_upmostnodecount();
	xui_treenode* root = m_tree->add_upmostnode(index, new onity_filterdata(NULL, new onity_propscenelayer(scenelayer)));

	std::list<NPRenderObject*> children = scenelayer->GetChildren();
	for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
	{
		NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, (*itor));
		u32 index = root->get_leafnodecount();
		root->add_leafnode(index, new onity_maprefdata(NULL, new onity_propmapref(transref)));
	}

	return root;
}
xui_method_explain(onity_hierarchy, add_maprefnode,			xui_treenode*		)( xui_treenode* root, NP2DSAsset* asset, NP2DSTransRef* src )
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
	return root->add_leafnode(index, new onity_maprefdata(NULL, new onity_propmapref(transref)));
}
xui_method_explain(onity_hierarchy, del_coursenode,			void				)( const std::vector<xui_treenode*>& nodes )
{
	if (m_editprop == NULL || onity_mainform::get_ptr()->was_gamerun())
		return;

	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode* node = nodes[i];
		if (node->get_rootnode())
			del_maprefnode(node);
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
xui_method_explain(onity_hierarchy, del_maprefnode,			void				)( xui_treenode* node )
{
	xui_treenode*		   root     = node->get_rootnode();
	onity_filterdata*	   rootdata = (onity_filterdata*)root->get_linkdata();
	onity_propscenelayer*  rootprop = dynamic_cast<onity_propscenelayer*>(rootdata->get_prop());
	onity_maprefdata*	   nodedata = (onity_maprefdata*)node->get_linkdata();
	onity_propmapref*      nodeprop = dynamic_cast<onity_propmapref*>(nodedata->get_prop());
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
			add_maprefnode(root, NULL, m_menuvec[i]);
	}

	m_menuvec.clear();
}