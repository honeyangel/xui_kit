#include "NPRender.h"
#include "NPConfig.h"
#include "NP2DSSceneFile.h"
#include "NP2DSSceneLayer.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"
#include "NPGUIDesktop.h"
#include "NPGUIImeManagerWin32.h"
#include "m3eFrameWork.h"
#include "Game/GameConfig.h"
#include "Entity/Component/Visual/WGVisualManager.h"

#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_toolbar.h"
#include "xui_linebox.h"
#include "xui_menuitem.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_action_ctrl.h"
#include "onity_propcourse.h"
#include "onity_propscenelayer.h"
#include "onity_propcoursenode.h"
#include "onity_filterdata.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_snaptool.h"
#include "onity_gradpane.h"
#include "onity_alignbox.h"
#include "onity_blankbox.h"
#include "onity_pivotbox.h"
#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_scene.h"

xui_implement_rtti(onity_scene, onity_asset);
extern bool gInitCompleted;

/*
//constructor
*/
xui_create_explain(onity_scene)( void )
: onity_asset()
{
	ini_namectrl(onity_resource::icon_scene, L"Scene");

	m_showrect	= new xui_toggle(xui_vector<s32>(20, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_showrect,	ini_drawer		)(onity_resource::icon_pivot);
	xui_method_ptrcall(m_showrect,	set_drawcolor	)(true);
	xui_method_ptrcall(m_showrect,	set_enable		)(false);
	xui_method_ptrcall(m_showrect,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_showrect,	xm_toggleclick	) += new xui_method_member<xui_method_args,	onity_scene>(this, &onity_scene::on_toggleclick);
	m_horzflip	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_horzflip,	ini_drawer		)(onity_resource::icon_leftright);
	xui_method_ptrcall(m_horzflip,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzflip,	set_enable		)(false);
	xui_method_ptrcall(m_horzflip,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzflip,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_scene>(this, &onity_scene::on_buttonclick);
	m_vertflip	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_vertflip,	ini_drawer		)(onity_resource::icon_topbottom);
	xui_method_ptrcall(m_vertflip,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertflip,	set_enable		)(false);
	xui_method_ptrcall(m_vertflip,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertflip,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_scene>(this, &onity_scene::on_buttonclick);
	xui_linebox* linebox1 = new xui_linebox(xui_vector<s32>(20));
	xui_method_ptrcall(linebox1,	set_corner		)(3);
	xui_method_ptrcall(linebox1,	add_linectrl	)(m_horzflip);
	xui_method_ptrcall(linebox1,	add_linectrl	)(m_vertflip);
	xui_method_ptrcall(linebox1,	refresh			)();

	m_cwrotate	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_cwrotate,	ini_drawer		)(onity_resource::icon_cwrotate);
	xui_method_ptrcall(m_cwrotate,	set_drawcolor	)(true);
	xui_method_ptrcall(m_cwrotate,	set_enable		)(false);
	xui_method_ptrcall(m_cwrotate,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_cwrotate,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_scene>(this, &onity_scene::on_buttonclick);
	m_ccrotate	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_ccrotate,	ini_drawer		)(onity_resource::icon_ccrotate);
	xui_method_ptrcall(m_ccrotate,	set_drawcolor	)(true);
	xui_method_ptrcall(m_ccrotate,	set_enable		)(false);
	xui_method_ptrcall(m_ccrotate,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_ccrotate,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_scene>(this, &onity_scene::on_buttonclick);
	xui_linebox* linebox2 = new xui_linebox(xui_vector<s32>(20));
	xui_method_ptrcall(linebox2,	set_corner		)(3);
	xui_method_ptrcall(linebox2,	add_linectrl	)(m_ccrotate);
	xui_method_ptrcall(linebox2,	add_linectrl	)(m_cwrotate);
	xui_method_ptrcall(linebox2,	refresh			)();

	m_snaptool	= new onity_snaptool(m_drawview);
	m_alignbox	= new onity_alignbox(xui_vector<s32>(20), get_selectedbounding);
	m_blankbox	= new onity_blankbox(xui_vector<s32>(20), get_selectedbounding);
	m_pivotbox	= new onity_pivotbox(m_drawpane);
	xui_method_ptrcall(m_linetool,	add_item		)(m_snaptool->get_snapctrl());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_alignbox->get_horzline());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_alignbox->get_vertline());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_blankbox->get_horzline());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_blankbox->get_vertline());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_showrect);
	xui_method_ptrcall(m_linetool,	add_item		)(linebox1);
	xui_method_ptrcall(m_linetool,	add_item		)(linebox2);
	xui_method_ptrcall(m_drawview,	xm_mousedragover) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragover);
	xui_method_ptrcall(m_drawview,	xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragdrop);

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_scene)( void )
{
	delete m_alignbox;
	delete m_blankbox;
	delete m_pivotbox;
}

