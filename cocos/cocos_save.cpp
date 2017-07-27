#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "xui_toolbar.h"
#include "xui_desktop.h"
#include "cocos_save.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_pathdata.h"
#include "cocos_filedata.h"
#include "cocos_fileview.h"
#include "cocos_tileview.h"
#include "cocos_propfile.h"
#include "cocos_proppath.h"

xui_implement_rtti(cocos_save, xui_window);

/*
//constructor
*/
xui_create_explain(cocos_save)( void )
: xui_window(xui_vector<s32>(480, 640), true)
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_header	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_header,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_header,	set_text			)(L"Unsaved files");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL, 20,  L"flag", NULL, 0, false, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, false));
	m_save	= new xui_treeview(xui_vector<s32>(220), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_save,		ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_save,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_save,		set_drawcolor		)(true);
	xui_method_ptrcall(m_save,		set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_save,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_save,		set_allowmulti		)(true);

	m_selectall	= new xui_toggle(xui_vector<s32>(16), TOGGLE_NORMAL);
	xui_method_ptrcall(m_selectall, xm_toggleclick		) += new xui_method_member<xui_method_args, cocos_save>(this, &cocos_save::on_toggleclick);
	xui_method_ptrcall(m_selectall,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_selectall,	ini_toggle			)(true);
	xui_method_ptrcall(m_selectall,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_selectall, set_corner			)(3);
	xui_method_ptrcall(m_selectall, set_drawcolor		)(true);
	xui_method_ptrcall(m_selectall, set_backcolor		)(xui_colour::darkgray);

	m_selecttxt = new xui_drawer(xui_vector<s32>(128, 16));
	xui_method_ptrcall(m_selecttxt,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_selecttxt, set_borderrt		)(xui_rect2d<s32>(20, 0, 0, 0));
	xui_method_ptrcall(m_selecttxt,	set_text			)(L"Select All");

	m_yes		= new xui_button(xui_vector<s32>(80, 24));
	xui_method_ptrcall(m_yes,		xm_buttonclick		) += new xui_method_member<xui_method_args, cocos_save>(this, &cocos_save::on_buttonclick);
	xui_method_ptrcall(m_yes,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_yes,		set_corner			)(3);
	xui_method_ptrcall(m_yes,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_yes,		set_drawcolor		)(true);
	xui_method_ptrcall(m_yes,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_yes,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_yes,		ini_drawer			)(L"Yes");

	m_no		= new xui_button(xui_vector<s32>(80, 24));
	xui_method_ptrcall(m_no,		xm_buttonclick		) += new xui_method_member<xui_method_args, cocos_save>(this, &cocos_save::on_buttonclick);
	xui_method_ptrcall(m_no,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_no,		set_corner			)(3);
	xui_method_ptrcall(m_no,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_no,		set_drawcolor		)(true);
	xui_method_ptrcall(m_no,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_no,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_no,		ini_drawer			)(L"No");

	m_cancel	= new xui_button(xui_vector<s32>(80, 24));
	xui_method_ptrcall(m_cancel,	xm_buttonclick		) += new xui_method_member<xui_method_args, cocos_save>(this, &cocos_save::on_buttonclick);
	xui_method_ptrcall(m_cancel,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_cancel,	set_corner			)(3);
	xui_method_ptrcall(m_cancel,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cancel,	set_drawcolor		)(true);
	xui_method_ptrcall(m_cancel,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_cancel,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_cancel,	ini_drawer			)(L"Cancel");

	m_tool		= new xui_toolbar(xui_vector<s32>(128, 32));
	xui_method_ptrcall(m_tool,		ini_component		)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_tool,		set_borderrt		)(xui_rect2d<s32>(4, 8, 0, 0));
	xui_method_ptrcall(m_tool,		set_grap			)(8);
	xui_method_ptrcall(m_tool,		add_item			)(m_yes);
	xui_method_ptrcall(m_tool,		add_item			)(m_no);
	xui_method_ptrcall(m_tool,		add_item			)(m_cancel);

	m_bottom	= new xui_panel(xui_vector<s32>(128, 52));
	xui_method_ptrcall(m_bottom,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom,	set_borderrt		)(xui_rect2d<s32>(0, 4, 0, 0));
	xui_method_ptrcall(m_bottom,	add_child			)(m_tool);
	xui_method_ptrcall(m_bottom,	add_child			)(m_selecttxt);
	xui_method_ptrcall(m_bottom,	add_child			)(m_selectall);

	add_child(m_header);
	add_child(m_bottom);
	add_child(m_save);

	load();
}

/*
//method
*/


/*
//callback
*/
xui_method_explain(cocos_save, on_updateself,		void)( xui_method_update& args )
{
	xui_window::on_updateself(args);

	bool selectall = true;
	for (u32 i = 0; i < m_save->get_upmostnodecount(); ++i)
		selectall &= m_save->get_upmostnode(i)->get_linkdata()->get_flag(0);

	m_selectall->ini_toggle(selectall);
}
xui_method_explain(cocos_save, on_accept,			void)( xui_component* sender, xui_method_args&  args )
{
	save();
	xui_window::on_accept(sender, args);
	quit();
}
xui_method_explain(cocos_save, on_cancel,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_window::on_cancel(sender, args);
	xui_desktop::get_ins()->del_child(this);
}

/*
//event
*/
xui_method_explain(cocos_save, on_buttonclick,		void)( xui_component* sender, xui_method_args&  args )
{
	if (sender == m_yes)
	{
		xui_method_args other_args;
		on_accept(this, other_args);
	}
	else
	if (sender == m_cancel)
	{
		xui_method_args other_args;
		on_cancel(this, other_args);
	}
	else
	if (sender == m_no)
	{
		for (u32 i = 0; i < m_save->get_upmostnodecount(); ++i)
		{
			cocos_treedata* data = (cocos_treedata*)m_save->get_upmostnode(i)->get_linkdata();
			cocos_propfile* prop = dynamic_cast<cocos_propfile*>(data->get_prop());
			prop->del_tempfile();
		}

		quit();
	}
}
xui_method_explain(cocos_save, on_toggleclick,		void)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_save->get_upmostnodecount(); ++i)
	{
		xui_treenode* node = m_save->get_upmostnode(i);
		node->get_linkdata()->set_flag(0, m_selectall->was_push());
		node->use_linkdata();
	}
}

/*
//method
*/
xui_method_explain(cocos_save, load,				void)( void )
{
	std::vector<cocos_propfile*> savevec;
	std::vector<xui_treenode*>   pathvec = cocos_mainform::get_ptr()->get_project()->get_pathview()->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());

		const xui_proproot_vec& filevec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = filevec.begin(); itor != filevec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (propfile->was_modify())
			{
				savevec.push_back(propfile);
			}
		}
	}

	if (savevec.size() > 0)
	{
		for (u32 i = 0; i < savevec.size(); ++i)
		{
			m_save->add_upmostnode(i, new cocos_treedata(
				savevec[i]->get_fileicon(), 
				savevec[i]->get_fullname(),
				true,
				savevec[i]));
		}
	}
	else
	{
		quit();
	}
}
xui_method_explain(cocos_save, save,				void)( void )
{
	for (u32 i = 0; i < m_save->get_upmostnodecount(); ++i)
	{
		cocos_treedata* data = (cocos_treedata*)m_save->get_upmostnode(i)->get_linkdata();
		cocos_propfile* prop = dynamic_cast<cocos_propfile*>(data->get_prop());
		if (data->get_flag(0))
		{
			prop->save();
		}
	}
}
xui_method_explain(cocos_save, quit,				void)( void )
{
	::PostQuitMessage(0);
}


