#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSTransRef.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NP2DSSceneLayer.h"

#include "xui_textbox.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "onity_propctrl_transref.h"
#include "onity_propctrl_sceneparam.h"
#include "onity_boundbox_coursenode.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_inspector.h"
#include "onity_propcoursenode.h"

/*
//constructor
*/
xui_create_explain(onity_propcoursenode)( NP2DSTransRef* ref )
: m_2dsref(ref)
{
	m_assetkind = new xui_propkind(this, L"Resource", "SceneAsset", xui_kindctrl::create, onity_resource::icon_design, true);
	m_assetkind->add_propdata(new onity_propdata_2dsasset(
		m_assetkind,
		L"Asset",
		onity_propctrl_asset::create,
		DROPTYPE_IMAGE|DROPTYPE_FRAME|DROPTYPE_ACTOR,
		get_asset,
		set_asset,
		this));

	add_propkind(m_transkind);
	add_propkind(m_assetkind);
	add_propkind(m_paramkind);

	m_boundctrl = new onity_boundbox_coursenode(this);
}

/*
//method
*/
xui_method_explain(onity_propcoursenode, set_newref,	void			)( NP2DSAsset* asset )
{
	NP2DSSceneLayer* scenelayer = NPDynamicCast(NP2DSSceneLayer, m_2dsref->GetParent());
	NP2DSTransRef* newref = NULL;
	if		(NPIsExaKindOf(NP2DSImage, asset)) newref = scenelayer->AddImageRef(asset->GetOwnedFile()->GetKey(), asset->GetKey());
	else if (NPIsExaKindOf(NP2DSFrame, asset)) newref = scenelayer->AddFrameRef(asset->GetOwnedFile()->GetKey(), asset->GetKey());
	else if (NPIsExaKindOf(NP2DSActor, asset)) newref = scenelayer->AddActorRef(asset->GetOwnedFile()->GetKey(), asset->GetKey());

	if (newref)
	{
		newref->Clone(m_2dsref);
		npu32 index = scenelayer->GetSceneRefIndex(m_2dsref);
		scenelayer->DelSceneRef(m_2dsref);
		scenelayer->SetSceneRefIndex(newref, index);
		m_2dsref = newref;

		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->get_propview()->reset();
	}
}
xui_method_explain(onity_propcoursenode, get_2dsref,	NP2DSTransRef*	)( void )
{
	return m_2dsref;
}
xui_method_explain(onity_propcoursenode, get_scale,		xui_vector<f64>	)( void )
{
	NPVector3 scale = m_2dsref->GetWorldScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_propcoursenode, set_scale,		void			)( const xui_vector<f64>& value )
{
	m_2dsref->SetWorldScale(NPVector3((npf32)value.x, (npf32)value.y, 1.0f));
}

/*
//static
*/
xui_method_explain(onity_propcoursenode, get_asset,		void*			)( void* userptr )
{
	onity_propcoursenode* prop = (onity_propcoursenode*)userptr;
	NP2DSTransRef* transref = prop->get_2dsref();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, transref);
	if (imageref) return imageref->GetImage();
	if (frameref) return frameref->GetFrame();
	if (actorref) return actorref->GetActor();

	return NULL;
}
xui_method_explain(onity_propcoursenode, set_asset,		void			)( void* userptr, void* value )
{
	onity_propcoursenode* prop = (onity_propcoursenode*)userptr;
	NP2DSAsset* asset = (NP2DSAsset*)value;
	if (asset)
	{
		NP2DSTransRef* transref = prop->get_2dsref();
		NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
		NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
		NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, transref);
		NP2DSImage*    image    = NPDynamicCast(NP2DSImage,    asset);
		NP2DSFrame*    frame    = NPDynamicCast(NP2DSFrame,    asset);
		NP2DSActor*    actor    = NPDynamicCast(NP2DSActor,    asset);
		if		(image && imageref) imageref->SetImage(image->GetOwnedFile()->GetKey(), image->GetKey());
		else if (frame && frameref) frameref->SetFrame(frame->GetOwnedFile()->GetKey(), frame->GetKey());
		else if (actor && actorref) actorref->SetActor(actor->GetOwnedFile()->GetKey(), actor->GetKey());
		else
		{
			prop->set_newref(asset);
		}
	}
	else
	{
		prop->get_2dsref()->SetAsset(-1, -1);
	}
}
