#include "NPFileName.h"
#include "NPPixelFormat.h"
#include "NPSourceTexture.h"
#include "NP2DSImageFile.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSTextureCache.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propctrl_object.h"
#include "xui_propctrl_vector.h"
#include "onity_propctrl_transref.h"
#include "onity_selector.h"
#include "onity_resource.h"
#include "onity_filedata.h"
#include "onity_propimage.h"
#include "onity_propmodule.h"

/*
//constructor
*/
xui_create_explain(onity_propmodule)( const std::wstring& full )
: onity_prop2dsres(onity_resource::icon_module, full)
{
	m_texkind = new xui_propkind(this, L"", "ImageFile", xui_kindctrl::create, NULL, true, false);

	std::map<s32, std::wstring> textmap;
	textmap[0]							  = L"R8G8B8A8";
	textmap[NPPixelFormat::PF_R5G6B5	] = L"R5G6B5";
	textmap[NPPixelFormat::PF_R5G5B5A1	] = L"R5G5B5A1";
	textmap[NPPixelFormat::PF_R4G4B4A4	] = L"R4G4B4A4";
	textmap[NPPixelFormat::PF_RGBAPVRTC4] = L"Compress";

	m_texkind->add_propdata(new xui_propdata_object_func(
		m_texkind,
		L"Texture",
		onity_propctrl_asset::create,
		"NPSourceTexture",
		onity_selector::get_ptr,
		get_texicon,
		get_texname,
		get_texfile,
		set_texfile,
		this));
	m_texkind->add_propdata(new xui_propdata_bool(
		m_texkind,
		L"Texture Split",
		xui_propctrl_bool::create,
		get_texsplit,
		set_texsplit,
		this));
	m_texkind->add_propdata(new xui_propdata_enum_func(
		m_texkind,
		L"Texture Format",
		xui_propctrl_enum::create,
		textmap,
		get_texformat,
		set_texformat,
		this));
	m_texkind->add_propdata(new xui_propdata_number_func(
		m_texkind,
		L"Texture Width",
		xui_propctrl_number::create,
		get_texwidth,
		NULL,
		this, 
		NT_UNSIGNEDINT));
	m_texkind->add_propdata(new xui_propdata_number_func(
		m_texkind,
		L"Texture Height",
		xui_propctrl_number::create,
		get_texheight,
		NULL,
		this, 
		NT_UNSIGNEDINT));

	add_propkind(m_texkind);
}

/*
//override
*/
xui_method_explain(onity_propmodule, new_subprop,	xui_proproot*	)( u32 id )
{
	return new onity_propimage(this, id);
}
xui_method_explain(onity_propmodule, get_resfile,	NP2DSAssetFile*	)( void )
{
	if (m_resfile == -1)
	{
		std::string  fullname = xui_global::unicode_to_ascii(m_fullname);
		std::string  pathname = NPFileNameHelper::PathName(fullname);
		std::string  filename = NPFileNameHelper::FileName(fullname);
		m_resfile = NP2DSImageFileMgr::GetIns()->GetFileID(pathname, filename, true);
	}

	if (m_resfile != -1)
	{
		return NP2DSImageFileMgr::GetIns()->GetFile(m_resfile);
	}

	return NULL;
}

/*
//static
*/
xui_method_explain(onity_propmodule, get_texicon,	xui_bitmap*		)( xui_propdata* propdata )
{
	return onity_resource::icon_texture;
}
xui_method_explain(onity_propmodule, get_texname,	std::wstring	)( xui_propdata* propdata )
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
xui_method_explain(onity_propmodule, get_texwidth,	f64				)( void* userptr )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
	{
		NPTexture* texture = file->GetSCTex();
		if (texture)
			return (f64)texture->GetPixelWidth();
	}

	return 0;
}
xui_method_explain(onity_propmodule, get_texheight,	f64				)( void* userptr )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
	{
		NPTexture* texture = file->GetSCTex();
		if (texture)
			return (f64)texture->GetPixelHeight();
	}

	return 0;
}
xui_method_explain(onity_propmodule, get_texfile,	void*			)( void* userptr )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
		return file->GetSCTex();

	return NULL;
}
xui_method_explain(onity_propmodule, set_texfile,	void			)( void* userptr, void* value )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
	{
		NPSourceTexture* texture = (NPSourceTexture*)value;
		file->SetSCTexID(texture == NULL ? -1 : texture->GetKey());
	}
}
xui_method_explain(onity_propmodule, get_texsplit,	bool			)( void* userptr )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
		return file->WasSCTexSplited();

	return false;
}
xui_method_explain(onity_propmodule, set_texsplit,	void			)( void* userptr, bool  value )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
		file->SetSCTexSplited(value);
}
xui_method_explain(onity_propmodule, get_texformat, s32				)( void* userptr )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
		return (s32)file->GetSCTexConvert();

	return NPPixelFormat::PF_R8G8B8A8;
}
xui_method_explain(onity_propmodule, set_texformat, void			)( void* userptr, s32   value )
{
	onity_propmodule* prop = (onity_propmodule*)userptr;
	NP2DSImageFile*   file = (NP2DSImageFile*)prop->get_resfile();
	if (file)
		return file->SetSCTexConvert((npu32)value);
}