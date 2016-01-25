#include "xui_itemtag.h"

/*
//constructor
*/
xui_create_explain(xui_itemtag)( const std::wstring& text )
{
	m_text = text;
	m_icon = NULL;
}
xui_create_explain(xui_itemtag)( const std::wstring& text, xui_bitmap* icon )
{
	m_icon = icon;
	m_text = text;
}

/*
//virtual
*/
xui_method_explain(xui_itemtag, get_text, std::wstring		)( void ) 
{
	return m_text;
}
xui_method_explain(xui_itemtag, get_icon, xui_bitmap*		)( void )
{
	return m_icon;
}