#include "NP2DSImage.h"
#include "NP2DSImageFile.h"
#include "NP2DSImageRef.h"
#include "xui_kindctrl.h"
#include "onity_propctrl_transref.h"
#include "onity_propimageref.h"

/*
//constructor
*/
xui_create_explain(onity_propimageref)( onity_propfile* propfile, NP2DSTransRef* transref )
: onity_proptransref(propfile, transref)
{
	m_reskind = new xui_propkind(this, L"Resource", "2DSFrameRes", xui_kindctrl::create, NULL, true);
	m_reskind->add_propdata(new onity_propdata_2dsasset(
		m_reskind, 
		L"Image", 
		xui_propctrl_object::create, 
		"NP2DSImage", 
		get_image, 
		set_image, 
		this));

	add_propkind(m_reskind);
}

/*
//method
*/
xui_method_explain(onity_propimageref, get_imageref,	NP2DSImageRef*	)( void )
{
	return NPDynamicCast(NP2DSImageRef, m_transref);
}

/*
//static
*/
xui_method_explain(onity_propimageref, get_image,		void*			)( void* userptr )
{
	onity_propimageref* propimageref = (onity_propimageref*)userptr;
	return propimageref->get_imageref()->GetImage();
}
xui_method_explain(onity_propimageref, set_image,		void			)( void* userptr, void* value )
{
	onity_propimageref* propimageref = (onity_propimageref*)userptr;
	NP2DSImage* image = (NP2DSImage*)value;
	npu32 resfile = (image == NULL) ? -1 : image->GetOwnedFile()->GetKey();
	npu32 imageid = (image == NULL) ? -1 : image->GetKey();
	propimageref->get_imageref()->SetImage(resfile, imageid);
}