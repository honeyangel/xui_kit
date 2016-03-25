#include "NP2DSStateCtrl.h"

#include "xui_bitmap.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "onity_resource.h"
#include "onity_state.h"
#include "onity_paramview.h"
#include "onity_stateview.h"
#include "onity_animator.h"

xui_implement_rtti(onity_animator, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_animator)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_editfile(NULL)
{
	ini_namectrl(onity_resource::icon_animator, L"Animator");

	xui_menu* menu = xui_menu::create(80);
	m_state	= menu->add_item(NULL, L"State");
	xui_method_ptrcall(m_state,		xm_click		) += new xui_method_member<xui_method_args, onity_animator>(this, &onity_animator::on_stateclick);

	m_create	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner		)(3);
	xui_method_ptrcall(m_create,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor	)(true);
	xui_method_ptrcall(m_create,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer		)(L"Create");
	xui_method_ptrcall(m_create,	set_menu		)(menu);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto	)(false);
	xui_method_ptrcall(m_head,		add_child		)(m_create);

	m_stateview = new onity_stateview;
	xui_method_ptrcall(m_stateview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_stateview, set_drawcolor	)(true);
	xui_method_ptrcall(m_stateview, set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_stateview,	set_sidecolor	)(xui_colour::black);
	xui_method_ptrcall(m_stateview,	set_sidestyle	)(SIDESTYLE_S);

	m_paramview = new onity_paramview;
	xui_method_ptrcall(m_paramview,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_paramview, set_drawcolor	)(true);
	xui_method_ptrcall(m_paramview, set_backcolor	)(xui_colour(1.0f, 0.25f));
	xui_method_ptrcall(m_paramview,	set_sidecolor	)(xui_colour::white);
	xui_method_ptrcall(m_paramview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_paramview, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_paramview, set_corner		)(5);

	add_pagectrl(m_head);
	add_pagectrl(m_stateview);
	add_pagectrl(m_paramview);

	//DEBUG
	set_editfile(new NP2DSStateCtrl);
}

/*
//method
*/
xui_method_explain(onity_animator, get_editfile,	NP2DSStateCtrl*)( void )
{
	return m_editfile;
}
xui_method_explain(onity_animator, set_editfile,	void			)( NP2DSStateCtrl* editfile )
{
	if (m_editfile != editfile)
	{
		m_editfile  = editfile;
		m_paramview->set_editfile(m_editfile);
		m_stateview->set_editfile(m_editfile);
	}
}
xui_method_explain(onity_animator, del_param,		void			)( NP2DSParam* param )
{
	xui_method_ptrcall(m_paramview, del_paramctrl)(param);
	xui_method_ptrcall(m_stateview, del_paramlink)(param);
	m_editfile->DelParam(param);
}
xui_method_explain(onity_animator, del_state,		void			)( NP2DSState* state )
{
	xui_method_ptrcall(m_stateview,	del_statectrl)(state);
	xui_method_ptrcall(m_stateview, del_statelink)(state);
	m_editfile->DelState(state);
}

/*
//event
*/
xui_method_explain(onity_animator, on_stateclick,	void			)( xui_component* sender, xui_method_args& args )
{
	if (m_editfile)
	{
		xui_vector<s32> pt;
		pt.x = m_stateview->get_renderw()/2 - onity_state::default_width /2;
		pt.y = m_stateview->get_renderh()/2 - onity_state::default_height/2;
		m_stateview->add_state(pt);
	}
}