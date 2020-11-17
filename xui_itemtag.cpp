#include "xui_itemtag.h"

xui_itemtag::xui_itemtag( const std::wstring& text )
{
	m_text = text;
	m_icon = NULL;
}

xui_itemtag::xui_itemtag( const std::wstring& text, xui_bitmap* icon )
{
	m_icon = icon;
	m_text = text;
}

std::wstring xui_itemtag::get_text( void ) 
{
	return m_text;
}

xui_bitmap* xui_itemtag::get_icon( void )
{
	return m_icon;
}