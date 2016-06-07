#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"
#include "NP2DSActorRef.h"

#include "xui_global.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "onity_resource.h"
#include "onity_proplayer.h"
#include "onity_propactor.h"
#include "onity_timeline.h"
#include "onity_mainform.h"
#include "onity_preview.h"
#include "onity_layerdata.h"

/*
//constructor
*/
xui_create_explain(onity_layerdata)( xui_proproot* prop )
: onity_timedata(onity_resource::icon_layer, prop)
{
	cal_keyframe();
}

/*
//override
*/
xui_method_explain(onity_layerdata, get_icon,		xui_bitmap*		)( u32 index )
{
	if (index == 0)
		return m_icon;

	return NULL;
}
xui_method_explain(onity_layerdata, get_text,		std::wstring	)( u32 index )
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
xui_method_explain(onity_layerdata, set_text,		void			)( u32 index, const std::wstring& text )
{
	if (index == 0)
	{
		onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
		NP2DSLayer* layer = proplayer->get_layer();
		layer->SetName(xui_global::unicode_to_ascii(text));

		proplayer->ntf_rename(text);
	}
}
xui_method_explain(onity_layerdata, get_flag,		bool			)( u32 index )
{
	switch (index)
	{
	case LAYER_FLAG_LEAD:
		{
			onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
			NP2DSLayer* layer = proplayer->get_layer();
			return layer->WasLead();
		}
		break;
	case LAYER_FLAG_SHOW:
		{
			onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
			onity_propactor* propactor = proplayer->get_actor();
			NP2DSActor* actor = NPDynamicCast(NP2DSActor, propactor->get_asset());
			u16 index = actor->GetLayerIndex(proplayer->get_layer());

			onity_timeline* timeline = onity_mainform::get_ptr()->get_timeline();
			onity_preview*  drawview = timeline->get_drawview();
			NP2DSActorRef*  drawnode = NPDynamicCast(NP2DSActorRef, drawview->get_drawnode());
			return drawnode->WasLayerShow(index);
		}
		break;
	}

	return false;
}
xui_method_explain(onity_layerdata, set_flag,		void			)( u32 index, bool flag )
{
	switch (index)
	{
	case LAYER_FLAG_LEAD:
		{
			onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
			onity_propactor* propactor = proplayer->get_actor();
			NP2DSActor* actor = NPDynamicCast(NP2DSActor, propactor->get_asset());
			for (u16 i = 0; i < actor->GetLayerCount(); ++i)
			{
				NP2DSLayer* layer = actor->GetLayer(i);
				if (layer == proplayer->get_layer())
					layer->SetLead(flag);
				else
					layer->SetLead(false);
			}

			xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_node->get_parent());
			for (u32 i = 0; i < treeview->get_upmostnodecount(); ++i)
			{
				xui_treenode* node = treeview->get_upmostnode(i);
				node->use_linkdata();
			}
		}
		break;
	case LAYER_FLAG_SHOW:
		{
			onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
			onity_propactor* propactor = proplayer->get_actor();
			NP2DSActor* actor = NPDynamicCast(NP2DSActor, propactor->get_asset());
			u16 index = actor->GetLayerIndex(proplayer->get_layer());

			onity_timeline* timeline = onity_mainform::get_ptr()->get_timeline();
			onity_preview*  drawview = timeline->get_drawview();
			NP2DSActorRef*  drawnode = NPDynamicCast(NP2DSActorRef, drawview->get_drawnode());
			drawnode->SetLayerShow(index, flag);
		}
		break;
	}
}
xui_method_explain(onity_layerdata, get_keycolor,	xui_colour		)( void ) const
{
	onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
	if (proplayer->get_layer()->WasLead())
		return xui_colour::red;

	return onity_timedata::get_keycolor();
}
xui_method_explain(onity_layerdata, cal_keyframe,	void			)( void )
{
	m_keyframe.clear();
	onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(m_prop);
	NP2DSLayer* layer = proplayer->get_layer();
	const NP2DSLayer::FrameKeyList& keylist = layer->GetFrameKeyList();
	for (NP2DSLayer::FrameKeyList::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		NP2DSFrameKey* framekey = (*itor);
		m_keyframe[(s32)framekey->GetTime()] = framekey->WasSmooth() ? KS_LINEAR : KS_STATIC;
	}
}