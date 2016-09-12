#include "NP2DSSceneFileMgr.h"
#include "NP2DSSceneFile.h"

#include "xui_global.h"
#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_resource.h"
#include "onity_propcourse.h"

/*
//constructor
*/
xui_create_explain(onity_propcourse)( const std::wstring& full )
: onity_propfile(onity_resource::icon_scene, full)
, m_scenefile(NULL)
{}

/*
//destructor
*/
xui_delete_explain(onity_propcourse)( void )
{
	delete m_scenefile;
}

/*
//method
*/
xui_method_explain(onity_propcourse, get_scenefile, NP2DSSceneFile*	)( void )
{
	if (m_scenefile == NULL)
	{
		m_scenefile = new NP2DSSceneFile(-1, "", NP2DSSceneFileMgr::GetIns());
		m_scenefile->LoadXml(xui_global::unicode_to_ascii(m_fullname));
	}

	return m_scenefile;
}
xui_method_explain(onity_propcourse, ntf_detach,	void			)( xui_proproot* nextfile )
{
	xui_proproot_vec vec;
	vec.push_back(nextfile);
	onity_propfile::on_detach(vec);
}
xui_method_explain(onity_propcourse, was_modify,	bool			)( void )
{
	if (m_scenefile)
		return m_scenefile->WasNeedSave();

	return false;
}
xui_method_explain(onity_propcourse, save,			void			)( void )
{
	NP2DSSceneFile* file = get_scenefile();
	xui_global::set_fwatchclose();
	file->SaveXml(xui_global::unicode_to_ascii(m_fullname));
	xui_global::set_fwatchstart(xui_global::get_workpath());
}
xui_method_explain(onity_propcourse, load,			void			)( void )
{
	if (m_scenefile)
	{
		delete m_scenefile;
		m_scenefile = NULL;
	}

	onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
	xui_proproot*    editprop  = hierarchy->get_editprop();
	if (editprop == this)
		hierarchy->reset();
}