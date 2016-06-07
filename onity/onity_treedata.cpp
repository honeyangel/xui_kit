#include "onity_propfile.h"
#include "onity_propleaf.h"
#include "onity_treedata.h"

/*
//constructor
*/
xui_create_explain(onity_treedata)( xui_bitmap* icon, xui_proproot* prop )
: xui_treedata(L"", icon)
, m_prop(prop)
{
	onity_propfile* propfile = dynamic_cast<onity_propfile*>(m_prop);
	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propfile)   propfile->set_linkdata(this);
	if (propleaf)   propleaf->set_linkdata(this);
}

/*
//destructor
*/
xui_delete_explain(onity_treedata)( void )
{
	onity_propfile* propfile = dynamic_cast<onity_propfile*>(m_prop);
	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propfile)   propfile->set_linkdata(NULL);
	if (propleaf)   propleaf->set_linkdata(NULL);
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