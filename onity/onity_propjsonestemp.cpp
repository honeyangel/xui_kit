#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_propjsones.h"
#include "onity_propkind_entitycomp.h"
#include "onity_mainform.h"
#include "onity_savekind.h"
#include "onity_inspector.h"
#include "onity_propjsonestemp.h"

/*
//constructor
*/
xui_create_explain(onity_propjsonestemp)( onity_propfile* propfile, const std::string& tempname )
: onity_propleaf(propfile)
, m_tempname(tempname)
{
	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(tempname), "JsonesTemplate", xui_kindctrl::create, onity_resource::icon_entity, true);
	m_basekind->xm_namechanged += new xui_method_member<xui_method_args, onity_propjsonestemp>(this, &onity_propjsonestemp::on_namechanged);
	m_menukind = new xui_propkind(this, L"", "ComponentAdd", onity_kindctrl_entitycompadd::create, NULL, true, false, true);

	m_propkind.clear();
	m_propkind.push_back(m_basekind);
	m_propkind.push_back(m_menukind);
	m_propkind.push_back(m_savekind);
	add_compkind();
}

/*
//method
*/
xui_method_explain(onity_propjsonestemp, get_template,		Omiga::EntityTemplate*	)( void )
{
	return Omiga::EntityManager::Instance()->GetEntityTemplate(m_tempname);
}
xui_method_explain(onity_propjsonestemp, rna_template,		void					)( const std::wstring& text )
{
	std::string name = xui_global::unicode_to_ascii(text);
	if (Omiga::EntityManager::Instance()->GetEntityTemplate(name))
		return;

	Omiga::EntityTemplate* temp = get_template();
	if (temp)
	{
		m_tempname = name;
		Omiga::EntityManager::Instance()->RenameEntityTemplate(temp, name);
		onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
		propjsones->set_modify(true);

		m_basekind->set_name(text);
		xui_kindctrl* kindctrl = m_basekind->get_ctrl();
		if (kindctrl)
			kindctrl->refresh();
	}
}
xui_method_explain(onity_propjsonestemp, pst_template,		void					)( Omiga::EntityTemplate* temp )
{
	Omiga::EntityTemplate* self = get_template();
	if (self)
	{
		self->ChangeTemplate(temp->GetNode());
		self->SetName(m_tempname);
		del_compkind();
		add_compkind();
	}
}
xui_method_explain(onity_propjsonestemp, get_components,	const xui_propkind_vec&	)( void ) const
{
	return m_compkind;
}
xui_method_explain(onity_propjsonestemp, add_component,		void					)( const std::string& type, const std::string& name )
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

	Omiga::EntityTemplate* temp = get_template();
	if (temp)
	{
		onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
		propjsones->set_modify(true);

		BreezeGame::Json::Value* json = temp->GetNode();
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
}
xui_method_explain(onity_propjsonestemp, del_component,		void					)( xui_propkind* propkind )
{
	Omiga::EntityTemplate* temp = get_template();
	if (temp)
	{
		onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
		propjsones->set_modify(true);

		onity_propkind_entitycomp* comp = dynamic_cast<onity_propkind_entitycomp*>(propkind);
		BreezeGame::Json::Value*   json = temp->GetNode();
		BreezeGame::Json::Value*   root = &(*json)["Component"];
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
}
xui_method_explain(onity_propjsonestemp, new_compkind,		void					)( BreezeGame::Json::Value* compnode )
{
	std::string   name = (*compnode)["ClassName"].asString();
	std::string   type = (*compnode)["Family"].asString() + "Component";
	xui_propkind* kind = new onity_propkind_entitycomp(this, xui_global::ascii_to_unicode(name), type, onity_resource::icon_component, compnode);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_propjsonestemp>(this, &onity_propjsonestemp::on_propchanged);
	m_compkind.push_back(kind);
	add_propkind(kind);
}
xui_method_explain(onity_propjsonestemp, add_compkind,		void					)( void )
{
	Omiga::EntityTemplate* temp = get_template();
	if (temp)
	{
		BreezeGame::Json::Value* json = temp->GetNode();
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
}
xui_method_explain(onity_propjsonestemp, del_compkind,		void					)( void )
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
xui_method_explain(onity_propjsonestemp, get_dragtype,		std::string				)( void )
{
	return "EntityTemplate";
}
xui_method_explain(onity_propjsonestemp, get_dragdata,		void*					)( void )
{
	return get_template();
}

/*
//event
*/
xui_method_explain(onity_propjsonestemp, on_propchanged,	void					)( xui_component* sender, xui_method_propdata& args )
{
	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(get_propfile());
	propjsones->set_modify(true);
}
xui_method_explain(onity_propjsonestemp, on_namechanged,	void					)( xui_component* sender, xui_method_args&     args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	rna_template(textbox->get_text());
	textbox->ini_drawer(xui_global::ascii_to_unicode(m_tempname));
	for (u32 i = 0; i < m_linkdatavec.size(); ++i)
	{
		m_linkdatavec[i]->get_node()->use_linkdata();
	}
}

