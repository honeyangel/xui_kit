#include "TransformComponent.h"
#include "NP2DSTransRef.h"

#include "xui_bitmap.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_propkind.h"
#include "xui_propdata.h"
#include "xui_proproot.h"
#include "xui_propview.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "xui_propctrl_object.h"
#include "xui_propctrl_slider.h"
#include "onity_resource.h"
#include "onity_inspector.h"

xui_implement_rtti(onity_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_inspector)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_R)
{
	ini_namectrl(onity_resource::icon_inspector, L"Inspector");

	m_view = new xui_propview(xui_vector<s32>(0));
	xui_method_ptrcall(m_view, ini_component)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view, set_borderrt )(xui_rect2d<s32>(4));
	add_pagectrl(m_view);
}

/*
//method
*/
xui_method_explain(onity_inspector, set_proproot, void	)( xui_proproot* proproot )
{
	m_view->set_proproot(proproot);
}
xui_method_explain(onity_inspector, set_proproot, void	)( const xui_proproot_vec& proproot )
{
	m_view->set_proproot(proproot);
}