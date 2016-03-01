#include "NPRender.h"
#include "NPConfig.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"
#include "NPGUILib.h"
#include "NPGUIImeManagerWin32.h"
#include "m3eFrameWork.h"

#include "xui_global.h"
#include "xui_bitmap.h"
#include "xui_panel.h"
#include "onity_game.h"

xui_implement_rtti(onity_game, xui_dockpage);

std::string	g_strQQ;
std::string	g_strPWD;

/*
//constructor
*/
xui_create_explain(onity_game)( void )
: xui_dockpage(xui_vector<s32>(100), AREALIMIT_F, 100, DOCKSTYLE_F)
{
	ini_namectrl(xui_bitmap::create("icon/game.png"), L"Game");

	m_view = new onity_renderview(xui_vector<s32>(100));
	xui_method_ptrcall(m_view, xm_load			) += new xui_method_member<xui_method_args,  onity_game>(this, &onity_game::on_viewload);
	xui_method_ptrcall(m_view, xm_updateself	) += new xui_method_member<xui_method_args,  onity_game>(this, &onity_game::on_viewupdateself);
	xui_method_ptrcall(m_view, xm_renderself	) += new xui_method_member<xui_method_args,  onity_game>(this, &onity_game::on_viewrenderself);
	xui_method_ptrcall(m_view, xm_setrendersz	) += new xui_method_member<xui_method_args,  onity_game>(this, &onity_game::on_viewsetrendersz);
	xui_method_ptrcall(m_view, xm_mousedown		) += new xui_method_member<xui_method_mouse, onity_game>(this, &onity_game::on_viewmousedown);
	xui_method_ptrcall(m_view, xm_mousemove		) += new xui_method_member<xui_method_mouse, onity_game>(this, &onity_game::on_viewmousemove);
	xui_method_ptrcall(m_view, xm_mouserise		) += new xui_method_member<xui_method_mouse, onity_game>(this, &onity_game::on_viewmouserise);
	xui_method_ptrcall(m_view, xm_keybddown		) += new xui_method_member<xui_method_keybd, onity_game>(this, &onity_game::on_viewkeybddown);
	xui_method_ptrcall(m_view, xm_keybdrise		) += new xui_method_member<xui_method_keybd, onity_game>(this, &onity_game::on_viewkeybdrise);
	xui_method_ptrcall(m_view, xm_keybdchar		) += new xui_method_member<xui_method_keybd, onity_game>(this, &onity_game::on_viewkeybdchar);
	xui_method_ptrcall(m_view, ini_component	)(0, 0, DOCKSTYLE_F);
	add_pagectrl(m_view);
}

/*
//event
*/
xui_method_explain(onity_game, on_viewload,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_dockpage::on_load(args);
	xui_global::set_workpath(L"D:/BreezeGameBranchFlyMode_V26/bin");
	xui_vector<s32>   size = sender->get_rendersz();
	m3eFrameWorkInit (size.w, size.h);
	m3eFrameWorkStart();
}
xui_method_explain(onity_game, on_viewupdateself,	void)( xui_component* sender, xui_method_args&  args )
{
	NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	if (imeManager)
		imeManager->Update();

	m3eFrameWorkUpdate();
}
xui_method_explain(onity_game, on_viewrenderself,	void)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	m3eFrameWorkRender();
}
xui_method_explain(onity_game, on_viewsetrendersz,	void)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> size = sender->get_rendersz();
	NPConfig::SetGUIDesktop(NPVector2(size.w, size.h));
	NPConfig::CalAdapterScaleValue();
	NPGUILib::DesktopVary();
}
xui_method_explain(onity_game, on_viewmousedown,	void)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse != MB_L)
		return;

	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEBEGAN, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewmousemove,	void)( xui_component* sender, xui_method_mouse& args )
{
	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEMOVED, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewmouserise,	void)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse != MB_L)
		return;

	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEENDED, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewkeybddown,	void)( xui_component* sender, xui_method_keybd& args )
{
	//NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	//if (imeManager)
	//	imeManager->OnKeyDown(wParam, lParam);

	if (args.kcode == KEY_SHIFT)
	{
		m3eFrameWorkTouch(M3E_Touch_MOUSEBEGAN, 2, 10, 480);
	}
}
xui_method_explain(onity_game, on_viewkeybdrise,	void)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_SHIFT)
	{
		m3eFrameWorkTouch(M3E_Touch_MOUSEENDED, 2, 10, 480);
	}
}
xui_method_explain(onity_game, on_viewkeybdchar,	void)( xui_component* sender, xui_method_keybd& args )
{
	//NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	//if (imeManager)
	//	imeManager->OnKeyChar(wParam, lParam);
}