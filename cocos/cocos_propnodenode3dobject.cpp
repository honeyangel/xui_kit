#include "Node3DObject.h"

#include "xui_kindctrl.h"
#include "xui_global.h"
#include "xui_propctrl.h"
#include "xui_numbbox.h"
#include "cocos_propfile.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propdata_c3b.h"
#include "cocos_propdata_material.h"
#include "cocos_propnodenode3dobject.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodenode3dobject)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_node3dobject(dynamic_cast<cocos2d::ui::Node3DObject*>(node))
, m_c3bfile(NULL)
, m_matfile(NULL)
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
	add_node3dobject();
}

/*
//method
*/
xui_method_explain(cocos_propnodenode3dobject, get_node3dobject,	cocos2d::ui::Node3DObject*	)( void )
{
	return m_node3dobject;
}
xui_method_explain(cocos_propnodenode3dobject, add_node3dobject,	void						)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "Node3DObject", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodenode3dobject>(this, &cocos_propnodenode3dobject::on_propchanged);
	kind->add_propdata(new cocos_propdata_c3b(
		kind,
		L"Mesh",
		get_mesh,
		set_mesh,
		this));
	kind->add_propdata(new cocos_propdata_material(
		kind,
		L"Material",
		get_material,
		set_material,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"PlayAnimation",
		xui_propctrl_bool::create,
		was_playanim,
		set_playanim,
		this));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Position3DX",
		xui_propctrl_number::create,
		get_position3dx,
		set_position3dx,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Position3DY",
		xui_propctrl_number::create,
		get_position3dy,
		set_position3dy,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Position3DZ",
		xui_propctrl_number::create,
		get_position3dz,
		set_position3dz,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Rotation3DX",
		xui_propctrl_number::create,
		get_rotation3dx,
		set_rotation3dx,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Rotation3DY",
		xui_propctrl_number::create,
		get_rotation3dy,
		set_rotation3dy,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Rotation3DZ",
		xui_propctrl_number::create,
		get_rotation3dz,
		set_rotation3dz,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Scale3DX",
		xui_propctrl_number::create,
		get_scale3dx,
		set_scale3dx,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Scale3DY",
		xui_propctrl_number::create,
		get_scale3dy,
		set_scale3dy,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Scale3DZ",
		xui_propctrl_number::create,
		get_scale3dz,
		set_scale3dz,
		this,
		NT_FLOAT));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodenode3dobject, get_c3bfile,			cocos_propfile*				)( void )
{
	if (m_c3bfile == NULL && m_node3dobject->getMesh().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_node3dobject->getMesh());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_c3bfile = project->get_pathfile(path, file);
	}

	return m_c3bfile;
}
xui_method_explain(cocos_propnodenode3dobject, set_c3bfile,			void						)( cocos_propfile* file )
{
	m_c3bfile = file;
	if (m_c3bfile)
	{
		m_node3dobject->setMesh(xui_global::unicode_to_ascii(m_c3bfile->get_fullname()));
	}
	else
	{
		//m_node3dobject->setMesh("");
	}
}
xui_method_explain(cocos_propnodenode3dobject, get_matfile,			cocos_propfile*				)( void )
{
	if (m_matfile == NULL && m_node3dobject->getMaterial().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_node3dobject->getMaterial());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_matfile = project->get_pathfile(path, file);
	}

	return m_matfile;
}
xui_method_explain(cocos_propnodenode3dobject, set_matfile,			void						)( cocos_propfile* file )
{
	m_matfile = file;
	if (m_matfile)
	{
		m_node3dobject->setMaterial(xui_global::unicode_to_ascii(m_c3bfile->get_fullname()));
	}
	else
	{
		//m_node3dobject->setMaterial("");
	}
}

/*
//static
*/
xui_method_explain(cocos_propnodenode3dobject, get_mesh,			void*						)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_c3bfile();
}
xui_method_explain(cocos_propnodenode3dobject, set_mesh,			void						)( void* userptr, void* value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->set_c3bfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodenode3dobject, get_material,		void*						)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_matfile();
}
xui_method_explain(cocos_propnodenode3dobject, set_material,		void						)( void* userptr, void* value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->set_matfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodenode3dobject, was_playanim,		bool						)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getPlayAnimation();
}
xui_method_explain(cocos_propnodenode3dobject, set_playanim,		void						)( void* userptr, bool  value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setPlayAnimation(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_position3dx,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getPosition3DX();
}
xui_method_explain(cocos_propnodenode3dobject, set_position3dx,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setPosition3DX(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_position3dy,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getPosition3DY();
}
xui_method_explain(cocos_propnodenode3dobject, set_position3dy,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setPosition3DY(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_position3dz,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getPosition3DZ();
}
xui_method_explain(cocos_propnodenode3dobject, set_position3dz,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setPosition3DZ(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_rotation3dx,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getRotation3DX();
}
xui_method_explain(cocos_propnodenode3dobject, set_rotation3dx,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setRotation3DX(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_rotation3dy,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getRotation3DY();
}
xui_method_explain(cocos_propnodenode3dobject, set_rotation3dy,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setRotation3DY(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_rotation3dz,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getRotation3DZ();
}
xui_method_explain(cocos_propnodenode3dobject, set_rotation3dz,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setRotation3DZ(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_scale3dx,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getScale3DX();
}
xui_method_explain(cocos_propnodenode3dobject, set_scale3dx,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setScale3DX(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_scale3dy,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getScale3DY();
}
xui_method_explain(cocos_propnodenode3dobject, set_scale3dy,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setScale3DY(value);
}
xui_method_explain(cocos_propnodenode3dobject, get_scale3dz,		f64							)( void* userptr )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	return prop->get_node3dobject()->getScale3DZ();
}
xui_method_explain(cocos_propnodenode3dobject, set_scale3dz,		void						)( void* userptr, f64   value )
{
	cocos_propnodenode3dobject* prop = (cocos_propnodenode3dobject*)userptr;
	prop->get_node3dobject()->setScale3DZ(value);
}