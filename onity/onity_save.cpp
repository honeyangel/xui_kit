#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "onity_save.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_pathdata.h"
#include "onity_filedata.h"
#include "onity_fileview.h"
#include "onity_tileview.h"
#include "onity_propfile.h"
#include "onity_proppath.h"

xui_implement_rtti(onity_save, xui_window);

/*
//constructor
*/
xui_create_explain(onity_save)( void )
: xui_window(xui_vector<s32>(480, 640), true)
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_drawer	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_drawer, ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_drawer, set_text			)(L"Restore your unsaved file");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL, 200, L"flag", NULL, 0, true, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_save	= new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_save, ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_save, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_save, set_drawcolor		)(true);
	xui_method_ptrcall(m_save, set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_save, set_hscrollauto	)(false);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept, xm_buttonclick		) += new xui_method_member<xui_method_args, onity_save>(this, &onity_save::on_buttonclick);
	xui_method_ptrcall(m_accept, ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_accept, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept, set_corner			)(3);
	xui_method_ptrcall(m_accept, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept, set_drawcolor		)(true);
	xui_method_ptrcall(m_accept, set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept, set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept, ini_drawer			)(L"Save and Exit");

	m_cancel = new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_cancel, xm_buttonclick) += new xui_method_member<xui_method_args, onity_save>(this, &onity_save::on_buttonclick);
	xui_method_ptrcall(m_cancel, ini_component)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_cancel, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_cancel, set_corner)(3);
	xui_method_ptrcall(m_cancel, set_borderrt)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cancel, set_drawcolor)(true);
	xui_method_ptrcall(m_cancel, set_iconsize)(xui_vector<s32>(0));
	xui_method_ptrcall(m_cancel, set_textalign)(TEXTALIGN_CC);
	xui_method_ptrcall(m_cancel, ini_drawer)(L"Cancel");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 32));
	xui_method_ptrcall(m_bottom, ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom, set_borderrt		)(xui_rect2d<s32>(0, 8, 0, 0));
	xui_method_ptrcall(m_bottom, add_child			)(m_accept);
	xui_method_ptrcall(m_bottom, add_child			)(m_cancel);

	add_child(m_drawer);
	add_child(m_bottom);
	add_child(m_save);
}

/*
//method
*/
xui_method_explain(onity_save, load_unsavedfiles, void)(void)
{
	m_unsavepropfiles.clear();
	std::vector<xui_treenode*> pathvec = onity_mainform::get_ptr()->get_project()->get_pathview()->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		const xui_proproot_vec& filevec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = filevec.begin(); itor != filevec.end(); ++itor)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(*itor);
			if (propfile->was_modify())
			{
				m_unsavepropfiles.push_back(propfile);
			}
		}
	}

	for (int i = 0; i < m_unsavepropfiles.size(); ++i)
	{
		m_save->add_upmostnode(i, new xui_treedata(m_unsavepropfiles.at(i)->get_fullname(), true));
	}
}

/*
//callback
*/
xui_method_explain(onity_save, on_accept,				void		)( xui_component* sender, xui_method_args&  args )
{
	save();
	xui_window::on_accept(sender, args);
}

xui_method_explain(onity_save, on_cancel,				void		)(xui_component* sender, xui_method_args&  args)
{
	xui_window::on_cancel(sender, args);
}

/*
//event
*/
xui_method_explain(onity_save, on_buttonclick,		void		)( xui_component* sender, xui_method_args&  args )
{
	if (sender == m_accept)
	{
		for (int i = 0; i < m_save->get_upmostnodecount(); ++i)
		{
			xui_treedata* data = m_save->get_upmostnode(i)->get_linkdata();
			if (data)
			{
				std::wstring full = data->get_text(0);
				if (data->get_flag(0))
				{
					xui_global::cpy_file(xui_global::get_workpath() + full + L".tmp", xui_global::get_workpath() + full);
				}
				xui_global::del_file(xui_global::get_workpath() + full + L".tmp");
			}
		}
		xui_method_args other_args;
		on_accept(this, other_args);
	}
	else
	{
		xui_method_args other_args;
		on_cancel(this, other_args);
	}
	
	
}

/*
//method
*/
xui_method_explain(onity_save, save,				void		)( void )
{
	for (int i = 0; i < m_save->get_upmostnodecount(); ++i)
	{
		xui_treedata* data = m_save->get_upmostnode(i)->get_linkdata();
		if (data)
		{
			std::wstring full = data->get_text(0);
			if (data->get_flag(0))
			{
				m_unsavepropfiles.at(i)->save();
			}
		}
	}
}


