#include "NP2DSStateCtrl.h"

#include "xui_global.h"
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
	onity_propfile::save();

	NP2DSStateCtrl* file = get_statectrl();
	file->SaveXmlFile(xui_global::unicode_to_ascii(m_fullname));
}
xui_method_explain(onity_propcontroller, load,			void			)( void )
{
	onity_propfile::load();

	if (m_statectrl)
	{
		delete m_statectrl;
		m_statectrl = NULL;
	}

	onity_animator* animator = onity_mainform::get_ptr()->get_animator();
	if (animator->get_editprop() == this)
	{
		animator->set_editprop(NULL);
		animator->set_editprop(this);
	}
}