#include "xui_global.h"
#include "onity_propentitytemp.h"
#include "onity_filterdata.h"

/*
//constructor
*/
xui_create_explain(onity_filterdata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//destructor
*/
xui_delete_explain(onity_filterdata)( void )
{
	if (m_prop)
	{
		delete m_prop;
		m_prop = NULL;
	}
}

/*
//virtual
*/
xui_method_explain(onity_filterdata, get_text, std::wstring)( u32 index )
{
	onity_propentitytemp* proptemp = dynamic_cast<onity_propentitytemp*>(m_prop);
	return xui_global::ascii_to_unicode(proptemp->get_tempname());
}
xui_method_explain(onity_filterdata, set_text, void			)( u32 index, const std::wstring& text )
{
	onity_propentitytemp* proptemp = dynamic_cast<onity_propentitytemp*>(m_prop);
	proptemp->rna_template(text);
}