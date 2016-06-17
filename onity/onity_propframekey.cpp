#include "NPFileName.h"
#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSAssetFile.h"
#include "NP2DSFrameKey.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NPParticleSFX.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_propctrl_object.h"
#include "onity_propctrl_transref.h"
#include "onity_timedata.h"
#include "onity_resource.h"
#include "onity_selector.h"
#include "onity_proplayer.h"
#include "onity_propframekey.h"

/*
//constructor
*/
xui_create_explain(onity_propframekey)( onity_propfile* propfile, onity_proplayer* proplayer, NP2DSFrameKey* framekey )
: onity_proptransref(propfile, framekey->GetTransRef())
, m_proplayer(proplayer)
, m_framekey(framekey)
{
	m_keykind = new xui_propkind(this, L"FrameKey", "FrameKey", xui_kindctrl::create, onity_resource::icon_framekey, true);
	m_keykind->add_propdata(new xui_propdata_bool(
		m_keykind,
		L"Smooth",
		xui_propctrl_bool::create,
		was_smooth,
		set_smooth,
		this));
	m_keykind->add_propdata(new onity_propdata_particle(
		m_keykind,
		L"Particle",
		get_spark,
		set_spark,
		this));
	m_keykind->add_propdata(new xui_propdata_string(
		m_keykind,
		L"Sound",
		xui_propctrl_string::create,
		get_sound,
		set_sound,
		this));
	m_keykind->add_propdata(new xui_propdata_string(
		m_keykind,
		L"Event",
		xui_propctrl_string::create,
		get_event,
		set_event,
		this));

	m_reskind = new xui_propkind(this, L"Resource", "ImageFrame", xui_kindctrl::create, onity_resource::icon_resource, true);
	m_reskind->add_propdata(new onity_propdata_2dsasset(
		m_reskind,
		L"Asset",
		onity_propctrl_asset::create,
		DROPTYPE_IMAGE|DROPTYPE_FRAME,
		get_asset,
		set_asset,
		this));

	m_propkind.insert(m_propkind.begin(), m_keykind);
	m_propkind.insert(m_propkind.begin(), m_reskind);
}

/*
//method
*/
xui_method_explain(onity_propframekey, set_transref,	void			)( void )
{
	m_transref = m_framekey->GetTransRef();
}
xui_method_explain(onity_propframekey, get_framekey,	NP2DSFrameKey*	)( void )
{
	return m_framekey;
}
xui_method_explain(onity_propframekey, get_layer,		onity_proplayer*)( void )
{
	return m_proplayer;
}

/*
//static
*/  
xui_method_explain(onity_propframekey, was_smooth,		bool			)( void* userptr )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	return propframekey->get_framekey()->WasSmooth();
}
xui_method_explain(onity_propframekey, set_smooth,		void			)( void* userptr, bool  value )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	propframekey->get_framekey()->SetSmooth(value);
	onity_proplayer* proplayer = propframekey->get_layer();
	onity_timedata* timedata = dynamic_cast<onity_timedata*>(proplayer->get_linkdata());
	timedata->cal_keyframe();
}
xui_method_explain(onity_propframekey, get_spark,		void*			)( void* userptr )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	return propframekey->get_framekey()->GetSpark();
}
xui_method_explain(onity_propframekey, set_spark,		void			)( void* userptr, void* value )
{
	NPParticleSFX* particle = (NPParticleSFX*)value;
	std::string filename = particle == NULL ? "" : particle->GetSourceFileName();
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	propframekey->get_framekey()->SetGfxfile(filename);
}
xui_method_explain(onity_propframekey, get_event,		std::wstring	)( void* userptr )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	std::string text = propframekey->get_framekey()->GetEvent();
	return xui_global::ascii_to_unicode(text);
}
xui_method_explain(onity_propframekey, set_event,		void			)( void* userptr, const std::wstring& value )
{
	std::string text = xui_global::unicode_to_ascii(value);
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	propframekey->get_framekey()->SetEvent(text);
}
xui_method_explain(onity_propframekey, get_sound,		std::wstring	)( void* userptr )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	std::string text = propframekey->get_framekey()->GetSound();
	return xui_global::ascii_to_unicode(text);
}
xui_method_explain(onity_propframekey, set_sound,		void			)( void* userptr, const std::wstring& value )
{
	std::string text = xui_global::unicode_to_ascii(value);
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	propframekey->get_framekey()->SetSound(text);
}
xui_method_explain(onity_propframekey, get_asset,		void*			)( void* userptr )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	NP2DSTransRef* transref = propframekey->get_transref();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
	if (imageref) return imageref->GetImage();
	if (frameref) return frameref->GetFrame();

	return NULL;
}
xui_method_explain(onity_propframekey, set_asset,		void			)( void* userptr, void* value )
{
	onity_propframekey* propframekey = (onity_propframekey*)userptr;
	NP2DSAsset* asset = (NP2DSAsset*)value;
	if (asset)
	{
		NP2DSImage* image = NPDynamicCast(NP2DSImage, asset);
		NP2DSFrame* frame = NPDynamicCast(NP2DSFrame, asset);
		if (image) propframekey->get_framekey()->SetImage(image->GetOwnedFile()->GetKey(), image->GetKey());
		if (frame) propframekey->get_framekey()->SetFrame(frame->GetOwnedFile()->GetKey(), frame->GetKey());
		propframekey->set_transref();
	}
	else
	{
		propframekey->get_transref()->SetAsset(-1, -1);
	}
}