#include "2d/CCSpriteFrame.h"
#include "renderer/CCTexture2D.h"

#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propctrl_asset.h"
#include "cocos_selector.h"
#include "cocos_mainform.h"
#include "cocos_filedata.h"
#include "cocos_project.h"
#include "cocos_propdata_image.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_image)(
	xui_propkind*		kind,
	const std::wstring& name,
	get_func			userget,
	set_func			userset,
	void*				userptr)
: xui_propdata_object_func(kind, name, cocos_propctrl_asset::create, "", cocos_selector::get_ptr, get_icon, get_name, userget, userset, userptr)
{
	m_droptype.clear();
	add_droptype("Texture2D");
	add_droptype("SpriteFrame");
	xm_doubleclick += new xui_method_member<xui_method_args, cocos_propdata_image>(this, &cocos_propdata_image::on_doubleclick);
}

/*
//event
*/
xui_method_explain(cocos_propdata_image, on_doubleclick,	void		)( xui_component* sender, xui_method_args& args )
{
	cocos2d::Ref*			value	= (cocos2d::Ref*)get_value();
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*	>(value);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(value);

	std::wstring full;
	std::wstring path;
	std::wstring file;
	std::wstring name;
	if (texture)
	{
		std::wstring work = xui_global::get_workpath();
		full = xui_global::ascii_to_unicode(texture->getPath());
		if (full == L"dummy" || full.length() <= work.length())
			return;

		full.erase(0, work.length()+1);
		path = cocos_filedata::get_path(full);
		file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length()-1);

	}
	if (frame)
	{
		full = xui_global::ascii_to_unicode(frame->getFileName());
		path = cocos_filedata::get_path(full);
		file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length()-1);
		name = xui_global::ascii_to_unicode(frame->getName());
	}

	cocos_project* project = cocos_mainform::get_ptr()->get_project();
	project->loc_filenode(path, file, name);
	cocos_mainform::get_ptr()->set_pageshow(project);
}

/*
//static
*/
xui_method_explain(cocos_propdata_image, get_icon,			xui_bitmap*	)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);

	cocos2d::Ref*			value	= (cocos2d::Ref*)dataobject->get_value();
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(value);
	if (texture && texture->getPath() != "dummy")
		return cocos_resource::icon_texture;

	return cocos_resource::icon_module;
}
xui_method_explain(cocos_propdata_image, get_name,			std::wstring)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);

	cocos2d::Ref*			value	= (cocos2d::Ref*)dataobject->get_value();
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*	>(value);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(value);
	if (texture)
	{
		std::wstring full = xui_global::ascii_to_unicode(texture->getPath());
		return cocos_filedata::get_file(full);
	}
	if (frame)
	{
		return xui_global::ascii_to_unicode(frame->getName());
	}

	return L"None";
}