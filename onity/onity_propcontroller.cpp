#include "NP2DSStateCtrl.h"

#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_dialog.h"
#include "onity_resource.h"
#include "onity_filedata.h"
#include "onity_mainform.h"
#include "onity_animator.h"
#include "onity_propcontroller.h"

/*
//constructor
*/
xui_create_explain(onity_propcontroller)( const std::wstring& full )
: onity_propfile(onity_resource::icon_state, full)
, m_statectrl(NULL)
{}

/*
//destructor
*/
xui_delete_explain(onity_propcontroller)( void )
{
	delete m_statectrl;
}

/*
//method
*/
xui_method_explain(onity_propcontroller, ntf_detach,	void			)( xui_proproot* nextfile )
{
	xui_proproot_vec vec;
	vec.push_back(nextfile);
	onity_propfile::on_detach(vec);
}
xui_method_explain(onity_propcontroller, get_statectrl, NP2DSStateCtrl*	)( void )
{
	if (m_statectrl == NULL)
	{
		m_statectrl = new NP2DSStateCtrl;
		m_statectrl->LoadXmlFile(xui_global::unicode_to_ascii(m_fullname));
	}

	return m_statectrl;
}
xui_method_explain(onity_propcontroller, was_modify,	bool			)( void )
{
	if (m_statectrl)
		return m_statectrl->WasNeedSave();

	return false;
}
xui_method_explain(onity_propcontroller, save,			void			)( void )
{
	NP2DSStateCtrl* file = get_statectrl();
	xui_global::set_fwatchclose();
	file->SaveXmlFile(xui_global::unicode_to_ascii(m_fullname));
	xui_global::set_fwatchstart(xui_global::get_workpath());
}
xui_method_explain(onity_propcontroller, load,			void			)( void )
{
	if (m_statectrl)
	{
		delete m_statectrl;
		m_statectrl = NULL;
	}

	onity_animator* animator = onity_mainform::get_ptr()->get_animator();
	xui_proproot*   editprop = animator->get_editprop();
	if (editprop == this)
		animator->reset();
}