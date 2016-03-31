#include "onity_pathctrl.h"
#include "onity_resource.h"
#include "onity_proppath.h"

/*
//constructor
*/
xui_create_explain(onity_proppath)( xui_proproot* root, const std::wstring& name, onity_pathdata* pathdata )
: xui_propkind(root, name, "Path", onity_pathctrl::create, onity_resource::icon_folder, true)
, m_pathdata(pathdata)
{}

/*
//method
*/
xui_method_explain(onity_proppath, get_pathdata, onity_pathdata*)( void ) const
{
	return m_pathdata;
}