#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"

#include "xui_global.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "onity_proplayer.h"
#include "onity_layerdata.h"

/*
//constructor
*/
xui_create_explain(onity_layerdata)( xui_proproot* prop )
: onity_timedata(prop)
{
	onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(prop);
	NP2DSLayer* layer = proplayer->get_layer();
	const NP2DSLayer::FrameKeyList& keylist = layer->GetFrameKeyList();
	for (NP2DSLayer::FrameKeyList::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		NP2DSFrameKey* framekey = (*itor);
		m_keyframe[(s32)framekey->GetTime()] = framekey->WasSmooth() ? KS_LINEAR : KS_STATIC;
	}
}

/*
//override
*/
xui_method_explain(onity_layerdata, get_text, std::wstring	)( u32 index )
{
	if (index == 0)
	{
		onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
		NP2DSLayer* layer = proplayer->get_layer();
		std::string name  = layer->GetName();
		if (name.length() == 0)
			name = "(No Name)";

		return xui_global::ascii_to_unicode(name);
	}
	else
	{
		return L"";
	}
}
xui_method_explain(onity_layerdata, set_text, void			)( u32 index, const std::wstring& text )
{
	if (index == 0)
	{
		onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
		NP2DSLayer* layer = proplayer->get_layer();
		layer->SetName(xui_global::unicode_to_ascii(text));

		proplayer->ntf_rename(text);
	}
}
xui_method_explain(onity_layerdata, get_flag, bool			)( u32 index )
{
	return false;
}
xui_method_explain(onity_layerdata, set_flag, void			)( u32 index, bool flag )
{

}