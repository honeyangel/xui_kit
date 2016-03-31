#include "NP2DSActor.h"
#include "NP2DSActorFile.h"
#include "NP2DSState.h"
#include "NP2DSStateCtrl.h"
#include "NP2DSTransition.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propctrl_object.h"
#include "onity_resource.h"
#include "onity_state.h"
#include "onity_propctrl_transition.h"
#include "onity_proptransition.h"
#include "onity_propstate.h"

/*
//constructor
*/
xui_create_explain(onity_propstate)( onity_state* statectrl )
: xui_proproot()
, m_statectrl(statectrl)
{
	NP2DSState* state = m_statectrl->get_state();
	const NP2DSState::TransitionVec& vec = state->GetTransitionVec();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		m_proptransition.push_back(new onity_proptransition(vec[i]));
	}

	m_statekind		= new xui_propkind(this, xui_global::ascii_to_unicode(state->GetName()), "State", xui_kindctrl::create, onity_resource::icon_state, true);
	xui_method_ptrcall(m_statekind, xm_namechanged) += new xui_method_member<xui_method_args, onity_propstate>(this, &onity_propstate::on_namechanged);
	m_retarget		= new xui_propdata_bool(
		m_statekind, 
		L"Retarget", 
		xui_propctrl_bool::create, 
		get_retarget, 
		set_retarget, 
		state);
	m_actor			= new xui_propdata_object_func(
		m_statekind, 
		L"Action", 
		xui_propctrl_object::create, 
		"NP2DSActor", 
		NULL, 
		get_actoricon, 
		get_actorname, 
		get_actor, 
		set_actor, 
		state);

	m_transition	= new onity_propdata_transition(
		m_statekind,
		L"",
		onity_propctrl_transition::create,
		m_statectrl);

	m_statekind->add_propdata(m_retarget);
	m_statekind->add_propdata(m_actor);
	m_statekind->add_propdata(m_transition);
	add_propkind(m_statekind);
}

/*
//destructor
*/
xui_delete_explain(onity_propstate)( void )
{
	for (u32 i = 0; i < m_proptransition.size(); ++i)
		delete m_proptransition[i];

	m_proptransition.clear();
}

/*
//method
*/
xui_method_explain(onity_propstate, get_proptransition,	onity_proptransition*	)( NP2DSTransition* transition )
{
	for (u32 i = 0; i < m_proptransition.size(); ++i)
	{
		if (m_proptransition[i]->get_transition() == transition)
			return m_proptransition[i];
	}

	return NULL;
}
xui_method_explain(onity_propstate, add_transition,		void					)( NP2DSTransition* transition )
{
	m_proptransition.push_back(new onity_proptransition(transition));
	m_transition->refresh();
}
xui_method_explain(onity_propstate, del_transition,		void					)( NP2DSTransition* transition )
{
	for (u32 i = 0; i < m_proptransition.size(); ++i)
	{
		if (m_proptransition[i]->get_transition() == transition)
		{
			delete m_proptransition[i];
			m_proptransition.erase(m_proptransition.begin()+i);
			break;
		}
	}

	m_transition->refresh();
}
xui_method_explain(onity_propstate, on_delstate,		void					)( NP2DSState* state )
{
	onity_propctrl_transition* propctrl = xui_dynamic_cast(onity_propctrl_transition, m_transition->get_ctrl());
	if (propctrl && propctrl->has_propdata(m_transition))
	{
		propctrl->on_delstate(state);
	}
}
xui_method_explain(onity_propstate, on_delparam,		void					)( NP2DSParam* param )
{
	for (u32 i = 0; i < m_proptransition.size(); ++i)
	{
		m_proptransition[i]->on_delparam(param);
	}
}
xui_method_explain(onity_propstate, on_addparam,		void					)( NP2DSParam* param )
{
	for (u32 i = 0; i < m_proptransition.size(); ++i)
	{
		m_proptransition[i]->on_addparam(param);
	}
}

/*
//event
*/
xui_method_explain(onity_propstate, on_namechanged,		void					)( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textbox->get_text();
	std::string  name = xui_global::unicode_to_ascii(text);

	bool has_same = false;
	NP2DSStateCtrl* editfile = m_statectrl->get_state()->GetStateCtrl();
	const NP2DSStateCtrl::StateVec& vec = editfile->GetStateVec();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (vec[i] != m_statectrl->get_state() && vec[i]->GetName() == name)
		{
			has_same = true;
			break;
		}
	}

	if (has_same || name.length() == 0)
	{
		s32 number = 0;
		while (true)
		{
			std::stringstream temp;
			temp << ((name.length() == 0) ? "New State" : name.c_str());
			temp << number;
			if (editfile->HasState(temp.str()) == false)
			{
				name = temp.str();
				text = xui_global::ascii_to_unicode(name);
				textbox->ini_textbox(text);
				break;
			}

			++number;
		}
	}

	m_statekind->set_name(text);
	m_statectrl->set_text(text);
	m_statectrl->get_state()->SetName(name);
}

/*
//static
*/
xui_method_explain(onity_propstate, get_retarget,		bool					)( void* userptr )
{
	NP2DSState* state = (NP2DSState*)userptr;
	return state->CanRetarget();
}
xui_method_explain(onity_propstate, set_retarget,		void					)( void* userptr, bool  value )
{
	NP2DSState* state = (NP2DSState*)userptr;
	state->SetRetarget(value);
}
xui_method_explain(onity_propstate, get_actor,			void*					)( void* userptr )
{
	NP2DSState* state = (NP2DSState*)userptr;
	return state->GetActor();
}
xui_method_explain(onity_propstate, set_actor,			void					)( void* userptr, void* value )
{
	NP2DSState* state = (NP2DSState*)userptr;
	NP2DSActor* actor = (NP2DSActor*)value;
	npu32 resfile = (actor == NULL) ? -1 : actor->GetOwnedFile()->GetKey();
	npu32 actorid = (actor == NULL) ? -1 : actor->GetKey();
	state->SetActor(resfile, actorid);
}
xui_method_explain(onity_propstate, get_actoricon,		xui_bitmap*				)( xui_propdata* propdata )
{
	return onity_resource::icon_animator;
}
xui_method_explain(onity_propstate, get_actorname,		std::wstring			)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = (xui_propdata_object*)propdata;
	NP2DSActor* actor = (NP2DSActor*)dataobject->get_value();
	if (actor)
	{
		return xui_global::ascii_to_unicode(actor->GetName());
	}

	return L"None";
}