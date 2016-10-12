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
#include "xui_menuitem.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_action_ctrl.h"
#include "onity_propcourse.h"
#include "onity_propscenelayer.h"
#include "onity_propedit.h"
#include "onity_filterdata.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_gradpane.h"
#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_scene.h"

xui_implement_rtti(onity_scene, xui_dockpage);
extern bool gInitCompleted;

/*
//constructor
*/
xui_create_explain(onity_scene)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_trans(xui_vector<s32>(0))
, m_ratio(1.0)
, m_dragview(false)
{
	ini_namectrl(onity_resource::icon_scene, L"Scene");

	m_headpane  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_headpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_headpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_headpane,	set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_headpane,	set_hscrollauto	)(false);

	m_horzgrad	= new onity_gradpane(FLOWSTYLE_H);
	m_vertgrad	= new onity_gradpane(FLOWSTYLE_V);
	m_drawview	= new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_drawview,	xm_updateself	) += new xui_method_member<xui_method_update,	onity_scene>(this, &onity_scene::on_drawviewupdateself);
	xui_method_ptrcall(m_drawview,	xm_renderself	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview,	xm_renderelse	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewrenderelse);
	xui_method_ptrcall(m_drawview,	xm_setrendersz	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewsetrendersz);
	xui_method_ptrcall(m_drawview,	xm_noncatch		) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewnoncatch);
	xui_method_ptrcall(m_drawview,	xm_keybddown	) += new xui_method_member<xui_method_keybd,	onity_scene>(this, &onity_scene::on_drawviewkeybddown);
	xui_method_ptrcall(m_drawview,	xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousemove);
	xui_method_ptrcall(m_drawview,	xm_mouserise	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmouserise);
	xui_method_ptrcall(m_drawview,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousewheel);
	xui_method_ptrcall(m_drawview,	xm_mousedragover) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragover);
	xui_method_ptrcall(m_drawview,	xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragdrop);

	m_small		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_small,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_small,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_small,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_small,		ini_drawer		)(onity_resource::icon_small,  xui_vector<s32>(12));
	xui_method_ptrcall(m_small,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_buttonclick);
	m_large		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_large,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_large,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_large,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_large,		ini_drawer		)(onity_resource::icon_large,  xui_vector<s32>(12));
	xui_method_ptrcall(m_large,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_buttonclick);
	m_reset		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_reset,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_reset,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_reset,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_reset,		ini_drawer		)(onity_resource::icon_reset,  xui_vector<s32>(12));
	xui_method_ptrcall(m_reset,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_buttonclick);
	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_clear,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_clear,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_clear,		ini_drawer		)(onity_resource::icon_delete, xui_vector<s32>(12));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_buttonclick);

	m_cubepane  = new xui_panel(xui_vector<s32>(40));
	xui_method_ptrcall(m_cubepane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_cubepane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_cubepane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_cubepane,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cubepane,	add_child		)(m_small);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_large);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_reset);
	xui_method_ptrcall(m_cubepane,	add_child		)(m_clear);

	m_fillpane  = new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fillpane,	xm_perform		) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_fillpaneperform);
	xui_method_ptrcall(m_fillpane,	xm_renderelse	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_fillpanerenderelse);
	xui_method_ptrcall(m_fillpane,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fillpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_fillpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_fillpane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_fillpane,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_fillpane,	set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_cubepane);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_horzgrad);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_vertgrad);
	xui_method_ptrcall(m_fillpane,	add_child		)(m_drawview);

	m_animctrl	= new xui_action_ctrl_impl<f64>(this);
	xui_method_ptrcall(m_animctrl,	set_soft		)(true);
	xui_method_ptrcall(m_animctrl,	xm_tick			) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_animctrltick);

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_scene)( void )
{
	delete m_animctrl;
}