/*
//override
*/
xui_method_explain(onity_scene, set_trans,					void					)( const xui_vector<s32>& trans )
{
	onity_asset::set_trans(trans);
	xui_method_ptrcall(m_snaptool, set_trans)(trans);
}
xui_method_explain(onity_scene, set_ratio,					void					)( f64 ratio )
{
	onity_asset::set_ratio(ratio);
	xui_method_ptrcall(m_snaptool, set_ratio)(ratio);
}
xui_method_explain(onity_scene, set_nodevisible,			void					)( onity_bounding* prop )
{
	if (onity_mainform::get_ptr()->was_gamerun())
		return;

	onity_asset::set_nodevisible(prop);
}
xui_method_explain(onity_scene, set_toolupdate,				void					)( void )
{
	onity_bounding_vec vec = get_selectedbounding();
	xui_method_ptrcall(m_horzflip,	set_enable	)(vec.size() > 1);
	xui_method_ptrcall(m_vertflip,	set_enable	)(vec.size() > 1);
	xui_method_ptrcall(m_cwrotate,	set_enable	)(vec.size() > 1);
	xui_method_ptrcall(m_ccrotate,	set_enable	)(vec.size() > 1);
	xui_method_ptrcall(m_showrect,	set_enable	)(vec.size() > 1);
	xui_method_ptrcall(m_showrect,	set_push	)(vec.size() > 1 && m_showrect->was_push());
	m_alignbox->set_lineupdate();
	m_blankbox->set_lineupdate();
	m_pivotbox->set_rectupdate();
}
xui_method_explain(onity_scene, hit_propvisible,			onity_proproot*			)( const xui_vector<s32>& pt )
{
	onity_proproot*  pick		= NULL;
	xui_proproot_vec propvec	= get_propcand();

	for (u32 i = 0; i < propvec.size(); ++i)
	{
		onity_propeditnode*  prop = dynamic_cast<onity_propeditnode*>(propvec[i]);
		xui_rect2d<s32> rt = prop->get_bounding(m_trans, m_ratio);
		if (rt.was_inside(pt))
		{
			pick = prop;
			break;
		}
	}

	return pick;
}

/*
//static
*/
xui_method_explain(onity_scene, get_selectedbounding,		onity_bounding_vec		)( void )
{
	onity_bounding_vec vec;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode* node = nodevec[i];
		if (node->get_rootnode())
		{
			onity_treedata*		data = (onity_treedata*)node->get_linkdata();
			onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
			vec.push_back(prop);
		}
	}

	return vec;
}

