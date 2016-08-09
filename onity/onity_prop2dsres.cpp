#include "NPFile.h"
#include "NPFileName.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"
#include "NP2DSAsset.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageFile.h"
#include "NP2DSFrameFile.h"
#include "NP2DSActorFile.h"

#include "xui_global.h"
#include "xui_propdata.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "onity_console.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_filedata.h"
#include "onity_resource.h"
#include "onity_prop2dsasset.h"
#include "onity_prop2dsres.h"

/*
//constructor
*/
xui_create_explain(onity_prop2dsres)( xui_bitmap* icon, const std::wstring& full )
: onity_propfile(icon, full)
, m_resfile(-1)
{
	m_reskind = new xui_propkind(this, L"AssetFile", "AssetFile", xui_kindctrl::create, NULL, true, false);
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
	m_reskind->add_propdata(new xui_propdata_bool(
		m_reskind,
		L"Entire Load",
		xui_propctrl_bool::create,
		get_loadtype,
		set_loadtype,
		this));

	add_propkind(m_reskind);
}

/*
//destructor
*/
xui_delete_explain(onity_prop2dsres)( void )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];
}

/*
//method
*/
xui_method_explain(onity_prop2dsres, get_subprop,	std::vector<xui_proproot*>	)( void )
{
	if (m_subprop.empty())
	{
		NP2DSAssetFile* file = get_resfile();
		if (file)
		{
			for (npu16 i = 0; i < file->GetAssetCount(); ++i)
			{
				npu32 id = file->GetAssetID(i);
				xui_proproot* prop = new_subprop(id);
				m_subprop.push_back(prop);
			}
		}
	}

	return m_subprop;
}
xui_method_explain(onity_prop2dsres, new_subprop,	xui_proproot*				)( u32 id )
{
	return NULL;
}
xui_method_explain(onity_prop2dsres, get_resfile,	NP2DSAssetFile*				)( void )
{
	return NULL;
}
xui_method_explain(onity_prop2dsres, was_modify,	bool						)( void )
{
	NP2DSAssetFile* file = get_resfile();
	if (file)
		return file->WasNeedSave();

	return false;
}
xui_method_explain(onity_prop2dsres, ntf_rename,	void						)( const std::wstring& last, const std::wstring& curr )
{
	std::string  lastfull = xui_global::unicode_to_ascii(m_fullname);
	std::string  lastpath = NPFileNameHelper::PathName(lastfull);
	std::string  lastfile = NPFileNameHelper::FileName(lastfull);

	onity_propfile::ntf_rename(last, curr);

	std::string  currfull = xui_global::unicode_to_ascii(m_fullname);
	std::string  currpath = NPFileNameHelper::PathName(currfull);
	std::string  currfile = NPFileNameHelper::FileName(currfull);

	std::wstring suffname = onity_filedata::get_suff(m_fullname);
	u08 type;
	NP2DSAssetFileMgr* file_mgr;
	if		(suffname == L".npModule") type = META_MODULE;
	else if (suffname == L".npSprite") type = META_SPRITE;
	else if (suffname == L".npAction") type = META_ACTION;
	else
	{}

	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	npu32 id = file_mgr->GetFileID(lastpath, lastfile);
	file_mgr->SetFilePH(id, currpath);
	file_mgr->SetFileFN(id, currfile);
	save_meta(type, id);

	if (lastfile != currfile)
		xui_global::del_file(last+L".meta");
}
xui_method_explain(onity_prop2dsres, load,			void						)( void )
{
	if (m_resfile != -1)
	{
		std::wstring suffname = onity_filedata::get_suff(m_fullname);
		u08 type;
		NP2DSAssetFileMgr* file_mgr;
		if		(suffname == L".npModule") type = META_MODULE;
		else if (suffname == L".npSprite") type = META_SPRITE;
		else if (suffname == L".npAction") type = META_ACTION;
		else
		{}

		switch (type)
		{
		case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
		case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
		case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
		}

		file_mgr->ResFile(m_resfile);

		for (u32 i = 0; i < m_subprop.size(); ++i)
			delete m_subprop[i];

		m_subprop.clear();

		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->ntf_load(this);
	}
}
xui_method_explain(onity_prop2dsres, save,			void						)( void )
{
	NP2DSAssetFile* file = get_resfile();
	if (file)
	{
		xui_global::set_fwatchclose();
		file->SaveXml(xui_global::unicode_to_ascii(m_fullname));
		xui_global::set_fwatchstart(xui_global::get_workpath());
	}
}

