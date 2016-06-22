#include "NPFileName.h"
#include "NPRender.h"
#include "NP2DSLib.h"
#include "m3eFrameWork.h"

#include "xui_desktop.h"
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
#include "onity_resource.h"
#include "onity_hierarchy.h"
#include "onity_inspector.h"
#include "onity_console.h"
#include "onity_project.h"
#include "onity_timeline.h"
#include "onity_game.h"
#include "onity_animator.h"
#include "onity_recent.h"
#include "onity_config.h"
#include "onity_mainform.h"

xui_implement_rtti(onity_mainform, xui_window);

/*
//constructor
*/
xui_create_explain(onity_mainform)( void )
: xui_window(xui_vector<s32>(0))
{
	m_dockstyle		= DOCKSTYLE_F;

	m_select		= xui_toggle::create(onity_resource::icon_select,		32);
	m_translate		= xui_toggle::create(onity_resource::icon_translate,	32);
	m_scale			= xui_toggle::create(onity_resource::icon_scale,		32);
	m_rotation		= xui_toggle::create(onity_resource::icon_rotation,		32);
	xui_method_ptrcall(m_select,		xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_scale,			xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_rotation,		xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		ini_toggle		)(true);

	//transform
	xui_linebox* line_transform	= xui_linebox::create();
	xui_method_ptrcall(line_transform,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_select);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_translate);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_scale);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_rotation);

	m_anchor		= xui_button::create(onity_resource::icon_pivot, L"Pivot", 80);
	m_coordinate	= xui_button::create(onity_resource::icon_world, L"World", 80);
	xui_method_ptrcall(m_anchor,		set_data		)((void*)TOA_PIVOT);
	xui_method_ptrcall(m_coordinate,	set_data		)((void*)TOC_WORLD);
	xui_method_ptrcall(m_anchor,		xm_buttonclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickanchor);
	xui_method_ptrcall(m_coordinate,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickcoordinate);

	//anchor
	//coordinate
	xui_linebox* line_operator	= xui_linebox::create();
	xui_method_ptrcall(line_operator,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_operator,	set_renderpt	)(xui_vector<s32>(160, 0));
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_anchor);
	xui_method_ptrcall(line_operator,	add_linectrl	)(m_coordinate);

	m_run			= xui_toggle::create(NULL, 32);
	m_pause			= xui_toggle::create(NULL, 32);
	m_step			= xui_button::create(NULL, 32);
	xui_method_ptrcall(m_run,			xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
	xui_method_ptrcall(m_pause,			xm_toggleclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
	xui_method_ptrcall(m_step,			xm_buttonclick	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
	xui_method_ptrcall(m_run,			xm_renderself	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_paintdebug);
	xui_method_ptrcall(m_pause,			xm_renderself	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_paintdebug);
	xui_method_ptrcall(m_step,			xm_renderself	) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_paintdebug);

	//run
	//pause
	//step
	xui_linebox* line_debug		= xui_linebox::create();
	xui_method_ptrcall(line_debug,		ini_component	)(ALIGNHORZ_C, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_run);
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_pause);
	xui_method_ptrcall(line_debug,		add_linectrl	)(m_step);

	xui_menu* menu	= xui_menu::create(100);
	m_hierarchy		= menu->add_item(onity_resource::icon_hierarchy,	L"Hierarchy");
	m_inspector		= menu->add_item(onity_resource::icon_inspector,	L"Inspector");
	m_project		= menu->add_item(onity_resource::icon_project,		L"Project");
	m_console		= menu->add_item(onity_resource::icon_console,		L"Console");
	m_timeline		= menu->add_item(onity_resource::icon_timeline,		L"Timeline");
	m_game			= menu->add_item(onity_resource::icon_game,			L"Game");
	m_scene			= menu->add_item(onity_resource::icon_scene,		L"Scene");
	m_animator		= menu->add_item(onity_resource::icon_animator,		L"Animator");
	xui_method_ptrcall(m_hierarchy,		set_data		)(new onity_hierarchy);
	xui_method_ptrcall(m_inspector,		set_data		)(new onity_inspector);
	xui_method_ptrcall(m_project,		set_data		)(new onity_project);
	xui_method_ptrcall(m_console,		set_data		)(new onity_console);
	xui_method_ptrcall(m_timeline,		set_data		)(new onity_timeline);
	xui_method_ptrcall(m_game,			set_data		)(new onity_game);
	xui_method_ptrcall(m_animator,		set_data		)(new onity_animator);
	xui_method_ptrcall(m_hierarchy,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_inspector,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_project,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_console,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_timeline,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_game,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_animator,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	menu->add_separate();
	m_save			= menu->add_item(NULL, L"Save");
	m_load			= menu->add_item(NULL, L"Load");
	m_reset			= menu->add_item(NULL, L"Reset");
	xui_method_ptrcall(m_save,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicksave);
	xui_method_ptrcall(m_load,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickload);
	xui_method_ptrcall(m_reset,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickreset);

	m_window		= xui_toggle::create(NULL, L"Window", 100);
	xui_method_ptrcall(m_window,		ini_drawer		)(NULL, xui_vector<s32>(0));
	xui_method_ptrcall(m_window,		set_menu		)(menu);

	//menu
	xui_toolbar* tool_menu		= xui_toolbar::create();
	xui_method_ptrcall(tool_menu,		ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(tool_menu,		add_item		)(m_window);

	//main
	m_toolpane		= new xui_panel(xui_vector<s32>(40));
	m_mainview		= xui_dockview::create();
	xui_method_ptrcall(m_mainview,		xm_invalid		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_mainviewinvalid);
	xui_method_ptrcall(m_toolpane,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_toolpane,		set_borderrt	)(xui_rect2d<s32>(8));
	xui_method_ptrcall(m_toolpane,		add_child		)(line_transform);
	xui_method_ptrcall(m_toolpane,		add_child		)(line_operator);
	xui_method_ptrcall(m_toolpane,		add_child		)(line_debug);
	xui_method_ptrcall(m_toolpane,		add_child		)(tool_menu);
	add_child(m_toolpane);
	add_child(m_mainview);
}

/*
//static
*/
xui_method_explain(onity_mainform, get_ptr,				onity_mainform*		)( void )
{
	const std::vector<xui_control*>& vec = xui_desktop::get_ins()->get_children();
	if (vec.size() > 0)
		return xui_dynamic_cast(onity_mainform, vec[0]);

	return NULL;
}
/*
//method
*/
xui_method_explain(onity_mainform, get_inspector,		onity_inspector*	)( void )
{
	return (onity_inspector*)	xui_method_ptrcall(m_inspector, get_data)();
}
xui_method_explain(onity_mainform, get_project,			onity_project*		)( void )
{
	return (onity_project*)		xui_method_ptrcall(m_project,	get_data)();
}
xui_method_explain(onity_mainform, get_game,			onity_game*			)( void )
{
	return (onity_game*)		xui_method_ptrcall(m_game,		get_data)();
}
xui_method_explain(onity_mainform, get_animator,		onity_animator*		)( void )
{
	return (onity_animator*)	xui_method_ptrcall(m_animator,	get_data)();
}
xui_method_explain(onity_mainform, get_console,			onity_console*		)( void )
{
	return (onity_console*)		xui_method_ptrcall(m_console,	get_data)();
}
xui_method_explain(onity_mainform, get_timeline,		onity_timeline*		)( void )
{
	return (onity_timeline*)	xui_method_ptrcall(m_timeline,	get_data)();
}

/*
//method
*/
xui_method_explain(onity_mainform, was_gamerun,			bool				)( void ) const
{
	return m_run->was_push();
}
xui_method_explain(onity_mainform, was_gameplay,		bool				)( void ) const
{
	return m_run->was_push() && m_pause->was_push() == false;
}
xui_method_explain(onity_mainform, set_pageshow,		void				)( xui_dockpage* page )
{
	xui_dockview* view = xui_dynamic_cast(xui_dockview, page->get_parent());
	if (view == NULL)
	{
		view = m_mainview;
		view->add_dockpage(page, page->get_initdock(), false, true);
	}

	if (view->has_dockpage(page))
	{
		view->set_showpage(page);
	}
}

/*
//callback
*/
xui_method_explain(onity_mainform, on_load,				void				)( xui_method_args& args )
{
	xui_window::on_load(args);
	on_clickload(NULL,  args);

	onity_recent* dialog = new onity_recent;
	dialog->xm_accept   += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_recentaccept);
	xui_desktop::get_ins()->add_child(dialog);
}

/*
//event
*/
xui_method_explain(onity_mainform, on_clicktransform,	void				)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(onity_mainform, on_clickanchor,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toa = (u08)drawer->get_data();
	switch (toa)
	{
	case TOA_CENTER:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_PIVOT);
		xui_method_ptrcall(drawer, ini_drawer	)(onity_resource::icon_pivot);
		xui_method_ptrcall(drawer, ini_drawer	)(L"Pivot");
		break;
	case TOA_PIVOT:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_CENTER);
		xui_method_ptrcall(drawer, ini_drawer	)(onity_resource::icon_center);
		xui_method_ptrcall(drawer, ini_drawer	)(L"Center");
		break;
	}
}
xui_method_explain(onity_mainform, on_clickcoordinate,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toc = (u08)drawer->get_data();
	switch (toc)
	{
	case TOC_LOCAL:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_WORLD);
		xui_method_ptrcall(drawer, ini_drawer	)(onity_resource::icon_world, xui_vector<s32>(16));
		xui_method_ptrcall(drawer, ini_drawer	)(L"World");
		break;
	case TOC_WORLD:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_LOCAL);
		xui_method_ptrcall(drawer, ini_drawer	)(onity_resource::icon_local, xui_vector<s32>(16));
		xui_method_ptrcall(drawer, ini_drawer	)(L"Local");
		break;
	}
}
xui_method_explain(onity_mainform, on_clickdebug,		void				)( xui_component* sender, xui_method_args& args )
{
	if (m_run->was_push() == false)
	{
		m_pause->ini_toggle(false);
	}
}
xui_method_explain(onity_mainform, on_clickwndmenu,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_dockpage* page = (xui_dockpage*)sender->get_data();
	if (page)
	{
		xui_dockview* rootview = xui_dynamic_cast(xui_dockview, page->get_parent());
		if (rootview)
		{
			rootview->del_dockpage(page);
		}
		else
		{
			rootview = m_mainview;
			rootview->add_dockpage(page, page->get_initdock(), false, true);
		}
	}
}
xui_method_explain(onity_mainform, on_clicksave,		void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_mainform, on_clickload,		void				)( xui_component* sender, xui_method_args& args )
{
	//TODO
	on_clickreset(sender, args);
}
xui_method_explain(onity_mainform, on_clickreset,		void				)( xui_component* sender, xui_method_args& args )
{
	del_allview();
	std::vector<xui_menuitem*> menulist;
	menulist.push_back(m_inspector);
	menulist.push_back(m_project);
	menulist.push_back(m_console);
	menulist.push_back(m_timeline);
	menulist.push_back(m_hierarchy);
	menulist.push_back(m_scene);
	menulist.push_back(m_game);
	menulist.push_back(m_animator);

	xui_vecptr_addloop(menulist)
	{
		xui_dockpage* page = (xui_dockpage*)menulist[i]->get_data();
		if (page)
		{
			page->on_perform_sz(xui_vector<s32>(300));
			m_mainview->add_dockpage(page, page->get_initdock(), false, true);
		}
	}
}
xui_method_explain(onity_mainform, on_paintdebug,		void				)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(onity_mainform, on_mainviewinvalid,	void				)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(onity_mainform, on_recentaccept,		void				)( xui_component* sender, xui_method_args& args )
{
	onity_recent* dialog = xui_dynamic_cast(onity_recent, sender);
	xui_global::set_workpath(dialog->get_selectpath());

	std::wstring srcpath = L"..\\rawConfig\\BinFiles\\client";
	std::wstring dstpath = L"Config";
	std::vector<std::wstring> filevec = xui_global::get_file(srcpath, L"*.bin");
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		std::wstring srcfile = srcpath+L"\\"+filevec[i];
		std::wstring dstfile = dstpath+L"\\"+filevec[i];
		xui_global::cpy_file(srcfile, dstfile);
	}

	dialog->set_visible(false);
	xui_desktop::get_ins()->del_child(dialog);

	onity_config* config = new onity_config;
	config->xm_accept   += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_configaccept);
	xui_desktop::get_ins()->add_child(config);
}
xui_method_explain(onity_mainform, on_configaccept,		void				)( xui_component* sender, xui_method_args& args )
{
	m_run->ini_toggle(true);

	NPRender::Init();
	NP2DSLib::Init();
	onity_project* project = get_project();
	onity_game*    game    = get_game();
	xui_method_ptrcall(project, ini_pathtree)();
	xui_method_ptrcall(game,	ini_game	)();
	xui_global::set_fwatchstart(xui_global::get_workpath());

	onity_config* dialog = xui_dynamic_cast(onity_config, sender);
	dialog->set_visible(false);
	xui_desktop::get_ins()->del_child(dialog);
}

/*
//method
*/
xui_method_explain(onity_mainform, del_allview,			void				)( void )
{
	std::vector<xui_menuitem*> menulist;
	menulist.push_back(m_hierarchy);
	menulist.push_back(m_inspector);
	menulist.push_back(m_console);
	menulist.push_back(m_project);
	menulist.push_back(m_timeline);
	menulist.push_back(m_game);
	menulist.push_back(m_scene);
	menulist.push_back(m_animator);

	xui_vecptr_addloop(menulist)
	{
		xui_dockpage* page = (xui_dockpage*)menulist[i]->get_data();
		if (page)
		{
			xui_dockview* rootview = xui_dynamic_cast(xui_dockview, page->get_parent());
			if (rootview)
				rootview->del_dockpage(page);
		}
	}
}