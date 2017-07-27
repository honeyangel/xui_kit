#include "cocos_resource.h"
#include "cocos_propmaterial.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propfragshader.h"

/*
//constructor
*/
xui_create_explain(cocos_propfragshader)(const std::wstring& full)
: cocos_propfile(cocos_resource::icon_setting, full)
{}

/*
//method
*/
xui_method_explain(cocos_propfragshader, ntf_modify,	void		)( void )
{
	xui_proproot_vec filevec;
	cocos_mainform::get_ptr()->get_project()->get_pathfile(FILTER_MATERIAL, filevec);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		cocos_propmaterial* prop = dynamic_cast<cocos_propmaterial*>(filevec[i]);
		if (prop && prop->get_fragshader() == this)
			prop->ntf_fragshadermodify();
	}
}
xui_method_explain(cocos_propfragshader, get_dragtype,	std::string	)(void)
{
	return "FragShader";
}
xui_method_explain(cocos_propfragshader, get_dragdata,	void*		)(void)
{
	return this;
}