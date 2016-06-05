#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"
#include "NP2DSAssetFile.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_kindctrl.h"
#include "onity_resource.h"
#include "onity_propframekey.h"
#include "onity_proplayer.h"

/*
//constructor
*/
xui_create_explain(onity_proplayer)( onity_propfile* propfile, onity_propactor* propactor, NP2DSLayer* layer )
: onity_propleaf(propfile)
, m_layer(layer)
, m_propactor(propactor)
{
	const std::list<NP2DSFrameKey*>& keylist = layer->GetFrameKeyList();
	for (std::list<NP2DSFrameKey*>::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		onity_propframekey* propframekey = new onity_propframekey(propfile, this, (*itor));
		m_propframekeys.push_back(propframekey);
	}

	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(layer->GetName()), "Layer", xui_kindctrl::create, onity_resource::icon_layer, true);
	m_basekind->xm_namechanged += new xui_method_member<xui_method_args, onity_proplayer>(this, &onity_proplayer::on_namechanged);
	m_basekind->add_propdata(new xui_propdata_bool(
		m_basekind,
		L"Particle",
		xui_propctrl_bool::create,
		has_particle,
		NULL,
		this));
	m_basekind->add_propdata(new xui_propdata_bool(
		m_basekind,
		L"Sound",
		xui_propctrl_bool::create,
		has_sound,
		NULL,
		this));
	m_basekind->add_propdata(new xui_propdata_bool(
		m_basekind,
		L"Event",
		xui_propctrl_bool::create,
		has_event,
		NULL,
		this));


	add_propkind(m_basekind);
}

/*
//destructor
*/
xui_delete_explain(onity_proplayer)( void )
{
	for (u32 i = 0; i < m_propframekeys.size(); ++i)
		delete m_propframekeys[i];
}

/*
//method
*/
xui_method_explain(onity_proplayer, get_actor,			onity_propactor*		)( void )
{
	return m_propactor;
}
xui_method_explain(onity_proplayer, get_layer,			NP2DSLayer*				)( void )
{
	return m_layer;
}
xui_method_explain(onity_proplayer, get_framekeys,		const xui_proproot_vec&	)( void ) const
{
	return m_propframekeys;
}
xui_method_explain(onity_proplayer, add_framekey,		void					)( xui_proproot* prop )
{
	//m_propframekeys.push_back(prop);
}
xui_method_explain(onity_proplayer, del_framekey,		void					)( s32 time )
{
	//for (u32 i = 0; i < m_propframekeys.size(); ++i)
	//{
	//	onity_propframekey* propframekey = dynamic_cast<onity_propframekey*>(m_propframekeys[i]);
	//	NP2DSFrameKey* framekey = propframekey->get_framekey();
	//	if ((s32)framekey->GetTime() == time)
	//	{
	//		delete m_propframekeys[i];
	//		m_propframekeys.erase(m_propframekeys.begin()+i);
	//		break;
	//	}
	//}
}
xui_method_explain(onity_proplayer, get_framekey,		xui_proproot*			)( s32 time )
{
	for (u32 i = 0; i < m_propframekeys.size(); ++i)
	{
		onity_propframekey* propframekey = dynamic_cast<onity_propframekey*>(m_propframekeys[i]);
		NP2DSFrameKey* framekey = propframekey->get_framekey();
		if ((s32)framekey->GetTime() == time)
			return propframekey;
	}

	return NULL;
}
xui_method_explain(onity_proplayer, ntf_rename,			void					)( const std::wstring& name )
{
	m_basekind->set_name(name);
	xui_kindctrl* kindctrl = m_basekind->get_ctrl();
	if (kindctrl)
		kindctrl->refresh();
}

/*
//static
*/
xui_method_explain(onity_proplayer, has_particle,		bool					)( void* userptr )
{
	bool result = false;
	onity_proplayer* proplayer = (onity_proplayer*)userptr;
	const std::list<NP2DSFrameKey*>& keylist = proplayer->get_layer()->GetFrameKeyList();
	for (std::list<NP2DSFrameKey*>::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		if ((*itor)->GetSpark())
		{
			result = true;
			break;
		}
	}

	return result;
}
xui_method_explain(onity_proplayer, has_sound,			bool					)( void* userptr )
{
	bool result = false;
	onity_proplayer* proplayer = (onity_proplayer*)userptr;
	const std::list<NP2DSFrameKey*>& keylist = proplayer->get_layer()->GetFrameKeyList();
	for (std::list<NP2DSFrameKey*>::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		if ((*itor)->GetSound().length() > 0)
		{
			result = true;
			break;
		}
	}

	return result;
}
xui_method_explain(onity_proplayer, has_event,			bool					)( void* userptr )
{
	bool result = false;
	onity_proplayer* proplayer = (onity_proplayer*)userptr;
	const std::list<NP2DSFrameKey*>& keylist = proplayer->get_layer()->GetFrameKeyList();
	for (std::list<NP2DSFrameKey*>::const_iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
	{
		if ((*itor)->GetEvent().length() > 0)
		{
			result = true;
			break;
		}
	}

	return result;
}

/*
//event
*/
xui_method_explain(onity_proplayer, on_namechanged,		void					)( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textbox->get_text();
	std::string  name = xui_global::unicode_to_ascii(text);
	m_basekind->set_name(text);

	NP2DSLayer* layer = get_layer();
	if (layer->GetName() != name)
	{
		layer->SetName(name);
		layer->GetOwnedFile()->SetNeedSave(true);
	}

	if (m_linkdata)
	{
		xui_treenode* node = m_linkdata->get_node();
		node->use_linkdata();
	}
}