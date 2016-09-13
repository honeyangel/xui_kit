#include "NP2DSTransRef.h"

#include "xui_global.h"
#include "xui_treenode.h"
#include "onity_prop2dsref.h"
#include "onity_2dsrefdata.h"

/*
//constructor
*/
xui_create_explain(onity_2dsrefdata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//destructor
*/
xui_delete_explain(onity_2dsrefdata)( void )
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
xui_method_explain(onity_2dsrefdata, get_text, std::wstring)( u32 index )
{
	onity_prop2dsref*  prop = dynamic_cast<onity_prop2dsref*>(m_prop);
	std::string        name = prop->get_2dsref()->GetParent()->GetName();
	std::wstringstream text;

	text << L"[";
	text << prop->get_2dsref()->GetSceneParamCount();
	text << L"]";
	text << xui_global::ascii_to_unicode(name);
	return text.str();
}
