#include "xui_proproot.h"
#include "onity_resource.h"
#include "onity_filedata.h"

/*
//constructor
*/
xui_create_explain(onity_filedata)( const std::wstring& full, xui_proproot* prop )
: onity_pathdata(full, false)
{
	m_icon = onity_resource::icon_file;
	m_prop = prop;
}

/*
//method
*/
xui_method_explain(onity_filedata, get_text,	std::wstring	)( u32 index )
{
	std::wstring temp = onity_pathdata::get_text(index);

	int npos  = temp.find_last_of(L'.');
	if (npos != -1)
		temp.erase(npos);

	return temp;
}

/*
//static
*/
xui_method_explain(onity_filedata, create_prop, xui_proproot*	)( const std::wstring& full )
{
	return new xui_proproot;
}