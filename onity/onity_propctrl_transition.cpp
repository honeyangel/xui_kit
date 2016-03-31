#include "NP2DSState.h"
#include "NP2DSTransition.h"

#include "xui_convas.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_propview.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "onity_state.h"
#include "onity_propstate.h"
#include "onity_proptransition.h"
#include "onity_propctrl_transition.h"

xui_implement_rtti(onity_propctrl_transition, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_transition)( void )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(L"Transitions");
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip,	set_parent			)(this);
	xui_method_ptrcall(m_nontip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip,	ini_component		)(true, false);
	xui_method_ptrcall(m_nontip,	ini_drawer			)(L"List is Empty");

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	set_enable			)(false);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_transition>(this, &onity_propctrl_transition::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_transition>(this, &onity_propctrl_transition::on_deleterenderself);

	m_sublst = xui_listview::create(xui_vector<s32>(0), false);
	xui_method_ptrcall(m_sublst,	set_parent			)(this);
	xui_method_ptrcall(m_sublst,	set_drawcolor		)(false);
	xui_method_ptrcall(m_sublst,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_sublst,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_sublst,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_transition>(this, &onity_propctrl_transition::on_sublstinvalid);
	xui_method_ptrcall(m_sublst,	xm_selectedchange	) += new xui_method_member<xui_method_args, onity_propctrl_transition>(this, &onity_propctrl_transition::on_sublstselection);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_sublst);
	m_widgetvec.push_back(m_nontip);
}

/*
//static
*/
xui_method_explain(onity_propctrl_transition, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_transition();
}

/*
//override
*/
xui_method_explain(onity_propctrl_transition, on_linkpropdata,		void			)( void )
{
	onity_propdata_transition* datatransition = dynamic_cast<onity_propdata_transition*>(m_propdata);
	onity_state* statectrl = datatransition->get_statectrl();
	const NP2DSState::TransitionVec& vec = statectrl->get_state()->GetTransitionVec();

	xui_method_ptrcall(m_sublst, del_itemall)();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		NP2DSTransition* transition = vec[i];
		std::string text;
		text += transition->GetSelfState()->GetName();
		text += " >> ";
		text += transition->GetNextState()->GetName();
		xui_listitem * item = m_sublst->add_item(xui_global::ascii_to_unicode(text));
		item->set_data(transition);
	}
	xui_method_ptrcall(m_sublst, refresh	)();
	xui_method_ptrcall(m_delete, set_enable	)(false);
	xui_method_ptrcall(m_nontip, set_visible)(vec.size() == 0);
}
xui_method_explain(onity_propctrl_transition, on_editvalue,			void			)( xui_propedit* sender )
{}

/*
//notify
*/
xui_method_explain(onity_propctrl_transition, on_delstate,			void			)( NP2DSState* state )
{
	for (s32 i = (s32)m_sublst->get_itemcount()-1; i >= 0; --i)
	{
		xui_listitem* item = m_sublst->get_item((u32)i);
		NP2DSTransition* transition = (NP2DSTransition*)item->get_data();
		if (transition->GetNextState() == state)
			m_sublst->del_item(item);
	}

	xui_method_ptrcall(m_sublst, refresh	)();
	xui_method_ptrcall(m_delete, set_enable	)(m_sublst->get_selecteditem().size() > 0);
	xui_method_ptrcall(m_nontip, set_visible)(m_sublst->get_itemcount() == 0);
}

/*
//callback
*/
xui_method_explain(onity_propctrl_transition, on_renderback,		void			)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_sublst->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_delete->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_sublst->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-24,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_delete->get_renderrtabs();
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
xui_method_explain(onity_propctrl_transition, on_invalid,			void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_delete->get_renderh();
	sz.h += m_sublst->get_renderh();
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_transition, on_perform,			void			)( xui_method_args& args )
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
	m_sublst->on_perform_x (pt.x+indent);
	m_sublst->on_perform_y (pt.y);
	m_sublst->on_perform_w (rt.get_w()-indent);
	m_nontip->on_perform_x (pt.x+indent);
	m_nontip->on_perform_y (pt.y+m_sublst->get_borderrt().ay);
	m_nontip->on_perform_w (rt.get_w()-indent);
	//delete
	pt.x  = rt.bx - m_delete->get_renderw();
	pt.y += m_sublst->get_renderh();
	m_delete->on_perform_x (pt.x);
	m_delete->on_perform_y (pt.y);
}

/*
//event
*/
xui_method_explain(onity_propctrl_transition, on_sublstinvalid,		void			)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_sublst->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_sublst->get_itemcount() * m_sublst->get_lineheight());
	h += m_sublst->get_borderrt().ay;
	h += m_sublst->get_borderrt().by;
	m_sublst->set_renderh(h);
}
xui_method_explain(onity_propctrl_transition, on_sublstselection,	void			)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_listitem*> vec = m_sublst->get_selecteditem();
	m_delete->set_enable(vec.size() > 0);

	xui_propview* propview = get_propview();
	propview->del_propelse();
	if (vec.size() > 0)
	{
		xui_listitem* item = vec.front();
		NP2DSTransition* transition = (NP2DSTransition*)item->get_data();

		onity_propdata_transition* datatransition = dynamic_cast<onity_propdata_transition*>(m_propdata);
		onity_state* statectrl = datatransition->get_statectrl();
		onity_proptransition* proptransition = statectrl->get_propstate()->get_proptransition(transition);

		propview->add_propelse(proptransition);
	}
}
xui_method_explain(onity_propctrl_transition, on_deleterenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax + rt.get_w()/2, rt.ay + rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_propctrl_transition, on_deleteclick,		void			)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_listitem*> vec = m_sublst->get_selecteditem();
	if (vec.size() > 0)
	{
		onity_propdata_transition* datatransition = dynamic_cast<onity_propdata_transition*>(m_propdata);

		NP2DSTransition* transition = (NP2DSTransition*)vec[0]->get_data();
		onity_state*	 statectrl  = datatransition->get_statectrl();
		statectrl->get_propstate()->del_transition(transition);
		statectrl->get_state()->DelTransition(transition);

		on_linkpropdata();
	}
}