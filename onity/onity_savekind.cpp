#include "onity_propfile.h"
#include "onity_savectrl.h"
#include "onity_savekind.h"

/*
//constructor
*/
xui_create_explain(onity_savekind)( xui_proproot* root, onity_propfile* propfile )
: xui_propkind(root, L"", "Save&Load", onity_savectrl::create, NULL, true, false, true)
, m_propfile(propfile)
{}

/*
//method
*/
xui_method_explain(onity_savekind, get_propfile, onity_propfile*)( void )
{
	return m_propfile;
}