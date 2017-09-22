#include "renderer/CCGLProgramCache.h"
#include "2d/WeCChartFontManager.h"
#include "platform/CCFileUtils.h"
#include "base/CCConfiguration.h"
#include "base/CCDirector.h"
#include "2d/CCScene.h"

#include "xui_desktop.h"
#include "xui_syswnd.h"
#include "xui_global.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_toolbar.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"
#include "cocos_propcsd.h"
#include "cocos_scene.h"
#include "cocos_resource.h"
#include "cocos_inspector.h"
#include "cocos_hierarchy.h"
#include "cocos_console.h"
#include "cocos_project.h"
#include "cocos_toolbox.h"
#include "cocos_game.h"
#include "cocos_recent.h"
#include "cocos_backup.h"
#include "cocos_save.h"
#include "cocos_glview.h"
#include "cocos_mainform.h"

xui_implement_rtti(cocos_mainform, xui_window);

/*
//constructor
*/
xui_create_explain(cocos_mainform)( void )
: xui_window(xui_vector<s32>(0), false)
, m_steptime(0)
, m_scene(NULL)
{
	xm_keybddown   += new xui_method_member<xui_method_keybd, cocos_mainform>(this, &cocos_mainform::on_globalkeybddown);
	m_dockstyle		= DOCKSTYLE_F;

	m_glview		= cocos_glview::create(NULL);
	m_cocosscene	= cocos2d::Scene::create();
	m_select		= xui_toggle::create(cocos_resource::icon_select,		32);
	m_translate		= xui_toggle::create(cocos_resource::icon_translate,	32);
	m_scale			= xui_toggle::create(cocos_resource::icon_scale,		32);
	m_rotation		= xui_toggle::create(cocos_resource::icon_rotation,		32);
	xui_method_ptrcall(m_select,		xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicktransform);
	xui_method_ptrcall(m_scale,			xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicktransform);
	xui_method_ptrcall(m_rotation,		xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		ini_toggle		)(true);

	//transform
	xui_linebox* line_transform	= xui_linebox::create();
	xui_method_ptrcall(line_transform,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_select);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_translate);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_scale);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_rotation);

	m_anchor		= xui_button::create(cocos_resource::icon_pivot, L"Pivot", 80);
	m_coordinate	= xui_button::create(cocos_resource::icon_world, L"World", 80);
	xui_method_ptrcall(m_anchor,		set_data		)((void*)TOA_PIVOT);
	xui_method_ptrcall(m_coordinate,	set_data		)((void*)TOC_WORLD);
	xui_method_ptrcall(m_anchor,		xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickanchor);
	xui_method_ptrcall(m_coordinate,	xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickcoordinate);

	//anchor
	//coordinate
	xui_linebox* line_operator	= xui_linebox::create();
	xui_method_ptrcall(line_operator,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_operator,	set_renderpt	)(xui_vector<s32>(160, 0));
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_anchor);
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_coordinate);

	m_build			= xui_button::create(cocos_resource::icon_build, L"Build", 80);
	m_run			= xui_toggle::create(NULL, 32);
	m_pause			= xui_toggle::create(NULL, 32);
	m_step			= xui_button::create(NULL, 32);
	xui_method_ptrcall(m_run,			xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickdebug);
	xui_method_ptrcall(m_pause,			xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickdebug);
	xui_method_ptrcall(m_build,			xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickbuild);
	xui_method_ptrcall(m_step,			xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickdebug);
	xui_method_ptrcall(m_run,			xm_renderself	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_paintdebug);
	xui_method_ptrcall(m_pause,			xm_renderself	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_paintdebug);
	xui_method_ptrcall(m_step,			xm_renderself	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_paintdebug);


	//run
	//pause
	//step
	xui_linebox* line_debug		= xui_linebox::create();
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_run);
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_pause);
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_step);

	xui_toolbar* tool_debug		= xui_toolbar::create();
	xui_method_ptrcall(tool_debug,		ini_component	)(ALIGNHORZ_C, ALIGNVERT_C, 0);
	xui_method_ptrcall(tool_debug,		set_grap		)(8);
	xui_method_ptrcall(tool_debug,		add_item		)(m_build);
	xui_method_ptrcall(tool_debug,		add_separate	)();
	xui_method_ptrcall(tool_debug,		add_item		)(line_debug);

	xui_menu* menu	= xui_menu::create(100);
	m_hierarchy		= menu->add_item(cocos_resource::icon_hierarchy,	L"Hierarchy");
	m_inspector		= menu->add_item(cocos_resource::icon_inspector,	L"Inspector");
	m_project		= menu->add_item(cocos_resource::icon_project,		L"Project");
	m_console		= menu->add_item(cocos_resource::icon_console,		L"Console");
	m_timeline		= menu->add_item(cocos_resource::icon_timeline,		L"Timeline");
	m_toolbox		= menu->add_item(cocos_resource::icon_setting,		L"Toolbox");
	m_game			= menu->add_item(cocos_resource::icon_game,			L"Game");
	xui_method_ptrcall(m_hierarchy,		set_data		)(new cocos_hierarchy);
	xui_method_ptrcall(m_inspector,		set_data		)(new cocos_inspector);
	xui_method_ptrcall(m_project,		set_data		)(new cocos_project);
	xui_method_ptrcall(m_console,		set_data		)(new cocos_console);
	xui_method_ptrcall(m_toolbox,		set_data		)(new cocos_toolbox);
	xui_method_ptrcall(m_game,			set_data		)(new cocos_game);
	//xui_method_ptrcall(m_timeline,		set_data		)(new cocos_timeline);
	xui_method_ptrcall(m_hierarchy,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_inspector,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_project,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_console,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_toolbox,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_game,			xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_timeline,		xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickwndmenu);
	menu->add_separate();
	m_save			= menu->add_item(NULL, L"Save");
	m_load			= menu->add_item(NULL, L"Load");
	m_reset			= menu->add_item(NULL, L"Reset");
	xui_method_ptrcall(m_save,			xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicksave);
	xui_method_ptrcall(m_load,			xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickload);
	xui_method_ptrcall(m_reset,			xm_click		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clickreset);

	m_saveall		= xui_button::create(cocos_resource::icon_save,		L"SaveAll", 80);
	m_setting		= xui_button::create(cocos_resource::icon_setting,	L"Setting",	80);
	m_window		= xui_toggle::create(cocos_resource::icon_window,	L"Window",	100);
	xui_method_ptrcall(m_window,		set_menu		)(menu);
	xui_method_ptrcall(m_saveall,		xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_clicksaveall);

	//menu
	xui_linebox* line_menu		= xui_linebox::create();
	xui_method_ptrcall(line_menu,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_menu,		add_linectrl	)(m_saveall);
	xui_method_ptrcall(line_menu,		add_linectrl	)(m_setting);
	xui_method_ptrcall(line_menu,		add_linectrl	)(m_window);

	//main
	m_toolpane		= new xui_panel(xui_vector<s32>(40));
	m_mainview		= xui_dockview::create();
	xui_method_ptrcall(m_mainview,		xm_invalid		) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_mainviewinvalid);
	xui_method_ptrcall(m_mainview,		xm_pagechanged	) += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_mainviewchanged);
	xui_method_ptrcall(m_toolpane,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_toolpane,		set_borderrt	)(xui_rect2d<s32>(8));
	xui_method_ptrcall(m_toolpane,		add_child		)(line_transform);
	xui_method_ptrcall(m_toolpane,		add_child		)(line_operator);
	xui_method_ptrcall(m_toolpane,		add_child		)(tool_debug);
	xui_method_ptrcall(m_toolpane,		add_child		)(line_menu);
	add_child(m_toolpane);
	add_child(m_mainview);
}