/*
//event
*/
xui_method_explain(onity_scene, on_buttonclick,				void					)( xui_component* sender, xui_method_args&		args )
{
	onity_asset::on_buttonclick(sender, args);

	if (sender == m_horzflip ||
		sender == m_vertflip)
	{
		m_pivotbox->set_visible(true);
		onity_bounding_vec vec = get_selectedbounding();
		mir_propnode(vec, m_pivotbox->ori_pivot(), (sender == m_horzflip ? 1 : -1));
		m_pivotbox->set_rectupdate(false);
	}
	else
	if (sender == m_cwrotate ||
		sender == m_ccrotate)
	{
		m_pivotbox->set_visible(true);
		onity_bounding_vec vec = get_selectedbounding();
		rot_propnode(vec, m_pivotbox->ori_pivot(), (sender == m_cwrotate) ? 1 : -1);
		m_pivotbox->set_rectupdate(false);
	}
}
xui_method_explain(onity_scene, on_fillpanekeybddown,		void					)( xui_component* sender, xui_method_keybd&		args )
{
	onity_asset::on_fillpanekeybddown(sender, args);

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	if (args.kcode == KEY_ESC)
	{
		m_pivotbox->set_visible(false);
		treeview->non_selectednode();
	}
	else
	if (args.kcode == KEY_DELETE)
	{
		m_pivotbox->set_visible(false);
		hierarchy->del_coursenode();
	}
}
xui_method_explain(onity_scene, on_drawviewnoncatch,		void					)( xui_component* sender, xui_method_args&		args )
{
	onity_asset::on_drawviewnoncatch(sender, args);

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	for (u32 i= 0; i < nodevec.size(); ++i)
	{
		xui_treenode*		node = nodevec[i];
		onity_treedata*		data = (onity_treedata*)node->get_linkdata();
		onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
		if (prop)
		{
			prop->set_lockdata(false);
		}
	}
}
xui_method_explain(onity_scene, on_drawviewupdateself,		void					)( xui_component* sender, xui_method_update&	args )
{
	onity_asset::on_drawviewupdateself(sender, args);

	if (gInitCompleted == false)
		return;

	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	m3eFrameWorkUpdate(args.delta);
}
xui_method_explain(onity_scene, on_drawviewrenderself,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.3f));
	if (gInitCompleted == false)
		return;

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	if (onity_mainform::get_ptr()->was_gamerun())
	{
		BreezeGame::WGVisualManager::Instance()->Display();
	}
	else
	{
		onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
		onity_propcourse* editprop = hierarchy->get_editprop();
		if (editprop)
		{
			std::vector<NP2DSSceneLayer*> currvec;
			std::vector<xui_treenode*>	  nodevec = hierarchy->get_treeview()->get_selectednode();
			for (u32 i = 0; i < nodevec.size(); ++i)
			{
				xui_treenode*			node = nodevec[i];
				xui_treenode*			root = (node->get_rootnode() == NULL) ? node : node->get_rootnode();
				onity_filterdata*		data = (onity_filterdata*)root->get_linkdata();
				onity_propscenelayer*	prop = dynamic_cast<onity_propscenelayer*>(data->get_prop());

				std::vector<NP2DSSceneLayer*>::iterator itor = std::find(
					currvec.begin(),
					currvec.end(),
					prop->get_scenelayer());

				if (itor == currvec.end())
					currvec.push_back(prop->get_scenelayer());
			}

			NP2DSSceneFile* scenefile = editprop->get_scenefile();
			for (u16 i = 0; i < scenefile->GetSceneLayerCount(); ++i)
			{
				NP2DSSceneLayer* scenelayer = scenefile->GetSceneLayer(i);
				std::vector<NP2DSSceneLayer*>::iterator itor = std::find(
					currvec.begin(),
					currvec.end(),
					scenelayer);

				f32 alpha = (itor == currvec.end()) ? 0.2f : 1.0f;
				scenelayer->RenderAlpha(alpha);
			}
		}
	}

	NPVector3 scale((f32)m_ratio,   (f32)m_ratio,   1.0f);
	NPVector3 trans((f32)m_trans.x, (f32)m_trans.y, 0.0f);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(trans);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(scale);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_scene, on_drawviewrenderelse,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_drawview->get_renderrtabs()));

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*>    nodevec = treeview->get_selectednode();
	std::vector<NP2DSSceneLayer*> currvec;

	xui_vector<s32> pt = m_drawview->get_screenpt();
	if (onity_mainform::get_ptr()->was_gamerun() == false)
	{
		onity_propcourse* editprop = hierarchy->get_editprop();
		if (editprop)
		{
			for (u32 i = 0; i < nodevec.size(); ++i)
			{
				xui_treenode*			node = nodevec[i];
				xui_treenode*			root = (node->get_rootnode() == NULL) ? node : node->get_rootnode();
				onity_filterdata*		data = (onity_filterdata*)root->get_linkdata();
				onity_propscenelayer*	prop = dynamic_cast<onity_propscenelayer*>(data->get_prop());

				std::vector<NP2DSSceneLayer*>::iterator itor = std::find(
					currvec.begin(),
					currvec.end(),
					prop->get_scenelayer());

				if (itor == currvec.end())
					currvec.push_back(prop->get_scenelayer());
			}
		}

		xui_vector<s32> p1;
		xui_vector<s32> p2;
		p1 = xui_vector<s32>(xui_round(m_trans.x*m_ratio),	0);
		p2 = xui_vector<s32>(xui_round(m_trans.x*m_ratio),	m_drawview->get_renderh());
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);
		p1 = xui_vector<s32>(0,							xui_round(m_trans.y*m_ratio));
		p2 = xui_vector<s32>(m_drawview->get_renderw(), xui_round(m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);

		std::vector<s32> linevec;
		linevec = m_horzgrad->get_lines();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			s32 line = linevec[i];
			p1 = xui_vector<s32>(xui_round(m_trans.x*m_ratio + line*m_ratio), 0);
			p2 = xui_vector<s32>(xui_round(m_trans.x*m_ratio + line*m_ratio), m_drawview->get_renderh());
			xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
		}
		linevec = m_vertgrad->get_lines();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			s32 line = linevec[i];
			p1 = xui_vector<s32>(0,							xui_round(m_trans.y*m_ratio + line*m_ratio));
			p2 = xui_vector<s32>(m_drawview->get_renderw(),	xui_round(m_trans.y*m_ratio + line*m_ratio));
			xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
		}

		for (u32 i = 0; i < currvec.size(); ++i)
		{
			NP2DSSceneLayer* layer = currvec[i];

			xui_rect2d<s32> rt;
			rt.ax  = xui_round(layer->GetWorldTrans().x*m_ratio + m_trans.x*m_ratio);
			rt.ay  = xui_round(layer->GetWorldTrans().y*m_ratio + m_trans.y*m_ratio);
			rt.set_w(xui_round(layer->GetCellW()*layer->GetCellC()*m_ratio));
			rt.set_h(xui_round(layer->GetCellH()*layer->GetCellR()*m_ratio));
			xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour::gray);
		}
	}

	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		if (nodevec[i]->get_rootnode() == NULL)
			continue;

		xui_treenode*		 node = nodevec[i];
		onity_treedata*		 data = (onity_treedata*)node->get_linkdata();
		onity_propeditnode*  prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
		xui_rect2d<s32> rt = prop->get_bounding(m_trans, m_ratio);
		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.7f));
	}

	if (m_dragprop)
	{
		onity_bounding_vec	selectedvec = get_selectedbounding();
		onity_bounding*		self = m_snaptool->cal_selfprop(selectedvec);
		onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(self);
		if (prop)
		{
			onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
			xui_treenode* root = prop->get_linkdata(hierarchy->get_treeview())->get_node()->get_rootnode();

			onity_bounding_vec vec;
			for (u32 i = 0; i < root->get_leafnodecount(); ++i)
			{
				if (root->get_leafnode(i)->was_selected())
					continue;

				xui_treenode*		node = root->get_leafnode(i);
				onity_treedata*		data = (onity_treedata*)node->get_linkdata();
				onity_propeditnode* temp = dynamic_cast<onity_propeditnode*>(data->get_prop());
				vec.push_back(temp);
			}

			m_snaptool->set_snapdraw(vec, selectedvec);
		}
	}
	
	if (nodevec.size() > 0)
	{
		xui_treenode*		head = nodevec.front();
		onity_treedata*		data = (onity_treedata*)head->get_linkdata();
		onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
		xui_rect2d<s32>		rect = prop->get_bounding(m_trans, m_ratio);
		draw_locknode(rect);
	}

	draw_multisel();
	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(onity_scene, on_drawviewmousedown,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	m_showrect->set_push(false);
	onity_asset::on_drawviewmousedown(sender, args);
}
xui_method_explain(onity_scene, on_drawviewmouserise,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	onity_asset::on_drawviewmouserise(sender, args);
	if (args.mouse == MB_L)
	{
		if (m_dragprop && m_multisel == false)
		{
			onity_bounding_vec	selectedvec = get_selectedbounding();
			onity_bounding*		self = m_snaptool->cal_selfprop(selectedvec);
			onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(self);
			if (prop)
			{
				onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
				xui_treenode* root = prop->get_linkdata(hierarchy->get_treeview())->get_node()->get_rootnode();

				onity_bounding_vec vec;
				for (u32 i = 0; i < root->get_leafnodecount(); ++i)
				{
					if (root->get_leafnode(i)->was_selected())
						continue;

					xui_treenode*		node = root->get_leafnode(i);
					onity_treedata*		data = (onity_treedata*)node->get_linkdata();
					onity_propeditnode* temp = dynamic_cast<onity_propeditnode*>(data->get_prop());
					vec.push_back(temp);
				}

				m_snaptool->use_snapinfo(vec, selectedvec);
			}
		}
	}
}
xui_method_explain(onity_scene, on_drawviewmousewheel,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	onity_asset::on_drawviewmousewheel(sender, args);
	m_showrect->set_push(false);
}
xui_method_explain(onity_scene, on_drawviewmousedragover,	void					)( xui_component* sender, xui_method_dragdrop&	args )
{
	if (onity_mainform::get_ptr()->was_gamerun())
		return;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	if (treeview->get_upmostnodecount() > 0)
	{
		if (args.type == "NP2DSImage" ||
			args.type == "NP2DSFrame" ||
			args.type == "NP2DSActor")
		{
			args.allow = true;
		}
	}
}
xui_method_explain(onity_scene, on_drawviewmousedragdrop,	void					)( xui_component* sender, xui_method_dragdrop&	args )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_vector<s32> pt = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
	xui_vector<s32> pos;
	pos.x = xui_round(pt.x/m_ratio) - m_trans.x;
	pos.y = xui_round(pt.y/m_ratio) - m_trans.y;
	hierarchy->add_coursenode(pos, (NP2DSAsset*)args.data);
}
xui_method_explain(onity_scene, on_toggleclick,				void					)( xui_component* sender, xui_method_args&		args )
{
	if (sender == m_showrect)
	{
		m_pivotbox->set_visible(m_showrect->was_push());
	}

	m_drawview->req_focus();
}

