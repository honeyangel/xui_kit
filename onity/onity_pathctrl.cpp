#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageFile.h"
#include "NP2DSFrameFile.h"
#include "NP2DSActorFile.h"
#include "NP2DSAsset.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "onity_prop2dsres.h"
#include "onity_pathdata.h"
#include "onity_proppath.h"
#include "onity_pathctrl.h"

xui_implement_rtti(onity_pathctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_pathctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	xui_menu* menu1 = xui_menu::create(80);
	m_auto		= menu1->add_item(NULL, L"Auto");
	m_never		= menu1->add_item(NULL, L"Never");
	m_immediate = menu1->add_item(NULL, L"Immediate");
	xui_method_ptrcall(m_auto,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_method_ptrcall(m_never,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_method_ptrcall(m_immediate,	xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_menu* menu2 = xui_menu::create(80);
	m_on		= menu2->add_item(NULL, L"On");
	m_off		= menu2->add_item(NULL, L"Off");
	xui_method_ptrcall(m_on,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_loadtypeclick);
	xui_method_ptrcall(m_off,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_loadtypeclick);

	m_freetype	= new xui_toggle(xui_vector<s32>(128, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_freetype,	set_parent		)(this);
	xui_method_ptrcall(m_freetype,	set_corner		)(3);
	xui_method_ptrcall(m_freetype,	set_borderrt	)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_freetype,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_freetype,	set_drawcolor	)(true);
	xui_method_ptrcall(m_freetype,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_freetype,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_freetype,	ini_drawer		)(L"Modify All FreeType");
	xui_method_ptrcall(m_freetype,	set_menu		)(menu1);

	m_loadtype	= new xui_toggle(xui_vector<s32>(128, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_loadtype,	set_parent		)(this);
	xui_method_ptrcall(m_loadtype,	set_corner		)(3);
	xui_method_ptrcall(m_loadtype,	set_borderrt	)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_loadtype,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_loadtype,	set_drawcolor	)(true);
	xui_method_ptrcall(m_loadtype,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_loadtype,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_loadtype,	ini_drawer		)(L"Modify All EntireLoad");
	xui_method_ptrcall(m_loadtype,	set_menu		)(menu2);

	m_widgetvec.push_back(m_freetype);
	m_widgetvec.push_back(m_loadtype);
}

/*
//static
*/
xui_method_explain(onity_pathctrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new onity_pathctrl(propkind);
}

/*
//callback
*/
xui_method_explain(onity_pathctrl, on_perform,			void			)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//freetype
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_freetype->get_renderh())/2;
	m_freetype->on_perform_pt(pt);
	m_freetype->on_perform_w (rt.get_w()-indent);
	//loadtype
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_loadtype->get_renderh())/2 + height;
	m_loadtype->on_perform_pt(pt);
	m_loadtype->on_perform_w (rt.get_w()-indent);
}

/*
//event
*/
xui_method_explain(onity_pathctrl, on_freetypeclick,	void			)( xui_component* sender, xui_method_args& args )
{
	npu32 style = 0;
	if		(sender == m_auto)		style = FS_AUTO;
	else if (sender == m_never)		style = FS_NEVER;
	else if (sender == m_immediate) style = FS_IMMEDIATE;
	else
	{}

	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		xui_propkind*   propkind = m_propkindvec[i];
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(propkind->get_root());
		std::string     pathname = xui_global::unicode_to_ascii(proppath->get_full()) + "/";
		set_freetype(META_MODULE, pathname, style);
		set_freetype(META_SPRITE, pathname, style);
		set_freetype(META_ACTION, pathname, style);
	}
}
xui_method_explain(onity_pathctrl, on_loadtypeclick,	void			)( xui_component* sender, xui_method_args& args )
{
	bool flag = (sender == m_on);
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		xui_propkind*   propkind = m_propkindvec[i];
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(propkind->get_root());
		std::string     pathname = xui_global::unicode_to_ascii(proppath->get_full()) + "/";
		set_loadtype(META_MODULE, pathname, flag);
		set_loadtype(META_SPRITE, pathname, flag);
		set_loadtype(META_ACTION, pathname, flag);
	}
}

/*
//method
*/
xui_method_explain(onity_pathctrl, set_freetype,		void			)( u08 type, const std::string& pathname, u32 style )
{
	NP2DSAssetFileMgr* file_mgr = NULL;
	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	for (npu32 i = 0; i < file_mgr->GetFileCount(); ++i)
	{
		std::string temppath = file_mgr->GetFilePH(i);
		std::string tempfile = file_mgr->GetFileFN(i);
		int index = temppath.find(pathname);
		if (index != -1)
		{
			NP2DSAssetFile*   file = NULL;
			switch (type)
			{
			case META_MODULE: file = NP2DSImageFileMgr::GetIns()->GetFile(i); break;
			case META_SPRITE: file = NP2DSFrameFileMgr::GetIns()->GetFile(i); break;
			case META_ACTION: file = NP2DSActorFileMgr::GetIns()->GetFile(i); break;
			}

			for (npu16 index = 0; index < file->GetAssetCount(); ++index)
			{
				npu32 id = file->GetAssetID(index);
				NP2DSAsset* asset = file->GetAsset(id);
				if (asset)
					asset->SetFree(style);
			}

			file->SaveXml(temppath+tempfile);
		}
	}
}
xui_method_explain(onity_pathctrl, set_loadtype,		void			)( u08 type, const std::string& pathname, bool flag )
{
	NP2DSAssetFileMgr* file_mgr = NULL;
	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	for (npu32 i = 0; i < file_mgr->GetFileCount(); ++i)
	{
		std::string temppath = file_mgr->GetFilePH(i);
		std::string tempfile = file_mgr->GetFileFN(i);
		int index = temppath.find(pathname);
		if (index != -1)
		{
			NP2DSAssetFile*   file = NULL;
			switch (type)
			{
			case META_MODULE: file = NP2DSImageFileMgr::GetIns()->GetFile(i); break;
			case META_SPRITE: file = NP2DSFrameFileMgr::GetIns()->GetFile(i); break;
			case META_ACTION: file = NP2DSActorFileMgr::GetIns()->GetFile(i); break;
			}

			file->SetEntireLoad(flag);
			file->SaveXml(temppath+tempfile);
		}
	}
}
xui_method_explain(onity_pathctrl, get_elsectrlsize,	s32				)( void )
{
	return 2*KIND_HEIGHT;
}
xui_method_explain(onity_pathctrl, get_prevctrlsize,	s32				)( void )
{
	return 2*KIND_HEIGHT;
}