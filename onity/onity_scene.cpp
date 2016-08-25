#include "NPRender.h"
#include "NPConfig.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"
#include "NPGUIDesktop.h"
#include "NPGUIImeManagerWin32.h"
#include "m3eFrameWork.h"
#include "Game/GameConfig.h"
#include "Entity/Component/Visual/WGVisualManager.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_scene.h"

xui_implement_rtti(onity_scene, xui_dockpage);
extern bool gInitCompleted;

/*
//constructor
*/
xui_create_explain(onity_scene)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
{
	ini_namectrl(onity_resource::icon_scene, L"Scene");

	m_head  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto	)(false);

	m_view = new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_view,		xm_updateself	) += new xui_method_member<xui_method_update,	onity_scene>(this, &onity_scene::on_viewupdateself);
	xui_method_ptrcall(m_view,		xm_renderself	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_viewrenderself);
	xui_method_ptrcall(m_view,		xm_setrendersz	) += new xui_method_member<xui_method_args,		onity_scene>(this, &onity_scene::on_viewsetrendersz);
	xui_method_ptrcall(m_view,		xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_viewmousedown);
	xui_method_ptrcall(m_view,		xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_viewmousemove);
	xui_method_ptrcall(m_view,		xm_mouserise	) += new xui_method_member<xui_method_mouse,	onity_scene>(this, &onity_scene::on_viewmouserise);
	xui_method_ptrcall(m_view,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_view,		set_sidecolor	)(xui_colour::black);
	add_pagectrl(m_head);
	add_pagectrl(m_view);
}

/*
//event
*/
xui_method_explain(onity_scene, on_viewupdateself,	void)( xui_component* sender, xui_method_update& args )
{
	if (gInitCompleted == false)
		return;

	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	m3eFrameWorkUpdate(args.delta);
}
xui_method_explain(onity_scene, on_viewrenderself,	void)( xui_component* sender, xui_method_args&	 args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	if (gInitCompleted == false)
		return;

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	BreezeGame::WGVisualManager::Instance()->Display();
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_scene, on_viewsetrendersz,	void)( xui_component* sender, xui_method_args&   args )
{
	if (gInitCompleted == false)
		return;

	//xui_vector<s32> size = sender->get_rendersz();
	//NPRender::GetIns()->SetResolutionW((npu32)size.w);
	//NPRender::GetIns()->SetResolutionH((npu32)size.h);
	//NPConfig::CalAdapterScaleValue();
	//BreezeGame::GameConfig::Instance()->InitAdaptation();
}
xui_method_explain(onity_scene, on_viewmousedown,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
}
xui_method_explain(onity_scene, on_viewmousemove,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
}
xui_method_explain(onity_scene, on_viewmouserise,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
}
