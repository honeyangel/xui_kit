#include "onity_treedata.h"

/*
//constructor
*/
xui_create_explain(onity_treedata)( xui_proproot* prop )
: xui_treedata()
, m_prop(prop)
{}

/*
//method
*/
xui_method_explain(onity_treedata, get_prop, xui_proproot*)( void )
{
	return m_prop;
}