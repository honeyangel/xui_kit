#include "cocostudio/ActionTimeline/CSLoader.h"
#include "2d/CCNode.h"

#include "xui_global.h"
#include "cocos_propnodelayer.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_scene.h"
#include "cocos_hierarchy.h"
#include "cocos_filedata.h"
#include "cocos_propcsd.h"

/*
//constructor
*/
xui_create_explain(cocos_propcsd)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_layer, full)
, m_node(NULL)
, m_prop(NULL)
, m_modify(false)
{}

/*
//destructor
*/
xui_delete_explain(cocos_propcsd)( void )
{
	delete m_prop;
}

/*
//method
*/
xui_method_explain(cocos_propcsd, get_node,		cocos2d::Node*		)( void )
{
	if (m_node == NULL)
	{
		if (xui_global::has_file(m_fullname))
			m_node = cocos2d::CSLoader::createNodeWithXMLFile(xui_global::unicode_to_ascii(m_fullname));
		else
		{
			m_node = cocos2d::Node::create();
			m_node->setName(xui_global::unicode_to_ascii(cocos_filedata::get_safe(m_fullname)));
			m_node->setContentSize(cocos2d::Size(640, 1136));
		}
	}
	if (m_prop == NULL)
	{
		m_prop = cocos_propnodebase::new_prop(this, m_node);
	}

	return m_node;
}
xui_method_explain(cocos_propcsd, get_prop,		cocos_propnodebase*	)( void )
{
	get_node();
	return m_prop;
}
xui_method_explain(cocos_propcsd, set_modify,	void				)( bool flag )
{
	m_modify = flag;
}
xui_method_explain(cocos_propcsd, ntf_rename,	void				)( const std::wstring& last, const std::wstring& curr )
{
	cocos_propfile::ntf_rename(last, curr);
	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene(this);
	if (scene)
	{
		scene->ini_namectrl(cocos_resource::icon_scene, cocos_filedata::get_safe(m_fullname));
	}
}
xui_method_explain(cocos_propcsd, was_modify,	bool				)( void )
{
	return m_modify;
}
xui_method_explain(cocos_propcsd, get_dragtype, std::string			)( void )
{
	return "CSD";
}
xui_method_explain(cocos_propcsd, get_dragdata, void*				)( void )
{
	return this;
}
xui_method_explain(cocos_propcsd, load,			void				)( void )
{
	m_modify = false;

	cocos_scene* scene = cocos_mainform::get_ptr()->get_scene(this);
	if (scene)
		scene->set_editprop(NULL);

	delete m_prop;
	m_node = NULL;
	m_prop = NULL;

	if (scene)
		scene->set_editprop(this);

	if (cocos_mainform::get_ptr()->get_scene() == scene)
	{
		cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
		hierarchy->reset();
	}
}
xui_method_explain(cocos_propcsd, save_as,		void				)( const std::wstring& fullname, bool modify )
{
	m_modify = modify;
	get_node();

	//TODO
	FILE* file = fopen(xui_global::unicode_to_ascii(fullname).c_str(), "w");
	if (file)
	{
		fclose(file);
	}
}
xui_method_explain(cocos_propcsd, on_detach,	void				)( const xui_proproot_vec& proproot )
{}