#include "NPFileName.h"
#include "NP2DSAsset.h"
#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NPParticleSFX.h"

#include "xui_global.h"
#include "xui_propview.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_resource.h"
#include "onity_selector.h"
#include "onity_proptransref.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_propctrl_transref.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propdata_particle)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	get_func				userget, 
	set_func				userset, 
	void*					userptr )
: xui_propdata_object_func(kind, name, onity_propctrl_asset::create, "NPParticleSFX", onity_selector::get_ptr, get_icon, get_name, userget, userset, userptr)
{
	xm_doubleclick += new xui_method_member<xui_method_args, onity_propdata_particle>(this, &onity_propdata_particle::on_doubleclick);
}

/*
//static
*/
xui_method_explain(onity_propdata_particle, get_icon,				xui_bitmap*			)( xui_propdata* propdata )
{
	return onity_resource::icon_particle;
}
xui_method_explain(onity_propdata_particle, get_name,				std::wstring		)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	NPParticleSFX* particle = (NPParticleSFX*)dataobject->get_value();
	if (particle)
	{
		std::string name = NPFileNameHelper::SafeName(particle->GetSourceFileName());
		return xui_global::ascii_to_unicode(name);
	}

	return L"None";
}

/*
//event
*/
xui_method_explain(onity_propdata_particle, on_doubleclick,			void				)( xui_component* sender, xui_method_args& args )
{
	NPParticleSFX* particle = (NPParticleSFX*)get_value();
	if (particle)
	{
		std::string full = particle->GetSourceFileName();
		std::string path = NPFileNameHelper::PathName(full);
		std::string file = NPFileNameHelper::FileName(full);
		path = path.substr(0, path.length()-1);
		file = NPFileNameHelper::SafeName(file);

		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->loc_filenode(xui_global::ascii_to_unicode(path), xui_global::ascii_to_unicode(file), 0);
		onity_mainform::get_ptr()->set_pageshow(project);
	}
}

/*
//constructor
*/
xui_create_explain(onity_propdata_2dsasset)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	xui_prop_newctrl		func, 
	u32						droptype, 
	get_func				userget, 
	set_func				userset, 
	void*					userptr )
: xui_propdata_object_func(kind, name, func, "", onity_selector::get_ptr, get_icon, get_name, userget, userset, userptr)
{
	m_droptype.clear();
	if (droptype & DROPTYPE_IMAGE)	add_droptype("NP2DSImage");
	if (droptype & DROPTYPE_FRAME)	add_droptype("NP2DSFrame");
	if (droptype & DROPTYPE_ACTOR)	add_droptype("NP2DSActor");

	xm_doubleclick += new xui_method_member<xui_method_args, onity_propdata_2dsasset>(this, &onity_propdata_2dsasset::on_doubleclick);
}

/*
//event
*/
xui_method_explain(onity_propdata_2dsasset, on_doubleclick,			void				)( xui_component* sender, xui_method_args& args )
{
	NP2DSAsset* asset = (NP2DSAsset*)get_value();
	if (asset)
	{
		NP2DSAssetFileMgr* filemgr = NULL;
		if (NPIsExaKindOf(NP2DSImage, asset)) filemgr = NP2DSImageFileMgr::GetIns();
		if (NPIsExaKindOf(NP2DSFrame, asset)) filemgr = NP2DSFrameFileMgr::GetIns();
		if (NPIsExaKindOf(NP2DSActor, asset)) filemgr = NP2DSActorFileMgr::GetIns();
		u32 id = asset->GetOwnedFile()->GetKey();
		std::string path = filemgr->GetFilePH(id);
		std::string file = filemgr->GetFileFN(id);
		path = path.substr(0, path.length()-1);
		file = NPFileNameHelper::SafeName(file);

		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->loc_filenode(xui_global::ascii_to_unicode(path), xui_global::ascii_to_unicode(file), asset->GetKey());
		onity_mainform::get_ptr()->set_pageshow(project);
	}
}

/*
//static
*/
xui_method_explain(onity_propdata_2dsasset, get_icon,				xui_bitmap*			)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	NP2DSAsset* asset = (NP2DSAsset*)dataobject->get_value();
	if (asset)
	{
		if		(NPIsExaKindOf(NP2DSImage, asset))			return onity_resource::icon_module;
		else if (NPIsExaKindOf(NP2DSFrame, asset))			return onity_resource::icon_sprite;
		else if (NPIsExaKindOf(NP2DSActor, asset))			return onity_resource::icon_action;
	}
	else
	{
		if		(dataobject->has_droptype("NP2DSImage"))	return onity_resource::icon_module;
		else if (dataobject->has_droptype("NP2DSFrame"))	return onity_resource::icon_sprite;
		else if (dataobject->has_droptype("NP2DSActor"))	return onity_resource::icon_action;
	}

	return NULL;
}
xui_method_explain(onity_propdata_2dsasset, get_name,				std::wstring		)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	NP2DSAsset* asset = (NP2DSAsset*)dataobject->get_value();
	std::string name  = asset->GetName();

	char temp[256];
	sprintf(temp, "%03d#%s", asset->GetKey(), name.c_str());
	return xui_global::ascii_to_unicode(std::string(temp));
}

