#include "NPFileName.h"
#include "NP2DSFrameFile.h"
#include "NP2DSFrameFileMgr.h"

#include "xui_global.h"
#include "onity_resource.h"
#include "onity_filedata.h"
#include "onity_propframe.h"
#include "onity_propsprite.h"

/*
//constructor
*/
xui_create_explain(onity_propsprite)( const std::wstring& full )
: onity_prop2dsres(onity_resource::icon_sprite, full)
{}

/*
//override
*/
xui_method_explain(onity_propsprite, get_resfile, NP2DSAssetFile*	)( void )
{
	if (m_resfile == -1)
	{
		std::string  fullname = xui_global::unicode_to_ascii(m_fullname);
		std::string  pathname = NPFileNameHelper::PathName(fullname);
		std::string  filename = NPFileNameHelper::FileName(fullname);
		m_resfile = NP2DSFrameFileMgr::GetIns()->GetFileID(pathname, filename, true);
	}

	if (m_resfile != -1)
	{
		return NP2DSFrameFileMgr::GetIns()->GetFile(m_resfile);
	}

	return NULL;
}
xui_method_explain(onity_propsprite, new_subprop, xui_proproot*		)( u32 id )
{
	return new onity_propframe(this, id);
}