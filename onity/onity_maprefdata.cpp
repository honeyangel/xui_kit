#include "NP2DSTransRef.h"

#include "xui_global.h"
#include "xui_treenode.h"
#include "onity_propmapref.h"
#include "onity_maprefdata.h"

/*
//constructor
*/
xui_create_explain(onity_maprefdata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//destructor
*/
xui_delete_explain(onity_maprefdata)( void )
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
xui_method_explain(onity_maprefdata, get_text, std::wstring)( u32 index )
{
	onity_propmapref*  prop = dynamic_cast<onity_propmapref*>(m_prop);
	std::wstringstream text;

	text << L"[Param]";
	text << prop->get_2dsref()->GetSceneParamCount();
	return text.str();
}
