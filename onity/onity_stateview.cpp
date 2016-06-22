#include "NPVector3.h"
#include "NPQuaternion.h"
#include "NP2DSState.h"
#include "NP2DSParam.h"
#include "NP2DSTransition.h"
#include "NP2DSStateCtrl.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_animator.h"
#include "onity_state.h"
#include "onity_propstate.h"
#include "onity_proptransition.h"
#include "onity_filedata.h"
#include "onity_propcontroller.h"
#include "onity_stateview.h"

xui_implement_rtti(onity_stateview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_stateview)( void )
: xui_control(xui_vector<s32>(0))
, m_mousedrag(false)
, m_dragvalue(xui_vector<s32>(0))
, m_maketrans(false)
, m_actstate(NULL)
, m_selstate(NULL)
, m_seltrans(NULL)
, m_editprop(NULL)
{
	m_menu			= xui_menu::create(80);
	m_create		= m_menu->add_item(NULL, L"Create State");
	m_transition	= m_menu->add_item(NULL, L"Make Transition");
	m_default		= m_menu->add_item(NULL, L"Set As Default");
	m_delete		= m_menu->add_item(NULL, L"Delete");

	xui_method_ptrcall(m_create,	 xm_click) += new xui_method_member<xui_method_args, onity_stateview>(this, &onity_stateview::on_createclick);
	xui_method_ptrcall(m_transition, xm_click) += new xui_method_member<xui_method_args, onity_stateview>(this, &onity_stateview::on_transitionclick);
	xui_method_ptrcall(m_default,	 xm_click) += new xui_method_member<xui_method_args, onity_stateview>(this, &onity_stateview::on_defaultclick);
	xui_method_ptrcall(m_delete,	 xm_click) += new xui_method_member<xui_method_args, onity_stateview>(this, &onity_stateview::on_deleteclick);
}

/*
//destructor
*/
xui_delete_explain(onity_stateview)( void )
{
	delete m_menu;
}

/*
//method
*/
xui_method_explain(onity_stateview, set_editprop,		void			)( onity_propcontroller* editprop )
{
	if (m_editprop != editprop)
	{
		del_statectrlall();
		m_editprop  = editprop;
		m_actstate  = NULL;
		m_selstate  = NULL;
		m_seltrans  = NULL;
		if (m_editprop)
		{
			const NP2DSStateCtrl::StateVec& vec = m_editprop->get_statectrl()->GetStateVec();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				NPVector2 position = vec[i]->GetPosition();
				add_statectrl(vec[i], xui_vector<s32>((s32)position.x, (s32)position.y));
			}
		}
	}
}
xui_method_explain(onity_stateview, add_state,			void			)( const xui_vector<s32>& pt )
{
	std::stringstream temp("New State");
	if (m_editprop->get_statectrl()->HasState(temp.str()))
	{
		s32 number = 0;
		while (true)
		{
			temp.str("");
			temp << "New State";
			temp << number;
			if (m_editprop->get_statectrl()->HasState(temp.str()) == false)
				break;

			++number;
		}
	}

	add_statectrl(m_editprop->get_statectrl()->AddState(temp.str()), pt);
}

