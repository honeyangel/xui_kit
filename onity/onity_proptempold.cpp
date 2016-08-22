#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_propjsones.h"
#include "onity_propkind_compold.h"
#include "onity_mainform.h"
#include "onity_savekind.h"
#include "onity_inspector.h"
#include "onity_proptempold.h"

/*
//constructor
*/
xui_create_explain(onity_proptempold)( onity_propfile* propfile, Omiga::EntityTemplate* temp )
: onity_propleaf(propfile)
, m_template(temp)
{
	std::string name = m_template->GetName();		
	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(name), "TemplateOld", xui_kindctrl::create, onity_resource::icon_entity, true);
	m_basekind->xm_namechanged += new xui_method_member<xui_method_args, onity_proptempold>(this, &onity_proptempold::on_namechanged);
	m_menukind = new xui_propkind(this, L"", "ComponentAdd", onity_kindctrl_compadd::create, NULL, true, false, true);

	m_propkind.clear();
	m_propkind.push_back(m_basekind);
	m_propkind.push_back(m_menukind);
	m_propkind.push_back(m_savekind);
	add_compkind();
}

/*
//method
*/
xui_method_explain(onity_proptempold, get_template,		Omiga::EntityTemplate*	)( void )
{
	return m_template;
}
xui_method_explain(onity_proptempold, rna_template,		void					)( const std::wstring& text )
{
	std::string name = xui_global::unicode_to_ascii(text);
	if (Omiga::EntityManager::Instance()->GetEntityTemplate(name))
		return;

	Omiga::EntityManager::Instance()->RenameEntityTemplate(m_template, name);
	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
	propjsones->set_modify(true);

	m_basekind->set_name(text);
	xui_kindctrl* kindctrl = m_basekind->get_ctrl();
	if (kindctrl)
		kindctrl->refresh();
}
xui_method_explain(onity_proptempold, pst_template,		void					)( Omiga::EntityTemplate* temp )
{
	std::string name = m_template->GetName();
	m_template->ChangeTemplate(temp->GetNode());
	m_template->SetName(name);
	add_compkind();
}
xui_method_explain(onity_proptempold, get_components,	const xui_propkind_vec&	)( void ) const
{
	return m_compkind;
}
xui_method_explain(onity_proptempold, add_component,	void					)( const std::string& type, const std::string& name )
{
	for (u32 i = 0; i < m_compkind.size(); ++i)
	{
		onity_propkind_compold* propcomp = dynamic_cast<onity_propkind_compold*>(m_compkind[i]);
		BreezeGame::Json::Value* node = propcomp->get_node();
		if ((*node)["Family"].asString() == type)
		{
			xui_desktop::get_ins()->show_message(L"Has same family component!", 1);
			return;
		}
	}

	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
	propjsones->set_modify(true);

	BreezeGame::Json::Value* json = m_template->GetNode();
	BreezeGame::Json::Value* root = NULL;
	BreezeGame::Json::Value  comp(BreezeGame::Json::objectValue);
	comp["Family"]    = type;
	comp["ClassName"] = name;

	if (json->isMember("Component"))
	{
		root = &(*json)["Component"];
		if (root->isArray() == false)
		{
			BreezeGame::Json::Value temp(BreezeGame::Json::arrayValue);
			temp.append(*root);
			(*root) = temp;
		}

		root->append(comp);
	}
	else
	{
		(*json)["Component"] = BreezeGame::Json::Value(BreezeGame::Json::objectValue);
		root = &(*json)["Component"];
		(*root) = comp;
	}

	del_compkind();
	add_compkind();

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}
xui_method_explain(onity_proptempold, del_component,	void					)( xui_propkind* propkind )
{
	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
	propjsones->set_modify(true);

	onity_propkind_compold* propcomp = dynamic_cast<onity_propkind_compold*>(propkind);
	BreezeGame::Json::Value* json = m_template->GetNode();
	BreezeGame::Json::Value* root = &(*json)["Component"];
	if (root->isArray() == false || root->size() == 1)
	{
		json->removeMember("Component");
	}
	else
	if (root->size() == 2)
	{
		BreezeGame::Json::Value temp(BreezeGame::Json::objectValue);
		for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
		{
			if (propcomp->get_node() != &(*itor))
				temp = (*itor);
		}

		(*root) = temp;
	}
	else
	{
		BreezeGame::Json::Value temp(BreezeGame::Json::arrayValue);
		for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
		{
			if (propcomp->get_node() != &(*itor))
				temp.append((*itor));
		}

		(*root) = temp;
	}

	del_compkind();
	add_compkind();

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}
xui_method_explain(onity_proptempold, new_compkind,		void					)( BreezeGame::Json::Value* compnode )
{
	std::string   name = (*compnode)["ClassName"].asString();
	std::string   type = (*compnode)["Family"].asString() + "Old";
	xui_propkind* kind = new onity_propkind_compold(this, xui_global::ascii_to_unicode(name), type, onity_resource::icon_component, compnode);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_proptempold>(this, &onity_proptempold::on_propchanged);
	m_compkind.push_back(kind);
	add_propkind(kind);
}
xui_method_explain(onity_proptempold, add_compkind,		void					)( void )
{
	BreezeGame::Json::Value* json = m_template->GetNode();
	if (json->isMember("Component"))
	{
		BreezeGame::Json::Value* root = &(*json)["Component"];
		if (root->isArray() == false)
		{
			new_compkind(root);
		}
		else
		{
			for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
			{
				new_compkind(&(*itor));
			}
		}
	}
}
xui_method_explain(onity_proptempold, del_compkind,		void					)( void )
{
	for (u32 i = 0; i < m_compkind.size(); ++i)
	{
		m_compkind[i]->non_ctrl();
		del_propkind(m_compkind[i]);
	}

	m_compkind.clear();
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
	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
	propjsones->set_modify(true);

	//onity_propkind_compold* propkind = dynamic_cast<onity_propkind_compold*>(args.propdata->get_kind());
	//BreezeGame::Json::Value* node = propkind->get_node();
}
xui_method_explain(onity_proptempold, on_namechanged,	void					)( xui_component* sender, xui_method_args&     args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	rna_template(textbox->get_text());
	textbox->ini_drawer(xui_global::ascii_to_unicode(m_template->GetName()));
	if (m_linkdata)
	{
		xui_treenode* node = m_linkdata->get_node();
		node->use_linkdata();
	}
}

