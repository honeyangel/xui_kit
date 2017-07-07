#include "cocos_propfile.h"
#include "cocos_savectrl.h"
#include "cocos_savekind.h"

/*
//constructor
*/
xui_create_explain(cocos_savekind)( xui_proproot* root, cocos_propfile* propfile )
: xui_propkind(root, L"", "Save&Load", cocos_savectrl::create, NULL, true, false, true)
, m_propfile(propfile)
{}

/*
//method
*/
xui_method_explain(cocos_savekind, get_propfile, cocos_propfile*)( void )
{
	return m_propfile;
}