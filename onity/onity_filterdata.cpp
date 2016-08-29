#include "onity_filterdata.h"

/*
//constructor
*/
xui_create_explain(onity_filterdata)( xui_bitmap* icon, const std::wstring& text, xui_proproot* prop )
: onity_treedata(icon, prop)
{
	m_text = text;
}

/*
//virtual
*/
//xui_method_explain(onity_filterdata, get_text, std::wstring)( u32 index )
//{
//	return m_text;
//}