#include "xui_itemtag.h"

/*
//constructor
*/
xui_create_explain(xui_itemtag)( void )
{}
xui_create_explain(xui_itemtag)( const std::wstring& text )
{
	m_text = text;
}

/*
//virtual
*/
xui_method_explain(xui_itemtag, get_text, std::wstring)( void ) 
{
	return m_text;
}