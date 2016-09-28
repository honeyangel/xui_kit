#include "NP2DSImage.h"
#include "NP2DSImageFile.h"
#include "NP2DSImageRef.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "onity_resource.h"
#include "onity_propctrl_transref.h"
#include "onity_propnodemodule.h"

/*
//constructor
*/
xui_create_explain(onity_propnodemodule)( NP2DSTransRef* transref )
: onity_propnode2dsref(transref)
{
	m_reskind = new xui_propkind(this, L"Resource", "Node2DSImageRes", xui_kindctrl::create, onity_resource::icon_resource, true);
	m_reskind->add_propdata(new onity_propdata_2dsasset(
		m_reskind, 
		L"Image", 
		onity_propctrl_asset::create,
		DROPTYPE_IMAGE, 
		get_image, 
		set_image, 
		this));

	m_propkind.insert(m_propkind.begin(), m_reskind);
}

/*
//static
*/
xui_method_explain(onity_propnodemodule, get_image,		void*			)( void* userptr )
{
	onity_propnodemodule* prop = (onity_propnodemodule*)userptr;
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, prop->get_2dsref());
	return imageref->GetImage();
}
xui_method_explain(onity_propnodemodule, set_image,		void			)( void* userptr, void* value )
{
	onity_propnodemodule* prop = (onity_propnodemodule*)userptr;
	NP2DSImage* image = (NP2DSImage*)value;
	npu32 resfile = (image == NULL) ? -1 : image->GetOwnedFile()->GetKey();
	npu32 imageid = (image == NULL) ? -1 : image->GetKey();
	NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, prop->get_2dsref());
	imageref->SetImage(resfile, imageid);
}