/*
//override
*/
xui_method_explain(onity_scene, on_keybdmoveimpl,			void					)( const xui_vector<s32>& delta )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();

	if (nodevec.size())
	{
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*		node = nodevec[i];
			onity_treedata*		data = (onity_treedata*)node->get_linkdata();
			onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
			if (prop)
			{
				prop->set_position(prop->ori_position()+delta);
			}
		}

		m_pivotbox->set_rectupdate();
	}
}
xui_method_explain(onity_scene, on_mousepickimpl,			void					)( onity_proproot* pick, bool alt, bool ctrl, bool shift )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	xui_treenode* node = pick->get_linkdata(treeview)->get_node();
	if (alt && onity_mainform::get_ptr()->was_gamerun() == false)
	{
		std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
		std::vector<xui_treenode*> nextvec;
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*			currnode = nodevec[i];
			onity_treedata*			currdata = (onity_treedata*)currnode->get_linkdata();
			onity_propcoursenode*	currprop = dynamic_cast<onity_propcoursenode*>(currdata->get_prop());
			xui_treenode*			nextnode = hierarchy->add_coursenode(currnode->get_rootnode(), NULL, currprop->get_2dsref());
			onity_treedata*			nextdata = (onity_treedata*)nextnode->get_linkdata();
			onity_propcoursenode*	nextprop = dynamic_cast<onity_propcoursenode*>(nextdata->get_prop());

			if (node == currnode)
			{
				node =  nextnode;
				pick =  nextprop;
			}

			nextvec.push_back(nextnode);
		}
		xui_method_ptrcall(treeview,	non_selectednode)(false);
		xui_method_ptrcall(treeview,	set_selectednode)(nextvec);
	}
	else
	if (ctrl && !shift)
	{
		xui_method_ptrcall(treeview,	set_selectednode)(node, !node->was_selected());
	}
	else
	if (node->was_selected() == false)
	{
		xui_method_ptrcall(treeview,	non_selectednode)(false);
		xui_method_ptrcall(treeview,	set_selectednode)(node, true);
	}

	if (m_drawview->has_catch() && node->was_selected())
	{
		m_dragprop = true;
		xui_method_ptrcall(m_drawview,	set_cursor		)(CURSOR_HAND);
		xui_static_inscall(xui_global,	set_cursor		)(CURSOR_HAND);

		//lock update bounding
		std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*		temp = nodevec[i];
			onity_treedata*		data = (onity_treedata*)temp->get_linkdata();
			onity_propeditnode* edit = dynamic_cast<onity_propeditnode*>(data->get_prop());
			if (edit)
			{
				edit->get_position(m_trans, m_ratio);
				edit->get_bounding(m_trans, m_ratio);
				edit->set_lockdata(true);
			}
		}

		u08 mode = DRAGMOVE_UNLIMIT;
		if		(shift && ctrl)	mode = DRAGMOVE_Y;
		else if (shift)			mode = DRAGMOVE_X;
		else
		{}

		xui_treenode*			root = node->get_rootnode();
		onity_treedata*			data = (onity_treedata*)root->get_linkdata();
		onity_propscenelayer*	prop = dynamic_cast<onity_propscenelayer*>(data->get_prop());
		onity_propeditnode*		self = dynamic_cast<onity_propeditnode*>(pick);

		std::vector<onity_bounding*> vec;
		for (u32 i = 0; i < root->get_leafnodecount(); ++i)
		{
			if (root->get_leafnode(i)->was_selected())
				continue;

			xui_treenode*		temp = root->get_leafnode(i);
			onity_treedata*		data = (onity_treedata*)temp->get_linkdata();
			onity_propeditnode* edit = dynamic_cast<onity_propeditnode*>(data->get_prop());
			vec.push_back(edit);
		}

		s32 snaplength = (prop == NULL) ? 5 : prop->get_snaplength();
		m_snaptool->cal_snapinfo(
			vec,
			self->ori_bounding(), 
			mode, 
			snaplength);
	}
}
xui_method_explain(onity_scene, on_mousedragimpl,			void					)( const xui_vector<s32>& delta )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	if (nodevec.size() > 0)
	{
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*		node = nodevec[i];
			onity_treedata*		data = (onity_treedata*)node->get_linkdata();
			onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(data->get_prop());
			if (prop)
			{
				prop->mov_position(delta);

				xui_vector<s32> pos = prop->get_position(m_trans, m_ratio);
				pos.x = xui_round(pos.x/m_ratio - m_trans.x);
				pos.y = xui_round(pos.y/m_ratio - m_trans.y);
				prop->set_position(pos);
			}
		}
	}
}
xui_method_explain(onity_scene, on_mulselectimpl,			void					)( const xui_rect2d<s32>& rt, bool ctrl )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	std::vector<xui_treenode*> nextvec;
	xui_proproot_vec propvec = get_propcand();
	for (u32 i = 0; i < propvec.size(); ++i)
	{
		onity_propeditnode* prop = dynamic_cast<onity_propeditnode*>(propvec[i]);
		xui_rect2d<s32>		temp = prop->get_bounding(m_trans, m_ratio);
		if (temp.get_inter(rt).was_valid())
		{
			nextvec.push_back(prop->get_linkdata(treeview)->get_node());
		}
	}

	if (nextvec.size() > 0)
	{
		if (ctrl == false)
			treeview->non_selectednode(false);

		treeview->set_selectednode(nextvec);
	}
}

