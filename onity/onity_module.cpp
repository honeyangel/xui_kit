#include "NP2DSImageFile.h"
#include "NP2DSImage.h"

#include "xui_convas.h"
#include "xui_button.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "xui_panel.h"
#include "xui_timer.h"
#include "xui_timermgr.h"
#include "onity_resource.h"
#include "onity_project.h"
#include "onity_propmodule.h"
#include "onity_propimage.h"
#include "onity_module.h"

xui_implement_rtti(onity_module, onity_asset);

/*
//constructor
*/
xui_create_explain(onity_module)( void )
: onity_asset()
{
	ini_namectrl(onity_resource::icon_scene, L"Module");

	m_automode	= new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_automode,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_automode,	set_corner		)(3);
	xui_method_ptrcall(m_automode,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_automode,	set_drawcolor	)(true);
	xui_method_ptrcall(m_automode,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_automode,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_automode,	ini_drawer		)(L"Auto Mode");
	xui_method_ptrcall(m_automode,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_module>(this, &onity_module::on_buttonclick);

	m_addtimer	= xui_timermgr::get_ins()->add_timer(this, 0.1f, NULL);
	xui_method_ptrcall(m_addtimer,	xm_tick			) += new xui_method_member<xui_method_args,	onity_module>(this, &onity_module::on_timertick);

	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_module)( void )
{
	xui_timermgr::get_ins()->del_timer(m_addtimer);
}

/*
//method
*/
xui_method_explain(onity_module, get_editprop,			onity_propmodule*	)( void )
{
	return m_editprop;
}
xui_method_explain(onity_module, set_editprop,			void				)( onity_propmodule* prop )
{

}
xui_method_explain(onity_module, hit_propvisible,		onity_proproot*		)( const xui_vector<s32>& pt )
{
	return NULL;
}

/*
//event
*/
xui_method_explain(onity_module, on_buttonclick,		void				)( xui_component* sender, xui_method_args& args )
{
	onity_asset::on_buttonclick(sender, args);
	if (sender == m_automode)
	{

	}
}
xui_method_explain(onity_module, on_drawviewrenderself, void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_module, on_drawviewrenderelse, void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_module, on_timertick,			void				)( xui_component* sender, xui_method_args& args )
{

}

/*
//override
*/
xui_method_explain(onity_module, on_keybdmoveimpl,		void				)( const xui_vector<s32>& delta )
{

}
xui_method_explain(onity_module, on_mousepickimpl,		void				)( onity_proproot* pick, bool alt, bool ctrl, bool shift )
{

}
xui_method_explain(onity_module, on_mousedragimpl,		void				)( const xui_vector<s32>& delta )
{

}
xui_method_explain(onity_module, on_mulselectimpl,		void				)( const xui_rect2d<s32>& rt, bool ctrl )
{

}