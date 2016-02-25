#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_panel.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_toolbar.h"
#include "xui_listview.h"
#include "onity_console.h"

xui_implement_rtti(onity_console, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_console)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	m_inborder	= xui_rect2d<s32>(4);
	ini_namectrl(xui_bitmap::create("icon/console.png"), L"Console");

	m_clear		= new xui_button(xui_vector<s32>(60, 20));
	xui_method_ptrcall(m_clear,		xm_click		) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_clearclick);
	xui_method_ptrcall(m_clear,		set_corner		)(3);
	xui_method_ptrcall(m_clear,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_clear,		set_drawcolor	)(true);
	xui_method_ptrcall(m_clear,		set_textalign	)(TA_CC);
	xui_method_ptrcall(m_clear,		ini_drawer		)(NULL, xui_vector<s32>(0));
	xui_method_ptrcall(m_clear,		ini_drawer		)(L"Clear");

	m_error		= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_error,		xm_click		) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_error,		xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_error,		set_corner		)(3);
	xui_method_ptrcall(m_error,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_error,		set_drawcolor	)(true);
	xui_method_ptrcall(m_error,		set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_error,		ini_drawer		)(xui_bitmap::create("icon/error.png"));
	xui_method_ptrcall(m_error,		set_text		)(L"0");

	m_warning	= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_warning,	xm_click		) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_warning,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_warning,	set_corner		)(3);
	xui_method_ptrcall(m_warning,	set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_warning,	set_drawcolor	)(true);
	xui_method_ptrcall(m_warning,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_warning,	ini_drawer		)(xui_bitmap::create("icon/warning.png"));
	xui_method_ptrcall(m_warning,	set_text		)(L"0");

	m_message	= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_message,	xm_click		) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_message,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_message,	set_corner		)(3);
	xui_method_ptrcall(m_message,	set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_message,	set_drawcolor	)(true);
	xui_method_ptrcall(m_message,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_message,	ini_drawer		)(xui_bitmap::create("icon/message.png"));
	xui_method_ptrcall(m_message,	set_text		)(L"0");

	xui_toolbar* toolbar = new xui_toolbar(xui_vector<s32>(0, 20));
	xui_method_ptrcall(toolbar,		ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(toolbar,		add_item		)(m_clear);
	xui_method_ptrcall(toolbar,		add_separate	)();
	xui_method_ptrcall(toolbar,		add_item		)(m_error);
	xui_method_ptrcall(toolbar,		add_item		)(m_warning);
	xui_method_ptrcall(toolbar,		add_item		)(m_message);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(true);
	xui_method_ptrcall(m_head,		set_corner		)(6);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		add_child		)(toolbar);
	m_list		= new xui_listview(xui_vector<s32>(200), false);
	xui_method_ptrcall(m_list,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_list,		set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_list,		set_iconsize	)(xui_vector<s32>(0));
	add_pagectrl(m_head);
	add_pagectrl(m_list);
}

/*
//event
*/
xui_method_explain(onity_console, on_toggletextchanged, void)( xui_component* sender, xui_method_args& args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);
	s32 w = 0;
	w += toggle->get_borderrt().ax + toggle->get_borderrt().bx;
	w += toggle->get_iconoffset().x;
	w += toggle->get_iconsize().w;
	w += toggle->get_textoffset().x;
	w += xui_convas::get_ins()->calc_size(toggle->get_text(), toggle->get_textfont(), 0, true).w;
	toggle->set_renderw(w);
}
xui_method_explain(onity_console, on_toggleclick,		void)( xui_component* sender, xui_method_args& args )
{
	m_list->add_item(L"[Error  ]: error",   xui_family_render(xui_colour::red));
	m_list->add_item(L"[Warning]: warning", xui_family_render(xui_colour(1.0f, 208.0f/255.0f, 170.0f/255.0f, 9.0f/255.0f)));
	m_list->add_item(L"[Message]: message", xui_family_render::default);
}
xui_method_explain(onity_console, on_clearclick,		void)( xui_component* sender, xui_method_args& args )
{

}