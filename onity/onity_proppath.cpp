#include "xui_global.h"
#include "onity_filedata.h"
#include "onity_pathctrl.h"
#include "onity_resource.h"
#include "onity_proppath.h"

/*
//constructor
*/
xui_create_explain(onity_proppath)( const std::wstring& full )
: onity_propfile(onity_resource::icon_folder, full)
, m_pathroll(0)
, m_fileroll(10)
, m_viewfile(NULL)
{
	m_basekind->set_icon(onity_resource::icon_folder);
	//m_pathkind = new xui_propkind(this, L"", "Path", onity_pathctrl::create, NULL, true, false);
	//add_propkind(m_pathkind);

	std::vector<std::wstring> filevec = xui_global::get_file(full);
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		std::wstring  name = full+L"/"+filevec[i];
		xui_proproot* prop = onity_filedata::new_fileprop(name);
		if (prop)
		{
			m_fileprop.push_back(prop);
		}
	}
}

/*
//destructor
*/
xui_delete_explain(onity_proppath)( void )
{
	for (u32 i = 0; i < m_fileprop.size(); ++i)
		delete m_fileprop[i];
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
xui_method_explain(onity_proppath, get_fileprop,	const xui_proproot_vec&	)( void ) const
{
	return m_fileprop;
}
xui_method_explain(onity_proppath, add_fileprop,	void					)( onity_propfile* proproot )
{
	m_fileprop.insert(m_fileprop.begin(), proproot);
}
