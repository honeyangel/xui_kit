#include "xui_global.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_textbox.h"
#include "xui_desktop.h"
#include "onity_tempctrl.h"
#include "onity_propkind_entitycomp.h"
#include "onity_resource.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_propentitytemp.h"

/*
//constructor
*/
xui_create_explain(onity_propentitytemp)( const std::string& tempname )
: onity_proproot()
, m_modify(false)
, m_tempname(tempname)
{
	Omiga::EntityTemplate* temp = get_template();
	if (temp)
	{
		BreezeGame::Json::Value* node = temp->GetNode();
		m_jsonnode = new BreezeGame::Json::Value(*node);
	}
	else
	{
		m_jsonnode = new BreezeGame::Json::Value(BreezeGame::Json::objectValue);
		(*m_jsonnode)["TemplateName"] = BreezeGame::Json::Value(m_tempname);
		(*m_jsonnode)["TemplateType"] = BreezeGame::Json::Value("Normal");
	}

	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(tempname), "EntityTemplate", xui_kindctrl::create, onity_resource::icon_entity, true);
	m_basekind->xm_namechanged += new xui_method_member<xui_method_args, onity_propentitytemp>(this, &onity_propentitytemp::on_namechanged);
	m_tempkind = new xui_propkind(this, L"", "TemplateCtrl", onity_tempctrl::create, NULL, true, false);
	m_menukind = new xui_propkind(this, L"", "ComponentAdd", onity_kindctrl_entitycompadd::create, NULL, true, false, true);

	m_propkind.clear();
	m_propkind.push_back(m_basekind);
	m_propkind.push_back(m_tempkind);
	m_propkind.push_back(m_menukind);
	add_compkind();
}

/*
//destructor
*/
xui_delete_explain(onity_propentitytemp)( void )
{
	delete m_jsonnode;
}

/*
//method
*/
xui_method_explain(onity_propentitytemp, was_modify,		bool					)( void ) const
{
	return m_modify;
}
xui_method_explain(onity_propentitytemp, set_modify,		void					)( bool flag )
{
	m_modify = flag;
}
xui_method_explain(onity_propentitytemp, get_tempname,		const std::string&		)( void ) const
{
	return m_tempname;
}
xui_method_explain(onity_propentitytemp, get_template,		Omiga::EntityTemplate*	)( void )
{
	return Omiga::EntityManager::Instance()->GetEntityTemplate(m_tempname);
}
xui_method_explain(onity_propentitytemp, rna_template,		void					)( const std::wstring& text )
{
	std::string name = xui_global::unicode_to_ascii(text);
	if (name == m_tempname)
		return;

	Omiga::EntityTemplate* temp = Omiga::EntityManager::Instance()->GetEntityTemplate(name);
	m_modify   = (temp == NULL);
	m_tempname =  name;
	m_basekind->set_name(text);
	if (temp)
	{
		BreezeGame::Json::Value* tempnode = temp->GetNode();
		(*m_jsonnode) = BreezeGame::Json::Value(*tempnode);

		del_compkind();
		add_compkind();
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->get_propview()->reset();
	}
	else
	{
		(*m_jsonnode)["TemplateName"] = BreezeGame::Json::Value(m_tempname);

		xui_kindctrl* kindctrl = m_basekind->get_ctrl();
		if (kindctrl)
			kindctrl->refresh();
	}
}
xui_method_explain(onity_propentitytemp, get_components,	const xui_propkind_vec&	)( void ) const
{
	return m_compkind;
}
xui_method_explain(onity_propentitytemp, add_component,		void					)( const std::string&  type, const std::string& name )
{
	for (u32 i = 0; i < m_compkind.size(); ++i)
	{
		onity_propkind_entitycomp* propcomp = dynamic_cast<onity_propkind_entitycomp*>(m_compkind[i]);
		BreezeGame::Json::Value* node = propcomp->get_node();
		if ((*node)["Family"].asString() == type)
		{
			xui_desktop::get_ins()->show_message(L"Has same family component!", 1);
			return;
		}
	}

	set_modify(true);

	BreezeGame::Json::Value* root = NULL;
	BreezeGame::Json::Value  comp(BreezeGame::Json::objectValue);
	comp["Family"]    = type;
	comp["ClassName"] = name;

	if (m_jsonnode->isMember("Component"))
	{
		root = &(*m_jsonnode)["Component"];
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
		(*m_jsonnode)["Component"] = BreezeGame::Json::Value(BreezeGame::Json::objectValue);
		root = &(*m_jsonnode)["Component"];
		(*root) = comp;
	}

	del_compkind();
	add_compkind();

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}
xui_method_explain(onity_propentitytemp, del_component,		void					)( xui_propkind* propkind )
{
	set_modify(true);

	onity_propkind_entitycomp* comp = dynamic_cast<onity_propkind_entitycomp*>(propkind);
	BreezeGame::Json::Value*   root = &(*m_jsonnode)["Component"];
	if (root->isArray() == false || root->size() == 1)
	{
		m_jsonnode->removeMember("Component");
	}
	else
	if (root->size() == 2)
	{
		BreezeGame::Json::Value temp(BreezeGame::Json::objectValue);
		for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
		{
			if (comp->get_node() != &(*itor))
				temp = (*itor);
		}

		(*root) = temp;
	}
	else
	{
		BreezeGame::Json::Value temp(BreezeGame::Json::arrayValue);
		for (BreezeGame::Json::Value::iterator itor = root->begin(); itor != root->end(); itor++)
		{
			if (comp->get_node() != &(*itor))
				temp.append((*itor));
		}

		(*root) = temp;
	}

	del_compkind();
	add_compkind();

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->get_propview()->reset();
}

xui_method_explain(onity_propentitytemp, new_compkind,		void					)( BreezeGame::Json::Value* compnode )
{
	std::string   name = (*compnode)["ClassName"].asString();
	std::string   type = (*compnode)["Family"].asString() + "Component";
	xui_propkind* kind = new onity_propkind_entitycomp(this, xui_global::ascii_to_unicode(name), type, onity_resource::icon_component, compnode);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_propentitytemp>(this, &onity_propentitytemp::on_propchanged);
	m_compkind.push_back(kind);
	add_propkind(kind);
}
xui_method_explain(onity_propentitytemp, add_compkind,		void					)( void )
{
	if (m_jsonnode->isMember("Component"))
	{
		BreezeGame::Json::Value* root = &(*m_jsonnode)["Component"];
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
xui_method_explain(onity_propentitytemp, del_compkind,		void					)( void )
{
	for (u32 i = 0; i < m_compkind.size(); ++i)
	{
		m_compkind[i]->non_ctrl();
		del_propkind(m_compkind[i]);
	}

	m_compkind.clear();
}

/*
//event
*/
xui_method_explain(onity_propentitytemp, on_namechanged,	void					)( xui_component* sender, xui_method_args&     args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	rna_template(textbox->get_text());
	textbox->ini_drawer(xui_global::ascii_to_unicode(m_tempname));
	if (m_linkdata)
	{
		xui_treenode* node = m_linkdata->get_node();
		node->use_linkdata();
	}
}
xui_method_explain(onity_propentitytemp, on_propchanged,	void					)( xui_component* sender, xui_method_propdata& args )
{
	set_modify(true);
}