/*
//constructor
*/
xui_create_explain(onity_propdata_transref)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	xui_prop_newctrl		func, 
	u32						droptype, 
	onity_proptransref*		proptransref,
	get_total				usergettotal, 
	void*					userptrtotal )
: onity_propdata_2dsasset(kind, name, func, droptype, get_asset, set_asset, proptransref)
, m_proptransref(proptransref)
, m_usergettotal(usergettotal)
, m_userptrtotal(userptrtotal)
{}

/*
//method
*/
xui_method_explain(onity_propdata_transref, get_proptransref,		onity_proptransref*	)( void )
{
	return m_proptransref;
}
xui_method_explain(onity_propdata_transref, get_proptotal,			xui_proproot_vec	)( void )
{
	if (m_usergettotal)
		return (*m_usergettotal)(m_userptrtotal);

	return xui_proproot_vec();
}

/*
//static
*/
xui_method_explain(onity_propdata_transref, get_asset,				void*				)( void* userptr )
{
	onity_proptransref* proptransref = (onity_proptransref*)userptr;
	NP2DSTransRef* transref = proptransref->get_transref();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, transref);
	NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, transref);
	NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, transref);
	if (imageref) return imageref->GetImage();
	if (frameref) return frameref->GetFrame();
	if (actorref) return actorref->GetActor();

	return NULL;
}
xui_method_explain(onity_propdata_transref, set_asset,				void				)( void* userptr, void* value )
{
	npu32 resfile = -1;
	npu32 assetid = -1;
	NP2DSAsset* asset = (NP2DSAsset*)value;
	if (asset)
	{
		resfile = asset->GetOwnedFile()->GetKey();
		assetid = asset->GetKey();
	}

	onity_proptransref* proptransref = (onity_proptransref*)userptr;
	NP2DSTransRef* transref = proptransref->get_transref();
	transref->SetAsset(resfile, assetid);
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_propctrl_asset,	xui_propctrl_object);

/*
//constructor
*/
xui_create_explain(onity_propctrl_asset)( xui_propdata* propdata )
: xui_propctrl_object(propdata)
{
	xui_control* editctrl = m_propedit->get_editctrl();
	editctrl->xm_mouseenter += new xui_method_member<xui_method_mouse, onity_propctrl_asset>(this, &onity_propctrl_asset::on_editctrlmouseenter);
	editctrl->xm_mouseleave += new xui_method_member<xui_method_mouse, onity_propctrl_asset>(this, &onity_propctrl_asset::on_editctrlmouseleave);
}

/*
//static
*/
xui_method_explain(onity_propctrl_asset,	create, xui_propctrl*)( xui_propdata* propdata )
{
	return new onity_propctrl_asset(propdata);
}

/*
//event
*/
xui_method_explain(onity_propctrl_asset,	on_editctrlmouseenter,	void				)( xui_component* sender, xui_method_mouse& args )
{
	bool same = true;
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	void* value = dataobject->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (value == NULL || same == false)
		return;

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->show_tips(this);
}
xui_method_explain(onity_propctrl_asset,	on_editctrlmouseleave,	void				)( xui_component* sender, xui_method_mouse& args )
{
	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->hide_tips();
}

xui_implement_rtti(onity_propctrl_transref, onity_propctrl_asset);

/*
//constructor
*/
xui_create_explain(onity_propctrl_transref)( xui_propdata* propdata )
: onity_propctrl_asset(propdata)
{
	xui_control* editctrl = m_propedit->get_editctrl();
	editctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, onity_propctrl_transref>(this, &onity_propctrl_transref::on_editctrlclick);
}

/*
//static
*/
xui_method_explain(onity_propctrl_transref, create,					xui_propctrl*		)( xui_propdata* propdata )
{
	return new onity_propctrl_transref(propdata);
}

/*
//event
*/
xui_method_explain(onity_propctrl_transref, on_editctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_propview* propview = get_propview();
		onity_propdata_transref* datatransref = dynamic_cast<onity_propdata_transref*>(m_propdata);
		propview->add_propelse(datatransref->get_proptotal(), datatransref->get_proptransref());
	}
}
