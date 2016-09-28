#include "NP2DSFrame.h"
#include "NP2DSFrameFile.h"
#include "NP2DSFrameRef.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "onity_resource.h"
#include "onity_propctrl_transref.h"
#include "onity_propnodesprite.h"

/*
//constructor
*/
xui_create_explain(onity_propnodesprite)( NP2DSTransRef* transref )
	: onity_propnode2dsref(transref)
{
	m_reskind = new xui_propkind(this, L"Resource", "Node2DSFrameRes", xui_kindctrl::create, onity_resource::icon_resource, true);
	m_reskind->add_propdata(new onity_propdata_2dsasset(
		m_reskind, 
		L"Frame", 
		onity_propctrl_asset::create,
		DROPTYPE_FRAME, 
		get_frame, 
		set_frame, 
		this));

	m_propkind.insert(m_propkind.begin(), m_reskind);
}

/*
//static
*/
xui_method_explain(onity_propnodesprite, get_frame,		void*			)( void* userptr )
{
	onity_propnodesprite* prop = (onity_propnodesprite*)userptr;
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, prop->get_2dsref());
	return frameref->GetFrame();
}
xui_method_explain(onity_propnodesprite, set_frame,		void			)( void* userptr, void* value )
{
	onity_propnodesprite* prop = (onity_propnodesprite*)userptr;
	NP2DSFrame* frame = (NP2DSFrame*)value;
	npu32 resfile = (frame == NULL) ? -1 : frame->GetOwnedFile()->GetKey();
	npu32 frameid = (frame == NULL) ? -1 : frame->GetKey();
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, prop->get_2dsref());
	frameref->SetFrame(resfile, frameid);
}