xui_method_explain(onity_stateview, add_statectrl,		void			)( NP2DSState* state, const xui_vector<s32>& pt )
{
	xui_vector<s32> finalpt = (pt+m_dragvalue)*10/10;
	onity_state*  statectrl = new onity_state(m_editprop, state);
	xui_method_ptrcall(statectrl, set_parent	)(this);
	xui_method_ptrcall(statectrl, set_drawcolor	)(true);
	xui_method_ptrcall(statectrl, set_backcolor	)(xui_colour(1.0f, 0.25f));
	xui_method_ptrcall(statectrl, set_sidecolor	)(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	xui_method_ptrcall(statectrl, set_corner	)(5);
	xui_method_ptrcall(statectrl, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(statectrl, set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(statectrl, set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(statectrl, set_singleline)(false);
	xui_method_ptrcall(statectrl, set_scrollpt	)(m_dragvalue);
	xui_method_ptrcall(statectrl, ini_drawer	)(xui_global::ascii_to_unicode(state->GetName()));
	xui_method_ptrcall(statectrl, on_perform_pt	)(finalpt);
	xui_method_ptrcall(statectrl, xm_mousedown	) += new xui_method_member<xui_method_mouse, onity_stateview>(this, &onity_stateview::on_statemousedown);
	xui_method_ptrcall(statectrl, xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_stateview>(this, &onity_stateview::on_statemouserise);
	m_widgetvec.push_back(statectrl);
	m_statectrl.push_back(statectrl);
	invalid();
}
xui_method_explain(onity_stateview, get_statectrl,		onity_state*	)( NP2DSState* state )
{
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		if (m_statectrl[i]->get_state() == state)
			return m_statectrl[i];
	}

	return NULL;
}
xui_method_explain(onity_stateview, get_statectrl,		onity_state*	)( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	xui_vector<s32> relative = pt + m_dragvalue;
	xui_vecptr_delloop(m_statectrl)
	{
		if (component = m_statectrl[i]->choose(relative))
			return xui_dynamic_cast(onity_state, component);
	}

	return NULL;
}
xui_method_explain(onity_stateview, del_statectrl,		void			)( NP2DSState* state )
{
	if (m_seltrans && m_seltrans->GetSelfState() == state)
		m_seltrans  = NULL;

	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		onity_state* statectrl = m_statectrl[i];
		if (statectrl->get_state() == state)
		{
			if (m_actstate == statectrl)
				m_actstate  = NULL;
			if (m_selstate == statectrl)
				m_selstate  = NULL;

			std::vector<xui_component*>::iterator itor = std::find(
				m_widgetvec.begin(), 
				m_widgetvec.end(), 
				statectrl);

			if (itor != m_widgetvec.end())
			{
				m_widgetvec.erase(itor);
				statectrl->set_parent(NULL);
				xui_desktop::get_ins()->move_recycle(statectrl);
			}

			m_statectrl.erase(m_statectrl.begin()+i);
			invalid();
			break;
		}
	}
}
xui_method_explain(onity_stateview, del_statectrlall,	void			)( void )
{
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		onity_state* statectrl = m_statectrl[i];
		std::vector<xui_component*>::iterator itor = std::find(
			m_widgetvec.begin(), 
			m_widgetvec.end(), 
			statectrl);

		if (itor != m_widgetvec.end())
		{
			m_widgetvec.erase(itor);
			statectrl->set_parent(NULL);
			xui_desktop::get_ins()->move_recycle(statectrl);
		}
	}

	m_statectrl.clear();
	m_actstate = NULL;
	m_selstate = NULL;
	m_seltrans = NULL;
	invalid();
}

/*
//notify
*/
xui_method_explain(onity_stateview, on_delstate,		void			)( NP2DSState* state )
{
	if (m_seltrans && m_seltrans->GetNextState() == state)
		m_seltrans  = NULL;

	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		m_statectrl[i]->on_delstate(state);
	}
}
xui_method_explain(onity_stateview, on_delparam,		void			)( NP2DSParam* param )
{
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		m_statectrl[i]->on_delparam(param);
	}
}
xui_method_explain(onity_stateview, on_addparam,		void			)( NP2DSParam* param )
{
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		m_statectrl[i]->on_addparam(param);
	}
}

/*
//select
*/
xui_method_explain(onity_stateview, sel_statectrl,		void			)( onity_state* statectrl )
{
	if (m_selstate != statectrl)
	{
		if (m_selstate)
			m_selstate->set_sidestyle(SIDESTYLE_N);
		m_selstate  = statectrl;
		if (m_selstate)
			m_selstate->set_sidestyle(SIDESTYLE_S);
	}

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	if (m_selstate)
	{
		m_seltrans = NULL;
		inspector->set_proproot(m_selstate->get_propstate());
	}
}
xui_method_explain(onity_stateview, sel_transprop,		void			)( const xui_vector<s32>& pt )
{
	NP2DSTransition* transition = NULL;
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		transition = find_transition(m_statectrl[i], pt);
		if (transition)
			break;
	}

	if (m_seltrans != transition)
	{
		m_seltrans  = transition;
	}

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	if (m_seltrans)
	{
		onity_state* statectrl = get_statectrl(m_seltrans->GetSelfState());
		inspector->set_proproot(statectrl->get_propstate()->get_proptransition(m_seltrans));
	}
}

/*
//override
*/
xui_method_explain(onity_stateview, choose_else,		xui_component*	)( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
	if (rt.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt() + m_dragvalue;
		xui_vecptr_delloop(m_statectrl)
		{
			if (component = m_statectrl[i]->choose(relative))
				return component;
		}
	}

	return component;
}