/*
//method
*/
xui_method_explain(onity_prop2dsres, add_subprop,	xui_proproot*				)( void )
{
	npu32 id = -1;
	NP2DSAssetFile* file = get_resfile();
	NP2DSImageFile* imagefile = NPDynamicCast(NP2DSImageFile, file);
	NP2DSFrameFile* framefile = NPDynamicCast(NP2DSFrameFile, file);
	NP2DSActorFile* actorfile = NPDynamicCast(NP2DSActorFile, file);
	if (imagefile)	id = imagefile->AddImage(0, 0, 50, 50);
	if (framefile)	id = framefile->AddFrame();
	if (actorfile)	id = actorfile->AddActor();

	if (id != -1)
	{
		xui_proproot* prop = new_subprop(id);
		m_subprop.push_back(prop);
		return prop;
	}

	return NULL;
}
xui_method_explain(onity_prop2dsres, del_subprop,	void						)( xui_proproot* prop )
{
	xui_proproot_vec::iterator itor = std::find(
		m_subprop.begin(),
		m_subprop.end(),
		prop);

	if (itor != m_subprop.end())
	{
		NP2DSAssetFile* file = get_resfile();
		onity_prop2dsasset* propasset = dynamic_cast<onity_prop2dsasset*>(prop);
		file->DelAsset(propasset->get_assetid());

		m_subprop.erase(itor);
		delete prop;
	}
}

/*
//static
*/
xui_method_explain(onity_prop2dsres, load_meta,		void						)( u08 type, const std::string& fullname )
{
	NPFile file;
	if (file.Open(fullname+".meta", NPFile::OM_READ))
	{
		std::string textname;	file.ReadLine(textname);
		std::string pathname;	file.ReadLine(pathname);
		std::string	filename;	file.ReadLine(filename);
		std::string textgrap;   file.ReadLine(textgrap);

		npu32 guid;
		std::stringstream ss(textname.c_str());
		ss >> guid;

		NP2DSAssetFileMgr* file_mgr = NULL;
		switch (type)
		{
		case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
		case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
		case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
		}

		// check
		npu32 id = file_mgr->GetFileID(guid);
		if (id != -1)
		{
			std::string temppath = file_mgr->GetFilePH(id);
			std::string tempfile = file_mgr->GetFileFN(id);
			std::string tempfull = temppath + tempfile;
			if (temppath != pathname || 
				tempfile != filename)
			{
				std::wstringstream message;
				message << L"Same Meta With";
				message << tempfull.c_str();
				onity_console* console = onity_mainform::get_ptr()->get_console();
				console->add_log(LOGTYPE_WARNING, xui_global::ascii_to_unicode(fullname), message.str());
			}

		}
		else
		{
			file_mgr->AddFileGuid(pathname, filename, guid);
		}
	}
}
xui_method_explain(onity_prop2dsres, save_meta,		void						)( u08 type, u32 id )
{
	NP2DSAssetFileMgr* file_mgr = NULL;
	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	std::string	pathname = file_mgr->GetFilePH(id);
	std::string filename = file_mgr->GetFileFN(id);
	std::string fullname = pathname+filename;

	NPFile file;
	if (file.Open(fullname+".meta", NPFile::OM_WRITE))
	{
		npu32 guid = file_mgr->GetFileGuid(id);
		std::stringstream guidtext;
		guidtext << guid;
		std::stringstream graptext;
		graptext << 1;
		file.WriteLine(guidtext.str());
		file.WriteLine(pathname);
		file.WriteLine(filename);
		file.WriteLine(graptext.str());
	}
}

/*
//static
*/
xui_method_explain(onity_prop2dsres, get_freetype,	s32							)( void* userptr )
{
	onity_prop2dsres* prop = (onity_prop2dsres*)userptr;
	NP2DSAssetFile*   file = prop->get_resfile();
	if (file)
	{
		bool same = true;
		s32  type = -1;
		for (npu16 i = 0; i < file->GetAssetCount(); ++i)
		{
			npu32 id = file->GetAssetID(i);
			NP2DSAsset* asset = file->GetAsset(id);
			if (i == 0)
				type  = (s32)asset->GetFree();
			if (type != (s32)asset->GetFree())
			{
				same  = false;
				break;
			}
		}

		return same ? type : -1;
	}

	return -1;
}
xui_method_explain(onity_prop2dsres, set_freetype,	void						)( void* userptr, s32 value )
{
	onity_prop2dsres* prop = (onity_prop2dsres*)userptr;
	NP2DSAssetFile*   file = prop->get_resfile();
	if (file)
	{
		for (npu16 i = 0; i < file->GetAssetCount(); ++i)
		{
			npu32 id = file->GetAssetID(i);
			NP2DSAsset* asset = file->GetAsset(id);
			if (asset)
				asset->SetFree((npu32)value);
		}
	}
}
xui_method_explain(onity_prop2dsres, get_loadtype,	bool						)( void* userptr )
{
	onity_prop2dsres* prop = (onity_prop2dsres*)userptr;
	NP2DSAssetFile*   file = prop->get_resfile();
	if (file)
		return file->WasEntireLoad();

	return false;
}
xui_method_explain(onity_prop2dsres, set_loadtype,	void						)( void* userptr, bool value )
{
	onity_prop2dsres* prop = (onity_prop2dsres*)userptr;
	NP2DSAssetFile*   file = prop->get_resfile();
	if (file)
		file->SetEntireLoad(value);
}