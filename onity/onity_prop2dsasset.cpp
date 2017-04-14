#include "NP2DSAsset.h"
#include "NP2DSAssetFile.h"

#include "xui_global.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "onity_savekind.h"
#include "onity_resource.h"
#include "onity_prop2dsres.h"
#include "onity_prop2dsasset.h"

/*
//constructor
*/
xui_create_explain(onity_prop2dsasset)( xui_bitmap* icon, onity_propfile* propfile, u32 id )
: onity_propleaf(propfile)
, m_assetid(id)
{
	NP2DSAsset* asset = get_asset();
	m_reskind = new xui_propkind(this, xui_global::ascii_to_unicode(asset->GetName()), "2DSAsset", xui_kindctrl::create, icon, true);
	m_reskind->xm_namechanged += new xui_method_member<xui_method_args, onity_prop2dsasset>(this, &onity_prop2dsasset::on_namechanged);
	std::map<s32, std::wstring> textmap;
	textmap[0] = L"Auto";
	textmap[1] = L"Never";
	textmap[2] = L"Immediate";
	m_reskind->add_propdata(new xui_propdata_enum_func(
		m_reskind,
		L"Free Style",
		xui_propctrl_enum::create,
		textmap,
		get_freetype,
		set_freetype,
		this));

	add_propkind(m_reskind);
}

/*
//method
*/
xui_method_explain(onity_prop2dsasset, get_asset,		NP2DSAsset*	)( void )
{
	onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(m_savekind->get_propfile());
	NP2DSAssetFile* file = prop2dsres->get_resfile();
	return file->GetAsset(m_assetid);
}
xui_method_explain(onity_prop2dsasset, get_assetid,		u32			)( void ) const
{
	return m_assetid;
}
xui_method_explain(onity_prop2dsasset, get_resicon,		xui_bitmap*	)( void )
{
	return m_reskind->get_icon();
}

/*
//override
*/
xui_method_explain(onity_prop2dsasset, get_dragtype,	std::string	)( void )
{
	NP2DSAsset* asset = get_asset();
	if (asset)
		return asset->GetRTTI()->GetName();

	return "";
}
xui_method_explain(onity_prop2dsasset, get_dragdata,	void*		)( void )
{
	return get_asset();
}

/*
//static
*/
xui_method_explain(onity_prop2dsasset, get_freetype,	s32			)( void* userptr )
{
	onity_prop2dsasset* prop = (onity_prop2dsasset*)userptr;
	NP2DSAsset* asset = prop->get_asset();
	return (s32)asset->GetFree();
}
xui_method_explain(onity_prop2dsasset, set_freetype,	void		)( void* userptr, s32 value )
{
	onity_prop2dsasset* prop = (onity_prop2dsasset*)userptr;
	NP2DSAsset* asset = prop->get_asset();
	asset->SetFree((npu32)value);
}

/*
//event
*/
xui_method_explain(onity_prop2dsasset, on_namechanged,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textbox->get_text();
	std::string  name = xui_global::unicode_to_ascii(text);
	m_reskind->set_name(text);

	NP2DSAsset* asset = get_asset();
	if (asset->GetName() != name)
	{
		asset->SetName(name);
		asset->GetOwnedFile()->SetNeedSave(true);
	}

	for (u32 i = 0; i < m_linkdatavec.size(); ++i)
	{
		m_linkdatavec[i]->get_node()->use_linkdata();
	}
}