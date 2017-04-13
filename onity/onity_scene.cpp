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
#include "onity_propedit.h"
#include "onity_propmapref.h"
#include "onity_filterdata.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_gradpane.h"
#include "onity_alignbox.h"
#include "onity_blankbox.h"
#include "onity_pivotbox.h"
#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_scene.h"

xui_implement_rtti(onity_scene, xui_dockpage);
extern bool gInitCompleted;

s32 xui_round( f64 value )
{
	return (s32)(value + (value > 0.0 ? 0.5 : -0.5));
}

/*
//constructor
*/
xui_create_explain(onity_scene)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_trans(xui_vector<s32>(0))
, m_ratio(1.0)
, m_dragview(false)
, m_dragprop(false)
, m_multisel(false)
{
	ini_namectrl(onity_resource::icon_scene, L"Scene");

	m_snapctrl	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_snapctrl,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_snapctrl,	set_corner		)(3);
	xui_method_ptrcall(m_snapctrl,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_snapctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_snapctrl,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_snapctrl,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_snapctrl,	ini_drawer		)(onity_resource::icon_magent);
	xui_method_ptrcall(m_snapctrl,	ini_drawer		)(L"Attract");
	xui_method_ptrcall(m_snapctrl,	ini_toggle		)(true);

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

	m_alignbox	= new onity_alignbox(xui_vector<s32>(20), get_selectedprop);
	m_blankbox	= new onity_blankbox(xui_vector<s32>(20), get_selectedprop);
	m_linetool	= new xui_toolbar(xui_vector<s32>(0, 20));
	xui_method_ptrcall(m_linetool,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_linetool,	add_item		)(m_snapctrl);
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

	m_headpane  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_headpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_headpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_headpane,	set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_headpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_headpane,	add_child		)(m_linetool);

	m_horzgrad	= new onity_gradpane(FLOWSTYLE_H);
	m_vertgrad	= new onity_gradpane(FLOWSTYLE_V);
	m_drawview	= new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_drawview,	xm_updateself	) += new xui_method_member<xui_method_update,	onity_scene>(this, &onity_scene::on_drawviewupdateself);
	xui_method_ptrcall(m_drawview,	xm_renderself	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview,	xm_renderelse	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewrenderelse);
	xui_method_ptrcall(m_drawview,	xm_setrendersz	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewsetrendersz);
	xui_method_ptrcall(m_drawview,	xm_noncatch		) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_drawviewnoncatch);
	xui_method_ptrcall(m_drawview,	xm_mouseenter	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmouseenter);
	xui_method_ptrcall(m_drawview,	xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousemove);
	xui_method_ptrcall(m_drawview,	xm_mouserise	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmouserise);
	xui_method_ptrcall(m_drawview,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_drawviewmousewheel);
	xui_method_ptrcall(m_drawview,	xm_mousedragover) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragover);
	xui_method_ptrcall(m_drawview,	xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop,	onity_scene>(this, &onity_scene::on_drawviewmousedragdrop);
	m_drawpane	= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_drawpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_drawpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	set_vscrollauto	)(false);
	xui_method_ptrcall(m_drawpane,	add_child		)(m_drawview);

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
	xui_method_ptrcall(m_fillpane,	xm_keybddown	) += new xui_method_member<xui_method_keybd,	onity_scene>(this, &onity_scene::on_fillpanekeybddown);
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
	xui_method_ptrcall(m_fillpane,	add_child		)(m_drawpane);

	m_pivotbox	= new onity_pivotbox(m_drawpane);
	m_animctrl	= new xui_action_ctrl_impl<f64>(this);
	xui_method_ptrcall(m_animctrl,	set_soft		)(true);
	xui_method_ptrcall(m_animctrl,	xm_tick			) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_animctrltick);

	xui_action_ctrl_impl<f32>* action = new xui_action_ctrl_impl<f32>(this);
	action->set_soft(true);
	action->add_time(0.0f);
	action->add_time(0.2f);
	action->add_time(0.4f);
	action->add_time(5.0f);
	action->add_data(1.0f);
	action->add_data(1.5f);
	action->add_data(1.0f);
	action->add_data(0.0f);
	m_lockctrl = action;

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_scene)( void )
{
	delete m_animctrl;
	delete m_lockctrl;
	delete m_alignbox;
	delete m_blankbox;
	delete m_pivotbox;
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
		xui_rect2d<s32> rt    = prop->ori_bounding();
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

		m_lockctrl->reset();
		m_lockctrl->set_play(true);
	}
}
xui_method_explain(onity_scene, set_toolupdate,				void					)( void )
{
	xui_proproot_vec vec = get_selectedprop();
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

/*
//override
*/
xui_method_explain(onity_scene, on_updateself,				void					)( xui_method_update& args )
{
	xui_dockpage::on_updateself(args);
	m_animctrl->update(args.delta);
	m_lockctrl->update(args.delta);
}

/*
//static
*/
xui_method_explain(onity_scene, get_selectedprop,			xui_proproot_vec		)( void )
{
	xui_proproot_vec vec;

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		if (prop)
		{
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
	else
	if (sender == m_horzflip ||
		sender == m_vertflip)
	{
		m_pivotbox->set_visible(true);
		xui_proproot_vec propvec = get_selectedprop();
		mir_proproot(propvec, m_pivotbox->ori_pivot(), (sender == m_horzflip ? 1 : -1));
		m_pivotbox->set_rectupdate(false);
	}
	else
	if (sender == m_cwrotate ||
		sender == m_ccrotate)
	{
		m_pivotbox->set_visible(true);
		xui_proproot_vec propvec = get_selectedprop();
		rot_proproot(propvec, m_pivotbox->ori_pivot(), (sender == m_cwrotate) ? 1 : -1);
		m_pivotbox->set_rectupdate(false);
	}

	m_drawview->req_focus();
}
xui_method_explain(onity_scene, on_toggleclick,				void					)( xui_component* sender, xui_method_args&		args )
{
	if (sender == m_showrect)
	{
		m_pivotbox->set_visible(m_showrect->was_push());
	}

	m_drawview->req_focus();
}
xui_method_explain(onity_scene, on_fillpaneperform,			void					)( xui_component* sender, xui_method_args&		args )
{
	xui_method_ptrcall(m_horzgrad, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_vertgrad, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_drawpane, on_perform_x)(m_vertgrad->get_renderw());
	xui_method_ptrcall(m_drawpane, on_perform_y)(m_horzgrad->get_renderh());
	xui_method_ptrcall(m_horzgrad, on_perform_w)(m_fillpane->get_renderw()-m_horzgrad->get_renderx());
	xui_method_ptrcall(m_vertgrad, on_perform_h)(m_fillpane->get_renderh()-m_vertgrad->get_rendery());
	xui_method_ptrcall(m_drawpane, on_perform_w)(m_horzgrad->get_renderw());
	xui_method_ptrcall(m_drawpane, on_perform_h)(m_vertgrad->get_renderh());
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
xui_method_explain(onity_scene, on_fillpanekeybddown,		void					)( xui_component* sender, xui_method_keybd&		args )
{
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
	else
	if (args.kcode == KEY_LARROW ||
		args.kcode == KEY_RARROW ||
		args.kcode == KEY_UARROW ||
		args.kcode == KEY_DARROW)
	{
		xui_vector<s32> move(0);
		switch (args.kcode)
		{
		case KEY_LARROW: move.x = -1; break;
		case KEY_RARROW: move.x =  1; break;
		case KEY_UARROW: move.y = -1; break;
		case KEY_DARROW: move.y =  1; break;
		}

		std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			xui_treenode*	node = nodevec[i];
			onity_treedata* data = (onity_treedata*)node->get_linkdata();
			onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
			if (prop)
			{
				prop->set_position(prop->ori_position()+move);
			}
		}

		m_pivotbox->set_rectupdate();
	}
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

		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		xui_rect2d<s32> rt   = prop->get_bounding(m_trans, m_ratio);
		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.7f));
	}

	if (m_dragprop && nodevec.size() > 0)
	{
		xui_vector<s32> snap;
		xui_vector<s32> step;
		xui_rect2d<s32> self;
		xui_rect2d<s32> horz;
		xui_rect2d<s32> vert;
		cal_snapmove(nodevec, snap, step, self, &horz, &vert);

		onity_snapinfo_map::iterator itor;
		if (step.x == 0)
		{
			itor = m_horzsnap.find(snap.x);
			if (itor != m_horzsnap.end())
				draw_horzsnap(self, (*itor).first, (*itor).second);
			itor = m_horzmidd.find(snap.x);
			if (itor != m_horzmidd.end())
				draw_horzsnap(self, (*itor).first, (*itor).second);
		}
		if (step.y == 0)
		{
			itor = m_vertsnap.find(snap.y);
			if (itor != m_vertsnap.end())
				draw_vertsnap(self, (*itor).first, (*itor).second);
			itor = m_vertmidd.find(snap.y);
			if (itor != m_vertmidd.end())
				draw_vertsnap(self, (*itor).first, (*itor).second);
		}

		if (step.x != 0)
		{
			itor = m_horzstep.find(step.x);
			if (itor != m_horzstep.end())
				draw_horzstep(self, (*itor).first, (*itor).second, horz);
		}
		if (step.y != 0)
		{
			itor = m_vertstep.find(step.y);
			if (itor != m_vertstep.end())
				draw_vertstep(self, (*itor).first, (*itor).second, vert);
		}
	}
	
	draw_locknode(nodevec);
	draw_multisel();
	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(onity_scene, on_drawviewsetrendersz,		void					)( xui_component* sender, xui_method_args&		args )
{
	if (gInitCompleted == false)
		return;
}
xui_method_explain(onity_scene, on_drawviewnoncatch,		void					)( xui_component* sender, xui_method_args&		args )
{
	m_dragview = false;
	m_dragprop = false;
	m_multisel = false;
	m_drawview->set_cursor(CURSOR_DEFAULT);
	xui_global::set_cursor(CURSOR_DEFAULT);

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
	for (u32 i= 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		if (prop)
		{
			prop->set_lockdata(false);
		}
	}
}
xui_method_explain(onity_scene, on_drawviewmouseenter,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	m_drawview->req_focus();
}
xui_method_explain(onity_scene, on_drawviewmousedown,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	m_showrect->set_push(false);

	if (args.mouse == MB_L)
	{
		xui_vector<s32>  pt			= m_drawview->get_renderpt(args.point);
		onity_propedit*  pick		= NULL;
		xui_proproot_vec propvec	= get_propcand();

		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> rt		= prop->get_bounding(m_trans, m_ratio);
			if (rt.was_inside(pt))
			{
				pick = prop;
				break;
			}
		}

		onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
		xui_treeview* treeview = hierarchy->get_treeview();

		if (pick)
		{
			xui_treenode* node = pick->get_linkdata()->get_node();
			if (args.alt  && onity_mainform::get_ptr()->was_gamerun() == false)
			{
				std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
				std::vector<xui_treenode*> nextvec;
				for (u32 i = 0; i < nodevec.size(); ++i)
				{
					xui_treenode*		currnode = nodevec[i];
					onity_treedata*		currdata = (onity_treedata*)currnode->get_linkdata();
					onity_propmapref*	currprop = dynamic_cast<onity_propmapref*>(currdata->get_prop());
					xui_treenode*		nextnode = hierarchy->add_maprefnode(currnode->get_rootnode(), NULL, currprop->get_2dsref());
					onity_treedata*		nextdata = (onity_treedata*)nextnode->get_linkdata();
					onity_propmapref*	nextprop = dynamic_cast<onity_propmapref*>(nextdata->get_prop());

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
			if (args.ctrl && !args.shift)
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
					xui_treenode*   temp = nodevec[i];
					onity_treedata* data = (onity_treedata*)temp->get_linkdata();
					onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
					if (prop)
					{
						prop->get_position(m_trans, m_ratio);
						prop->get_bounding(m_trans, m_ratio);
						prop->set_lockdata(true);
					}
				}

				u08 mode = DRAGMOVE_UNLIMIT;
				if		(args.shift && args.ctrl)	mode = DRAGMOVE_Y;
				else if (args.shift)				mode = DRAGMOVE_X;
				else
				{}

				xui_rect2d<s32> self = pick->ori_bounding();
				xui_treenode*   root = node->get_rootnode();
				cal_snapinfo(self, root, mode);
			}
		}
		else
		{
			m_multisel = true;
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

	xui_vector<s32> move(0);
	xui_vector<s32> drag = xui_desktop::get_ins()->get_mousemove();

	if (m_dragview)
	{
		move.x = xui_round(drag.x/m_ratio);
		move.y = xui_round(drag.y/m_ratio);
		set_trans(m_trans+move);
	}
	else
	if (m_dragprop)
	{
		onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
		xui_treeview* treeview = hierarchy->get_treeview();
		std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
		if (nodevec.size() > 0)
		{
			u08 mode = DRAGMOVE_UNLIMIT;
			if		(args.shift && args.ctrl)	mode = DRAGMOVE_Y;
			else if (args.shift)				mode = DRAGMOVE_X;
			else
			{}

			if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_X)
				move.x = drag.x;
			if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_Y)
				move.y = drag.y;

			for (u32 i = 0; i < nodevec.size(); ++i)
			{
				xui_treenode*	node = nodevec[i];
				onity_treedata* data = (onity_treedata*)node->get_linkdata();
				onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
				if (prop)
				{
					prop->mov_position(move);

					xui_vector<s32> pos = prop->get_position(m_trans, m_ratio);
					pos.x = xui_round(pos.x/m_ratio - m_trans.x);
					pos.y = xui_round(pos.y/m_ratio - m_trans.y);
					prop->set_position(pos);
				}
			}
		}
	}
}
xui_method_explain(onity_scene, on_drawviewmouserise,		void					)( xui_component* sender, xui_method_mouse&		args )
{
	if (gInitCompleted == false)
		return;

	if (args.mouse == MB_L)
	{
		if (m_dragprop)
		{
			use_snapinfo();
		}
		else
		if (m_multisel)
		{
			xui_vector<s32> p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
			xui_vector<s32> p2 = m_drawview->get_renderpt(args.point);
			xui_rect2d<s32> rt;
			rt.ax = xui_min(p1.x, p2.x);
			rt.bx = xui_max(p1.x, p2.x);
			rt.ay = xui_min(p1.y, p2.y);
			rt.by = xui_max(p1.y, p2.y);

			std::vector<xui_treenode*> nextvec;
			xui_proproot_vec propvec = get_propcand();
			for (u32 i = 0; i < propvec.size(); ++i)
			{
				onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
				xui_rect2d<s32> temp = prop->get_bounding(m_trans, m_ratio);
				if (temp.get_inter(rt).was_valid())
				{
					nextvec.push_back(prop->get_linkdata()->get_node());
				}
			}

			if (nextvec.size() > 0)
			{
				onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
				xui_treeview* treeview = hierarchy->get_treeview();
				if (args.ctrl == false)
					treeview->non_selectednode(false);

				treeview->set_selectednode(nextvec);
			}
		}
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
		m_showrect->set_push(false);

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
	pos.x = xui_round(pt.x/m_ratio) - m_trans.x;
	pos.y = xui_round(pt.y/m_ratio) - m_trans.y;
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
xui_method_explain(onity_scene, cal_snapinfo,				void					)( const xui_rect2d<s32>& self, xui_treenode* root, u08 mode )
{
	m_horzsnap.clear();
	m_vertsnap.clear();
	m_horzstep.clear();
	m_vertstep.clear();
	m_horzmidd.clear();
	m_vertmidd.clear();

	if (m_snapctrl->was_push() == false)
		return;

	s32 snaplength = 0;
	onity_treedata*		  rootdata = (onity_treedata*)root->get_linkdata();
	onity_propscenelayer* rootprop = dynamic_cast<onity_propscenelayer*>(rootdata->get_prop());
	if (rootprop)
	{
		snaplength = rootprop->get_snaplength();
	}

	for (u32 i = 0; i < root->get_leafnodecount(); ++i)
	{
		xui_treenode*   currnode = root->get_leafnode(i);
		if (currnode->was_selected())
			continue;

		onity_treedata* currdata = (onity_treedata*)currnode->get_linkdata();
		onity_propedit* currprop = dynamic_cast<onity_propedit*>(currdata->get_prop());
		xui_rect2d<s32> currrect = currprop->ori_bounding();
		if (snaplength > 0)
		{
			if (currrect.bx < self.ax-snaplength ||
				currrect.ax > self.bx+snaplength)
				continue;
		}

		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_X)
		{
			s32 l = currrect.ax;
			s32 r = currrect.bx;
			s32 c = currrect.ax+currrect.get_w()/2;
			m_horzsnap[l].push_back(snap_info(currprop));
			m_horzsnap[r].push_back(snap_info(currprop));
			m_horzmidd[c].push_back(snap_info(currprop));
		}
		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_Y)
		{
			s32 t = currrect.ay;
			s32 b = currrect.by;
			s32 c = currrect.ay+currrect.get_h()/2;
			m_vertsnap[t].push_back(snap_info(currprop));
			m_vertsnap[b].push_back(snap_info(currprop));
			m_vertmidd[c].push_back(snap_info(currprop));
		}
	}

	if (root->get_leafnodecount() < 2)
		return;

	for (u32 i = 0; i < root->get_leafnodecount()-1; ++i)
	{
		xui_treenode*   currnode = root->get_leafnode(i);
		xui_treenode*   nextnode = root->get_leafnode(i+1);
		if (currnode->was_selected() ||
			nextnode->was_selected())
			continue;

		onity_treedata* currdata = (onity_treedata*)currnode->get_linkdata();
		onity_propedit* currprop = dynamic_cast<onity_propedit*>(currdata->get_prop());
		xui_rect2d<s32> currrect = currprop->ori_bounding();
		onity_treedata* nextdata = (onity_treedata*)nextnode->get_linkdata();
		onity_propedit* nextprop = dynamic_cast<onity_propedit*>(nextdata->get_prop());
		xui_rect2d<s32> nextrect = nextprop->ori_bounding();

		if (snaplength > 0)
		{
			if (currrect.bx < self.ax-snaplength ||
				currrect.ax > self.bx+snaplength)
				continue;
		}

		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_X)
		{
			s32 horzstep = 0;
			if		(nextrect.ax > currrect.bx) horzstep = nextrect.ax - currrect.bx;
			else if (nextrect.bx < currrect.ax) horzstep = currrect.ax - nextrect.bx;
			//else if (nextrect.ax > currrect.ax) horzstep = nextrect.ax - currrect.ax;
			//else if (nextrect.ax < currrect.ax) horzstep = currrect.ax - nextrect.ax;
			else
			{}

			if (horzstep > 0)
				m_horzstep[horzstep].push_back(snap_info(currprop, nextprop));
		}
		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_Y)
		{
			s32 vertstep = 0;
			if		(nextrect.ay > currrect.by) vertstep = nextrect.ay - currrect.by;
			else if (nextrect.by < currrect.ay) vertstep = currrect.ay - nextrect.by;
			//else if (nextrect.ay > currrect.ay) vertstep = nextrect.ay - currrect.ay;
			//else if (nextrect.ay < currrect.ay) vertstep = currrect.ay - nextrect.ay;
			else
			{}

			if (vertstep > 0)
				m_vertstep[vertstep].push_back(snap_info(currprop, nextprop));
		}
	}
}
xui_method_explain(onity_scene, use_snapinfo,				void					)( void )
{
	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_treeview* treeview = hierarchy->get_treeview();
	std::vector<xui_treenode*> nodevec = treeview->get_selectednode();

	xui_vector<s32> snap;
	xui_vector<s32> step;
	xui_rect2d<s32> self;
	xui_vector<s32> move = cal_snapmove(nodevec, snap, step, self, NULL, NULL);
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		if (prop)
		{
			prop->set_position(prop->ori_position()+move);
		}
	}
}
xui_method_explain(onity_scene, cal_linestep,				bool					)( s32& last, s32 temp )
{
	if (last == 0 || abs(last) > abs(temp))
	{
		last = temp;
		return true;
	}

	return false;
}
xui_method_explain(onity_scene, cal_snapmove,				xui_vector<s32>			)( const std::vector<xui_treenode*>& nodevec, xui_vector<s32>& snap, xui_vector<s32>& step, xui_rect2d<s32>& self, xui_rect2d<s32>* horz, xui_rect2d<s32>* vert )
{
	xui_vector<s32> move(0);
	xui_vector<s32> curr = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());

	if (nodevec.size() > 0)
	{
		xui_treenode* root = NULL;
		for (u32 i = 0; i < nodevec.size(); ++i)
		{
			if (nodevec[i]->get_rootnode() == NULL)
				continue;

			xui_treenode*   node = nodevec[i];
			onity_treedata* data = (onity_treedata*)node->get_linkdata();
			onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
			xui_rect2d<s32> rect = prop->get_bounding(m_trans, m_ratio);
			if (rect.was_inside(curr))
			{
				root = node->get_rootnode();
				self = prop->ori_bounding();
				break;
			}
		}

		onity_snapinfo_map::iterator itor;
		for (itor = m_horzsnap.begin(); itor != m_horzsnap.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (self.ax >= line-2 && self.ax <= line+2 && cal_linestep(move.x, line-self.ax))
				snap.x   = line;
			if (self.bx >= line-2 && self.bx <= line+2 && cal_linestep(move.x, line-self.bx))
				snap.x   = line;
		}
		for (itor = m_vertsnap.begin(); itor != m_vertsnap.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (self.ay >= line-2 && self.ay <= line+2 && cal_linestep(move.y, line-self.ay))
				snap.y   = line;
			if (self.by >= line-2 && self.by <= line+2 && cal_linestep(move.y, line-self.by))
				snap.y   = line;
		}

		xui_vector<s32> midd(self.ax+self.get_w()/2, self.ay+self.get_h()/2);
		for (itor = m_horzmidd.begin(); itor != m_horzmidd.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (midd.x  >= line-2 && midd.x  <= line+2 && cal_linestep(move.x, line-midd.x))
				snap.x   = line;
		}
		for (itor = m_vertmidd.begin(); itor != m_vertmidd.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (midd.y  >= line-2 && midd.y  <= line+2 && cal_linestep(move.y, line-midd.y))
				snap.y   = line;
		}

		for (u32 i = 0; i < root->get_leafnodecount(); ++i)
		{
			if (root->get_leafnode(i)->was_selected())
				continue;

			xui_treenode*   leaf = root->get_leafnode(i);
			onity_treedata* data = (onity_treedata*)leaf->get_linkdata();
			onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
			xui_rect2d<s32> rect = prop->ori_bounding();
			xui_rect2d<s32> temp = rect.get_inter(self);

			s32 horzstep = 0;
			s32 vertstep = 0;
			if (temp.get_h() > 0)
			{
				if		(self.ax > rect.bx) horzstep = self.ax - rect.bx;
				else if (self.bx < rect.ax) horzstep = rect.ax - self.bx;
				//else if (self.ax > rect.ax) horzstep = self.ax - rect.ax;
				//else if (self.ax < rect.ax) horzstep = rect.ax - self.ax;
				else
				{}
			}
			if (temp.get_w() > 0)
			{
				if		(self.ay > rect.by) vertstep = self.ay - rect.by;
				else if (self.by < rect.ay) vertstep = rect.ay - self.by;
				//else if (self.ay > rect.ay) vertstep = self.ay - rect.ay;
				//else if (self.ay < rect.ay) vertstep = rect.ay - self.ay;
				else
				{}
			}

			s32 steparray[5] = {0, -1, 1, -2, 2};
			if (horzstep > 0)
			{
				for (u08 istep = 0; istep < 5; ++istep)
				{
					s32 line = horzstep + steparray[istep];
					if (step.x != 0 && line >= step.x)
						continue;

					itor = m_horzstep.find(line);
					if (itor != m_horzstep.end())
					{
						if		(self.ax > rect.bx && cal_linestep(move.x, rect.bx+line-self.ax)) { step.x = line; if (horz) *horz = rect; }
						else if (self.bx < rect.ax && cal_linestep(move.x, rect.ax-line-self.bx)) { step.x = line; if (horz) *horz = rect; }
						//else if (self.ax > rect.ax && cal_linestep(move.x, rect.ax+line-self.ax)) { step.x = line; if (horz) *horz = rect; }
						//else if (self.ax < rect.ax && cal_linestep(move.x, rect.ax-line-self.ax)) { step.x = line; if (horz) *horz = rect; }
						else
						{}

						break;
					}
				}
			}
			if (vertstep > 0)
			{
				for (u08 istep = 0; istep < 5; ++istep)
				{
					s32 line = vertstep + steparray[istep];
					if (step.y != 0 && line >= step.y)
						continue;

					itor = m_vertstep.find(line);
					if (itor != m_vertstep.end())
					{
						if		(self.ay > rect.by && cal_linestep(move.y, rect.by+line-self.ay)) { step.y = line; if (vert) *vert = rect; }
						else if (self.by < rect.ay && cal_linestep(move.y, rect.ay-line-self.by)) { step.y = line; if (vert) *vert = rect; }
						//else if (self.ay > rect.ay && cal_linestep(move.y, rect.ay+line-self.ay)) { step.y = line; if (vert) *vert = rect; }
						//else if (self.ay < rect.ay && cal_linestep(move.y, rect.ay-line-self.ay)) { step.y = line; if (vert) *vert = rect; }
						else
						{}

						break;
					}
				}
			}
		}
	}

	return xui_vector<s32>(
		((move.x == 10) ? 0 : move.x),
		((move.y == 10) ? 0 : move.y));
}
xui_method_explain(onity_scene, mir_proproot,				void					)( const xui_proproot_vec& propvec, const xui_vector<s32>& pivot, s32 style )
{
	for (u32 i = 0; i < propvec.size(); ++i)
	{
		onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
		if (prop)
		{
			xui_vector<s32> pt = prop->ori_position();
			xui_rect2d<s32> rt = prop->ori_bounding();

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
}
xui_method_explain(onity_scene, rot_proproot,				void					)( const xui_proproot_vec& propvec, const xui_vector<s32>& pivot, s32 style )
{
	for (u32 i = 0; i < propvec.size(); ++i)
	{
		onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
		if (prop)
		{
			xui_vector<s32> pt = prop->ori_position();
			xui_rect2d<s32> rt = prop->ori_bounding();

			s32 w = rt.get_w() / 2;
			s32 h = rt.get_h() / 2;
			xui_vector<s32> center = rt.get_pt() + xui_vector<s32>(w, h);
			xui_vector<s32> offset = pt - center;
			pt.x = pivot.x - (center.y-pivot.y)*style + offset.x;
			pt.y = pivot.y + (center.x-pivot.x)*style + offset.y;
			prop->set_position(pt);
		}
	}
}

/*
//drawsnap
*/
xui_method_explain(onity_scene, draw_horzsnap,				void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec )
{
	s32 ymin = self.ay;
	s32 ymax = self.by;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propedit* prop = dynamic_cast<onity_propedit*>(vec[i].curr);
		xui_rect2d<s32> rect = prop->ori_bounding();
		ymin = xui_min(ymin, rect.ay);
		ymax = xui_max(ymax, rect.by);
	}

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1(xui_round(snap*m_ratio + m_trans.x*m_ratio), xui_round(ymin*m_ratio + m_trans.y*m_ratio));
	xui_vector<s32> p2(xui_round(snap*m_ratio + m_trans.x*m_ratio), xui_round(ymax*m_ratio + m_trans.y*m_ratio));
	xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::blue);
}
xui_method_explain(onity_scene, draw_vertsnap,				void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec )
{
	s32 xmin = self.ax;
	s32 xmax = self.bx;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propedit* prop = dynamic_cast<onity_propedit*>(vec[i].curr);
		xui_rect2d<s32> rect = prop->ori_bounding();
		xmin = xui_min(xmin, rect.ax);
		xmax = xui_max(xmax, rect.bx);
	}

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1(xui_round(xmin*m_ratio + m_trans.x*m_ratio), xui_round(snap*m_ratio + m_trans.y*m_ratio));
	xui_vector<s32> p2(xui_round(xmax*m_ratio + m_trans.x*m_ratio), xui_round(snap*m_ratio + m_trans.y*m_ratio));
	xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::blue);
}
xui_method_explain(onity_scene, draw_horzstep,				void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr )
{
	std::vector< xui_rect2d<s32> > currrectvec;
	std::vector< xui_rect2d<s32> > nextrectvec;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propedit* currprop = dynamic_cast<onity_propedit*>(vec[i].curr);
		onity_propedit* nextprop = dynamic_cast<onity_propedit*>(vec[i].next);
		currrectvec.push_back(currprop->ori_bounding());
		nextrectvec.push_back(nextprop->ori_bounding());
	}

	currrectvec.push_back(curr);
	nextrectvec.push_back(self);

	xui_vector<s32> pt = m_drawview->get_screenpt();
	u32 count = xui_min(currrectvec.size(), nextrectvec.size());
	for (u32 i = 0; i < count; ++i)
	{
		xui_rect2d<s32>& currrect = currrectvec[i];
		xui_rect2d<s32>& nextrect = nextrectvec[i];
		xui_rect2d<s32>  temprect = nextrect.get_inter(currrect);
		s32 xmin = 0;
		if		(nextrect.ax > currrect.bx) xmin = currrect.bx;
		else if (nextrect.bx < currrect.ax) xmin = nextrect.bx;
		//else if (nextrect.ax > currrect.ax) xmin = currrect.ax;
		//else if (nextrect.ax < currrect.ax) xmin = nextrect.ax;
		else
		{}

		s32 xmax = xmin + snap;
		s32 ymid = (temprect.get_h() > 0) ? (temprect.ay+temprect.get_h()/2) : (nextrect.ay+nextrect.get_h()/2); 

		xui_vector<s32> p1;
		xui_vector<s32> p2;
		p1 = xui_vector<s32>(xui_round(xmin*m_ratio + m_trans.x*m_ratio), xui_round(ymid*m_ratio + m_trans.y*m_ratio));
		p2 = xui_vector<s32>(xui_round(xmax*m_ratio + m_trans.x*m_ratio), xui_round(ymid*m_ratio + m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::white);
		if (temprect.get_h() > 0)
			continue;

		s32 ymin = xui_min(currrect.ay, nextrect.ay);
		s32 ymax = xui_max(currrect.by, nextrect.by);
		s32 temp = nextrect.ax > currrect.bx ? currrect.bx : currrect.ax;
		p1 = xui_vector<s32>(xui_round(temp*m_ratio + m_trans.x*m_ratio), xui_round(ymin*m_ratio + m_trans.y*m_ratio));
		p2 = xui_vector<s32>(xui_round(temp*m_ratio + m_trans.x*m_ratio), xui_round(ymax*m_ratio + m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::red);
	}
}
xui_method_explain(onity_scene, draw_vertstep,				void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr )
{
	std::vector< xui_rect2d<s32> > currrectvec;
	std::vector< xui_rect2d<s32> > nextrectvec;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propedit* currprop = dynamic_cast<onity_propedit*>(vec[i].curr);
		onity_propedit* nextprop = dynamic_cast<onity_propedit*>(vec[i].next);
		currrectvec.push_back(currprop->ori_bounding());
		nextrectvec.push_back(nextprop->ori_bounding());
	}

	currrectvec.push_back(curr);
	nextrectvec.push_back(self);

	xui_vector<s32> pt = m_drawview->get_screenpt();
	u32 count = xui_min(currrectvec.size(), nextrectvec.size());
	for (u32 i = 0; i < count; ++i)
	{
		xui_rect2d<s32>& currrect = currrectvec[i];
		xui_rect2d<s32>& nextrect = nextrectvec[i];
		xui_rect2d<s32>  temprect = nextrect.get_inter(currrect);
		s32 ymin = 0;
		if		(nextrect.ay > currrect.by) ymin = currrect.by;
		else if (nextrect.by < currrect.ay) ymin = nextrect.by;
		//else if (nextrect.ay > currrect.ay) ymin = currrect.ay;
		//else if (nextrect.ay < currrect.ay) ymin = nextrect.ay;
		else
		{}

		s32 ymax = ymin + snap;
		s32 xmid = (temprect.get_w() > 0) ? (temprect.ax+temprect.get_w()/2) : (nextrect.ax+nextrect.get_w()/2); 

		xui_vector<s32> p1;
		xui_vector<s32> p2;
		p1 = xui_vector<s32>(xui_round(xmid*m_ratio + m_trans.x*m_ratio), xui_round(ymin*m_ratio + m_trans.y*m_ratio));
		p2 = xui_vector<s32>(xui_round(xmid*m_ratio + m_trans.x*m_ratio), xui_round(ymax*m_ratio + m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::white);
		if (temprect.get_w() > 0)
			continue;

		s32 xmin = xui_min(currrect.ax, nextrect.ax);
		s32 xmax = xui_max(currrect.bx, nextrect.bx);
		s32 temp = nextrect.ay > currrect.by ? currrect.by : currrect.ay;
		p1 = xui_vector<s32>(xui_round(xmin*m_ratio + m_trans.x*m_ratio), xui_round(temp*m_ratio + m_trans.y*m_ratio));
		p2 = xui_vector<s32>(xui_round(xmax*m_ratio + m_trans.x*m_ratio), xui_round(temp*m_ratio + m_trans.y*m_ratio));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::red);
	}
}
xui_method_explain(onity_scene, draw_locknode,				void					)( const std::vector<xui_treenode*>& nodevec )
{
	if (m_lockctrl->was_play() && nodevec.size() > 0)
	{
		xui_treenode*   node = nodevec.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_propedit* prop = dynamic_cast<onity_propedit*>(data->get_prop());
		if (prop)
		{
			xui_rect2d<s32> rect = prop->get_bounding(m_trans, m_ratio);
			xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_lockctrl;
			f32 sa = action->sample();
			f32 s  = xui_max(sa, 1.0f);
			f32 ox = -0.5f * rect.get_w() * s + 0.5f * rect.get_w();
			f32 oy = -0.5f * rect.get_h() * s + 0.5f * rect.get_h();
			f32 fw =  s    * rect.get_w();
			f32 fh =  s    * rect.get_h();
			xui_vector<s32> drawpt = m_drawview->get_screenpt();
			xui_rect2d<s32> drawrt = rect + drawpt;
			drawrt.oft_x((s32)ox);
			drawrt.oft_y((s32)oy);
			drawrt.set_w((s32)fw);
			drawrt.set_h((s32)fh);
			xui_convas::get_ins()->draw_round(drawrt, xui_colour(sa, 1.0f, 0.0f, 0.0f), xui_rect2d<s32>(3), 3);
		}
	}
}
xui_method_explain(onity_scene, draw_multisel,				void					)( void )
{
	if (m_multisel)
	{
		xui_vector<s32> pt = m_drawview->get_screenpt();
		xui_vector<s32> p1 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousedown());
		xui_vector<s32> p2 = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		xui_rect2d<s32> rt;
		rt.ax = xui_min(p1.x, p2.x);
		rt.bx = xui_max(p1.x, p2.x);
		rt.ay = xui_min(p1.y, p2.y);
		rt.by = xui_max(p1.y, p2.y);
		xui_convas::get_ins()->fill_rectangle(rt+pt, xui_colour(0.5f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
		xui_convas::get_ins()->draw_rectangle(rt+pt, xui_colour(1.0f, 0.0f, 0.9f, 0.9f));
	}
}