#include "xui_global.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "cocos_proptexture.h"
#include "cocos_propatlas.h"
#include "cocos_propparticle.h"
#include "cocos_propmaterial.h"
#include "cocos_propvertshader.h"
#include "cocos_propfragshader.h"
#include "cocos_propttf.h"
#include "cocos_propfnt.h"
#include "cocos_pathdata.h"
#include "cocos_filedata.h"
#include "cocos_pathctrl.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_proppath.h"

/*
//constructor
*/
xui_create_explain(cocos_proppath)( xui_bitmap* icon, const std::wstring& full )
: cocos_propfile(icon, full)
, m_pathroll(0)
, m_fileroll(10)
, m_viewfile(NULL)
{
	m_basekind->set_icon(cocos_resource::icon_folder);
}

/*
//destructor
*/
xui_delete_explain(cocos_proppath)( void )
{
	for (u32 i = 0; i < m_fileprop.size(); ++i)
		delete m_fileprop[i];

	for (u32 i = 0; i < m_pathprop.size(); ++i)
		delete m_pathprop[i];
}

/*
//method
*/
xui_method_explain(cocos_proppath, get_pathroll,	s32						)( void ) const
{
	return m_pathroll;
}
xui_method_explain(cocos_proppath, set_pathroll,	void					)( s32 value )
{
	m_pathroll = value;
}
xui_method_explain(cocos_proppath, get_fileroll,	s32						)( void ) const
{
	return m_fileroll;
}
xui_method_explain(cocos_proppath, set_fileroll,	void					)( s32 value )
{
	m_fileroll = value;
}
xui_method_explain(cocos_proppath, get_viewfile,	cocos_propfile*			)( void )
{
	return m_viewfile;
}
xui_method_explain(cocos_proppath, set_viewfile,	void					)( cocos_propfile* viewfile )
{
	m_viewfile = viewfile;
}
xui_method_explain(cocos_proppath, has_fileprop,	bool					)( cocos_propfile* proproot )
{
	xui_proproot_vec::iterator itor = std::find(m_fileprop.begin(), m_fileprop.end(), proproot);
	return itor != m_fileprop.end();
}
xui_method_explain(cocos_proppath, get_fileprop,	const xui_proproot_vec&	)( void ) const
{
	return m_fileprop;
}
xui_method_explain(cocos_proppath, add_fileprop,	void					)( cocos_propfile* proproot )
{
	if (has_fileprop(proproot) == false)
		m_fileprop.insert(m_fileprop.begin(), proproot);
}
xui_method_explain(cocos_proppath, del_fileprop,	void					)( cocos_propfile* proproot )
{
	xui_proproot_vec::iterator itor = std::find(m_fileprop.begin(), m_fileprop.end(), proproot);
	if (itor != m_fileprop.end())
	{
		m_fileprop.erase(itor);
	}
}
xui_method_explain(cocos_proppath, get_pathprop,	const xui_proproot_vec&	)( void ) const
{
	return m_pathprop;
}
xui_method_explain(cocos_proppath, add_pathprop,	void					)( cocos_proppath* proproot )
{
	m_pathprop.insert(m_pathprop.begin(), proproot);
}
xui_method_explain(cocos_proppath, ini_treeprop,	void					)( xui_treenode* root )
{
	std::vector<std::wstring> filevec = xui_global::get_file(m_fullname);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		std::wstring	full = (m_fullname.length() > 0) ? m_fullname+L"/"+filevec[i] : filevec[i];;
		cocos_propfile* prop = new_fileprop(full);
		if (prop)
		{
			m_fileprop.push_back(prop);
		}		
	}

	std::vector<std::wstring> pathvec = xui_global::get_path(m_fullname);
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		std::wstring	full = (m_fullname.length() > 0) ? m_fullname+L"/"+pathvec[i] : pathvec[i];
		cocos_proppath* prop = new_pathprop(full);
		if (prop)
		{
			xui_treenode* node = root->add_leafnode(root->get_leafnodecount(), new cocos_pathdata(cocos_resource::icon_folder, full, prop));
			m_pathprop.push_back(prop);
			prop->ini_treeprop(node);
		}
	}
}
xui_method_explain(cocos_proppath, refresh,			void					)( void )
{
	for (u32 i = 0; i < m_pathprop.size(); ++i)
	{
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(m_pathprop[i]);
		prop->refresh();
	}

	std::vector<std::wstring> filevec = xui_global::get_file(m_fullname);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		std::wstring full = (m_fullname.length() > 0) ? m_fullname+L"/"+filevec[i] : filevec[i];
		bool has_exist = false;
		for (xui_proproot_vec::iterator itor = m_fileprop.begin(); itor != m_fileprop.end(); ++itor)
		{
			if (dynamic_cast<cocos_propfile*>(*itor)->get_fullname() == full)
			{
				has_exist = true;
				break;
			}
		}

		if (has_exist)
			continue;

		cocos_propfile* prop = new_fileprop(full);
		if (prop)
		{
			m_fileprop.push_back(prop);
		}
	}

	xui_treeview* view = cocos_mainform::get_ptr()->get_project()->get_pathview();
	xui_treedata* data = get_linkdata(view);
	xui_treenode* root = data->get_node();
	std::vector<std::wstring> pathvec = xui_global::get_path(m_fullname);
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		std::wstring full = (m_fullname.length() > 0) ? m_fullname+L"/"+pathvec[i] : pathvec[i];
		bool has_exist = false;
		for (xui_proproot_vec::iterator itor = m_pathprop.begin(); itor != m_pathprop.end(); ++itor)
		{
			if (dynamic_cast<cocos_proppath*>(*itor)->get_fullname() == full)
			{
				has_exist = true;
				break;
			}
		}

		if (has_exist)
			continue;

		cocos_proppath* prop = new_pathprop(full);
		if (prop)
		{
			xui_treenode*	node = root->add_leafnode(root->get_leafnodecount(), new cocos_pathdata(cocos_resource::icon_folder, full, prop));
			m_pathprop.push_back(prop);
			prop->ini_treeprop(node);
		}
	}
}
xui_method_explain(cocos_proppath, new_fileprop,	cocos_propfile*			)( const std::wstring& full )
{
	std::wstring suff = cocos_filedata::get_suff(full);

	if (suff == L".png")
	{
		std::wstring temp = cocos_filedata::get_path(full) + cocos_filedata::get_safe(full) + L".plist";
		if (xui_global::has_file(temp) && cocos_filedata::was_particle(temp) == false)
			return NULL;
	}

	cocos_propfile* propfile = NULL;
	if (cocos_filedata::was_particle(full))	
		propfile = new cocos_propparticle(full);
	else
	{
		if		(suff == L".png")			propfile = new cocos_proptexture	(full);
		else if (suff == L".plist")			propfile = new cocos_propatlas		(full);
		else if (suff == L".mat")			propfile = new cocos_propmaterial	(full);
		else if (suff == L".vert")			propfile = new cocos_propvertshader	(full);
		else if (suff == L".frag")			propfile = new cocos_propfragshader	(full);
		else if (suff == L".ttf")			propfile = new cocos_propttf		(full);
		else if (suff == L".fnt")			propfile = new cocos_propfnt		(full);
	}
	//else if (suff == L".npSprite")		propfile = new cocos_propsprite		(full);
	//else if (suff == L".npAction")		propfile = new cocos_propaction		(full);
	//else if (suff == L".npCourse")		propfile = new cocos_propcourse		(full);
	//else if (suff == L".particle")		propfile = new cocos_propparticle	(full);
	//else if (suff == L".controller")	propfile = new cocos_propcontroller	(full);
	//else if (suff == L".json")			propfile = new cocos_propjsones		(full);

	if (propfile && xui_global::has_file(full+L".tmp"))
		cocos_mainform::get_ptr()->add_backupfile(propfile);

	return propfile;
}
xui_method_explain(cocos_proppath, new_pathprop,	cocos_proppath*			)( const std::wstring& full )
{
	if (cocos_mainform::get_ptr()->was_ignorepath(full))
		return NULL;

	return new cocos_proppath(cocos_resource::icon_folder, full);
}