/*
//method
*/
xui_method_explain(onity_scene, get_propcand,				xui_proproot_vec		)( void )
{
	xui_proproot_vec vec;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	std::vector<xui_treenode*> rootvec;
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	if (onity_mainform::get_ptr()->was_gamerun() || nodevec.empty())
	{
		rootvec = treeview->get_upmostnodearray();
	}
	else
	{
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode* root = nodevec[i];
			if (root->get_rootnode())
				root = root->get_rootnode();

			std::vector<xui_treenode*>::iterator itor = std::find(
				rootvec.begin(),
				rootvec.end(),
				root);
			if (itor == rootvec.end())
				rootvec.push_back(root);
		}
	}

	for (std::vector<xui_treenode*>::iterator itor = rootvec.begin(); itor != rootvec.end(); ++itor)
	{
		std::vector<xui_treenode*> nodes;
		(*itor)->get_leafnodetotal(nodes);
		for (u32 i = 1; i < nodes.size(); ++i)
		{
			onity_treedata* data = (onity_treedata*)nodes[i]->get_linkdata();
			vec.push_back(data->get_prop());
		}
	}

	return vec;
}
xui_method_explain(onity_scene, mir_propnode,				void					)( const onity_bounding_vec& vec, const xui_vector<s32>& pivot, s32 style )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_bounding* prop = vec[i];
		xui_vector<s32> pt   = prop->ori_position();
		xui_rect2d<s32> rt	 = prop->ori_bounding();

		s32 w = rt.get_w() / 2;
		s32 h = rt.get_h() / 2;
		xui_vector<s32> center = rt.get_pt() + xui_vector<s32>(w, h);
		xui_vector<s32> offset = pt - center;
		if (style > 0)
			pt.x = pivot.x + (pivot.x-center.x) + offset.x;
		if (style < 0)
			pt.y = pivot.y + (pivot.y-center.y) + offset.y;

		prop->set_position(pt);
	}
}
xui_method_explain(onity_scene, rot_propnode,				void					)( const onity_bounding_vec& vec, const xui_vector<s32>& pivot, s32 style )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_bounding* prop = vec[i];
		xui_vector<s32> pt   = prop->ori_position();
		xui_rect2d<s32> rt   = prop->ori_bounding();

		s32 w = rt.get_w() / 2;
		s32 h = rt.get_h() / 2;
		xui_vector<s32> center = rt.get_pt() + xui_vector<s32>(w, h);
		xui_vector<s32> offset = pt - center;
		pt.x = pivot.x - (center.y-pivot.y)*style + offset.x;
		pt.y = pivot.y + (center.x-pivot.x)*style + offset.y;
		prop->set_position(pt);
	}
}

