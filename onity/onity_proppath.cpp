#include "xui_global.h"
#include "onity_filedata.h"
#include "onity_pathctrl.h"
#include "onity_resource.h"
#include "onity_proppath.h"

/*
//constructor
*/
xui_create_explain(onity_proppath)( const std::wstring& full )
: onity_propfile(full)
, m_slider(0)
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
xui_method_explain(onity_proppath, get_slider,		s32						)( void ) const
{
	return m_slider;
}
xui_method_explain(onity_proppath, set_slider,		void					)( s32 slider )
{
	m_slider = slider;
}
xui_method_explain(onity_proppath, get_fileprop,	const xui_proproot_vec&	)( void ) const
{
	return m_fileprop;
}
xui_method_explain(onity_proppath, add_fileprop,	void					)( xui_proproot* proproot )
{
	m_fileprop.insert(m_fileprop.begin(), proproot);
}
