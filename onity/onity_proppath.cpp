#include "onity_pathctrl.h"
#include "onity_resource.h"
#include "onity_proppath.h"

/*
//constructor
*/
xui_create_explain(onity_proppath)( const std::wstring& full )
: onity_propfile(full)
{
	m_basekind->set_icon(onity_resource::icon_folder);
	//m_pathkind = new xui_propkind(this, L"", "Path", onity_pathctrl::create, NULL, true, false);
	//add_propkind(m_pathkind);
}