/*
//method
*/
xui_method_explain(onity_scene, get_trans,					const xui_vector<s32>&	)( void ) const
{
	return m_trans;
}
xui_method_explain(onity_scene, set_trans,					void					)( const xui_vector<s32>& trans )
{
	m_trans = trans;
	m_horzgrad->set_trans(trans.x);
	m_vertgrad->set_trans(trans.y);
}
xui_method_explain(onity_scene, get_ratio,					f64						)( void ) const
{
	return m_ratio;
}
xui_method_explain(onity_scene, set_ratio,					void					)( f64 ratio )
{
	m_ratio = ratio;
	m_horzgrad->set_ratio(ratio);
	m_vertgrad->set_ratio(ratio);
	m_animctrl->clear();
}
xui_method_explain(onity_scene, set_nodevisible,			void					)( xui_treenode* node )
{
	if (onity_mainform::get_ptr()->was_gamerun())
		return;

	onity_treedata* data = (onity_treedata*)node->get_linkdata();
	onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
	if (prop)
	{
		xui_rect2d<s32> rt    = prop->get_bounding();
		xui_vector<s32> start = -m_trans;
		xui_vector<s32> range = (m_drawview->get_rendersz().to<f64>() / m_ratio).to<s32>();
		if (start.x > rt.ax)
			start.x = rt.ax;
		if (start.y > rt.ay)
			start.y = rt.ay;
		if (start.x+range.w < rt.bx)
			start.x = rt.bx-range.w;
		if (start.y+range.h < rt.by)
			start.y = rt.by-range.h;

		set_trans(-start);
	}
}

/*
//override
*/
xui_method_explain(onity_scene, on_updateself,				void					)( xui_method_update& args )
{
	xui_dockpage::on_updateself(args);
	m_animctrl->update(args.delta);
}

