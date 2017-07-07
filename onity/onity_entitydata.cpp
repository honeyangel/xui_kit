#include "NP2DSTransRef.h"
#include "NP2DSAssetFile.h"
#include "NPFileName.h"
#include "Entity.h"
#include "onity_propentitynode.h"
#include "onity_propentitystar.h"
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
xui_method_explain(onity_entitydata, get_textdraw,	xui_family_render	)( u32 index )
{
	xui_family_render   draw = onity_treedata::get_textdraw(index);
	onity_propeditnode* node = dynamic_cast<onity_propeditnode*>(m_prop);
	if (node->get_2dsref())
		draw.normalcolor = xui_colour::red;

	return draw;
}
xui_method_explain(onity_entitydata, get_text,		std::wstring		)( u32 index )
{
	std::wstringstream    text;
	onity_propeditnode*   node			= dynamic_cast<onity_propeditnode*>(m_prop);
	onity_propentitynode* entitynode	= dynamic_cast<onity_propentitynode*>(node);
	onity_propentitystar* entitystar	= dynamic_cast<onity_propentitystar*>(node);
	if (entitynode)
	{
		text << L"[ID]";
		text << entitynode->get_entity()->GetID();
	}
	if (entitystar)
	{
		text << L"Star";
	}

	NP2DSTransRef* ref = node->get_2dsref();
	if (ref)
	{
		std::string fullname = ref->GetOwnedFile()->GetName();
		text << "(";
		text << NPFileNameHelper::SafeName(fullname).c_str();
		text << ")";
		npu32 count = ref->GetSceneParamCount();
		if (count > 0)
		{
			text << "[";
			text << count;
			text << "]";
		}
	}

	return text.str();
}