/*
//callback
*/
xui_method_explain(onity_stateview, on_noncatch,		void			)( xui_method_args&  args )
{
	xui_control::on_noncatch(args);
	m_mousedrag = false;
}
xui_method_explain(onity_stateview, on_invalid,			void			)( xui_method_args&  args )
{
	drag_view(xui_vector<s32>(0));
}
xui_method_explain(onity_stateview, on_renderself,		void			)( xui_method_args&  args )
{
	xui_control::on_renderself(args);

	xui_vector<s32> pt = get_screenpt();
	xui_vector<s32> p1;
	xui_vector<s32> p2;

	s32 value;
	s32 start;
	s32 final;

	value = m_dragvalue.x;
	start = value / 10;
	final = start + get_renderw() / 10 + 1;
	for (s32 i = start; i <= final; ++i)
	{
		if (i%10 == 0)
			continue;

		p1 = pt + xui_vector<s32>(i *10 - value, 0);
		p2 = pt + xui_vector<s32>(i *10 - value, get_renderh());
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour(1.0f, 0.1f));
	}
	value = m_dragvalue.y;
	start = value / 10;
	final = start + get_renderh() / 10 + 1;
	for (s32 i = start; i <= final; ++i)
	{
		if (i%10 == 0)
			continue;

		p1 = pt + xui_vector<s32>(0,			 i* 10 - value);
		p2 = pt + xui_vector<s32>(get_renderw(), i* 10 - value);
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour(1.0f, 0.1f));
	}

	value = m_dragvalue.x;
	start = value / 100;
	final = start + get_renderw() / 100 + 1;
	for (s32 i = start; i <= final; ++i)
	{
		p1 = pt + xui_vector<s32>(i *100 - value, 0);
		p2 = pt + xui_vector<s32>(i *100 - value, get_renderh());
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour::black);
	}
	value = m_dragvalue.y;
	start = value / 100;
	final = start + get_renderh() / 100 + 1;
	for (s32 i = start; i <= final; ++i)
	{
		p1 = pt + xui_vector<s32>(0,			 i* 100 - value);
		p2 = pt + xui_vector<s32>(get_renderw(), i* 100 - value);
		xui_convas::get_ins()->draw_line(p1, p2, xui_colour::black);
	}


	for (std::vector<onity_state*>::iterator itor = m_statectrl.begin(); itor != m_statectrl.end(); ++itor)
	{
		NP2DSState* state = (*itor)->get_state();
		const NP2DSState::TransitionVec& vec = state->GetTransitionVec();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			onity_state* nextstate = get_statectrl(vec[i]->GetNextState());
			if (nextstate)
			{
				xui_colour color = (vec[i] != m_seltrans) ? xui_colour::white : xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
				draw_transition((*itor), nextstate, color);
			}
		}
	}
	if (m_maketrans && m_actstate)
	{
		xui_vector<s32> pt = get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		onity_state* nextstate = get_statectrl(pt);
		draw_transition(m_actstate, nextstate, xui_colour::white);
	}
}
xui_method_explain(onity_stateview, on_keybddown,		void			)( xui_method_keybd& args )
{
	xui_control::on_keybddown(args);
	if (args.kcode == KEY_ESC)
	{
		m_maketrans = false;
	}
	else 
	if (args.kcode == KEY_DELETE && m_seltrans)
	{
		onity_state* statectrl = get_statectrl(m_seltrans->GetSelfState());
		statectrl->get_propstate()->del_transition(m_seltrans);
		statectrl->get_state()->DelTransition(m_seltrans);
		m_seltrans = NULL;
	}
}
xui_method_explain(onity_stateview, on_mousedown,		void			)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	m_maketrans = false;

	if (args.mouse == MB_L)
	{
		sel_statectrl(NULL);
		sel_transprop(args.point);
	}

	if (args.mouse == MB_M)
	{
		m_mousedrag = true;
		xui_desktop::get_ins()->set_catchctrl(this);
	}
}
xui_method_explain(onity_stateview, on_mousemove,		void			)( xui_method_mouse& args )
{
	xui_control::on_mousemove(args);
	if (m_mousedrag)
	{
		drag_view(xui_desktop::get_ins()->get_mousemove());
	}
}
xui_method_explain(onity_stateview, on_mouserise,		void			)( xui_method_mouse& args )
{
	xui_control::on_mouserise(args);
	if (args.mouse == MB_M)
	{
		xui_desktop::get_ins()->set_catchctrl(NULL);	
	}

	if (args.mouse == MB_R)
	{
		show_menu(NULL);
	}
}

