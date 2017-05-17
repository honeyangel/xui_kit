#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "onity_backup.h"
#include "onity_filedata.h"
#include "onity_propfile.h"
#include "onity_treedata.h"
#include "onity_mainform.h"

xui_implement_rtti(onity_backup, xui_window);

/*
//constructor
*/
xui_create_explain(onity_backup)( void )
: xui_window(xui_vector<s32>(480, 640), true)
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_header	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_header,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_header,	set_text			)(L"Restore your backup file");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL, 20,  L"flag", NULL, 0, true, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_backup	= new xui_treeview(xui_vector<s32>(225), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_backup,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_backup,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_backup,	set_drawcolor		)(true);
	xui_method_ptrcall(m_backup,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_backup,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_backup,	set_allowmulti		)(true);

	m_selectall	= new xui_toggle(xui_vector<s32>(16), TOGGLE_NORMAL);
	xui_method_ptrcall(m_selectall, xm_toggleclick		) += new xui_method_member<xui_method_args, onity_backup>(this, &onity_backup::on_toggleclick);
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

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_backup>(this, &onity_backup::on_buttonclick);
	xui_method_ptrcall(m_accept,	ini_component		)(ALIGNHORZ_C, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_accept,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept,	set_corner			)(3);
	xui_method_ptrcall(m_accept,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept,	set_drawcolor		)(true);
	xui_method_ptrcall(m_accept,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept,	ini_drawer			)(L"Restore");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 48));
	xui_method_ptrcall(m_bottom,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom,	set_borderrt		)(xui_rect2d<s32>(0, 4, 0, 4));
	xui_method_ptrcall(m_bottom,	add_child			)(m_accept);
	xui_method_ptrcall(m_bottom,	add_child			)(m_selecttxt);
	xui_method_ptrcall(m_bottom,	add_child			)(m_selectall);

	add_child(m_header);
	add_child(m_bottom);
	add_child(m_backup);

	load();
}

/*
//callback
*/
xui_method_explain(onity_backup, on_updateself,		void)( xui_method_update& args )
{
	xui_window::on_updateself(args);

	bool selectall = true;
	for (u32 i = 0; i < m_backup->get_upmostnodecount(); ++i)
		selectall &= m_backup->get_upmostnode(i)->get_linkdata()->get_flag(0);

	m_selectall->ini_toggle(selectall);
}
xui_method_explain(onity_backup, on_accept,			void)( xui_component* sender, xui_method_args& args )
{
	restore();
	xui_window::on_accept(sender, args);
}

/*
//event
*/
xui_method_explain(onity_backup, on_buttonclick,	void)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_accept)
	{
		xui_method_args other_args;
		on_accept(this, other_args);
	}
}
xui_method_explain(onity_backup, on_toggleclick,	void)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_backup->get_upmostnodecount(); ++i)
	{
		xui_treenode* node = m_backup->get_upmostnode(i);
		node->get_linkdata()->set_flag(0, m_selectall->was_push());
		node->use_linkdata();
	}
}
/*
//method
*/
xui_method_explain(onity_backup, restore,			void)( void )
{
	for (u32 i = 0; i < m_backup->get_upmostnodecount(); ++i)
	{
		xui_treedata* data = m_backup->get_upmostnode(i)->get_linkdata();
		std::wstring  full = data->get_text(0);
		if (data->get_flag(0))
		{
			xui_global::cpy_file(full+L".tmp", full);
		}

		xui_global::del_file(full+L".tmp");
	}
}
xui_method_explain(onity_backup, load,				void)( void )
{
	onity_mainform* mainform = onity_mainform::get_ptr();
	for (u32 i = 0; i < mainform->get_backupfilenum(); ++i)
	{
		onity_propfile* prop = dynamic_cast<onity_propfile*>(mainform->get_backupfile(i));
		m_backup->add_upmostnode(i, new onity_treedata(
			prop->get_fileicon(), 
			prop->get_fullname(),
			true,
			prop));
	}
}