/*
//destructor
*/
xui_delete_explain(cocos_mainform)( void )
{
	delete m_glview;
	delete m_cocosscene;
}

/*
//static
*/
xui_method_explain(cocos_mainform, get_ptr,				cocos_mainform*		)( void )
{
	const std::vector<xui_control*>& vec = xui_desktop::get_ins()->get_children();
	if (vec.size() > 0)
		return xui_dynamic_cast(cocos_mainform, vec[0]);

	return NULL;
}
xui_method_explain(cocos_mainform, get_pagename,		std::string			)( xui_dockpage* page )
{
	cocos_mainform* mainform = get_ptr();
	if		(page == mainform->get_project  ())	return "project";
	else if	(page == mainform->get_hierarchy())	return "hierarchy";
	else if (page == mainform->get_inspector())	return "inspector";
	else if (page == mainform->get_console	())	return "console";
	else if (page == mainform->get_toolbox  ()) return "toolbox";
	//else if (page == mainform->get_timeline	())	return "timeline";
	else
	{
		return "";
	}
}
xui_method_explain(cocos_mainform, get_pagectrl,		xui_dockpage*		)( const std::string& name )
{
	cocos_mainform* mainform = get_ptr();
	if		(name == "project")		return mainform->get_project	();
	else if	(name == "hierarchy")	return mainform->get_hierarchy	();
	else if (name == "inspector")	return mainform->get_inspector	();
	else if (name == "console")		return mainform->get_console	();
	else if (name == "toolbox")		return mainform->get_toolbox	();
	//else if (name == "timeline")	return mainform->get_timeline();
	else
	{
		return NULL;
	}
}

