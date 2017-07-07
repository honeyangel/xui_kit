#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "cocos_project.h"
#include "cocos_mainform.h"
#include "cocos_resource.h"
#include "cocos_proppath.h"
#include "cocos_pathdata.h"

/*
//constructor
*/
xui_create_explain(cocos_pathdata)( const std::wstring& full, xui_proproot* prop, xui_treenode* link )
: cocos_filedata(cocos_resource::icon_folder, full, prop)
{
	m_link = link;
	if (m_prop == NULL)
		m_prop =  new cocos_proppath(full);
}

/*
//destructor
*/
xui_delete_explain(cocos_pathdata)( void )
{
	if (m_link == NULL)
		delete m_prop;
}

/*
//virtual
*/
xui_method_explain(cocos_pathdata, ntf_rename,		void					)( const std::wstring& last, const std::wstring& curr )
{
	cocos_filedata::ntf_rename(last, curr);

	cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(m_prop);
	const xui_proproot_vec& vec = proppath->get_fileprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(vec[i]);
		if (propfile)
			propfile->ntf_rename(last, curr);
	}
}

/*
//override
*/
xui_method_explain(cocos_pathdata, get_text,		std::wstring			)( u32 index )
{
	return get_file();
}
xui_method_explain(cocos_pathdata, set_text,		void					)( u32 index, const std::wstring& text )
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
				cocos_filedata* data = (cocos_filedata*)nodes[i]->get_linkdata();
				data->ntf_rename(last, curr);
			}

			//cocos_project* project = cocos_mainform::get_ptr()->get_project();
			//project->ntf_rename(last, curr);
		}
		xui_global::set_fwatchstart(work);
	}
}

/*
//static
*/
xui_method_explain(cocos_pathdata, new_leafnode,	void					)( xui_treenode* root )
{
	cocos_pathdata* data = (cocos_pathdata*)root->get_linkdata();
	std::vector<std::wstring> vec = xui_global::get_path(data->get_full());
	for (u32 i = 0; i < vec.size(); ++i)
	{
		std::wstring  full = data->get_full()+L"/"+vec[i];
		xui_treenode* node = root->add_leafnode(i, new cocos_pathdata(full, NULL, NULL));
		cocos_pathdata::new_leafnode(node);
	}
}
