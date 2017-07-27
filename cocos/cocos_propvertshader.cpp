#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propmaterial.h"
#include "cocos_propvertshader.h"

/*
//constructor
*/
xui_create_explain(cocos_propvertshader)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_setting, full)
{}

/*
//method
*/
xui_method_explain(cocos_propvertshader, ntf_modify,	void		)( void )
{
	xui_proproot_vec filevec;
	cocos_mainform::get_ptr()->get_project()->get_pathfile(FILTER_MATERIAL, filevec);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		cocos_propmaterial* prop = dynamic_cast<cocos_propmaterial*>(filevec[i]);
		if (prop && prop->get_vertshader() == this)
			prop->ntf_vertshadermodify();
	}
}
xui_method_explain(cocos_propvertshader, get_dragtype,	std::string	)( void )
{
	return "VertShader";
}
xui_method_explain(cocos_propvertshader, get_dragdata,	void*		)( void )
{
	return this;
}