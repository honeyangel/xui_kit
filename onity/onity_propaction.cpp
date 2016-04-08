#include "NPFileName.h"
#include "NP2DSActorFile.h"
#include "NP2DSActorFileMgr.h"

#include "xui_global.h"
#include "onity_filedata.h"
#include "onity_propaction.h"

/*
//constructor
*/
xui_create_explain(onity_propaction)( const std::wstring& full )
: onity_prop2dsres(full)
{}

/*
//override
*/
xui_method_explain(onity_propaction, get_resfile, NP2DSAssetFile*)( void )
{
	if (m_resfile == -1)
	{
		std::string  fullname = xui_global::unicode_to_ascii(m_fullname);
		std::string  pathname = NPFileNameHelper::PathName(fullname);
		std::string  filename = NPFileNameHelper::FileName(fullname);
		m_resfile = NP2DSActorFileMgr::GetIns()->GetFileID(pathname, filename, true);
	}

	if (m_resfile != -1)
	{
		return NP2DSActorFileMgr::GetIns()->GetFile(m_resfile);
	}

	return NULL;
}