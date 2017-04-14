#include "onity_proproot.h"
#include "onity_treedata.h"

/*
//constructor
*/
xui_create_explain(onity_treedata)( xui_bitmap* icon, const std::wstring& text, bool flag, xui_proproot* prop )
: xui_treedata(text, icon, flag)
, m_prop(prop)
{
	onity_proproot* proproot = dynamic_cast<onity_proproot*>(m_prop);
	if (proproot)
		proproot->add_linkdata(this);
}
xui_create_explain(onity_treedata)( xui_bitmap* icon, xui_proproot* prop )
: xui_treedata(L"", icon)
, m_prop(prop)
{
	onity_proproot* proproot = dynamic_cast<onity_proproot*>(m_prop);
	if (proproot)
		proproot->add_linkdata(this);
}

/*
//destructor
*/
xui_delete_explain(onity_treedata)( void )
{
	onity_proproot* proproot = dynamic_cast<onity_proproot*>(m_prop);
	if (proproot)   
		proproot->del_linkdata(this);
}

/*
//method
*/
xui_method_explain(onity_treedata, get_prop, xui_proproot*	)( void )
{
	return m_prop;
}
xui_method_explain(onity_treedata, set_null, void			)( void )
{
	m_prop = NULL;
}