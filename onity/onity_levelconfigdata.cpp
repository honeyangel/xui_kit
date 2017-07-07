#include "NP2DSSceneFile.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_global.h"
#include "onity_filedata.h"
#include "onity_proplevelconfig.h"
#include "onity_levelconfigdata.h"

/*
//constructor
*/
xui_create_explain(onity_levelconfigdata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
, m_selected(true)
{}

/*
//destructor
*/
xui_delete_explain(onity_levelconfigdata)( void )
{
	delete m_prop;
}

/*
//virtual
*/
xui_method_explain(onity_levelconfigdata, get_text, std::wstring	)( u32 index )
{
	onity_proplevelconfig* propconfig = dynamic_cast<onity_proplevelconfig*>(m_prop);
	std::string name = propconfig->get_level()->GetName();
	return onity_filedata::get_safe(xui_global::ascii_to_unicode(name));
}
xui_method_explain(onity_levelconfigdata, get_flag, bool			)( u32 index )
{
	return m_selected;
}
xui_method_explain(onity_levelconfigdata, set_flag, void			)( u32 index, bool flag )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_node->get_parent());
	if (treeview->get_upmostnode(0) != m_node)
	{
		m_selected = flag;
	}
}