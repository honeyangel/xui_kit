#include "NP2DSAsset.h"
#include "NP2DSAssetFile.h"

#include "xui_global.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_treenode.h"
#include "onity_resource.h"
#include "onity_prop2dsasset.h"
#include "onity_2dsassetdata.h"

/*
//constructor
*/
xui_create_explain(onity_2dsassetdata)( xui_proproot* prop )
: onity_treedata(prop)
{
	m_icon = onity_resource::icon_file;
}

/*
//override
*/
xui_method_explain(onity_2dsassetdata, get_text, std::wstring	)( u32 index )
{
	onity_prop2dsasset* prop = dynamic_cast<onity_prop2dsasset*>(m_prop);
	NP2DSAsset* asset = prop->get_asset();
	std::string name  = asset->GetName();

	char temp[256];
	sprintf(temp, "%03d#%s", prop->get_assetid(), name.c_str());
	return xui_global::ascii_to_unicode(std::string(temp));
}
xui_method_explain(onity_2dsassetdata, set_text, void			)( u32 index, const std::wstring& text )
{
	std::string name = xui_global::unicode_to_ascii(text);
	int npos = name.find_first_of('#');
	if (npos != -1)
		name.erase(0, npos+1);

	onity_prop2dsasset* prop = dynamic_cast<onity_prop2dsasset*>(m_prop);
	NP2DSAsset* asset = prop->get_asset();
	if (asset->GetName() != name)
	{
		asset->SetName(name);
		asset->GetOwnedFile()->SetNeedSave(true);
		m_node->use_linkdata();

		xui_propkind* propkind = prop->get_propkind().front();
		propkind->set_name(xui_global::ascii_to_unicode(name));
		xui_kindctrl* kindctrl = propkind->get_ctrl();
		if (kindctrl)
			kindctrl->refresh();
	}
}