#include "NPFile.h"
#include "xui_global.h"

#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_filedata.h"
#include "onity_proppath.h"
#include "onity_resource.h"
#include "onity_proplike.h"

/*
//constructor
*/
xui_create_explain(onity_proplike)( void )
: onity_proppath(onity_resource::icon_favorite, L"")
{
	NPFile file;
	if (file.Open("onity.favorite", NPFile::OM_READ))
	{
		while (file.End() == false)
		{
			std::string full;
			file.ReadLine(full);
			if (full.length() == 0)
				continue;

			std::wstring path = onity_filedata::get_path(xui_global::ascii_to_unicode(full));
			std::wstring file = onity_filedata::get_file(xui_global::ascii_to_unicode(full));
			path = path.substr(0, path.length()-1);
			file = onity_filedata::get_safe(file);

			onity_propfile* propfile = onity_mainform::get_ptr()->get_project()->get_pathfile(path, file);
			if (propfile)
			{
				m_fileprop.push_back(propfile);
			}
		}

		file.Close();
	}
}

/*
//destructor
*/
xui_delete_explain(onity_proplike)( void )
{
	m_fileprop.clear();
	m_pathprop.clear();
}

/*
//method
*/
xui_method_explain(onity_proplike, save, void)( void )
{
	NPFile file;
	if (file.Open("onity.favorite", NPFile::OM_WRITE))
	{
		for (u32 i = 0; i < m_fileprop.size(); ++i)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(m_fileprop[i]);
			std::wstring	fullname = propfile->get_fullname();
			file.WriteLine(xui_global::unicode_to_ascii(fullname));
		}

		file.Close();
	}
}