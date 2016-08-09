#include "NPRender.h"
#include "NPConfig.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"
#include "NPGUIDesktop.h"
#include "NPGUIImeManagerWin32.h"
#include "m3eFrameWork.h"
#include "Game/GameConfig.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_game.h"

xui_implement_rtti(onity_game, xui_dockpage);

std::string	g_strQQ;
std::string	g_strPWD;
extern bool gInitCompleted;

/*
//constructor
*/
xui_create_explain(onity_game)( void )
: xui_dockpage(xui_vector<s32>(200), AREALIMIT_A, 200, DOCKSTYLE_F)
{
	ini_namectrl(onity_resource::icon_game, L"Game");

	xui_menu* menu = xui_menu::create(80);
	m_fill		= menu->add_item(NULL, L"Fill");
	m_iphone4	= menu->add_item(NULL, L"iPhone4");
	m_iphone5	= menu->add_item(NULL, L"iPhone5");
	m_iphone6	= menu->add_item(NULL, L"iPhone6");
	m_iphone6p	= menu->add_item(NULL, L"iPhone6 Plus");
	m_ipad		= menu->add_item(NULL, L"iPad");
	xui_method_ptrcall(m_iphone4,	set_flag		)(true);
	xui_method_ptrcall(m_fill,		xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);
	xui_method_ptrcall(m_iphone4,	xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);
	xui_method_ptrcall(m_iphone5,	xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);
	xui_method_ptrcall(m_iphone6,	xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);
	xui_method_ptrcall(m_iphone6p,	xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);
	xui_method_ptrcall(m_ipad,		xm_click		) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_menuclick);

	m_aspect = new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_aspect,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_aspect,	set_corner		)(3);
	xui_method_ptrcall(m_aspect,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_aspect,	set_drawcolor	)(true);
	xui_method_ptrcall(m_aspect,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_aspect,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_aspect,	ini_drawer		)(L"Aspect");
	xui_method_ptrcall(m_aspect,	set_menu		)(menu);

	m_head  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto	)(false);
	xui_method_ptrcall(m_head,		add_child		)(m_aspect);

	m_view = new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048));
	xui_method_ptrcall(m_view,		xm_updateself	) += new xui_method_member<xui_method_update,	onity_game>(this, &onity_game::on_viewupdateself);
	xui_method_ptrcall(m_view,		xm_renderself	) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_viewrenderself);
	xui_method_ptrcall(m_view,		xm_setrendersz	) += new xui_method_member<xui_method_args,		onity_game>(this, &onity_game::on_viewsetrendersz);
	xui_method_ptrcall(m_view,		xm_mousedown	) += new xui_method_member<xui_method_mouse,	onity_game>(this, &onity_game::on_viewmousedown);
	xui_method_ptrcall(m_view,		xm_mousemove	) += new xui_method_member<xui_method_mouse,	onity_game>(this, &onity_game::on_viewmousemove);
	xui_method_ptrcall(m_view,		xm_mouserise	) += new xui_method_member<xui_method_mouse,	onity_game>(this, &onity_game::on_viewmouserise);
	xui_method_ptrcall(m_view,		xm_keybddown	) += new xui_method_member<xui_method_keybd,	onity_game>(this, &onity_game::on_viewkeybddown);
	xui_method_ptrcall(m_view,		xm_keybdrise	) += new xui_method_member<xui_method_keybd,	onity_game>(this, &onity_game::on_viewkeybdrise);
	xui_method_ptrcall(m_view,		xm_keybdchar	) += new xui_method_member<xui_method_keybd,	onity_game>(this, &onity_game::on_viewkeybdchar);
	xui_method_ptrcall(m_view,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_view,		set_sidecolor	)(xui_colour::black);
	add_pagectrl(m_head);
	add_pagectrl(m_view);
}

/*
//method
*/
xui_method_explain(onity_game, ini_game,			void)( void )
{
	xui_vector<s32>   size = m_view->get_rendersz();
	m3eFrameWorkInit (size.w, size.h);
	m3eFrameWorkStart();
}

