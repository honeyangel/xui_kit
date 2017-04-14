#include "onity_propleaf.h"
#include "onity_timedata.h"

/*
//constructor
*/
xui_create_explain(onity_timedata)( xui_bitmap* icon, xui_proproot* prop )
: xui_timedata()
, m_prop(prop)
{
	m_icon = icon;

	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propleaf)   propleaf->add_linkdata(this);
}

/*
//destructor
*/
xui_delete_explain(onity_timedata)( void )
{
	onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(m_prop);
	if (propleaf)   propleaf->del_linkdata(this);
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