/*
//method
*/
xui_method_explain(cocos_mainform, get_cocosscene,		cocos2d::Scene*		)( void )
{
	return m_cocosscene;
}
xui_method_explain(cocos_mainform, get_hierarchy,		cocos_hierarchy*	)( void )
{
	return (cocos_hierarchy*)	xui_method_ptrcall(m_hierarchy, get_data)();
}
xui_method_explain(cocos_mainform, get_inspector,		cocos_inspector*	)( void )
{
	return (cocos_inspector*)	xui_method_ptrcall(m_inspector, get_data)();
}
xui_method_explain(cocos_mainform, get_project,			cocos_project*		)( void )
{
	return (cocos_project*)		xui_method_ptrcall(m_project,	get_data)();
}
xui_method_explain(cocos_mainform, get_console,			cocos_console*		)( void )
{
	return (cocos_console*)		xui_method_ptrcall(m_console,	get_data)();
}
xui_method_explain(cocos_mainform, get_toolbox,			cocos_toolbox*		)( void )
{
	return (cocos_toolbox*)		xui_method_ptrcall(m_toolbox,	get_data)();
}
xui_method_explain(cocos_mainform, get_game,			cocos_game*			)( void )
{
	return (cocos_game*)		xui_method_ptrcall(m_game,		get_data)();
}
xui_method_explain(cocos_mainform, get_timeline,		cocos_timeline*		)( void )
{
	return (cocos_timeline*)	xui_method_ptrcall(m_timeline,	get_data)();
}

/*
//method
*/
xui_method_explain(cocos_mainform, get_scene,			cocos_scene*		)( void )
{
	return xui_dynamic_cast(cocos_scene, m_mainview->get_showpage());
}
xui_method_explain(cocos_mainform, get_scene,			cocos_scene*		)( cocos_propcsd* prop )
{
	std::vector<xui_dockpage*> pagelist = m_mainview->get_pagelist();
	for (u32 i = 0; i < pagelist.size(); ++i)
	{
		cocos_scene* scene = xui_dynamic_cast(cocos_scene, pagelist[i]);
		if (scene && scene->get_editprop() == prop)
			return scene;
	}

	return NULL;
}
xui_method_explain(cocos_mainform, add_scene,			cocos_scene*		)( cocos_propcsd* prop )
{
	cocos_scene* newpage = get_scene(prop);
	if (newpage == NULL)
	{
		newpage  = new cocos_scene(prop);
		m_mainview->set_pageshow(newpage, true);
		m_mainview->refresh();
		newpage->set_rootvisible();
	}

	m_mainview->set_showpage(newpage);
	return newpage;
}
xui_method_explain(cocos_mainform, was_gamerun,			bool				)( void ) const
{
	return m_run->was_push();
}
xui_method_explain(cocos_mainform, was_gameplay,		bool				)( void ) const
{
	return m_run->was_push() && (m_pause->was_push() == false || m_steptime > 0);
}
xui_method_explain(cocos_mainform, set_pageshow,		void				)( xui_dockpage* page )
{
	xui_dockview* view = xui_dynamic_cast(xui_dockview, page->get_parent());
	if (view == NULL)
		view = m_mainview;

	view->set_pageshow(page, true);
	view->set_showpage(page);
}

