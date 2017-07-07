#include "xui_global.h"
#include "xui_treenode.h"
#include "onity_propmodule.h"
#include "onity_propsprite.h"
#include "onity_propaction.h"
#include "onity_propcourse.h"
#include "onity_propjsones.h"
#include "onity_proptexture.h"
#include "onity_propparticle.h"
#include "onity_propcontroller.h"
#include "onity_pathdata.h"
#include "onity_filedata.h"
#include "onity_pathctrl.h"
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_proppath.h"

/*
//constructor
*/
xui_create_explain(onity_proppath)( xui_bitmap* icon, const std::wstring& full )
: onity_propfile(icon, full)
, m_pathroll(0)
, m_fileroll(10)
, m_viewfile(NULL)
{
	m_basekind->set_icon(onity_resource::icon_folder);
}

/*
//destructor
*/
xui_delete_explain(onity_proppath)( void )
{
	for (u32 i = 0; i < m_fileprop.size(); ++i)
		delete m_fileprop[i];

	for (u32 i = 0; i < m_pathprop.size(); ++i)
		delete m_pathprop[i];
}

/*
//method
*/
xui_method_explain(onity_proppath, get_pathroll,	s32						)( void ) const
{
	return m_pathroll;
}
xui_method_explain(onity_proppath, set_pathroll,	void					)( s32 value )
{
	m_pathroll = value;
}
xui_method_explain(onity_proppath, get_fileroll,	s32						)( void ) const
{
	return m_fileroll;
}
xui_method_explain(onity_proppath, set_fileroll,	void					)( s32 value )
{
	m_fileroll = value;
}
xui_method_explain(onity_proppath, get_viewfile,	onity_propfile*			)( void )
{
	return m_viewfile;
}
xui_method_explain(onity_proppath, set_viewfile,	void					)( onity_propfile* viewfile )
{
	m_viewfile = viewfile;
}
xui_method_explain(onity_proppath, has_fileprop,	bool					)( onity_propfile* proproot )
{
	xui_proproot_vec::iterator itor = std::find(m_fileprop.begin(), m_fileprop.end(), proproot);
	return itor != m_fileprop.end();
}
xui_method_explain(onity_proppath, get_fileprop,	const xui_proproot_vec&	)( void ) const
{
	return m_fileprop;
}
xui_method_explain(onity_proppath, add_fileprop,	void					)( onity_propfile* proproot )
{
	if (has_fileprop(proproot) == false)
		m_fileprop.insert(m_fileprop.begin(), proproot);
}
xui_method_explain(onity_proppath, del_fileprop,	void					)( onity_propfile* proproot )
{
	xui_proproot_vec::iterator itor = std::find(m_fileprop.begin(), m_fileprop.end(), proproot);
	if (itor != m_fileprop.end())
	{
		m_fileprop.erase(itor);
	}
}
xui_method_explain(onity_proppath, get_pathprop,	const xui_proproot_vec&	)( void ) const
{
	return m_pathprop;
}
xui_method_explain(onity_proppath, add_pathprop,	void					)( onity_proppath* proproot )
{
	m_pathprop.insert(m_pathprop.begin(), proproot);
}
xui_method_explain(onity_proppath, ini_treeprop,	void					)( xui_treenode* root )
{
	std::vector<std::wstring> filevec = xui_global::get_file(m_fullname);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		std::wstring	full = m_fullname+L"/"+filevec[i];
		onity_propfile* prop = new_fileprop(full);
		if (prop)
		{
			m_fileprop.push_back(prop);
		}		
	}

	//m_pathkind = new xui_propkind(this, L"", "Path", onity_pathctrl::create, NULL, true, false);
	//add_propkind(m_pathkind);

	std::vector<std::wstring> pathvec = xui_global::get_path(m_fullname);
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		std::wstring	full = (m_fullname.length() > 0) ? m_fullname+L"/"+pathvec[i] : pathvec[i];
		onity_proppath* prop = new_pathprop(full);
		xui_treenode*	node = root->add_leafnode(i, new onity_pathdata(onity_resource::icon_folder, full, prop));
		m_pathprop.push_back(prop);

		prop->ini_treeprop(node);
	}
}
xui_method_explain(onity_proppath, new_fileprop,	onity_propfile*			)( const std::wstring& full )
{
	std::wstring suff = onity_filedata::get_suff(full);

	u08 type = META_NONE;
	if		(suff == L".npModule") type = META_MODULE;
	else if (suff == L".npSprite") type = META_SPRITE;
	else if (suff == L".npAction") type = META_ACTION;
	else
	{}

	if (type != META_NONE)
		onity_prop2dsres::load_meta(type, xui_global::unicode_to_ascii(full));

	if (suff == L".png" && full.find(L"MODULE") != -1 && full.find(L"BIG") == -1)
		return NULL;

	onity_propfile* propfile = NULL;
	if		(suff == L".png")			propfile = new onity_proptexture	(full);
	else if (suff == L".npModule")		propfile = new onity_propmodule		(full);
	else if (suff == L".npSprite")		propfile = new onity_propsprite		(full);
	else if (suff == L".npAction")		propfile = new onity_propaction		(full);
	else if (suff == L".npCourse")		propfile = new onity_propcourse		(full);
	else if (suff == L".particle")		propfile = new onity_propparticle	(full);
	else if (suff == L".controller")	propfile = new onity_propcontroller	(full);
	else if (suff == L".json")			propfile = new onity_propjsones		(full);
	else
	{}

	if (propfile && xui_global::has_file(full+L".tmp"))
		onity_mainform::get_ptr()->add_backupfile(propfile);

	return propfile;
}
xui_method_explain(onity_proppath, new_pathprop,	onity_proppath*			)( const std::wstring& full )
{
	return new onity_proppath(onity_resource::icon_folder, full);
}