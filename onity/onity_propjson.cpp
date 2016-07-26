#include "m3eFile.h"
#include "EntityManager.h"
#include "EntityTemplate.h"
#include "xui_global.h"
#include "onity_resource.h"
#include "onity_proptempold.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_propjson.h"

/*
//constructor
*/
xui_create_explain(onity_propjson)( const std::wstring& fullname )
: onity_propfile(onity_resource::icon_local, fullname)
, m_modify(false)
{}

/*
//destructor
*/
xui_delete_explain(onity_propjson)( void )
{
	for (u32 i = 0; i < m_templates.size(); ++i)
		delete m_templates[i];
}

/*
//method
*/
xui_method_explain(onity_propjson, get_templates,	xui_proproot_vec	)( void )
{
	if (m_templates.empty())
	{
		load();
	}

	return m_templates;
}
xui_method_explain(onity_propjson, set_modify,		void				)( bool flag )
{
	m_modify = flag;
}

/*
//virtual
*/
xui_method_explain(onity_propjson, can_rename,		bool				)( void )
{
	return false;
}
xui_method_explain(onity_propjson, was_modify,		bool				)( void )
{
	return m_modify;
}
xui_method_explain(onity_propjson, load,			void				)( void )
{
	m_modify = false;
	for (u32 i = 0; i < m_templates.size(); ++i)
		delete m_templates[i];

	m_templates.clear();

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

		BreezeGame::Json::Value* value = &root["EntitySystem"]["EntityTemplate"]["Template"];
		for(BreezeGame::Json::Value::iterator itor = value->begin(); itor != value->end(); itor++)
		{
			BreezeGame::Json::Value* node = &(*itor);
			std::string name = (*node)["TemplateName"].asString();
			Omiga::EntityTemplate* temp = Omiga::EntityManager::Instance()->GetEntityTemplate(name);
			if (temp)
			{
				temp->ChangeTemplate(node);
			}
			else
			{
				temp = new Omiga::EntityTemplate(node);
				Omiga::EntityManager::Instance()->AddEntityTemplate(temp);
			}

			m_templates.push_back(new onity_proptempold(this, temp));
		}
	}

	onity_project* project = onity_mainform::get_ptr()->get_project();
	project->ntf_load(this);
}
xui_method_explain(onity_propjson, save,			void				)( void )
{

}