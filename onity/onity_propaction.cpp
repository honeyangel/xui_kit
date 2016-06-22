#include "NPFileName.h"
#include "NP2DSActorFile.h"
#include "NP2DSActorFileMgr.h"

#include "xui_global.h"
#include "onity_resource.h"
#include "onity_timeline.h"
#include "onity_mainform.h"
#include "onity_filedata.h"
#include "onity_propactor.h"
#include "onity_propaction.h"

/*
//constructor
*/
xui_create_explain(onity_propaction)( const std::wstring& full )
: onity_prop2dsres(onity_resource::icon_action, full)
{}

/*
//override
*/
xui_method_explain(onity_propaction, new_subprop, xui_proproot*		)( u32 id )
{
	return new onity_propactor(this, id);
}
xui_method_explain(onity_propaction, get_resfile, NP2DSAssetFile*	)( void )
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
xui_method_explain(onity_propaction, load,		void				)( void )
{
	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop && editprop->get_propfile() == this)
		timeline->set_editprop(NULL);

	onity_prop2dsres::load();
}