#include "NP2DSState.h"
#include "NP2DSStateCtrl.h"

#include "xui_bitmap.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_toolbar.h"
#include "onity_resource.h"
#include "onity_state.h"
#include "onity_propcontroller.h"
#include "onity_paramview.h"
#include "onity_stateview.h"
#include "onity_animator.h"

xui_implement_rtti(onity_animator, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_animator)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_A, 200, DOCKSTYLE_F)
, m_editprop(NULL)
{
	ini_namectrl(onity_resource::icon_animator, L"Animator");

	xui_menu* menu1 = xui_menu::create(80);
	m_state	= menu1->add_item(NULL, L"State");
	xui_method_ptrcall(m_state,		xm_click		) += new xui_method_member<xui_method_args, onity_animator>(this, &onity_animator::on_stateclick);
	xui_menu* menu2 = xui_menu::create(80);
	m_on	= menu2->add_item(NULL, L"On");
	m_off	= menu2->add_item(NULL, L"Off");
	xui_method_ptrcall(m_on,		xm_click		) += new xui_method_member<xui_method_args, onity_animator>(this, &onity_animator::on_retargetclick);
	xui_method_ptrcall(m_off,		xm_click		) += new xui_method_member<xui_method_args, onity_animator>(this, &onity_animator::on_retargetclick);

	m_create	= new xui_toggle(xui_vector<s32>( 80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner		)(3);
	xui_method_ptrcall(m_create,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor	)(true);
	xui_method_ptrcall(m_create,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer		)(L"Create");
	xui_method_ptrcall(m_create,	set_menu		)(menu1);

	m_retarget	= new xui_toggle(xui_vector<s32>(128, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_retarget,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_retarget,	set_corner		)(3);
	xui_method_ptrcall(m_retarget,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_retarget,	set_drawcolor	)(true);
	xui_method_ptrcall(m_retarget,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_retarget,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_retarget,	ini_drawer		)(L"All Retarget");
	xui_method_ptrcall(m_retarget,	set_menu		)(menu2);

	m_head		= new xui_toolbar(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor	)(false);
	xui_method_ptrcall(m_head,		set_borderrt	)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_grap		)(4);
	xui_method_ptrcall(m_head,		add_item		)(m_create);
	xui_method_ptrcall(m_head,		add_item		)(m_retarget);

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
}

/*
//reset
*/
xui_method_explain(onity_animator, reset,				void					)( void )
{
	m_paramview->set_editprop(NULL);
	m_stateview->set_editprop(NULL);
	m_paramview->set_editprop(m_editprop);
	m_stateview->set_editprop(m_editprop);
}

/*
//method
*/
xui_method_explain(onity_animator, get_editprop,		onity_propcontroller*	)( void )
{
	return m_editprop;
}
xui_method_explain(onity_animator, set_editprop,		void					)( onity_propcontroller* editprop )
{
	if (m_editprop != editprop)
	{
		if (m_editprop)
		{
			m_editprop->ntf_detach(editprop);
		}

		m_editprop  = editprop;
		m_paramview->set_editprop(m_editprop);
		m_stateview->set_editprop(m_editprop);
	}
}
xui_method_explain(onity_animator, get_stateview,		onity_stateview*		)( void )
{
	return m_stateview;
}
xui_method_explain(onity_animator, get_paramview,		onity_paramview*		)( void )
{
	return m_paramview;
}

/*
//event
*/
xui_method_explain(onity_animator, on_stateclick,		void					)( xui_component* sender, xui_method_args& args )
{
	if (m_editprop)
	{
		xui_vector<s32> pt;
		pt.x = m_stateview->get_renderw()/2 - onity_state::default_width /2;
		pt.y = m_stateview->get_renderh()/2 - onity_state::default_height/2;
		m_stateview->add_state(pt);
	}
}
xui_method_explain(onity_animator, on_retargetclick,	void					)( xui_component* sender, xui_method_args& args )
{
	if (m_editprop)
	{
		bool flag = (sender == m_on);
		const NP2DSStateCtrl::StateVec& vec = m_editprop->get_statectrl()->GetStateVec();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			vec[i]->SetRetarget(flag);
		}
	}
}