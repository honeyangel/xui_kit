#include "xui_droplist.h"

xui_implement_rtti(xui_droplist, xui_listview)

xui_droplist::xui_droplist( const xui_vector<s32>& size, bool drawtick )
: xui_listview(size, drawtick)
{
	xui_action_ctrl_impl<f32>* action = new xui_action_ctrl_impl<f32>(this);
	action->add_time(0.0f);
	action->add_time(0.1f);
	action->add_data(0.2f);
	action->add_data(1.0f);
	action->set_soft(true);
	action->xm_tick += new xui_method_member<xui_method_args, xui_droplist>(this, &xui_droplist::on_popactiontick);
	m_popaction	= action;
}

void xui_droplist::on_popactiontick( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_popaction;
	m_backscale = xui_vector<f32>(1.0f, action->sample());
}