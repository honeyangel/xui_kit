#include "xui_global.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_filedata.h"
#include "cocos_proppath.h"
#include "cocos_resource.h"
#include "cocos_proplike.h"

/*
//constructor
*/
xui_create_explain(cocos_proplike)( void )
: cocos_proppath(cocos_resource::icon_favorite, L"")
{
	load();
}

/*
//destructor
*/
xui_delete_explain(cocos_proplike)( void )
{
	m_fileprop.clear();
	m_pathprop.clear();
}

/*
//method
*/
xui_method_explain(cocos_proplike, load, void)( void )
{
	m_fileprop.clear();
	m_pathprop.clear();

	FILE* file = fopen("cocos_editor/cocos_editor.favorite", "r");
	if (file)
	{
		while (feof(file) == false)
		{
			std::string line = xui_global::get_fileline(file);
			if (line.length() == 0)
				continue;

			std::wstring path = cocos_filedata::get_path(xui_global::ascii_to_unicode(line));
			std::wstring file = cocos_filedata::get_file(xui_global::ascii_to_unicode(line));
			path = path.substr(0, path.length() - 1);

			cocos_propfile* propfile = cocos_mainform::get_ptr()->get_project()->get_pathfile(path, file);
			if (propfile)
			{
				m_fileprop.push_back(propfile);
			}
		}

		fclose(file);
	}
}
xui_method_explain(cocos_proplike, save, void)( void )
{
	FILE* file = fopen("cocos_editor/cocos_editor.favorite", "w");
	if (file)
	{
		for (u32 i = 0; i < m_fileprop.size(); ++i)
		{
			cocos_propfile* prop = dynamic_cast<cocos_propfile*>(m_fileprop[i]);
			std::wstring	full = prop->get_fullname();
			std::string		line = xui_global::unicode_to_ascii(full) + "\n";
			fwrite((void*)line.c_str(), 1, line.length(), file);
		}

		fclose(file);
	}
}