/*
//event
*/
xui_method_explain(onity_stateview, on_createclick,		void			)( xui_component* sender, xui_method_args&  args )
{
	add_state(m_menu->get_renderpt() - get_screenpt());
}
xui_method_explain(onity_stateview, on_defaultclick,	void			)( xui_component* sender, xui_method_args&  args )
{
	if (m_actstate)
	{
		m_editprop->get_statectrl()->SetDefState(m_actstate->get_state());
	}
}
xui_method_explain(onity_stateview, on_transitionclick, void			)( xui_component* sender, xui_method_args&  args )
{
	m_maketrans = true;
}
xui_method_explain(onity_stateview, on_deleteclick,		void			)( xui_component* sender, xui_method_args&  args )
{
	if (m_actstate)
	{
		NP2DSState* state = m_actstate->get_state();
		del_statectrl(state);
		on_delstate  (state);
		m_editprop->get_statectrl()->DelState(state);
	}
}
xui_method_explain(onity_stateview, on_statemousedown,	void			)( xui_component* sender, xui_method_mouse& args )
{
	onity_state* statectrl = xui_dynamic_cast(onity_state, sender);
	if (args.mouse == MB_L)
	{
		if (m_maketrans && m_actstate)
		{
			NP2DSTransition* transition = m_actstate->get_state()->AddTransition(statectrl->get_state());
			m_actstate->get_propstate()->add_transition(transition);
		}
		else
		{
			sel_statectrl(statectrl);
		}
	}
	if (args.mouse == MB_M)
	{
		m_mousedrag = true;
		xui_desktop::get_ins()->set_catchctrl(this);
	}

	m_maketrans = false;
}
xui_method_explain(onity_stateview, on_statemouserise,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_R)
	{
		show_menu(xui_dynamic_cast(onity_state, sender));
	}
}

