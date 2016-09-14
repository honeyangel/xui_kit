#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
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
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_prop2dsref.h"

/*
//constructor
*/
xui_create_explain(onity_prop2dsref)( NP2DSTransRef* ref )
: onity_proproot()
, m_2dsref(ref)
{
	m_transkind = new xui_propkind(this, L"Transform", "Transform", xui_kindctrl::create, NULL, true);
	m_transkind->add_propdata(new xui_propdata_vector(
		m_transkind, 
		L"Position", 
		xui_propctrl_vector::create, 
		get_position, 
		set_position, 
		this, 
		NT_INT));
	m_transkind->add_propdata(new xui_propdata_vector(
		m_transkind,
		L"Scale",
		xui_propctrl_vector::create,
		get_scale,
		set_scale,
		this,
		NT_FLOAT));
	m_transkind->add_propdata(new xui_propdata_number_func(
		m_transkind,
		L"Rotation",
		xui_propctrl_number::create,
		get_rotation,
		set_rotation,
		this,
		NT_INT));

	m_paramkind = new xui_propkind(this, L"Design", "SceneDesign", xui_kindctrl::create, NULL, true);
	m_paramkind->add_propdata(new onity_propdata_2dsasset(
		m_paramkind,
		L"Asset",
		onity_propctrl_asset::create,
		DROPTYPE_IMAGE|DROPTYPE_FRAME|DROPTYPE_ACTOR,
		get_asset,
		set_asset,
		this));
	m_paramkind->add_propdata(new xui_propdata_stdvec_func<NP2DSTransRef::SParam>(
		m_paramkind,
		L"Parameters",
		xui_propctrl_stdvec::create,
		add_param,
		del_param,
		new_paramprop,
		get_params,
		this));

	add_propkind(m_transkind);
	add_propkind(m_paramkind);
}

/*
//method
*/
xui_method_explain(onity_prop2dsref, get_2dsref,	NP2DSTransRef*	)( void )
{
	return m_2dsref;
}
xui_method_explain(onity_prop2dsref, set_newref,	void			)( NP2DSAsset* asset )
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

/*
//static
*/
xui_method_explain(onity_prop2dsref, get_position,	xui_vector<f64>	)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	NPVector3 trans = prop->get_2dsref()->GetWorldTrans();
	return xui_vector<f64>((f64)trans.x, (f64)trans.y);
}
xui_method_explain(onity_prop2dsref, set_position,	void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	prop->get_2dsref()->SetWorldTrans(NPVector3((npf32)value.x, (npf32)value.y, 0.0f));
}
xui_method_explain(onity_prop2dsref, get_scale,		xui_vector<f64>	)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	NPVector3 scale = prop->get_2dsref()->GetWorldScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_prop2dsref, set_scale,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	prop->get_2dsref()->SetWorldScale(NPVector3((npf32)value.x, (npf32)value.y, 1.0f));
}
xui_method_explain(onity_prop2dsref, get_rotation,	f64				)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	return (f64)prop->get_2dsref()->GetWorldAngle();
}
xui_method_explain(onity_prop2dsref, set_rotation,	void			)( void* userptr, f64   value )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	prop->get_2dsref()->SetWorldAngle((npf32)value);
}
xui_method_explain(onity_prop2dsref, get_asset,		void*			)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	NP2DSTransRef* transref = prop->get_2dsref();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, transref);
	if (imageref) return imageref->GetImage();
	if (frameref) return frameref->GetFrame();
	if (actorref) return actorref->GetActor();

	return NULL;
}
xui_method_explain(onity_prop2dsref, set_asset,		void			)( void* userptr, void* value )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
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
xui_method_explain(onity_prop2dsref, get_params,	ParamVec&		)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	return prop->get_2dsref()->GetSceneParamVec();
}
xui_method_explain(onity_prop2dsref, add_param,		void			)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	NP2DSTransRef::SParam param;
	prop->get_2dsref()->AddSceneParam(param);
}
xui_method_explain(onity_prop2dsref, del_param,		void			)( void* userptr )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	u32 index = prop->get_2dsref()->GetSceneParamCount()-1;
	prop->get_2dsref()->DelSceneParam(index);
}
xui_method_explain(onity_prop2dsref, new_paramprop, xui_propdata*	)( void* userptr, u32 i, xui_propkind* propkind )
{
	onity_prop2dsref* prop = (onity_prop2dsref*)userptr;
	NP2DSTransRef::SParam* param = &(prop->get_2dsref()->GetSceneParamVec()[i]);
	return new onity_propdata_sceneparam(propkind, L"Param", onity_propctrl_sceneparam::create, param);
}