#include "onity_timedata.h"

/*
//constructor
*/
xui_create_explain(onity_timedata)( xui_proproot* prop )
: xui_timedata()
, m_prop(prop)
{}

/*
//method
*/
xui_method_explain(onity_timedata, get_prop, xui_proproot*)( void )
{
	return m_prop;
}