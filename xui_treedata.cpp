#include "xui_treedata.h"

/*
//constructor
*/
xui_create_explain(xui_treedata)( void )
{
	m_node = NULL;
	m_icon = NULL;
	m_flag = false;
}
xui_create_explain(xui_treedata)( const std::wstring& text )
{
	m_node = NULL;
	m_icon = NULL;
	m_text = text;
	m_flag = false;
}
xui_create_explain(xui_treedata)( const std::wstring& text, xui_bitmap* icon )
{
	m_node = NULL;
	m_icon = icon;
	m_text = text;
	m_flag = false;
}

/*
//method
*/
xui_method_explain(xui_treedata, get_node,		xui_treenode*	)( void )
{
	return m_node;
}
xui_method_explain(xui_treedata, set_node,		void			)( xui_treenode* node )
{
	m_node = node;
}

/*
//interface
*/
xui_method_explain(xui_treedata, get_flag,		bool			)( u32 index )
{
	return m_flag;
}
xui_method_explain(xui_treedata, get_icon,		xui_bitmap*		)( u32 index )
{
	return m_icon;
}
xui_method_explain(xui_treedata, get_text,		std::wstring	)( u32 index )
{
	return m_text;
}
xui_method_explain(xui_treedata, get_textfont,	xui_family		)( u32 index )
{
	return xui_family();
}
xui_method_explain(xui_treedata, get_textdraw, xui_family_render)( u32 index )
{
	return xui_family_render();
}
xui_method_explain(xui_treedata, set_text,		void			)( u32 index, const std::wstring& text )
{
	m_text = text;
}
xui_method_explain(xui_treedata, set_flag,		void			)( u32 index, bool flag )
{
	m_flag = flag;
}