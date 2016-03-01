#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_panel.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_toolbar.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "onity_console.h"

xui_implement_rtti(onity_console, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_console)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(xui_bitmap::create("icon/console.png"), L"Console");

	m_clear		= new xui_button(xui_vector<s32>(60, 20));
	xui_method_ptrcall(m_clear,		xm_buttonclick	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_clearclick);
	xui_method_ptrcall(m_clear,		set_corner		)(3);
	xui_method_ptrcall(m_clear,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_clear,		set_drawcolor	)(true);
	xui_method_ptrcall(m_clear,		set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_clear,		ini_drawer		)(NULL, xui_vector<s32>(0));
	xui_method_ptrcall(m_clear,		ini_drawer		)(L"Clear");

	m_error		= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_error,		xm_toggleclick	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_error,		xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_error,		set_corner		)(3);
	xui_method_ptrcall(m_error,		set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_error,		set_drawcolor	)(true);
	xui_method_ptrcall(m_error,		set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_error,		ini_drawer		)(xui_bitmap::create("icon/error.png"));
	xui_method_ptrcall(m_error,		ini_toggle		)(true);
	xui_method_ptrcall(m_error,		set_text		)(L"0");

	m_warning	= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_warning,	xm_toggleclick	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_warning,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_warning,	set_corner		)(3);
	xui_method_ptrcall(m_warning,	set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_warning,	set_drawcolor	)(true);
	xui_method_ptrcall(m_warning,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_warning,	ini_drawer		)(xui_bitmap::create("icon/warning.png"));
	xui_method_ptrcall(m_warning,	ini_toggle		)(true);
	xui_method_ptrcall(m_warning,	set_text		)(L"0");

	m_message	= new xui_toggle(xui_vector<s32>(60, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_message,	xm_toggleclick	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggleclick);
	xui_method_ptrcall(m_message,	xm_textchanged	) += new xui_method_member<xui_method_args, onity_console>(this, &onity_console::on_toggletextchanged);
	xui_method_ptrcall(m_message,	set_corner		)(3);
	xui_method_ptrcall(m_message,	set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_message,	set_drawcolor	)(true);
	xui_method_ptrcall(m_message,	set_textoffset	)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_message,	ini_drawer		)(xui_bitmap::create("icon/message.png"));
	xui_method_ptrcall(m_message,	ini_toggle		)(true);
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
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_corner		)(6);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		add_child		)(toolbar);

	m_list  = new xui_listview(xui_vector<s32>(100), false);
	xui_method_ptrcall(m_list,		ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_list,		set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_list,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_list,		set_hscrollauto )(false);
	xui_method_ptrcall(m_list,		set_lineheight	)(36);
	xui_method_ptrcall(m_list,		set_iconsize	)(xui_vector<s32>(24));
	xui_method_ptrcall(m_list,		set_itemborder	)(xui_rect2d<s32>(4));
	add_pagectrl(m_head);
	add_pagectrl(m_list);
}

/*
//destructor
*/
xui_delete_explain(onity_console)( void )
{
	for (u08 itype = 0; itype < LOGTYPE_MAX; ++itype)
	{
		xui_vecptr_addloop(m_logvec[itype])
			delete m_logvec[itype][i];
	}
}

/*
//method
*/
xui_method_explain(onity_console, add_log,				void)( u08 type, const std::wstring& file, const std::wstring& text )
{
	std::wstringstream	temp;
	xui_bitmap*			icon = NULL;
	xui_toggle*			togg = NULL;
	switch (type)
	{
	case LOGTYPE_ERROR:		
		togg = m_error;	
		icon = xui_bitmap::create("icon/bigerror.png");
		temp << L"error:";
		break;
	case LOGTYPE_WARNING:	
		togg = m_warning; 
		icon = xui_bitmap::create("icon/bigwarning.png");
		temp << L"warning:";
		break;
	case LOGTYPE_MESSAGE:	
		togg = m_message; 
		icon = xui_bitmap::create("icon/bigmessage.png");
		temp << L"log:";
		break;
	}

	temp << text.c_str();
	temp << L"\n";
	temp << file.c_str();
	xui_listitem* item = new xui_listitem(xui_rect2d<s32>(0), false);
	xui_method_ptrcall(item, ini_drawer		)(icon);
	xui_method_ptrcall(item, ini_drawer		)(temp.str());
	xui_method_ptrcall(item, set_singleline )(false);
	xui_method_ptrcall(item, set_data		)((void*)type);
	if (togg->was_push())
	{
		add_item(type, item);
	}
	else
	{
		m_logvec[type].push_back(item);
	}
}
xui_method_explain(onity_console, del_logall,			void)( void )
{
	m_list->del_itemall();
	for (u08 itype = 0; itype < LOGTYPE_MAX; ++itype)
	{
		xui_vecptr_addloop(m_logvec[itype])
			delete m_logvec[itype][i];

		m_logvec[itype].clear();
	}
}

/*
//callback
*/
//DEBUG
xui_method_explain(onity_console, on_load,				void)( xui_method_args& args )
{
	xui_dockpage::on_load(args);
	add_log(LOGTYPE_MESSAGE,	L"Asset/Action/pet.npModule",		L"module has not texture");
	add_log(LOGTYPE_MESSAGE,	L"Asset/Script/func.lua",			L"print lua function");
	add_log(LOGTYPE_ERROR,		L"Asset/Script/main.lua",			L"lua compiler");
	add_log(LOGTYPE_WARNING,	L"Asset/Script/func.lua",			L"function idle has not call");
	add_log(LOGTYPE_ERROR,		L"Asset/Particle/fire.particle",	L"texture not find");
	add_log(LOGTYPE_MESSAGE,	L"Asset/Action/pet.npModule",		L"module has not texture");
	add_log(LOGTYPE_MESSAGE,	L"Asset/Action/pet.npModule",		L"module has not texture");
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
	xui_toggle* togg = NULL;
	u08         type = 0;

	std::vector<xui_toggle*> vec;
	vec.push_back(m_error  );
	vec.push_back(m_warning);
	vec.push_back(m_message);
	for (u08 i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == sender)
		{
			togg = vec[i];
			type = i;
			break;
		}
	}

	if (togg->was_push())	add_item(type);
	else					del_item(type);
}
xui_method_explain(onity_console, on_clearclick,		void)( xui_component* sender, xui_method_args& args )
{
	del_logall();
}

/*
//method
*/
xui_method_explain(onity_console, add_item,				void)( u08 type, xui_listitem* item )
{
	u32 index = m_list->get_itemcount();
	for (u32 i = 0; i < m_list->get_itemcount(); ++i)
	{
		u08 itemtype = (u08)m_list->get_item(i)->get_data();
		if (itemtype >= type)
		{
			index = i;
			break;
		}
	}

	m_list->add_item(item, index);
}
xui_method_explain(onity_console, add_item,				void)( u08 type )
{
	std::vector<xui_listitem*>& vec = m_logvec[type];
	xui_vecptr_addloop(vec)
	{
		add_item(type, vec[i]);
	}

	vec.clear();
}
xui_method_explain(onity_console, del_item,				void)( u08 type )
{
	for (s32 i = (s32)m_list->get_itemcount()-1; i >= 0; --i)
	{
		xui_listitem* item = m_list->get_item((u32)i);
		if ((u08)item->get_data() == type)
		{
			m_list->del_item(item, false);
			m_logvec[type].push_back(item);
		}
	}
}