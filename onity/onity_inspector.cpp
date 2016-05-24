#include "NP2DSTransRef.h"

#include "xui_desktop.h"
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
#include "onity_preview.h"
#include "onity_inspector.h"

xui_implement_rtti(onity_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_inspector)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_R)
{
	ini_namectrl(onity_resource::icon_inspector, L"Inspector");

	m_propview	= new xui_propview(xui_vector<s32>(200));
	xui_method_ptrcall(m_propview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_propview,	set_borderrt	)(xui_rect2d<s32>(4));
	m_drawview	= new onity_preview();
	xui_method_ptrcall(m_drawview,	set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor	)(true);
	xui_method_ptrcall(m_drawview,	set_borderrt	)(xui_rect2d<s32>(0, 6, 0, 6));
	xui_method_ptrcall(m_drawview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_drawview,	ini_component	)(true, false);
	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_inspector>(this, &onity_inspector::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor		)(CURSOR_NS);

	add_pagectrl(m_drawview);
	add_pagectrl(m_propview);
	add_pagectrl(m_sizectrl);
}

/*
//method
*/
xui_method_explain(onity_inspector,	get_preview,			onity_preview*	)( void )
{
	return m_drawview;
}
xui_method_explain(onity_inspector, set_proproot,			void			)( xui_proproot* proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot);
	m_drawview->set_needshow();
	refresh();
}
xui_method_explain(onity_inspector, set_proproot,			void			)( const xui_proproot_vec& proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot.size() > 0 ? proproot.front() : NULL);
	m_drawview->set_needshow();
	refresh();
}

/*
//callback
*/
xui_method_explain(onity_inspector, on_perform,				void			)( xui_method_args& args )
{
	xui_dockpage::on_perform(args);
	xui_rect2d<s32> rt = m_propview->get_renderrt()+m_propview->get_renderpt();
	m_sizectrl->on_perform_w(rt.get_w());
	m_sizectrl->on_perform_x(rt.ax);
	m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
}

/*
//event
*/
xui_method_explain(onity_inspector, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch() && m_drawview->was_visible())
	{
		s32 delta  = xui_desktop::get_ins()->get_mousemove().y;
		s32 height = m_drawview->get_renderh() - delta;
		height = xui_max(height, 100);
		height = xui_min(height, 512);
		m_drawview->set_renderh(height);		
	}
}