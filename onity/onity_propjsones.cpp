#include "m3eFile.h"
#include "EntityManager.h"
#include "EntityTemplate.h"
#include "xui_global.h"
#include "onity_resource.h"
#include "onity_proptempold.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_propjsones.h"

/*
//constructor
*/
xui_create_explain(onity_propjsones)( const std::wstring& fullname )
: onity_propfile(onity_resource::icon_entity, fullname)
, m_modify(false)
{}

/*
//destructor
*/
xui_delete_explain(onity_propjsones)( void )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];
}

/*
//method
*/
xui_method_explain(onity_propjsones, get_subprop,	xui_proproot_vec	)( void )
{
	if (m_subprop.empty())
	{
		loadfromfile(false);
	}

	return m_subprop;
}
xui_method_explain(onity_propjsones, add_subprop,	xui_proproot*		)( void )
{
	std::stringstream text;
	text << "New Entity Template";
	if (Omiga::EntityManager::Instance()->GetEntityTemplate(text.str()))
	{
		s32 number = 0;
		while (true)
		{
			text.str("");
			text << L"New Entity Template";
			text << number;
			if (Omiga::EntityManager::Instance()->GetEntityTemplate(text.str()) == NULL)
				break;

			++number;
		}
	}

	Omiga::EntityTemplate* temp = new Omiga::EntityTemplate(text.str());
	Omiga::EntityManager::Instance()->AddEntityTemplate(temp);
	onity_proptempold* prop = new onity_proptempold(this, temp);
	m_subprop.push_back(prop);
	m_modify = true;

	return prop;
}
xui_method_explain(onity_propjsones, del_subprop,	void				)( xui_proproot* prop )
{
	xui_proproot_vec::iterator itor = std::find(
		m_subprop.begin(),
		m_subprop.end(),
		prop);

	if (itor != m_subprop.end())
	{
		onity_proptempold* proptemp = dynamic_cast<onity_proptempold*>(prop);
		Omiga::EntityManager::Instance()->RemoveEntityTemplate(proptemp->get_template());

		m_subprop.erase(itor);
		delete prop;
		m_modify = true;
	}
}
xui_method_explain(onity_propjsones, set_modify,	void				)( bool flag )
{
	m_modify = flag;
}

/*
//virtual
*/
xui_method_explain(onity_propjsones, can_rename,	bool				)( void )
{
	return false;
}
xui_method_explain(onity_propjsones, was_modify,	bool				)( void )
{
	return m_modify;
}
xui_method_explain(onity_propjsones, load,			void				)( void )
{
	loadfromfile(true);
}
xui_method_explain(onity_propjsones, save,			void				)( void )
{
	m_modify = false;
	xui_global::set_fwatchclose();

	BreezeGame::Json::Value  root;
	BreezeGame::Json::Value* node = &root;
	(*node) = BreezeGame::Json::Value(BreezeGame::Json::objectValue);
	node = &((*node)["EntitySystem"]);
	(*node) = BreezeGame::Json::Value(BreezeGame::Json::objectValue);
	node = &((*node)["EntityTemplate"]);
	(*node) = BreezeGame::Json::Value(BreezeGame::Json::objectValue);
	node = &((*node)["Template"]);
	(*node) = BreezeGame::Json::Value(BreezeGame::Json::arrayValue);
	for (u32 i = 0; i < m_subprop.size(); ++i)
	{
		onity_proptempold* proptemp = dynamic_cast<onity_proptempold*>(m_subprop[i]);
		BreezeGame::Json::Value* temp = proptemp->get_template()->GetNode();
		node->append(*temp);
	}
	BreezeGame::Json::StyledWriter writer;
	std::string text = writer.write(root);

	FILE* file = fopen(xui_global::unicode_to_ascii(m_fullname).c_str(), "w");
	if (file)
	{
		fwrite(text.c_str(), 1, text.length(), file);
		fclose(file);
	}

	xui_global::set_fwatchstart(xui_global::get_workpath());
}
xui_method_explain(onity_propjsones, loadfromfile,	void				)( bool notify )
{
	m_modify = false;
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];

	m_subprop.clear();

	m3eFile* file = m3eFileOpen(xui_global::unicode_to_ascii(m_fullname).c_str(), "rb", NULL);
	if (file)
	{
		m3eFileSeek(file, 0, M3E_FILESEEK_END);
		s32 size = m3eFileTell( file );
		m3eFileSeek(file, 0, M3E_FILESEEK_SET);
		std::string text;
		char* buffer = new char[size+1];
		buffer[size] = 0;
		m3eFileRead(buffer, 1, size, file);

		BreezeGame::Json::Reader reader;
		BreezeGame::Json::Value  root;
		reader.parse(std::string(buffer), root);
		delete [] buffer;
		m3eFileClose(file);

		std::map<Omiga::EntityTemplate*, xui_proproot*> propmap;
		BreezeGame::Json::Value* value = &root["EntitySystem"]["EntityTemplate"]["Template"];
		for(BreezeGame::Json::Value::iterator itor = value->begin(); itor != value->end(); itor++)
		{
			BreezeGame::Json::Value* node = &(*itor);
			std::string name = (*node)["TemplateName"].asString();
			Omiga::EntityTemplate* temp = Omiga::EntityManager::Instance()->GetEntityTemplate(name);
			if (temp)
			{
				if (propmap.find(temp) == propmap.end())
					temp->ChangeTemplate(node);
			}
			else
			{
				temp = new Omiga::EntityTemplate(node);
				Omiga::EntityManager::Instance()->AddEntityTemplate(temp);
			}

			if (propmap.find(temp) == propmap.end())
			{
				onity_proptempold* proptemp = new onity_proptempold(this, temp);
				m_subprop.push_back(proptemp);
				propmap[temp] = proptemp;
			}
		}
	}

	if (notify)
	{
		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->ntf_load(this);
	}
}
