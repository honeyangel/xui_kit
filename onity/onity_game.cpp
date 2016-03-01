#include "NPRender.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"

#include "xui_bitmap.h"
#include "xui_panel.h"
#include "onity_game.h"

xui_implement_rtti(onity_game, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_game)( void )
: xui_dockpage(xui_vector<s32>(100), AREALIMIT_F, 100, DOCKSTYLE_F)
{
	ini_namectrl(xui_bitmap::create("icon/game.png"), L"Game");

	m_view = new onity_renderview(xui_vector<s32>(100));
	xui_method_ptrcall(m_view, xm_renderself) += new xui_method_member<xui_method_args, onity_game>(this, &onity_game::on_viewrenderself);
	xui_method_ptrcall(m_view, ini_component)(0, 0, DOCKSTYLE_F);
	add_pagectrl(m_view);
}

/*
//event
*/
xui_method_explain(onity_game, on_viewrenderself, void)( xui_component* sender, xui_method_args& args )
{
	xui_vector<s32> size = get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);

	NP2DSRenderUtil::GetIns()->DrawRect(NPVector2(50.0f, 50.0f), NPVector2(300.0f, 200.0f), NPMatrix4x4::Identity, NPColor::Blue);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}