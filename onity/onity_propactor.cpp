#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSCollider.h"

#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_stdvec.h"
#include "onity_savekind.h"
#include "onity_propcollider.h"
#include "onity_proplayer.h"
#include "onity_propactor.h"

/*
//constructor
*/
xui_create_explain(onity_propactor)( onity_propfile* propfile, u32 id )
: onity_prop2dsasset(propfile, id)
{
	NP2DSActor* actor = (NP2DSActor*)get_asset();
	for (npu32 i = 0; i < actor->GetDummyCount(); ++i)
	{
		NP2DSCollider* collider = actor->GetDummy(i);
		onity_propcollider* propcollider = new onity_propcollider(propfile, collider);
		m_propdummys.push_back(propcollider);
	}
	for (npu16 i = 0; i < actor->GetLayerCount(); ++i)
	{
		NP2DSLayer* layer = actor->GetLayer(i);
		onity_proplayer* proplayer = new onity_proplayer(propfile, this, layer);
		m_proplayers.push_back(proplayer);
	}

	m_actorkind = new xui_propkind(this, L"", "2DSActor", xui_kindctrl::create, NULL, true, false);
	std::vector<xui_propdata*>       subprop;
	std::map<s32, std::vector<u32> > showmap;
	showmap[1].push_back(0);
	subprop.push_back(new xui_propdata_number_func(
		m_actorkind,
		L"Loop Start",
		xui_propctrl_number::create,
		get_loopstart,
		set_loopstart,
		this,
		NT_UNSIGNEDINT));

	m_actorkind->add_propdata(new xui_propdata_expand_bool(
		m_actorkind,
		L"Loop",
		xui_propctrl_expand_bool::create,
		subprop,
		was_loop,
		set_loop,
		this, 
		true,
		showmap,
		xui_expandvary::empty_map));
	m_actorkind->add_propdata(new xui_propdata_stdvec_root(
		m_actorkind,
		L"Colliders",
		xui_propctrl_stdvec_root::create,
		add_dummy,
		del_dummy,
		get_dummys,
		this));

	add_propkind(m_actorkind);
}

/*
//destructor
*/
xui_delete_explain(onity_propactor)( void )
{
	for (u32 i = 0; i < m_propdummys.size(); ++i)
		delete m_propdummys[i];
	for (u32 i = 0; i < m_proplayers.size(); ++i)
		delete m_proplayers[i];
}

/*
//method
*/
xui_method_explain(onity_propactor, get_dummys,		const xui_proproot_vec&	)( void ) const
{
	return m_propdummys;
}
xui_method_explain(onity_propactor, add_dummy,		void					)( void )
{
	NP2DSActor*			actor		 = (NP2DSActor*)get_asset();
	NP2DSCollider*		collider	 = actor->AddDummy();
	collider->SetName("Collider");

	onity_propcollider* propcollider = new onity_propcollider(m_savekind->get_propfile(), collider);
	m_propdummys.push_back(propcollider);
}
xui_method_explain(onity_propactor, del_dummy,		void					)( xui_proproot* proproot )
{
	NP2DSActor* actor = (NP2DSActor*)get_asset();
	for (u32 i = 0; i < m_propdummys.size(); ++i)
	{
		onity_propcollider* propcollider = dynamic_cast<onity_propcollider*>(m_propdummys[i]);
		if (propcollider == proproot)
		{
			m_propdummys.erase(m_propdummys.begin()+i);
			actor->DelDummy(propcollider->get_collider());
			delete propcollider;
			break;
		}
	}
}
xui_method_explain(onity_propactor, get_layers,		const xui_proproot_vec&	)( void ) const
{
	return m_proplayers;
}
xui_method_explain(onity_propactor, add_layer,		xui_proproot*			)( void )
{
	NP2DSActor* actor = NPDynamicCast(NP2DSActor, get_asset());
	NP2DSLayer* layer = actor->AddLayer();
	layer->SetName("New Layer");
	onity_proplayer* proplayer = new onity_proplayer(get_propfile(), this, layer);
	m_proplayers.insert(m_proplayers.begin(), proplayer);
	return proplayer;
}
xui_method_explain(onity_propactor, del_layer,		void					)( xui_proproot* proproot )
{
	for (u32 i = 0; i < m_proplayers.size(); ++i)
	{
		if (m_proplayers[i] == proproot)
		{
			onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(proproot);
			NP2DSActor* actor = NPDynamicCast(NP2DSActor, get_asset());
			actor->DelLayer(proplayer->get_layer());

			delete m_proplayers[i];
			m_proplayers.erase(m_proplayers.begin()+i);
			break;
		}
	}
}

/*
//static
*/
xui_method_explain(onity_propactor, was_loop,		bool					)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	return actor->WasLoop();
}
xui_method_explain(onity_propactor, set_loop,		void					)( void* userptr, bool value )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	actor->SetLoop(value);
}
xui_method_explain(onity_propactor, get_loopstart,	f64						)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	return (f64)actor->GetLoopStart();
}
xui_method_explain(onity_propactor, set_loopstart,	void					)( void* userptr, f64  value )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	actor->SetLoopStart((npu16)value);
}
xui_method_explain(onity_propactor, get_dummys,		xui_proproot_vec		)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	return propactor->get_dummys();
}
xui_method_explain(onity_propactor, add_dummy,		void					)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	propactor->add_dummy();
}
xui_method_explain(onity_propactor, del_dummy,		void					)( void* userptr, xui_proproot* proproot )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	propactor->del_dummy(proproot);
}