#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propctrl_asset.h"
#include "cocos_selector.h"
#include "cocos_mainform.h"
#include "cocos_propfile.h"
#include "cocos_filedata.h"
#include "cocos_project.h"
#include "cocos_propdata_shader.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_shader)(
	xui_propkind*		kind,
	const std::wstring& name,
	const std::string&  droptype,
	get_func			userget,
	set_func			userset,
	void*				userptr)
	: xui_propdata_object_func(kind, name, xui_propctrl_object::create, droptype, cocos_selector::get_ptr, get_icon, get_name, userget, userset, userptr)
{
	xm_doubleclick += new xui_method_member<xui_method_args, cocos_propdata_shader>(this, &cocos_propdata_shader::on_doubleclick);
}

/*
//event
*/
xui_method_explain(cocos_propdata_shader, on_doubleclick,	void		)( xui_component* sender, xui_method_args& args )
{
	cocos_propfile* prop = (cocos_propfile*)get_value();
	if (prop)
	{
		std::wstring full = prop->get_fullname();
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		project->loc_filenode(path, file, 0);
		cocos_mainform::get_ptr()->set_pageshow(project);
	}
}

/*
//static
*/
xui_method_explain(cocos_propdata_shader, get_icon,			xui_bitmap*	)( xui_propdata* propdata )
{
	return cocos_resource::icon_game;
}
xui_method_explain(cocos_propdata_shader, get_name,			std::wstring)( xui_propdata* propdata )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	cocos_propfile* prop = (cocos_propfile*)dataobject->get_value();
	if (prop)
	{
		std::wstring full = prop->get_fullname();
		return cocos_filedata::get_file(full);
	}

	return L"None";
}