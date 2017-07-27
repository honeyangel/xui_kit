#include "renderer/CCTexture2D.h"

#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propctrl_asset.h"
#include "cocos_selector.h"
#include "cocos_mainform.h"
#include "cocos_filedata.h"
#include "cocos_project.h"
#include "cocos_propdata_texture.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_texture)( 
	xui_propkind*		kind, 
	const std::wstring& name, 
	get_func			userget, 
	set_func			userset, 
	void*				userptr )
: xui_propdata_object_func(kind, name, cocos_propctrl_asset::create, "Texture2D", cocos_selector::get_ptr, get_texicon, get_texname, userget, userset, userptr)
{
	xm_doubleclick += new xui_method_member<xui_method_args, cocos_propdata_texture>(this, &cocos_propdata_texture::on_doubleclick);
}

/*
//event
*/
xui_method_explain(cocos_propdata_texture, on_doubleclick,	void		)( xui_component* sender, xui_method_args& args )
{
	cocos2d::Texture2D* texture = (cocos2d::Texture2D*)get_value();
	if (texture)
	{
		std::wstring full = xui_global::ascii_to_unicode(texture->getFileName());
		std::wstring work = xui_global::get_workpath();
		if (full != L"dummy" && full.length() > work.length())
		{
			full.erase(0, work.length() + 1);
			std::wstring  path = cocos_filedata::get_path(full);
			std::wstring  file = cocos_filedata::get_file(full);
			path = path.substr(0, path.length() - 1);

			cocos_project* project = cocos_mainform::get_ptr()->get_project();
			project->loc_filenode(path, file, 0);
			cocos_mainform::get_ptr()->set_pageshow(project);
		}
	}
}

/*
//static
*/
xui_method_explain(cocos_propdata_texture, get_texicon,		xui_bitmap*	)( xui_propdata* propdata )
{
	return cocos_resource::icon_texture;
}
xui_method_explain(cocos_propdata_texture, get_texname,		std::wstring)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	cocos2d::Texture2D* texture = (cocos2d::Texture2D*)dataobject->get_value();
	if (texture)
	{
		std::wstring full = xui_global::ascii_to_unicode(texture->getFileName());
		return cocos_filedata::get_safe(full);
	}

	return L"None";
}