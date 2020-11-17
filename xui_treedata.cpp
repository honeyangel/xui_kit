#include "xui_treedata.h"

xui_treedata::xui_treedata( void )
{
	m_node = NULL;
	m_icon = NULL;
	m_flag = false;
}

xui_treedata::xui_treedata( const std::wstring& text )
{
	m_node = NULL;
	m_icon = NULL;
	m_text = text;
	m_flag = false;
}

xui_treedata::xui_treedata( const std::wstring& text, xui_bitmap* icon )
{
	m_node = NULL;
	m_icon = icon;
	m_text = text;
	m_flag = false;
}

xui_treedata::xui_treedata( const std::wstring& text, xui_bitmap* icon, bool flag )
{
	m_node = NULL;
	m_icon = icon;
	m_text = text;
	m_flag = flag;
}

xui_treenode* xui_treedata::get_node( void )
{
	return m_node;
}

void xui_treedata::set_node( xui_treenode* node )
{
	m_node = node;
}

bool xui_treedata::get_flag( u32 index )
{
	return m_flag;
}

xui_bitmap* xui_treedata::get_icon( u32 index )
{
	return m_icon;
}

std::wstring xui_treedata::get_text( u32 index )
{
	return m_text;
}

xui_family xui_treedata::get_textfont( u32 index )
{
	return xui_family::k_default;
}

xui_family_render xui_treedata::get_textdraw( u32 index )
{
	return xui_family_render::k_default;
}

void xui_treedata::set_text( u32 index, const std::wstring& text )
{
	m_text = text;
}

void xui_treedata::set_flag( u32 index, bool flag )
{
	m_flag = flag;
}