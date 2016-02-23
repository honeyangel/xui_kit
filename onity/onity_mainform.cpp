#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_toolbar.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"
#include "onity_mainform.h"

/*
//constructor
*/
xui_create_explain(onity_mainform)( void )
: xui_window(xui_vector<s32>(0))
{
	m_dockstyle		= DOCKSTYLE_F;

	m_select		= xui_toggle::create(xui_bitmap::create("icon/select.png"),		32);
	m_translate		= xui_toggle::create(xui_bitmap::create("icon/translate.png"),	32);
	m_scale			= xui_toggle::create(xui_bitmap::create("icon/scale.png"),		32);
	m_rotation		= xui_toggle::create(xui_bitmap::create("icon/rotation.png"),	32);
	xui_method_ptrcall(m_select,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_scale,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_rotation,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clicktransform);
	xui_method_ptrcall(m_translate,		ini_toggle		)(true);

	//transform
	xui_linebox* line_transform	= xui_linebox::create();
	xui_method_ptrcall(line_transform,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_select);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_translate);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_scale);
	xui_method_ptrcall(line_transform,	add_linectrl	)(m_rotation);

	m_anchor		= xui_button::create(xui_bitmap::create("icon/pivot.png"), L"Pivot", 80);
	m_coordinate	= xui_button::create(xui_bitmap::create("icon/world.png"), L"World", 80);
	xui_method_ptrcall(m_anchor,		set_data		)((void*)TOA_PIVOT);
	xui_method_ptrcall(m_coordinate,	set_data		)((void*)TOC_WORLD);
	xui_method_ptrcall(m_anchor,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickanchor);
	xui_method_ptrcall(m_coordinate,	xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickcoordinate);

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
	xui_method_ptrcall(m_run,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
	xui_method_ptrcall(m_pause,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
	xui_method_ptrcall(m_step,			xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickdebug);
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
	m_hierarchy		= menu->add_item(xui_bitmap::create("icon/hierarchy.png"),	L"Hierarchy");
	m_inspector		= menu->add_item(xui_bitmap::create("icon/inspector.png"),	L"Inspector");
	m_project		= menu->add_item(xui_bitmap::create("icon/project.png"),	L"Project");
	m_console		= menu->add_item(xui_bitmap::create("icon/console.png"),	L"Console");
	m_timeline		= menu->add_item(xui_bitmap::create("icon/timeline.png"),	L"Timeline");
	m_game			= menu->add_item(xui_bitmap::create("icon/game.png"),		L"Game");
	m_scene			= menu->add_item(xui_bitmap::create("icon/scene.png"),		L"Scene");
	m_animator		= menu->add_item(xui_bitmap::create("icon/animator.png"),	L"Animator");
	xui_method_ptrcall(m_hierarchy,		set_data		)(xui_dockpage::create(xui_bitmap::create("icon/hierarchy.png"), L"Hierarchy", 200, AREALIMIT_L, 200));
	xui_method_ptrcall(m_inspector,		set_data		)(xui_dockpage::create(xui_bitmap::create("icon/inspector.png"), L"Inspector", 200, AREALIMIT_R, 200));
	xui_method_ptrcall(m_hierarchy,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	xui_method_ptrcall(m_inspector,		xm_click		) += new xui_method_member<xui_method_args, onity_mainform>(this, &onity_mainform::on_clickwndmenu);
	menu->add_separate();
	m_save			= menu->add_item(NULL, L"Save");
	m_load			= menu->add_item(NULL, L"Load");
	m_reset			= menu->add_item(NULL, L"Reset");

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
//event
*/
xui_method_explain(onity_mainform, on_clicktransform,	void)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(onity_mainform, on_clickanchor,		void)( xui_component* sender, xui_method_args& args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toa = (u08)drawer->get_data();
	switch (toa)
	{
	case TOA_CENTER:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_PIVOT);
		xui_method_ptrcall(drawer, ini_drawer	)(xui_bitmap::create("icon/pivot.png"));
		xui_method_ptrcall(drawer, ini_drawer	)(L"Pivot");
		break;
	case TOA_PIVOT:
		xui_method_ptrcall(drawer, set_data		)((void*)TOA_CENTER);
		xui_method_ptrcall(drawer, ini_drawer	)(xui_bitmap::create("icon/center.png"));
		xui_method_ptrcall(drawer, ini_drawer	)(L"Center");
		break;
	}
}
xui_method_explain(onity_mainform, on_clickcoordinate,	void)( xui_component* sender, xui_method_args& args )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, sender);
	u08 toc = (u08)drawer->get_data();
	switch (toc)
	{
	case TOC_LOCAL:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_WORLD);
		xui_method_ptrcall(drawer, ini_drawer	)(xui_bitmap::create("icon/world.png"));
		xui_method_ptrcall(drawer, ini_drawer	)(L"World");
		break;
	case TOC_WORLD:
		xui_method_ptrcall(drawer, set_data		)((void*)TOC_LOCAL);
		xui_method_ptrcall(drawer, ini_drawer	)(xui_bitmap::create("icon/local.png"));
		xui_method_ptrcall(drawer, ini_drawer	)(L"Local");
		break;
	}
}
xui_method_explain(onity_mainform, on_clickdebug,		void)( xui_component* sender, xui_method_args& args )
{
	if (m_run->was_push() == false)
	{
		m_pause->ini_toggle(false);
	}
}
xui_method_explain(onity_mainform, on_clickwndmenu,		void)( xui_component* sender, xui_method_args& args )
{
	xui_dockpage* dockpage = (xui_dockpage*)sender->get_data();
	xui_dockview* rootview = xui_dynamic_cast(xui_dockview, dockpage->get_parent());
	if (rootview)
	{
		rootview->del_dockpage(dockpage);
	}
	else
	{
		if		(sender == m_hierarchy)	m_mainview->add_dockpage(dockpage, DOCKSTYLE_L, false);
		else if (sender == m_inspector) m_mainview->add_dockpage(dockpage, DOCKSTYLE_R, false);
		else
		{}
	}
}
xui_method_explain(onity_mainform, on_clicksave,		void)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_mainform, on_clickload,		void)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_mainform, on_clickreset,		void)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_mainform, on_paintdebug,		void)( xui_component* sender, xui_method_args& args )
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