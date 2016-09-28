#include "NPFileName.h"
#include "NP2DSActor.h"
#include "NP2DSActorFile.h"
#include "NP2DSActorRef.h"
#include "NP2DSStateCtrl.h"
#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "onity_resource.h"
#include "onity_selector.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_propctrl_transref.h"
#include "onity_propnodeaction.h"

/*
//constructor
*/
xui_create_explain(onity_propnodeaction)( NP2DSTransRef* transref )
: onity_propnode2dsref(transref)
{
	m_reskind = new xui_propkind(this, L"Resource", "Node2DSActorRes", xui_kindctrl::create, onity_resource::icon_resource, true);
	m_reskind->add_propdata(new onity_propdata_2dsasset(
		m_reskind, 
		L"Actor", 
		onity_propctrl_asset::create,
		DROPTYPE_ACTOR, 
		get_actor, 
		set_actor, 
		this));
	m_reskind->add_propdata(new onity_propdata_statectrl(
		m_reskind,
		L"Controller",
		get_statectrl,
		set_statectrl,
		this));
	m_reskind->add_propdata(new xui_propdata_bool(
		m_reskind,
		L"Play",
		xui_propctrl_bool::create,
		was_play,
		set_play,
		this));
	m_reskind->add_propdata(new xui_propdata_number_func(
		m_reskind,
		L"Time",
		xui_propctrl_number::create,
		get_time,
		set_time,
		this,
		NT_INT));

	m_propkind.insert(m_propkind.begin(), m_reskind);
}

/*
//static
*/
xui_method_explain(onity_propnodeaction, was_play,		bool	)( void* userptr )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	return actorref->WasPlay();
}
xui_method_explain(onity_propnodeaction, set_play,		void	)( void* userptr, bool  value )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	actorref->SetPlay(value);
}
xui_method_explain(onity_propnodeaction, get_time,		f64		)( void* userptr )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	return (f64)actorref->GetTime();
}
xui_method_explain(onity_propnodeaction, set_time,		void	)( void* userptr, f64   value )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	actorref->SetTime((npu16)value);
}
xui_method_explain(onity_propnodeaction, get_actor,		void*	)( void* userptr )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	return actorref->GetActor();
}
xui_method_explain(onity_propnodeaction, set_actor,		void	)( void* userptr, void* value )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActor* actor = (NP2DSActor*)value;
	npu32 resfile = (actor == NULL) ? -1 : actor->GetOwnedFile()->GetKey();
	npu32 actorid = (actor == NULL) ? -1 : actor->GetKey();
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	actorref->SetActor(resfile, actorid);
}
xui_method_explain(onity_propnodeaction, get_statectrl, void*	)( void* userptr )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	return actorref->GetStateCtrl();
}
xui_method_explain(onity_propnodeaction, set_statectrl, void	)( void* userptr, void* value )
{
	onity_propnodeaction* prop = (onity_propnodeaction*)userptr;
	NP2DSStateCtrl* statectrl = (NP2DSStateCtrl*)value;
	std::string filename = (statectrl == NULL) ? "" : statectrl->GetSourceFile();
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, prop->get_2dsref());
	actorref->SetStateCtrl(filename);
}