/*
//event
*/
xui_method_explain(onity_scene, on_buttonclick,				void					)( xui_component* sender, xui_method_args&		args )
{
	if (sender == m_large)
	{
		set_ratio(get_incratio());
	}
	else
	if (sender == m_small)
	{
		set_ratio(get_decratio());
	}
	else
	if (sender == m_reset)
	{
		set_trans(xui_vector<s32>(0));
		set_ratio(1.0);
	}
	else
	if (sender == m_clear)
	{
		m_horzgrad->del_lines();
		m_vertgrad->del_lines();
	}
}
xui_method_explain(onity_scene, on_fillpaneperform,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_method_ptrcall(m_horzgrad, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_vertgrad, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_drawview, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_drawview, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_horzgrad, on_perform_w)(m_fillpane->get_renderw()-m_horzgrad->get_renderx());
	xui_method_ptrcall(m_vertgrad, on_perform_h)(m_fillpane->get_renderh()-m_vertgrad->get_rendery());
	xui_method_ptrcall(m_drawview, on_perform_w)(m_horzgrad->get_renderw());
	xui_method_ptrcall(m_drawview, on_perform_h)(m_vertgrad->get_renderh());
}
xui_method_explain(onity_scene, on_fillpanerenderelse,		void					)( xui_component* sender, xui_method_args&		args )
{
	xui_vector<s32> p1;
	xui_vector<s32> p2;
	xui_vector<s32> pt = m_fillpane->get_screenpt();
	p1 = xui_vector<s32>(0,							m_horzgrad->get_renderh());
	p2 = xui_vector<s32>(m_fillpane->get_renderw(), m_horzgrad->get_renderh());
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::black);
	p1 = xui_vector<s32>(m_vertgrad->get_renderw(), 0);
	p2 = xui_vector<s32>(m_vertgrad->get_renderw(), m_fillpane->get_renderh());
	xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::black);
}
xui_method_explain(onity_scene, on_drawviewupdateself,		void					)( xui_component* sender, xui_method_update&	args )
{
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
	std::vector<xui_treenode*>    nodevec = hierarchy->get_treeview()->get_selectednode();;
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
		p1 = xui_vector<s32>((s32)(m_trans.x*m_ratio),	0);
		p2 = xui_vector<s32>((s32)(m_trans.x*m_ratio),	m_drawview->get_renderh());
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);
		p1 = xui_vector<s32>(0,							(s32)(m_trans.y*m_ratio));
		p2 = xui_vector<s32>(m_drawview->get_renderw(), (s32)(m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour::gray);

		std::vector<s32> linevec;
		linevec = m_horzgrad->get_lines();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			s32 line = linevec[i];
			p1 = xui_vector<s32>((s32)(m_trans.x*m_ratio + line*m_ratio), 0);
			p2 = xui_vector<s32>((s32)(m_trans.x*m_ratio + line*m_ratio), m_drawview->get_renderh());
			xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
		}
		linevec = m_vertgrad->get_lines();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			s32 line = linevec[i];
			p1 = xui_vector<s32>(0,							(s32)(m_trans.y*m_ratio + line*m_ratio));
			p2 = xui_vector<s32>(m_drawview->get_renderw(),	(s32)(m_trans.y*m_ratio + line*m_ratio));
			xui_convas::get_ins()->draw_line(pt+p1, pt+p2, xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
		}

		for (u32 i = 0; i < currvec.size(); ++i)
		{
			NP2DSSceneLayer* layer = currvec[i];

			xui_rect2d<s32> rt;
			rt.ax  = (s32)(layer->GetWorldTrans().x*m_ratio + m_trans.x*m_ratio);
			rt.ay  = (s32)(layer->GetWorldTrans().y*m_ratio + m_trans.y*m_ratio);
			rt.set_w((s32)(layer->GetCellW()*layer->GetCellC()*m_ratio));
			rt.set_h((s32)(layer->GetCellH()*layer->GetCellR()*m_ratio));
			xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour::gray);
		}
	}

	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		if (nodevec[i]->get_rootnode() == NULL)
			continue;

		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		xui_rect2d<s32> rt   = prop->get_bounding();
		rt.ax = (s32)(rt.ax*m_ratio + m_trans.x*m_ratio);
		rt.ay = (s32)(rt.ay*m_ratio + m_trans.y*m_ratio);
		rt.bx = (s32)(rt.bx*m_ratio + m_trans.x*m_ratio);
		rt.by = (s32)(rt.by*m_ratio + m_trans.y*m_ratio);
		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.7f));
	}

	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(onity_scene, on_drawviewsetrendersz,		void					)( xui_component* sender, xui_method_args&		args )
{
	if (gInitCompleted == false)
		return;

	//xui_vector<s32> size = sender->get_rendersz();
	//NPRender::GetIns()->SetResolutionW((npu32)size.w);
	//NPRender::GetIns()->SetResolutionH((npu32)size.h);
	//NPConfig::CalAdapterScaleValue();
	//BreezeGame::GameConfig::Instance()->InitAdaptation();
}
xui_method_explain(onity_scene, on_drawviewnoncatch,		void					)( xui_component* sender, xui_method_args&		args )
{
	m_dragview = false;
	m_drawview->set_cursor(CURSOR_DEFAULT);
	xui_global::set_cursor(CURSOR_DEFAULT);
}
xui_method_explain(onity_scene, on_drawviewkeybddown,		void					)( xui_component* sender, xui_method_keybd&		args )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	if (args.kcode == KEY_ESC)
	{
		treeview->non_selectednode();
	}
	else
	if (args.kcode == KEY_DELETE)
	{
		hierarchy->del_coursenode();
	}
	else
	if (args.kcode == KEY_LARROW ||
		args.kcode == KEY_RARROW ||
		args.kcode == KEY_UARROW ||
		args.kcode == KEY_DARROW)
	{
		xui_vector<s32> delta(0);
		switch (args.kcode)
		{
		case KEY_LARROW: delta.x = -1; break;
		case KEY_RARROW: delta.x =  1; break;
		case KEY_UARROW: delta.y = -1; break;
		case KEY_DARROW: delta.y =  1; break;
		}

		std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*	node = nodevec[i];
			onity_treedata* data = (onity_treedata*)node->get_linkdata();
			onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
			xui_vector<s32> pos  = prop->get_position();
			prop->set_position(pos+delta);
		}
	}
}
xui_method_explain(onity_scene, on_drawviewmousedown,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (args.mouse == MB_L)
	{
		xui_vector<s32>  pt			= m_drawview->get_renderpt(args.point);
		onity_propedit*  pickprop	= NULL;
		xui_proproot_vec propvec	= get_propcand();

		s32 transx = (s32)(m_trans.x*m_ratio);
		s32 transy = (s32)(m_trans.y*m_ratio);
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> rt		= prop->get_bounding();
			if (pt.x > (s32)(transx + rt.ax*m_ratio) &&
				pt.y > (s32)(transy + rt.ay*m_ratio) &&
				pt.x < (s32)(transx + rt.bx*m_ratio) &&
				pt.y < (s32)(transy + rt.by*m_ratio))
			{
				pickprop = prop;
				break;
			}
		}

		onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
		xui_treeview* treeview = hierarchy->get_treeview();
		if (pickprop)
		{
			xui_treenode* node = pickprop->get_linkdata()->get_node();
			if (args.ctrl)
			{
				treeview->set_selectednode(node, !node->was_selected());
			}
			else
			if (node->was_selected() == false)
			{
				treeview->non_selectednode(false);
				treeview->set_selectednode(node, true);
			}

			if (m_drawview->has_catch() && node->was_selected())
			{
				m_drawview->set_cursor(CURSOR_HAND);
				xui_global::set_cursor(CURSOR_HAND);
			}
		}
		else
		{
			//Multi-Select
		}
	}
	else
	if (args.mouse == MB_R)
	{
		m_dragview  = true;
		m_drawview->set_cursor(CURSOR_HAND);
		xui_global::set_cursor(CURSOR_HAND);
		xui_desktop::get_ins()->set_catchctrl(m_drawview);
	}
}
xui_method_explain(onity_scene, on_drawviewmousemove,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (m_dragview)
	{
		xui_vector<s32> delta = xui_desktop::get_ins()->get_mousemove();
		delta.x = (s32)(delta.x / m_ratio);
		delta.y = (s32)(delta.y / m_ratio);
		set_trans(m_trans+delta);
	}
	else
	if (has_catch())
	{
		//Drag
	}
}
xui_method_explain(onity_scene, on_drawviewmouserise,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (args.mouse == MB_L)
	{
		//Multi-Select
	}
	else
	if (args.mouse == MB_R)
	{
		xui_desktop::get_ins()->set_catchctrl(NULL);
	}
}
xui_method_explain(onity_scene, on_drawviewmousewheel,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (args.handle == false)
	{
		args.handle  = true;

		f64 start = m_ratio;
		f64 final = m_ratio;
		if (args.wheel > 0) final = get_incratio();
		else 				final = get_decratio();

		xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
		action->clear();
		action->add_time(0.0f);
		action->add_time(0.5f);
		action->add_data(start);
		action->add_data(final);
		action->set_play(true);
	}
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
	pos.x = -m_trans.x + (s32)(pt.x/m_ratio);
	pos.y = -m_trans.y + (s32)(pt.y/m_ratio);
	hierarchy->add_maprefnode(pos, (NP2DSAsset*)args.data);
}
xui_method_explain(onity_scene, on_animctrltick,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	m_ratio = action->sample();
	m_horzgrad->set_ratio(m_ratio);
	m_vertgrad->set_ratio(m_ratio);
	if (m_ratio == 0.1 || m_ratio == 5.0)
		m_animctrl->clear();
}

/*
//method
*/
xui_method_explain(onity_scene, get_incratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio < 1.0)	ratio += 0.1;
	else				ratio += 1.0;

	return xui_min(ratio, 5.0);
}
xui_method_explain(onity_scene, get_decratio,				f64						)( void )
{
	xui_action_ctrl_impl<f64>* action = (xui_action_ctrl_impl<f64>*)m_animctrl;
	f64 ratio = action->has_data() ? action->get_data(1) : m_ratio;
	if (ratio <= 1.0)	ratio -= 0.1;
	else				ratio -= 1.0;

	return xui_max(ratio, 0.1);
}
xui_method_explain(onity_scene, get_propcand,				xui_proproot_vec		)( void )
{
	xui_proproot_vec vec;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();

	std::vector<xui_treenode*> rootvec;
	std::vector<xui_treenode*> selnode = treeview->get_selectednode();
	if (onity_mainform::get_ptr()->was_gamerun() || selnode.empty())
	{
		rootvec = treeview->get_upmostnodearray();
	}
	else
	{
		for (u32 i = 0; i < selnode.size(); ++i)
		{
			xui_treenode* root = selnode[i];
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