/*
//callback
*/
xui_method_explain(onity_game, on_perform,			void)( xui_method_args& args )
{
	xui_dockpage::on_perform(args);

	if (m_fill->get_flag() == false)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		xui_vector<s32> sz = rt.get_sz();
		sz.h -= m_head->get_renderh();

		s32 w = 0;
		s32 h = 0;
		if		(xui_method_ptrcall(m_iphone4,  get_flag)())	{ w =  960; h =  640; } 
		else if (xui_method_ptrcall(m_iphone5,  get_flag)())	{ w = 1036; h =  640; }
		else if (xui_method_ptrcall(m_iphone6,  get_flag)())	{ w = 1334; h =  750; }
		else if (xui_method_ptrcall(m_iphone6p, get_flag)())	{ w = 1920; h = 1080; }
		else if (xui_method_ptrcall(m_ipad,     get_flag)())	{ w = 1024; h =  768; }

		f32 sw = (f32)sz.w / (f32)w;
		f32 sh = (f32)sz.h / (f32)h;
		f32 s  = NPMathWrap::Min(1.0f, (sw < sh ? sw : sh));
		s32 fw = (s32)(w*s);
		s32 fh = (s32)(h*s);
		m_view->on_perform_w(fw);
		m_view->on_perform_h(fh);
		m_view->on_perform_x((sz.w-fw) / 2);
		m_view->on_perform_y((sz.h-fh) / 2 + rt.ay + m_head->get_renderh());
	}
}
xui_method_explain(onity_game, on_renderself,		void)( xui_method_args& args )
{
	xui_dockpage::on_renderself(args);
	xui_rect2d<s32> rt = m_head->get_renderrtabs();
	xui_vector<s32> p1 = xui_vector<s32>(rt.ax,	rt.by);
	xui_vector<s32> p2 = xui_vector<s32>(rt.bx,	rt.by);
	xui_convas::get_ins()->draw_line(p1, p2, xui_colour::black);
}

/*
//event
*/
xui_method_explain(onity_game, on_menuclick,		void)( xui_component* sender, xui_method_args&	 args )
{
	std::vector<xui_menuitem*> itemvec;
	itemvec.push_back(m_fill);
	itemvec.push_back(m_iphone4);
	itemvec.push_back(m_iphone5);
	itemvec.push_back(m_iphone6);
	itemvec.push_back(m_iphone6p);
	itemvec.push_back(m_ipad);
	for (u32 i = 0; i < itemvec.size(); ++i)
	{
		itemvec[i]->set_flag(sender == itemvec[i]);
	}

	m_view->ini_component(0, 0, (sender == m_fill ? DOCKSTYLE_F : DOCKSTYLE_N));
	refresh();
}
xui_method_explain(onity_game, on_viewupdateself,	void)( xui_component* sender, xui_method_update& args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	if (imeManager)
		imeManager->Update();

	m3eFrameWorkUpdate();
}
xui_method_explain(onity_game, on_viewrenderself,	void)( xui_component* sender, xui_method_args&	 args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gamerun() == false)
		return;

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	m3eFrameWorkRender();
}
xui_method_explain(onity_game, on_viewsetrendersz,	void)( xui_component* sender, xui_method_args&   args )
{
	if (gInitCompleted == false)
		return;

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW((npu32)size.w);
	NPRender::GetIns()->SetResolutionH((npu32)size.h);
	NPConfig::SetGUIDesktop(NPVector2((f32)size.w, (f32)size.h));
	NPConfig::CalAdapterScaleValue();
	NPGUIDesktop::GetIns()->Realign();
	BreezeGame::GameConfig::Instance()->InitAdaptation();
}
xui_method_explain(onity_game, on_viewmousedown,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	if (args.mouse != MB_L)
		return;

	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEBEGAN, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewmousemove,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEMOVED, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewmouserise,	void)( xui_component* sender, xui_method_mouse&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	if (args.mouse != MB_L)
		return;

	xui_vector<s32> pt = sender->get_renderpt(args.point);
	m3eFrameWorkTouch(M3E_Touch_MOUSEENDED, 0, (s16)pt.x, (s16)pt.y);
}
xui_method_explain(onity_game, on_viewkeybddown,	void)( xui_component* sender, xui_method_keybd&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	if (args.kcode == KEY_SHIFT)
	{
		m3eFrameWorkTouch(M3E_Touch_MOUSEBEGAN, 2, 10, 480);
	}

	NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	if (imeManager->GetTextField() == NULL)
		return;

	switch(args.kcode)
	{
	case KEY_LARROW:	imeManager->DoLeft	();	break;
	case KEY_RARROW:	imeManager->DoRight	();	break;
	case KEY_BACK:		imeManager->DoBack	();	break;
	case KEY_ENTER:		imeManager->DoEnter	();	break;
	case KEY_DELETE: 	imeManager->DoDelete();	break;
	}


}
xui_method_explain(onity_game, on_viewkeybdrise,	void)( xui_component* sender, xui_method_keybd&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	if (args.kcode == KEY_SHIFT)
	{
		m3eFrameWorkTouch(M3E_Touch_MOUSEENDED, 2, 10, 480);
	}
}
xui_method_explain(onity_game, on_viewkeybdchar,	void)( xui_component* sender, xui_method_keybd&  args )
{
	if (gInitCompleted == false)
		return;
	if (onity_mainform::get_ptr()->was_gameplay() == false)
		return;

	std::wstring text;
	text.push_back((wchar_t)args.wchar);
	std::string  temp = xui_global::unicode_to_ascii(text);
	NPGUIImeManagerWin32* imeManager = (NPGUIImeManagerWin32*)NPGUIImeManager::GetIns();
	for (u32 i = 0; i < temp.length(); ++i)
		imeManager->OnKeyChar(temp[i], 0);
}