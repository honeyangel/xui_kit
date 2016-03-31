#include "NPFileName.h"
#include "NPFile.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"

#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_resource.h"
#include "onity_proppath.h"
#include "onity_pathdata.h"

/*
//constructor
*/
xui_create_explain(onity_pathdata)( const std::wstring& full, bool create_prop )
: xui_treedata(full, onity_resource::icon_folder)
, m_prop(NULL)
{
	if (create_prop)
	{
		m_prop = new xui_proproot;
		m_prop->add_propkind(new onity_proppath(m_prop, get_text(0), this));
	}
}

/*
//destructor
*/
xui_delete_explain(onity_pathdata)( void )
{
	delete m_prop;
}

/*
//method
*/
xui_method_explain(onity_pathdata, get_prop,		xui_proproot*		)( void )
{
	return m_prop;
}
xui_method_explain(onity_pathdata, get_full,		const std::wstring&	)( void ) const
{
	return m_text;
}
xui_method_explain(onity_pathdata, set_full,		void				)( const std::wstring& full )
{
	if (m_text != full)
	{
		m_text  = full;
		m_node->use_linkdata();
		if (m_prop)
		{
			xui_propkind* propkind = m_prop->get_propkind().front();
			propkind->set_name(get_text(0));

			xui_kindctrl* kindctrl = propkind->get_ctrl();
			if (kindctrl)
				kindctrl->refresh();
		}
	}
}
xui_method_explain(onity_pathdata, get_path,		std::wstring		)( void ) const
{
	std::wstring  temp = m_text;

	int index  = temp.find_last_of(L'/');
	if (index == -1)
		index  = temp.find_last_of(L'\\');
	if (index == -1)
		return L"";

	return temp.erase(index+1);
}
xui_method_explain(onity_pathdata, get_text,		std::wstring		)( u32 index )
{
	std::wstring temp = m_text;

	int npos  = temp.find_last_of(L'/');
	if (npos == -1)
		npos  = temp.find_last_of(L'\\');
	if (npos != -1)
		temp.erase(0, npos+1);

	return temp;
}
xui_method_explain(onity_pathdata, set_text,		void				)( u32 index, const std::wstring& text )
{
	xui_global::set_fwatchclose();
	std::wstring path = get_path()+text;
	if (xui_global::rna_file(m_text, path))
	{
		std::wstring last = m_text;
		onity_project* project = onity_mainform::get_ptr()->get_project();
		if (m_node->was_ancestor(project))
			project->on_pathrename(last, path, text);
	}
	xui_global::set_fwatchstart(xui_global::get_workpath());
}

/*
//static
*/
xui_method_explain(onity_pathdata, load_meta,		void				)( u08 type, const std::string& fullname )
{
	NPFile file;
	if (file.Open(fullname+".meta", NPFile::OM_READ))
	{
		std::string textname;	file.ReadLine(textname);
		std::string pathname;	file.ReadLine(pathname);
		std::string	filename;	file.ReadLine(filename);
		std::string textgrap;   file.ReadLine(textgrap);

		npu32 guid;
		std::stringstream ss(textname.c_str());
		ss >> guid;

		NP2DSAssetFileMgr* file_mgr = NULL;
		switch (type)
		{
		case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
		case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
		case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
		}

		// check
		npu32 id = file_mgr->GetFileID(guid);
		if (id != -1)
		{
			std::string temppath = file_mgr->GetFilePH(id);
			std::string tempfile = file_mgr->GetFileFN(id);
			std::string tempfull = temppath + tempfile;
			if (temppath != pathname || 
				tempfile != filename)
			{
				std::wstringstream message;
				message << L"Error Meta\n";
				message << fullname.c_str();
				message << "\n";
				message << tempfull.c_str();
				xui_desktop::get_ins()->show_message(message.str(), 1);
			}

		}
		else
		{
			file_mgr->AddFileGuid(pathname, filename, guid);
		}
	}
}
xui_method_explain(onity_pathdata, create_subnode,	void				)( xui_treenode* root )
{
	std::vector<std::wstring> vec;
	onity_pathdata* data = (onity_pathdata*)root->get_linkdata();

	u32 index = 0;
	vec = xui_global::get_path(data->get_full());
	for (u32 i = 0; i < vec.size(); ++i,++index)
	{
		std::wstring  full = data->get_full()+L"/"+vec[i];
		xui_treenode* node = root->add_leafnode(index, new onity_pathdata(full, false));
		onity_pathdata::create_subnode(node);
	}

	std::string pathname = xui_global::unicode_to_ascii(data->get_full()) + "/";
	vec = xui_global::get_file(data->get_full());
	for (u32 i = 0; i < vec.size(); ++i)
	{
		std::string filename = xui_global::unicode_to_ascii(vec[i]);
		std::string suffname = NPFileNameHelper::SuffName(filename);
		u08 type = META_NONE;
		if		(suffname == ".npModule") type = META_MODULE;
		else if (suffname == ".npSprite") type = META_SPRITE;
		else if (suffname == ".npAction") type = META_ACTION;
		else
		{}

		if (type != META_NONE)
		{
			load_meta(type, pathname+filename);
		}
	}
}