/*
//callback
*/
xui_method_explain(cocos_mainform, on_load,				void				)( xui_method_args&   args )
{
	xui_window::on_load(args);
	on_clickreset(NULL, args);
	on_clickload (NULL, args);

	cocos_recent* dialog = new cocos_recent;
	dialog->xm_accept   += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_recentaccept);
	xui_desktop::get_ins()->add_child(dialog);
}
xui_method_explain(cocos_mainform, on_updateself,		void				)( xui_method_update& args )
{
	xui_window::on_updateself(args);
	if (m_steptime > 0)
	{
		s32 deltams = (s32)(args.delta * 1000.0f);
		m_steptime -= xui_min(m_steptime, deltams);
	}
}

/*
//event
*/
xui_method_explain(cocos_mainform, on_clicktransform,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_toggle* operator_toggle[4];
	operator_toggle[0] = m_select;
	operator_toggle[1] = m_translate;
	operator_toggle[2] = m_scale;
	operator_toggle[3] = m_rotation;
	for (u32 i = 0; i < 4; ++i)
	{
		operator_toggle[i]->ini_toggle(sender == operator_toggle[i]);
	}
}
xui_method_explain(cocos_mainform, on_clickanchor,		void				)( xui_component* sender, xui_method_args&  args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toa = (u08)drawer->get_data();
	switch (toa)
	{
	case TOA_CENTER:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_PIVOT);
		xui_method_ptrcall(drawer, ini_drawer	)(cocos_resource::icon_pivot);
		xui_method_ptrcall(drawer, ini_drawer	)(L"Pivot");
		break;
	case TOA_PIVOT:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_CENTER);
		xui_method_ptrcall(drawer, ini_drawer	)(cocos_resource::icon_center);
		xui_method_ptrcall(drawer, ini_drawer	)(L"Center");
		break;
	}
}
xui_method_explain(cocos_mainform, on_clickcoordinate,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toc = (u08)drawer->get_data();
	switch (toc)
	{
	case TOC_LOCAL:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_WORLD);
		xui_method_ptrcall(drawer, ini_drawer	)(cocos_resource::icon_world, xui_vector<s32>(16));
		xui_method_ptrcall(drawer, ini_drawer	)(L"World");
		break;
	case TOC_WORLD:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_LOCAL);
		xui_method_ptrcall(drawer, ini_drawer	)(cocos_resource::icon_local, xui_vector<s32>(16));
		xui_method_ptrcall(drawer, ini_drawer	)(L"Local");
		break;
	}
}
xui_method_explain(cocos_mainform, on_clickbuild,		void				)( xui_component* sender, xui_method_args&  args )
{
	set_pageshow(get_game());
}
xui_method_explain(cocos_mainform, on_clickdebug,		void				)( xui_component* sender, xui_method_args&  args )
{
	m_steptime = 0;

	if (sender == m_run)
	{
		//cocos_hierarchy* hierarchy = get_hierarchy();
		//hierarchy->reset(true);

		//cocos_scene* scene = get_scene();
		//scene->set_trans(xui_vector<s32>(0));
		//scene->set_ratio(1.0);

		//if (m_run->was_push())
		//{
		//	OnityEditMode = false;
		//	m_run->set_data(NULL);
		//	BreezeGame::Game::Instance()->GetLoader()->Load(BreezeGame::LT_Splash);
		//	BreezeGame::LocalSaveHelper::Instance()->SaveLocalInfo();
		//	BreezeGame::Game::Instance()->Resume();

		//	cocos_game* game = get_game();
		//	if (m_mainview->get_showpage() != game && hierarchy->get_editprop())
		//	{
		//		OnityEditMode = true;
		//		m_run->set_data((void*)0x1);
		//		BreezeGame::GameConfig::Instance()->SetGameVersion(BreezeGame::GV_DEBUG);
		//		BreezeGame::ProfileManager::Instance()->SetLogin(true);
		//		BreezeGame::AccountLocalSaveHelper::Instance()->Load();
		//		BreezeGame::Game::Instance()->GetLoader()->Load(BreezeGame::LT_MainMenu);

		//		set_pageshow(game);
		//	}

		//	m3eFrameWorkUpdate(1.0f);
		//}
		//else
		//{
		//	m_pause->ini_toggle(false);

		//	BreezeGame::ProfileManager::Instance()->Logout(true);
		//	NPLoginService::GetInstance()->Logout();
		//	BreezeGame::Game::Instance()->GetLoader()->Load(BreezeGame::LT_None);
		//	BreezeGame::Game::Instance()->Pause();
		//	m3eFrameWorkUpdate(1.0f);

		//	if (m_run->get_data())
		//	{
		//		set_pageshow(get_scene());
		//	}
		//}
	}
	else
	if (sender == m_step && m_run->was_push())
	{
		m_pause->set_push(true);
		m_steptime = 200;
	}
}
xui_method_explain(cocos_mainform, on_clickwndmenu,		void				)( xui_component* sender, xui_method_args&  args )
{
	xui_dockpage* page = (xui_dockpage*)sender->get_data();
	if (page)
	{
		xui_dockview* view = xui_dynamic_cast(xui_dockview, page->get_parent());
		if (view == NULL)
			view = m_mainview;

		view->set_pageshow(page, !view->was_pageshow(page));
	}
}
xui_method_explain(cocos_mainform, on_clicksave,		void				)( xui_component* sender, xui_method_args&  args )
{
	extern std::wstring TOOLPATH;
	std::string filename = xui_global::unicode_to_ascii(TOOLPATH) + "cocos_editor.dock";
	FILE* file = fopen(filename.c_str(), "w");
	if (file)
	{
		extern int WNDPOSX;
		extern int WNDPOSY;
		extern int WNDWIDTH;
		extern int WNDHEIGHT;

		char buffer[256];
		sprintf(buffer, "%d,%d,%d,%d\n", WNDPOSX, WNDPOSY, WNDWIDTH, WNDHEIGHT);
		fwrite(buffer, 1, strlen(buffer), file);

		m_mainview->save_config(file, get_pagename, 0);
		std::vector<xui_window*> wndvec;
		std::vector<xui_syswnd*> sysvec = xui_global::get_syswndall();
		for (u32 i = 0; i < sysvec.size(); ++i)
		{
			xui_syswnd* syswnd = sysvec[i];
			xui_window* wnd = syswnd->get_popupctrl();
			const std::vector<xui_control*>& children = wnd->get_children();
			if (children.size() > 0 && xui_issub_kindof(xui_dockview, children.front()))
				wndvec.push_back(wnd);
		}

		u32 wndcount = wndvec.size();
		sprintf(buffer, "popupcount=%d\n", wndcount); fwrite(buffer, 1, strlen(buffer), file);
		for (u32 i = 0; i < wndcount; ++i)
		{
			xui_window* popupwnd = wndvec[i];
			xui_vector<s32> pt = popupwnd->get_renderpt();
			xui_vector<s32> sz = popupwnd->get_rendersz();
			sprintf(buffer, "viewrectangle=%d,%d,%d,%d\n", pt.x, pt.y, sz.w, sz.h);
			fwrite(buffer, 1, strlen(buffer), file);

			const std::vector<xui_control*>& children = popupwnd->get_children();
			xui_dockview* dockview = xui_dynamic_cast(xui_dockview, children.front());
			dockview->save_config(file, get_pagename, 4);
		}

		fclose(file);
	}
}
xui_method_explain(cocos_mainform, on_clickload,		void				)( xui_component* sender, xui_method_args&  args )
{
	extern std::wstring TOOLPATH;
	std::string filename = xui_global::unicode_to_ascii(TOOLPATH) + "cocos_editor.dock";
	FILE* file = fopen(filename.c_str(), "r");
	if (file)
	{
		del_allview();

		std::string line;
		line = xui_global::get_fileline(file);
		xui_method_ptrcall(m_mainview, load_config	)(file, get_pagectrl);
		xui_method_ptrcall(m_mainview, use_portions	)();

		u32 wndcount = 0;
		line = xui_global::get_fileline(file);
		if (line.length() > 0)
		{
			std::string temp = line.substr(line.find_first_not_of(' '));
			sscanf(temp.c_str(), "popupcount=%d", &wndcount);
		}
		for (u32 i = 0; i < wndcount; ++i)
		{
			line = xui_global::get_fileline(file);
			if (line.empty())
				continue;

			xui_window* popupwnd = new xui_window(xui_vector<s32>(0), false, true);
			s32 x = 0;
			s32 y = 0;
			s32 w = 0;
			s32 h = 0;
			std::string temp = line.substr(line.find_first_not_of(' '));
			sscanf(temp.c_str(), "viewrectangle=%d,%d,%d,%d", &x, &y, &w, &h);
			xui_dockview* fillview = new xui_dockview(xui_vector<s32>(0), DOCKSTYLE_F);
			xui_method_ptrcall(fillview, load_config	)(file, get_pagectrl);
			xui_method_ptrcall(popupwnd, add_child		)(fillview);
			xui_method_ptrcall(popupwnd, set_renderpt	)(xui_vector<s32>(x, y));
			xui_method_ptrcall(popupwnd, set_rendersz	)(xui_vector<s32>(w, h));
			xui_desktop::get_ins()->add_child(popupwnd);
		}

		fclose(file);
	}
}
xui_method_explain(cocos_mainform, on_clickreset,		void				)( xui_component* sender, xui_method_args&  args )
{
	del_allview();
	std::vector<xui_menuitem*> menulist;
	menulist.push_back(m_inspector);
	menulist.push_back(m_project);
	menulist.push_back(m_console);
	menulist.push_back(m_toolbox);
	//menulist.push_back(m_timeline);
	menulist.push_back(m_hierarchy);

	for (u32 i = 0; i < menulist.size(); ++i)
	{
		xui_dockpage* page = (xui_dockpage*)menulist[i]->get_data();
		if (page)
		{
			page->on_perform_sz(xui_vector<s32>(300));
			m_mainview->add_dockpage(page, page->get_initdock(), false, true);
		}
	}
}
xui_method_explain(cocos_mainform, on_clicksaveall,		void				)( xui_component* sender, xui_method_args&  args )
{
	xui_proproot_vec filevec;
	cocos_project* project = get_project();
	project->get_pathfile(FILTER_ALL, filevec);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		cocos_propfile* file = dynamic_cast<cocos_propfile*>(filevec[i]);
		if (file->was_modify())
			file->save();
	}
}
xui_method_explain(cocos_mainform, on_paintdebug,		void				)( xui_component* sender, xui_method_args&  args )
{
	xui_colour      color	= sender->get_vertexcolor();
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (sender == m_run)
	{
		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(center.x-8,	center.y-8);
		poly[1] = xui_vector<s32>(center.x+8,	center.y);
		poly[2] = xui_vector<s32>(center.x-8,	center.y+8);
		xui_convas::get_ins()->fill_poly(poly, 3, color);
	}
	else
	if (sender == m_pause)
	{
		rt.ax = center.x-6;
		rt.ay = center.y-7;
		rt.set_w( 4);
		rt.set_h(14);
		xui_convas::get_ins()->fill_rectangle(rt, color);
		rt.oft_x( 8);
		xui_convas::get_ins()->fill_rectangle(rt, color);
	}
	else
	if (sender == m_step)
	{
		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(center.x-8,	center.y-8);
		poly[1] = xui_vector<s32>(center.x+8,	center.y);
		poly[2] = xui_vector<s32>(center.x-8,	center.y+8);
		xui_convas::get_ins()->fill_poly(poly, 3, color);

		rt.ax = center.x+6;
		rt.ay = center.y-7;
		rt.set_w( 3);
		rt.set_h(14);
		xui_convas::get_ins()->fill_rectangle(rt, color);
	}
}
xui_method_explain(cocos_mainform, on_mainviewinvalid,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> minlimit = m_mainview->get_minlimit();
	minlimit.h += m_toolpane->get_renderh();

	xui_vector<s32> sz = get_clientsz();
	sz.w = xui_max(sz.w, 800);
	sz.w = xui_max(sz.w, minlimit.w);
	sz.h = xui_max(sz.h, minlimit.h);
	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
}
xui_method_explain(cocos_mainform, on_mainviewchanged,	void				)( xui_component* sender, xui_method_args&	args )
{
	cocos_scene* scene = xui_dynamic_cast(cocos_scene, m_mainview->get_showpage());
	if (scene && m_scene != scene)
	{
		m_scene = scene;
		cocos_hierarchy* hierarchy = get_hierarchy();
		hierarchy->reset();
	}
}
xui_method_explain(cocos_mainform, on_recentaccept,		void				)( xui_component* sender, xui_method_args&  args )
{
	cocos_recent* dialog = xui_dynamic_cast(cocos_recent, sender);
	xui_global::set_workpath(dialog->get_selectpath());
	xui_global::add_path(L"cocos_editor");
	dialog->set_visible(false);
	xui_desktop::get_ins()->del_child(dialog);

	FILE* file = fopen("cocos_editor/cocos_editor.ignore", "r");
	if (file)
	{
		while (feof(file) == false)
		{
			std::string line = xui_global::get_fileline(file);
			if (line.length() > 0)
				m_ignorepaths.push_back(xui_global::ascii_to_unicode(line));
		}

		fclose(file);
	}

	cocos2d::Configuration::getInstance()->gatherGPUInfo();
	cocos2d::FileUtils::getInstance()->addSearchPath(xui_global::unicode_to_ascii(dialog->get_selectpath()) + "/", true);
	cocos2d::Director::getInstance()->setOpenGLView(m_glview);
	cocos2d::Director::getInstance()->replaceScene(m_cocosscene);

	cocos_project* project = get_project();
	m_backupfiles.clear();
	xui_method_ptrcall(project, ini_pathtree)();

	if (m_backupfiles.size() > 0)
	{
		cocos_backup* backup = new cocos_backup;
		backup->xm_accept += new xui_method_member<xui_method_args, cocos_mainform>(this, &cocos_mainform::on_backupaccept);
		xui_desktop::get_ins()->add_child(backup);
	}
	else
	{
		cocos2d::GLProgramCache::getInstance()->reloadDefaultGLPrograms();
		xui_global::set_fwatchstart(xui_global::get_workpath());
	}
}
xui_method_explain(cocos_mainform, on_backupaccept,		void				)( xui_component* sender, xui_method_args&  args )
{
	cocos_backup* backup = xui_dynamic_cast(cocos_backup, sender);
	backup->set_visible(false);
	xui_desktop::get_ins()->del_child(backup);
	m_backupfiles.clear();

	cocos2d::GLProgramCache::getInstance()->reloadDefaultGLPrograms();
	xui_global::set_fwatchstart(xui_global::get_workpath());
}
xui_method_explain(cocos_mainform, on_globalkeybddown,	void				)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_S && args.ctrl && args.handle == false)
	{
		//cocos_propfile* propfile = get_hierarchy()->get_editprop();
		//if (propfile && propfile->was_modify())
		//	propfile->save();
	}
}

