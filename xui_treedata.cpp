#include "xui_treedata.h"

/*
//constructor
*/
xui_create_explain(xui_treedata)( void )
{
	m_node = NULL;
}
xui_create_explain(xui_treedata)( const std::wstring& text )
{
	m_node = NULL;
	m_text = text;
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
xui_method_explain(xui_treedata, get_flag,		bool			)( u32 index )
{
	return false;
}
xui_method_explain(xui_treedata, get_icon,		xui_bitmap*		)( u32 index )
{
	return NULL;
}
xui_method_explain(xui_treedata, set_text,		void			)( u32 index, const std::wstring& text )
{
	m_text = text;
}
xui_method_explain(xui_treedata, set_flag,		void			)( u32 index, bool flag )
{

}