#include "NPFileName.h"
#include "NPSourceTexture.h"
#include "NP2DSTextureCache.h"

#include "xui_global.h"
#include "onity_resource.h"
#include "onity_propctrl_transref.h"
#include "onity_selector.h"
#include "onity_propdata_texture.h"

/*
//constructor
*/
xui_create_explain(onity_propdata_texture)( 
	xui_propkind*		kind, 
	const std::wstring& name, 
	get_func			userget, 
	set_func			userset, 
	void*				userptr )
: xui_propdata_object_func(kind, name, onity_propctrl_asset::create, "NPSourceTexture", onity_selector::get_ptr, get_texicon, get_texname, userget, userset, userptr)
{}

/*
//static
*/
xui_method_explain(onity_propdata_texture, get_texicon, xui_bitmap*	)( xui_propdata* propdata )
{
	return onity_resource::icon_texture;
}
xui_method_explain(onity_propdata_texture, get_texname, std::wstring)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	NPSourceTexture* texture = (NPSourceTexture*)dataobject->get_value();
	if (texture)
	{
		npu32 id = texture->GetKey();
		std::string name = NPFileNameHelper::SafeName(NP2DSTextureCache::GetIns()->GetTextureFN(id));
		return xui_global::ascii_to_unicode(name);
	}

	return L"None";
}