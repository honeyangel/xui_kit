#include "Entity.h"
#include "onity_propentity.h"
#include "onity_entitydata.h"

/*
//constructor
*/
xui_create_explain(onity_entitydata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//destructor
*/
xui_delete_explain(onity_entitydata)( void )
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
xui_method_explain(onity_entitydata, get_text, std::wstring)( u32 index )
{
	onity_propentity*  prop = dynamic_cast<onity_propentity*>(m_prop);
	std::wstringstream text;

	text << L"[ID]";
	text << prop->get_entity()->GetID();
	return text.str();
}