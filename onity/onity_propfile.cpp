#include "onity_filedata.h"
#include "onity_filectrl.h"
#include "onity_resource.h"
#include "onity_propfile.h"

/*
//constructor
*/
xui_create_explain(onity_propfile)( const std::wstring& full )
: xui_proproot()
, m_fullname(full)
{
	m_basekind = new xui_propkind(this, onity_filedata::get_safe(full), "File", onity_filectrl::create, onity_resource::icon_file, true);
	add_propkind(m_basekind);
}

/*
//method
*/
xui_method_explain(onity_propfile, get_full,	const std::wstring&	)( void ) const
{
	return m_fullname;
}

/*
//virtual
*/
xui_method_explain(onity_propfile, was_modify,	bool				)( void )
{
	return false;
}
xui_method_explain(onity_propfile, load,		void				)( void )
{}
xui_method_explain(onity_propfile, save,		void				)( void )
{}
xui_method_explain(onity_propfile, ntf_rename,	void				)( const std::wstring& last, const std::wstring& curr )
{
	int npos = m_fullname.find(last);
	m_fullname.replace(npos, last.length(), curr);

	m_basekind->set_name(onity_filedata::get_safe(m_fullname));
	xui_kindctrl* kindctrl = m_basekind->get_ctrl();
	if (kindctrl)
		kindctrl->refresh();
}