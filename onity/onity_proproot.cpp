#include "onity_treedata.h"
#include "onity_timedata.h"
#include "onity_proproot.h"

/*
//constructor
*/
xui_create_explain(onity_proproot)( void )
: xui_proproot()
, m_linkdata(NULL)
{}

/*
//destructor
*/
xui_delete_explain(onity_proproot)( void )
{
	onity_treedata* treedata = dynamic_cast<onity_treedata*>(m_linkdata);
	onity_timedata* timedata = dynamic_cast<onity_timedata*>(m_linkdata);
	if (treedata)   treedata->set_null();
	if (timedata)	timedata->set_null();
}

/*
//method
*/
xui_method_explain(onity_proproot, get_linkdata,	xui_treedata*	)( void )
{
	return m_linkdata;
}
xui_method_explain(onity_proproot, set_linkdata,	void			)( xui_treedata* linkdata )
{
	m_linkdata = linkdata;
}