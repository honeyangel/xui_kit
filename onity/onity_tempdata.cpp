#include "xui_global.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_treenode.h"
#include "onity_resource.h"
#include "onity_proptempold.h"
#include "onity_tempdata.h"

/*
//constructor
*/
xui_create_explain(onity_tempdata)( xui_bitmap* icon, xui_proproot* prop )
: onity_treedata(icon, prop)
{}

/*
//override
*/
xui_method_explain(onity_tempdata, get_text, std::wstring	)( u32 index )
{
	onity_proptempold* prop = dynamic_cast<onity_proptempold*>(m_prop);
	std::string name = prop->get_template()->GetName();
	return xui_global::ascii_to_unicode(name);
}
xui_method_explain(onity_tempdata, set_text, void			)( u32 index, const std::wstring& text )
{
	onity_proptempold* prop = dynamic_cast<onity_proptempold*>(m_prop);
	prop->rna_template(text);
}
