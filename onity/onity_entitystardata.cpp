#include "NP2DSTransRef.h"
#include "NP2DSAssetFile.h"
#include "NPFileName.h"
#include "Entity.h"
#include "onity_propentitystar.h"
#include "onity_entitystardata.h"

/*
//constructor
*/
xui_create_explain(onity_entitystardata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//destructor
*/
xui_delete_explain(onity_entitystardata)( void )
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
xui_method_explain(onity_entitystardata, get_textdraw,	xui_family_render	)( u32 index )
{
	xui_family_render draw = onity_treedata::get_textdraw(index);
	draw.normalcolor = xui_colour::red;
	return draw;
}
xui_method_explain(onity_entitystardata, get_text,		std::wstring		)( u32 index )
{
	return L"Star";
	//onity_propentity*  prop = dynamic_cast<onity_propentity*>(m_prop);
	//std::wstringstream text;
	//text << L"[ID]";
	//text << prop->get_entity()->GetID();

	//NP2DSTransRef* linkref = prop->get_entity()->GetLinkRef();
	//if (linkref)
	//{
	//	std::string fullname = linkref->GetOwnedFile()->GetName();
	//	text << "(";
	//	text << NPFileNameHelper::SafeName(fullname).c_str();
	//	text << ")";
	//}

	//return text.str();
}