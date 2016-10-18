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
#include "onity_propmapref.h"

/*
//constructor
*/
xui_create_explain(onity_propmapref)( NP2DSTransRef* ref )
: onity_propnode2dsref(ref)
{
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

	add_propkind(m_paramkind);
}

/*
//method
*/
xui_method_explain(onity_propmapref, set_newref,	void			)( NP2DSAsset* asset )
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
xui_method_explain(onity_propmapref, ori_bounding,	xui_rect2d<s32>	)( void )
{
	NP2DSTransRef* transref = get_2dsref();
	if (transref)
	{
		NPRect rt = transref->GetWorldBounding();
		return xui_rect2d<s32>(rt.LT, rt.TP, rt.RT, rt.BM);
	}

	return xui_rect2d<s32>(0);
}
xui_method_explain(onity_propmapref, ori_position,	xui_vector<s32>	)( void )
{
	NP2DSTransRef* transref = get_2dsref();
	if (transref)
	{
		NPVector3 pos = transref->GetWorldTrans();
		return xui_vector<s32>((s32)pos.x, (s32)pos.y);
	}

	return xui_vector<s32>(0);
}
xui_method_explain(onity_propmapref, set_position,	void			)( const xui_vector<s32>& pos )
{
	NP2DSTransRef* transref = get_2dsref();
	if (transref)
	{
		transref->SetWorldTrans(NPVector3((f32)pos.x, (f32)pos.y, 0.0f));
	}
}

/*
//static
*/
xui_method_explain(onity_propmapref, get_asset,		void*			)( void* userptr )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
	NP2DSTransRef* transref = prop->get_2dsref();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, transref);
	if (imageref) return imageref->GetImage();
	if (frameref) return frameref->GetFrame();
	if (actorref) return actorref->GetActor();

	return NULL;
}
xui_method_explain(onity_propmapref, set_asset,		void			)( void* userptr, void* value )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
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
xui_method_explain(onity_propmapref, get_params,	ParamVec&		)( void* userptr )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
	return prop->get_2dsref()->GetSceneParamVec();
}
xui_method_explain(onity_propmapref, add_param,		void			)( void* userptr )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
	NP2DSTransRef::SParam param;
	prop->get_2dsref()->AddSceneParam(param);
}
xui_method_explain(onity_propmapref, del_param,		void			)( void* userptr )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
	u32 index = prop->get_2dsref()->GetSceneParamCount()-1;
	prop->get_2dsref()->DelSceneParam(index);
}
xui_method_explain(onity_propmapref, new_paramprop, xui_propdata*	)( void* userptr, u32 i, xui_propkind* propkind )
{
	onity_propmapref* prop = (onity_propmapref*)userptr;
	NP2DSTransRef::SParam* param = &(prop->get_2dsref()->GetSceneParamVec()[i]);
	return new onity_propdata_sceneparam(propkind, L"Param", onity_propctrl_sceneparam::create, param);
}