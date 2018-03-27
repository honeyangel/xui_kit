#include "ShaderRect.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "cocos_propmaterial.h"
#include "cocos_filedata.h"
#include "cocos_project.h"
#include "cocos_mainform.h"
#include "cocos_propdata_material.h"
#include "cocos_propnodeshaderrect.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeshaderrect)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_matfile(NULL)
, m_shaderrect(dynamic_cast<cocos2d::ui::ShaderRect*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, NULL);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
	add_custom		();
	add_shaderrect	();
}

/*
//method
*/
xui_method_explain(cocos_propnodeshaderrect, get_shaderrect,	cocos2d::ui::ShaderRect*)( void )
{
	return m_shaderrect;
}
xui_method_explain(cocos_propnodeshaderrect, add_shaderrect,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "ShaderRect", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeshaderrect>(this, &cocos_propnodeshaderrect::on_propchanged);
	kind->add_propdata(new cocos_propdata_material(
		kind,
		L"Material",
		get_material,
		set_material,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodeshaderrect, get_matfile,		cocos_propfile*			)( void )
{
	if (m_matfile == NULL && m_shaderrect->getCustomMaterial().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_shaderrect->getCustomMaterial());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_matfile = project->get_pathfile(path, file);
	}

	return m_matfile;
}
xui_method_explain(cocos_propnodeshaderrect, set_matfile,		void					)( cocos_propfile* file )
{
	m_matfile = file;
	if (m_matfile)
	{
		m_shaderrect->setCustomMaterial(xui_global::unicode_to_ascii(m_matfile->get_fullname()));
	}
	else
	{
		m_shaderrect->setCustomMaterial("");
	}
}

/*
//static
*/
xui_method_explain(cocos_propnodeshaderrect, get_material,		void*					)( void* userptr )
{
	cocos_propnodeshaderrect* prop = (cocos_propnodeshaderrect*)userptr;
	return prop->get_matfile();
}
xui_method_explain(cocos_propnodeshaderrect, set_material,		void					)( void* userptr, void* value )
{
	cocos_propnodeshaderrect* prop = (cocos_propnodeshaderrect*)userptr;
	prop->set_matfile((cocos_propfile*)value);
}

/*
//override
*/
xui_method_explain(cocos_propnodeshaderrect, on_propchanged,	void					)( xui_component* sender, xui_method_propdata& args )
{
	cocos_propnodebase::on_propchanged(sender, args);
	m_shaderrect->setShaderDirty(true);
}