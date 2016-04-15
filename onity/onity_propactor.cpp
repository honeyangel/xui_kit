#include "NP2DSActor.h"

#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_expand.h"
#include "onity_propactor.h"

/*
//constructor
*/
xui_create_explain(onity_propactor)( onity_propfile* propfile, u32 id )
: onity_prop2dsasset(propfile, id)
{
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

	add_propkind(m_actorkind);
}

/*
//static
*/
xui_method_explain(onity_propactor, was_loop,		bool)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	return actor->WasLoop();
}
xui_method_explain(onity_propactor, set_loop,		void)( void* userptr, bool value )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	actor->SetLoop(value);
}
xui_method_explain(onity_propactor, get_loopstart,	f64	)( void* userptr )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	return (f64)actor->GetLoopStart();
}
xui_method_explain(onity_propactor, set_loopstart,	void)( void* userptr, f64  value )
{
	onity_propactor* propactor = (onity_propactor*)userptr;
	NP2DSActor* actor = (NP2DSActor*)propactor->get_asset();
	actor->SetLoopStart((npu16)value);
}