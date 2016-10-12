#include "onity_propnoderender.h"

/*
//constructor
*/
xui_create_explain(onity_propnoderender)( NPNode* node )
: onity_propedit()
, m_node(node)
{}

/*
//method
*/
xui_method_explain(onity_propnoderender, get_node, NPNode*)( void )
{
	return m_node;
}