#include "NP2DSCondition.h"
#include "NP2DSParam.h"
#include "NP2DSTransition.h"
#include "NP2DSState.h"
#include "NP2DSStateCtrl.h"

#include "xui_convas.h"
#include "xui_drawer.h"
#include "xui_panel.h"
#include "xui_button.h"
#include "onity_condition.h"
#include "onity_propctrl_condition.h"

xui_implement_rtti(onity_propctrl_condition, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_condition)( void )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(L"Conditions");
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip,	set_parent			)(this);
	xui_method_ptrcall(m_nontip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip,	ini_component		)(true, false);
	xui_method_ptrcall(m_nontip,	ini_drawer			)(L"When play over");

	m_insert = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_insert,	set_parent			)(this);
	xui_method_ptrcall(m_insert,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_insert,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_insert,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_insertclick);
	xui_method_ptrcall(m_insert,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_insertrenderself);

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_deleterenderself);

	m_middle = new xui_panel(xui_vector<s32>(0));
	xui_method_ptrcall(m_middle,	set_parent			)(this);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_middle,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_middle,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_middleinvalid);
	xui_method_ptrcall(m_middle,	xm_perform			) += new xui_method_member<xui_method_args, onity_propctrl_condition>(this, &onity_propctrl_condition::on_middleperform);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
}

/*
//static
*/
xui_method_explain(onity_propctrl_condition, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_condition;
}

/*
//override
*/
xui_method_explain(onity_propctrl_condition, on_linkpropdata,		void			)( void )
{
	onity_propdata_condition* datacondition = dynamic_cast<onity_propdata_condition*>(m_propdata);
	NP2DSTransition* transition = datacondition->get_transition();
	const NP2DSTransition::ConditionVec& vec = transition->GetConditionVec();

	xui_method_ptrcall(m_middle, del_children	)();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		m_middle->add_child(new onity_condition(vec[i]));
	}
	xui_method_ptrcall(m_middle, refresh		)();
	xui_method_ptrcall(m_insert, set_enable		)(transition->GetSelfState()->GetStateCtrl()->GetParamVec().size() > 0);
	xui_method_ptrcall(m_delete, set_enable		)(vec.size() >  0);
	xui_method_ptrcall(m_nontip, set_visible	)(vec.size() == 0);
}
xui_method_explain(onity_propctrl_condition, on_editvalue,			void			)( xui_propedit* sender )
{

}

/*
//method
*/
//xui_method_explain(onity_propctrl_condition, del_paramlink,			void			)( NP2DSParam* param )
//{
//
//}

/*
//callback
*/
xui_method_explain(onity_propctrl_condition, on_renderback,			void			)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-48,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by-3	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-3,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.bx-6, 
		rt.by-6,			
		rt.bx,		   
		rt.by), color,  0, 90, 1);
}
xui_method_explain(onity_propctrl_condition, on_invalid,			void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_insert->get_renderh();
	sz.h += m_middle->get_renderh();
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_condition, on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x (pt.x+indent);
	m_header->on_perform_y (pt.y);
	m_header->on_perform_w (rt.get_w()-indent);
	//listview
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x (pt.x+indent);
	m_middle->on_perform_y (pt.y);
	m_middle->on_perform_w (rt.get_w()-indent);
	m_nontip->on_perform_x (pt.x+indent);
	m_nontip->on_perform_y (pt.y+m_middle->get_borderrt().ay);
	m_nontip->on_perform_w (rt.get_w()-indent);
	//delete
	pt.x  = rt.bx - m_delete->get_renderw();
	pt.y += m_middle->get_renderh();
	m_delete->on_perform_x (pt.x);
	m_delete->on_perform_y (pt.y);
	//insert
	pt.x -= m_insert->get_renderw();
	m_insert->on_perform_x (pt.x);
	m_insert->on_perform_y (pt.y);
}

/*
//event
*/
xui_method_explain(onity_propctrl_condition, on_middleinvalid,		void			)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_childcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_childcount() * onity_condition::default_height);
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(onity_propctrl_condition, on_middleperform,		void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_middle->get_renderrtins();
	xui_rect2d<s32> bd = m_middle->get_borderrt();
	xui_vector<s32> pt = bd.get_pt();

	const std::vector<xui_control*>& vec = m_middle->get_children();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		vec[i]->on_perform_pt(pt);
		vec[i]->on_perform_w (rt.get_w());
		pt.y += onity_condition::default_height;
	}
}
xui_method_explain(onity_propctrl_condition, on_deleteclick,		void			)( xui_component* sender, xui_method_args& args )
{
	u32 childcount = m_middle->get_childcount();
	if (childcount > 0)
	{
		onity_propdata_condition* datacondition = dynamic_cast<onity_propdata_condition*>(m_propdata);
		NP2DSTransition* transition = datacondition->get_transition();

		onity_condition* ctrl = xui_dynamic_cast(onity_condition, m_middle->get_child(childcount-1));
		transition->DelCondition(ctrl->get_condition());
		xui_method_ptrcall(m_middle, del_child		)(ctrl);
		xui_method_ptrcall(m_middle, refresh		)();
		xui_method_ptrcall(m_delete, set_enable		)(m_middle->get_childcount() >  0);
		xui_method_ptrcall(m_nontip, set_visible	)(m_middle->get_childcount() == 0);
	}
}
xui_method_explain(onity_propctrl_condition, on_deleterenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_propctrl_condition, on_insertclick,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_propdata_condition* datacondition = dynamic_cast<onity_propdata_condition*>(m_propdata);
	NP2DSTransition* transition = datacondition->get_transition();
	NP2DSStateCtrl* stateCtrl = transition->GetSelfState()->GetStateCtrl();
	const NP2DSStateCtrl::ParamVec& vec = stateCtrl->GetParamVec();
	if (vec.size() > 0)
	{
		NP2DSCondition* condition = transition->AddCondition(vec[0]);
		xui_method_ptrcall(m_middle, add_child		)(new onity_condition(condition));
		xui_method_ptrcall(m_middle, refresh		)();
		xui_method_ptrcall(m_delete, set_enable		)(true);
		xui_method_ptrcall(m_nontip, set_visible	)(false);
	}
}
xui_method_explain(onity_propctrl_condition, on_insertrenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}