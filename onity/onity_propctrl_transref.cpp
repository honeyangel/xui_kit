#include "NP2DSAsset.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"

#include "xui_global.h"
#include "xui_propview.h"
#include "onity_resource.h"
#include "onity_proptransref.h"
#include "onity_propctrl_transref.h"

/*
//constructor
*/
xui_create_explain(onity_propdata_transref)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	xui_prop_newctrl		func, 
	const std::string&		droptype, 
	onity_proptransref*		proptransref,
	get_total				usergettotal, 
	void*					userptrtotal )
: xui_propdata_object_func(kind, name, func, droptype, NULL, get_icon, get_name, get_asset, set_asset, proptransref)
, m_proptransref(proptransref)
, m_usergettotal(usergettotal)
, m_userptrtotal(userptrtotal)
{}

/*
//method
*/
xui_method_explain(onity_propdata_transref, get_proptransref,	onity_proptransref*	)( void )
{
	return m_proptransref;
}
xui_method_explain(onity_propdata_transref, get_proptotal,		xui_proproot_vec	)( void )
{
	if (m_usergettotal)
		return (*m_usergettotal)(m_userptrtotal);

	return xui_proproot_vec();
}

/*
//static
*/
xui_method_explain(onity_propdata_transref, get_icon,			xui_bitmap*			)( xui_propdata* propdata )
{
	return onity_resource::icon_animator;
}
xui_method_explain(onity_propdata_transref, get_name,			std::wstring		)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	NP2DSAsset* asset = (NP2DSAsset*)dataobject->get_value();
	std::string name  = asset->GetName();

	char temp[256];
	sprintf(temp, "%03d#%s", asset->GetKey(), name.c_str());
	return xui_global::ascii_to_unicode(std::string(temp));
}
xui_method_explain(onity_propdata_transref, get_asset,			void*				)( void* userptr )
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
xui_method_explain(onity_propdata_transref, set_asset,			void				)( void* userptr, void* value )
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

xui_implement_rtti(onity_propctrl_transref, xui_propctrl_object);

/*
//constructor
*/
xui_create_explain(onity_propctrl_transref)( xui_propdata* propdata )
: xui_propctrl_object(propdata)
{
	xui_control* editctrl = m_propedit->get_editctrl();
	editctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, onity_propctrl_transref>(this, &onity_propctrl_transref::on_editctrlclick);
}

/*
//static
*/
xui_method_explain(onity_propctrl_transref, create,				xui_propctrl*		)( xui_propdata* propdata )
{
	return new onity_propctrl_transref(propdata);
}

/*
//event
*/
xui_method_explain(onity_propctrl_transref, on_editctrlclick,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_propview* propview = get_propview();
		onity_propdata_transref* datatransref = dynamic_cast<onity_propdata_transref*>(m_propdata);
		propview->add_propelse(datatransref->get_proptotal(), datatransref->get_proptransref());
	}
}
