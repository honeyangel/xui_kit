#include "NPFileName.h"
#include "NPSourceTexture.h"
#include "NP2DSTextureCache.h"

#include "xui_global.h"
#include "onity_resource.h"
#include "onity_proptexture.h"

/*
//constructor
*/
xui_create_explain(onity_proptexture)( const std::wstring& full )
: onity_propfile(onity_resource::icon_texture, full)
{}

/*
//method
*/
xui_method_explain(onity_proptexture, get_texture,	NPSourceTexture*)( void )
{
	std::string fullname = xui_global::unicode_to_ascii(get_fullname());
	std::string pathname = NPFileNameHelper::PathName(fullname);
	std::string filename = NPFileNameHelper::FileName(fullname);
	u32 id = NP2DSTextureCache::GetIns()->GetTextureID(pathname, filename, true);

	return NPDynamicCast(NPSourceTexture, NP2DSTextureCache::GetIns()->GetTexture(id));
}

/*
//override
*/
xui_method_explain(onity_proptexture, get_dragtype, std::string		)( void )
{
	return "NPSourceTexture";
}
xui_method_explain(onity_proptexture, get_dragdata, void*			)( void )
{
	return get_texture();
}
xui_method_explain(onity_proptexture, ntf_modify,	void			)( void )
{
	std::string fullname = xui_global::unicode_to_ascii(get_fullname());
	std::string pathname = NPFileNameHelper::PathName(fullname);
	std::string filename = NPFileNameHelper::FileName(fullname);
	u32 id = NP2DSTextureCache::GetIns()->GetTextureID(pathname, filename, true);
	NP2DSTextureCache::GetIns()->ResTexture(id);
}