/*
//method
*/
xui_method_explain(cocos_mainform, del_allview,			void				)( void )
{
	std::vector<xui_menuitem*> menulist;
	menulist.push_back(m_inspector);
	menulist.push_back(m_console);
	menulist.push_back(m_project);
	menulist.push_back(m_toolbox);
	menulist.push_back(m_game);
	//menulist.push_back(m_timeline);
	menulist.push_back(m_hierarchy);

	for (u32 i = 0; i < menulist.size(); ++i)
	{
		xui_dockpage* page = (xui_dockpage*)menulist[i]->get_data();
		if (page)
		{
			xui_dockview* rootview = xui_dynamic_cast(xui_dockview, page->get_parent());
			if (rootview)
				rootview->del_dockpage(page, false);
		}
	}
}
xui_method_explain(cocos_mainform, add_backupfile,		void				)( xui_proproot* prop )
{
	m_backupfiles.push_back(prop);
}
xui_method_explain(cocos_mainform, get_backupfilenum,	u32					)( void ) const
{
	return m_backupfiles.size();
}
xui_method_explain(cocos_mainform, get_backupfile,		xui_proproot*		)( u32 index ) const
{
	return m_backupfiles[index];
}
xui_method_explain(cocos_mainform, was_ignorepath,		bool				)( const std::wstring& path )
{
	std::vector<std::wstring>::iterator itor = std::find(
		m_ignorepaths.begin(),
		m_ignorepaths.end(),
		path);

	return itor != m_ignorepaths.end();
}
xui_method_explain(cocos_mainform, set_saveshow,		void				)( void )
{
	xui_desktop::get_ins()->add_child(new cocos_save);
}