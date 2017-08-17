#include "xui_convas.h"
#include "xui_panel.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_toolpane.h"
#include "cocos_toolbox.h"

xui_implement_rtti(cocos_toolbox, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_toolbox)(void)
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T, 200, DOCKSTYLE_T)
{
	ini_namectrl(cocos_resource::icon_setting, L"Toolbox");

	m_object		= new cocos_toolpane(L"Object");
	//xui_method_ptrcall(m_object,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_object,		add_toolctrl	)(cocos_resource::icon_custom, L"Particle");
	xui_method_ptrcall(m_object,		add_toolctrl	)(cocos_resource::icon_custom, L"Node");
	xui_method_ptrcall(m_object,		add_toolctrl	)(cocos_resource::icon_custom, L"Sprite");
	m_controller	= new cocos_toolpane(L"Controller");
	//xui_method_ptrcall(m_controller,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"Button");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"CheckBox");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"ImageView");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"Text");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"TextAtlas");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"TextBMFont");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"LoadingBar");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"Slider");
	xui_method_ptrcall(m_controller,	add_toolctrl	)(cocos_resource::icon_custom, L"TextField");
	m_container		= new cocos_toolpane(L"Container");
	//xui_method_ptrcall(m_container,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_container,		add_toolctrl	)(cocos_resource::icon_custom, L"Layout");
	xui_method_ptrcall(m_container,		add_toolctrl	)(cocos_resource::icon_custom, L"ListView");
	xui_method_ptrcall(m_container,		add_toolctrl	)(cocos_resource::icon_custom, L"PageView");
	xui_method_ptrcall(m_container,		add_toolctrl	)(cocos_resource::icon_custom, L"ScrollView");
	m_fill			= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fill,			ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fill,			set_drawcolor	)(false);
	xui_method_ptrcall(m_fill,			set_hscrollauto	)(false);
	xui_method_ptrcall(m_fill,			add_child		)(m_object);
	xui_method_ptrcall(m_fill,			add_child		)(m_controller);
	xui_method_ptrcall(m_fill,			add_child		)(m_container);
	xui_method_ptrcall(m_fill,			xm_perform		) += new xui_method_member<xui_method_args, cocos_toolbox>(this, &cocos_toolbox::on_fillperform);
	xui_method_ptrcall(m_fill,			xm_renderelse	) += new xui_method_member<xui_method_args, cocos_toolbox>(this, &cocos_toolbox::on_fillrenderelse);

	add_pagectrl(m_fill);
}

/*
//override
*/
xui_method_explain(cocos_toolbox, on_fillperform,		void)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_fill->get_renderrtins();

	s32 x = rt.ax;
	s32 y = rt.ay;
	std::vector<xui_control*> children = m_fill->get_children();
	for (u32 i = 0; i < children.size(); ++i)
	{
		children[i]->on_perform_x(rt.ax);
		children[i]->on_perform_y(y);
		children[i]->on_perform_w(rt.get_w());
		y += children[i]->get_renderh();
	}
}
xui_method_explain(cocos_toolbox, on_fillrenderelse,	void)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_fill->get_renderrtabs()));
	std::vector<xui_control*> children = m_fill->get_children();
	for (u32 i = 0; i < children.size()-1; ++i)
	{
		xui_rect2d<s32> rt = children[i]->get_renderrtabs();
		xui_vector<s32> p1(rt.ax, rt.by);
		xui_vector<s32> p2(rt.bx, rt.by);
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour::black);
	}
	xui_convas::get_ins()->set_cliprect(cliprect);
}