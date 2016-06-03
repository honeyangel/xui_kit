#include "onity_propleaf.h"
#include "onity_timedata.h"

/*
//constructor
*/
xui_create_explain(onity_timedata)( xui_proproot* prop )
: xui_timedata()
, m_prop(prop)
{
	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propleaf)   propleaf->set_linkdata(this);
}

/*
//destructor
*/
xui_delete_explain(onity_timedata)( void )
{
	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propleaf)   propleaf->set_linkdata(NULL);
}

/*
//method
*/
xui_method_explain(onity_timedata, get_prop, xui_proproot*	)( void )
{
	return m_prop;
}
xui_method_explain(onity_timedata, set_null, void			)( void )
{
	m_prop = NULL;
}