/*
//method
*/
xui_method_explain(onity_stateview, show_menu,			void			)( onity_state* statectrl )
{
	m_actstate = statectrl;
	xui_method_ptrcall(m_create,		set_enable)(statectrl == NULL && m_editprop != NULL);
	xui_method_ptrcall(m_transition,	set_enable)(statectrl != NULL);
	xui_method_ptrcall(m_default,		set_enable)(statectrl != NULL);
	xui_method_ptrcall(m_delete,		set_enable)(statectrl != NULL);

	xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
	xui_method_ptrcall(m_menu, refresh			)();
	xui_method_ptrcall(m_menu, set_renderpt		)(pt);
	xui_method_ptrcall(m_menu, set_showsubmenu	)(NULL);
	xui_method_ptrcall(m_menu, req_focus		)();
	xui_desktop::get_ins()->set_floatctrl(m_menu);
}
xui_method_explain(onity_stateview, drag_view,			void			)( const xui_vector<s32>& delta )
{
	s32 llimit = 0;
	s32 rlimit = 0;
	s32 tlimit = 0;
	s32 blimit = 0;
	if (m_statectrl.size() > 0)
	{
		xui_rect2d<s32> rt = m_statectrl[0]->get_renderrt() + m_statectrl[0]->get_renderpt();
		rlimit = rt.bx - get_renderw();
		blimit = rt.by - get_renderh();
		llimit = rt.ax;
		tlimit = rt.ay;
		for (u32 i = 1; i < m_statectrl.size(); ++i)
		{
			xui_rect2d<s32> rt = m_statectrl[i]->get_renderrt() + m_statectrl[i]->get_renderpt();
			rlimit = xui_max(rlimit, rt.bx - get_renderw());
			blimit = xui_max(blimit, rt.by - get_renderh());
			llimit = xui_min(llimit, rt.ax);
			tlimit = xui_min(tlimit, rt.ay);
		}
	}

	xui_vector<s32> minvalue;
	xui_vector<s32> maxvalue;
	minvalue.x = xui_min(llimit, rlimit);
	minvalue.y = xui_min(tlimit, blimit);
	maxvalue.x = xui_max(llimit, rlimit);
	maxvalue.y = xui_max(tlimit, blimit);

	m_dragvalue  -= delta;
	m_dragvalue.x = xui_max(m_dragvalue.x, minvalue.x);
	m_dragvalue.y = xui_max(m_dragvalue.y, minvalue.y);
	m_dragvalue.x = xui_min(m_dragvalue.x, maxvalue.x);
	m_dragvalue.y = xui_min(m_dragvalue.y, maxvalue.y);
	for (u32 i = 0; i < m_statectrl.size(); ++i)
	{
		m_statectrl[i]->set_scrollpt(m_dragvalue);
	}
}
xui_method_explain(onity_stateview, find_transition,	NP2DSTransition*)( onity_state* selfstate, const xui_vector<s32>& pt )
{
	const NP2DSState::TransitionVec& vec = selfstate->get_state()->GetTransitionVec();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_state* nextstate	= get_statectrl(vec[i]->GetNextState());
		xui_rect2d<s32> selfrt	= selfstate->get_renderrtabs();
		xui_rect2d<s32> nextrt	= nextstate->get_renderrtabs();
		xui_vector<s32> self	= xui_vector<s32>(selfrt.ax+selfrt.get_w()/2, selfrt.ay+selfrt.get_h()/2);
		xui_vector<s32> next	= xui_vector<s32>(nextrt.ax+nextrt.get_w()/2, nextrt.ay+nextrt.get_h()/2);

		xui_vector<s32> offset	= calc_offset(self, next);
		self += offset;
		next += offset;
		xui_vector<s32> normal	= next - self;

		s32 d = (s32)sqrtl((f64)(normal.x*normal.x) + (f64)(normal.y*normal.y));
		f32 radian = 0.0f;
		NPVector3 v1((f32)normal.x, (f32)normal.y, 0.0f);
		NPVector3 v2(1.0f,          0.0f,          0.0f);
		if (v1 != NPVector3::Zero)
		{
			radian = v1.GetAngle(v2);
			if (v1.GetCross(v2).z > 0.0f)
				radian *= -1.0f;
		}

		NPVector2 v((f32)(pt.x-self.x), (f32)(pt.y-self.y));
		NPQuaternion q;
		q.SetRotationZ(-radian);
		v = q*v;
		if (v.x >=  0.0f && v.x <= d &&
			v.y >= -8.0f && v.y <= 8.0f)
			return vec[i];
	}

	return NULL;
}
xui_method_explain(onity_stateview, draw_transition,	void			)( onity_state* selfstate, onity_state* nextstate, const xui_colour& color )
{
	xui_rect2d<s32> rt		= selfstate->get_renderrtabs();
	xui_vector<s32> self	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_vector<s32> next	= xui_desktop::get_ins()->get_mousecurr();
	if (nextstate)
	{
		rt		= nextstate->get_renderrtabs();
		next	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	}

	xui_vector<s32> offset = calc_offset(self, next);
	draw_transition(self+offset, (nextstate == NULL) ? next : next+offset, color);
}
xui_method_explain(onity_stateview, draw_transition,	void			)( const xui_vector<s32>& self, const xui_vector<s32>& next, const xui_colour& color )
{
	xui_vector<s32> normal = next - self;

	s32 d = (s32)sqrtl((f64)(normal.x*normal.x) + (f64)(normal.y*normal.y));
	xui_rect2d<s32> rt;
	rt.ax =  0;
	rt.ay = -1;
	rt.bx =  d;
	rt.by =  1;
	f32 angle = 0.0f;
	NPVector3 v1((f32)normal.x, (f32)normal.y, 0.0f);
	NPVector3 v2(1.0f,          0.0f,          0.0f);
	if (v1 != NPVector3::Zero)
	{
		angle = v1.GetAngle(v2);
		if (v1.GetCross(v2).z > 0.0f)
			angle *= -1.0f;
	}

	xui_convas::get_ins()->set_transform(angle, xui_vector<f32>(1.0f), self.to<f32>());
	xui_convas::get_ins()->fill_rectangle(rt, color, true);
	xui_vector<s32> temp = self + normal/2;
	xui_convas::get_ins()->set_transform(angle, xui_vector<f32>(1.0f), temp.to<f32>());
	xui_convas::get_ins()->fill_triangle(xui_vector<s32>(0), 3, TRIANGLE_RIGHT, color);
	xui_convas::get_ins()->non_transform();
}
xui_method_explain(onity_stateview, calc_offset,		xui_vector<s32>	)( const xui_vector<s32>& self, const xui_vector<s32>& next )
{
	xui_vector<s32> normal = next - self;

	s32 d = (s32)sqrtl((f64)(normal.x*normal.x) + (f64)(normal.y*normal.y));
	f32 angle = 0.0f;
	NPVector3 v1((f32)normal.x, (f32)normal.y, 0.0f);
	NPVector3 v2(1.0f,          0.0f,          0.0f);
	if (v1 != NPVector3::Zero)
	{
		angle = v1.GetAngle(v2);
		if (v1.GetCross(v2).z > 0.0f)
			angle *= -1.0f;
	}

	xui_vector<s32> offset;
	offset.x = (s32)(-8.0f * NPMathWrap::Cos(NPMathWrap::PIHalf-angle));
	offset.y = (s32)( 8.0f * NPMathWrap::Sin(NPMathWrap::PIHalf-angle));

	return offset;
}