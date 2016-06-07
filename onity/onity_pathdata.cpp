#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "onity_project.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_proppath.h"
#include "onity_pathdata.h"

/*
//constructor
*/
xui_create_explain(onity_pathdata)( const std::wstring& full, xui_proproot* prop, xui_treenode* link )
: onity_filedata(onity_resource::icon_folder, full, prop)
{
	m_link = link;
	if (m_prop == NULL)
		m_prop =  new onity_proppath(full);
}

/*
//destructor
*/
xui_delete_explain(onity_pathdata)( void )
{
	if (m_link == NULL)
		delete m_prop;
}

/*
//virtual
*/
xui_method_explain(onity_pathdata, ntf_rename,		void					)( const std::wstring& last, const std::wstring& curr )
{
	onity_filedata::ntf_rename(last, curr);

	onity_proppath* proppath = dynamic_cast<onity_proppath*>(m_prop);
	const xui_proproot_vec& vec = proppath->get_fileprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propfile* propfile = dynamic_cast<onity_propfile*>(vec[i]);
		if (propfile)
			propfile->ntf_rename(last, curr);
	}
}

/*
//override
*/
xui_method_explain(onity_pathdata, get_text,		std::wstring			)( u32 index )
{
	return get_file();
}
xui_method_explain(onity_pathdata, set_text,		void					)( u32 index, const std::wstring& text )
{
	//代表右边节点
	if (m_link)
	{
		m_link->get_linkdata()->set_text(index, text);
	}
	//代表左边节点
	else
	{
		std::wstring work = xui_global::get_workpath();
		std::wstring last = m_text;
		std::wstring curr = get_path()+text;

		xui_global::set_fwatchclose();
		if (xui_global::rna_file(last, curr))
		{
			std::vector<xui_treenode*> nodes;
			m_node->get_leafnodetotal( nodes );
			for (u32 i = 0; i < nodes.size(); ++i)
			{
				onity_filedata* data = (onity_filedata*)nodes[i]->get_linkdata();
				data->ntf_rename(last, curr);
			}

			onity_project* project = onity_mainform::get_ptr()->get_project();
			project->ntf_rename(last, curr);
		}
		xui_global::set_fwatchstart(work);
	}
}

/*
//static
*/
xui_method_explain(onity_pathdata, new_leafnode,	void					)( xui_treenode* root )
{
	onity_pathdata* data = (onity_pathdata*)root->get_linkdata();
	std::vector<std::wstring> vec = xui_global::get_path(data->get_full());
	for (u32 i = 0; i < vec.size(); ++i)
	{
		std::wstring  full = data->get_full()+L"/"+vec[i];
		xui_treenode* node = root->add_leafnode(i, new onity_pathdata(full, NULL, NULL));
		onity_pathdata::new_leafnode(node);
	}
}
