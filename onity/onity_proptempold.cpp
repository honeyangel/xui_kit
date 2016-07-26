#include "xui_global.h"
#include "xui_kindctrl.h"
#include "onity_resource.h"
#include "onity_propjson.h"
#include "onity_propkind_compold.h"
#include "onity_proptempold.h"

/*
//constructor
*/
xui_create_explain(onity_proptempold)( onity_propfile* propfile, Omiga::EntityTemplate* temp )
: onity_propleaf(propfile)
, m_template(temp)
{
	std::string name = m_template->GetName();		
	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(name), "TemplateOld", xui_kindctrl::create, onity_resource::icon_local, true);
	add_propkind(m_basekind);

	BreezeGame::Json::Value* json = m_template->GetNode();
	BreezeGame::Json::Value* root = &(*json)["Component"];
	if (root->isArray() == false)
	{
		new_compkind(root);
	}
	else
	{
		for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
		{
			BreezeGame::Json::Value* node = &(*itor);
			new_compkind(node);
		}
	}
}

/*
//method
*/
xui_method_explain(onity_proptempold, get_template,		Omiga::EntityTemplate*	)( void )
{
	return m_template;
}
xui_method_explain(onity_proptempold, new_compkind,		void					)( BreezeGame::Json::Value* compnode )
{
	std::string   name = (*compnode)["ClassName"].asString();
	xui_propkind* kind = new onity_propkind_compold(this, xui_global::ascii_to_unicode(name), onity_resource::icon_local, compnode);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_proptempold>(this, &onity_proptempold::on_propchanged);
	m_compkind.push_back(kind);
	add_propkind(kind);
}

/*
//override
*/
xui_method_explain(onity_proptempold, get_dragtype,		std::string				)( void )
{
	return "EntityTemplate";
}
xui_method_explain(onity_proptempold, get_dragdata,		void*					)( void )
{
	return m_template;
}

/*
//event
*/
xui_method_explain(onity_proptempold, on_propchanged,	void					)( xui_component* sender, xui_method_propdata& args )
{
	onity_propjson* propjson = dynamic_cast<onity_propjson*>(get_propfile());
	propjson->set_modify(true);

	onity_propkind_compold* propkind = dynamic_cast<onity_propkind_compold*>(args.propdata->get_kind());
	BreezeGame::Json::Value* node = propkind->get_node();
}
