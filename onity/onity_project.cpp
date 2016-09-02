#include "NPFileName.h"
#include "NPStringUtil.h"
#include "NP2DSStateCtrl.h"
#include "NP2DSSceneFile.h"
#include "NP2DSImageFile.h"
#include "NP2DSFrameFile.h"
#include "NP2DSActorFile.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"

#include "xui_timer.h"
#include "xui_timermgr.h"
#include "xui_bitmap.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_toggle.h"
#include "xui_button.h"
#include "xui_textbox.h"
#include "xui_panel.h"
#include "xui_linebox.h"
#include "xui_dropbox.h"
#include "xui_treeview.h"
#include "xui_listview.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_slider.h"
#include "xui_toolbar.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_pathdata.h"
#include "onity_filedata.h"
#include "onity_fileview.h"
#include "onity_tileview.h"
#include "onity_propfile.h"
#include "onity_prop2dsres.h"
#include "onity_prop2dsasset.h"
#include "onity_2dsassetdata.h"
#include "onity_propcontroller.h"
#include "onity_propparticle.h"
#include "onity_proppath.h"
#include "onity_propactor.h"
#include "onity_propjsones.h"
#include "onity_propcourse.h"
#include "onity_propjsonestemp.h"
#include "onity_jsonestempdata.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_mainform.h"
#include "onity_animator.h"
#include "onity_timeline.h"
#include "onity_hierarchy.h"
#include "onity_project.h"

xui_implement_rtti(onity_project, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_project)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
, m_curridx(0)
{
	ini_namectrl(onity_resource::icon_project, L"Project");

	xui_menu* menu1	= xui_menu::create(80);
	m_folder		= menu1->add_item(onity_resource::icon_folder,	 L"Folder");
	menu1->add_separate();
	m_controller	= menu1->add_item(onity_resource::icon_animator, L"Animation Controller");
	m_particle		= menu1->add_item(onity_resource::icon_particle, L"Particle");
	m_course		= menu1->add_item(onity_resource::icon_scene,	 L"Scene");
	xui_method_ptrcall(m_folder,	xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_folderclick);
	xui_method_ptrcall(m_controller,xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_controllerclick);
	xui_method_ptrcall(m_particle,	xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_particleclick);
	xui_method_ptrcall(m_course,	xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_courseclick);

	m_timer		= xui_timermgr::get_ins()->add_timer(this, 1.0f, NULL);
	xui_method_ptrcall(m_timer,		xm_tick				) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_timertick);

	m_create	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu1);

	m_search	= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_searchtextchanged);
	xui_method_ptrcall(m_search,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer			)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);
	xui_method_ptrcall(m_search,	set_hintdraw		)(xui_family_render(xui_colour::gray));

	m_clear		= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(onity_resource::icon_clear);

	m_backpath	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_backpath,	ini_drawer			)(onity_resource::icon_left);
	xui_method_ptrcall(m_backpath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_backpath,	set_enable			)(false);
	xui_method_ptrcall(m_backpath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_backpath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_forepath	= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_forepath,	ini_drawer			)(onity_resource::icon_right);
	xui_method_ptrcall(m_forepath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_forepath,	set_enable			)(false);
	xui_method_ptrcall(m_forepath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_forepath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_add		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_add,		ini_drawer			)(onity_resource::icon_add);
	xui_method_ptrcall(m_add,		set_drawcolor		)(true);
	xui_method_ptrcall(m_add,		set_enable			)(false);
	xui_method_ptrcall(m_add,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_add,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_del		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_del,		ini_drawer			)(onity_resource::icon_remove);
	xui_method_ptrcall(m_del,		set_drawcolor		)(true);
	xui_method_ptrcall(m_del,		set_enable			)(false);
	xui_method_ptrcall(m_del,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_del,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_copy		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_copy,		ini_drawer			)(onity_resource::icon_copy);
	xui_method_ptrcall(m_copy,		set_drawcolor		)(true);
	xui_method_ptrcall(m_copy,		set_enable			)(false);
	xui_method_ptrcall(m_copy,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_copy,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_move		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_move,		ini_drawer			)(onity_resource::icon_cut);
	xui_method_ptrcall(m_move,		set_drawcolor		)(true);
	xui_method_ptrcall(m_move,		set_enable			)(false);
	xui_method_ptrcall(m_move,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_move,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	m_paste		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_paste,		ini_drawer			)(onity_resource::icon_paste);
	xui_method_ptrcall(m_paste,		set_drawcolor		)(true);
	xui_method_ptrcall(m_paste,		set_enable			)(false);
	xui_method_ptrcall(m_paste,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_paste,		xm_buttonclick		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_linetoolclick);

	xui_linebox* linebox1 = new xui_linebox(xui_vector<s32>(20));
	xui_method_ptrcall(linebox1,	set_corner			)(3);
	xui_method_ptrcall(linebox1,	add_linectrl		)(m_backpath);
	xui_method_ptrcall(linebox1,	add_linectrl		)(m_forepath);

	xui_linebox* linebox2 = new xui_linebox(xui_vector<s32>(20));
	xui_method_ptrcall(linebox2,	set_corner			)(3);
	xui_method_ptrcall(linebox2,	add_linectrl		)(m_add);
	xui_method_ptrcall(linebox2,	add_linectrl		)(m_del);

	xui_linebox* linebox3 = new xui_linebox(xui_vector<s32>(20));
	xui_method_ptrcall(linebox3,	set_corner			)(3);
	xui_method_ptrcall(linebox3,	add_linectrl		)(m_copy);
	xui_method_ptrcall(linebox3,	add_linectrl		)(m_move);
	xui_method_ptrcall(linebox3,	add_linectrl		)(m_paste);

	m_linetool	= new xui_toolbar(xui_vector<s32>(0, 20));
	xui_method_ptrcall(m_linetool,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_linetool,	add_item			)(linebox1);
	xui_method_ptrcall(m_linetool,	add_separate		)();
	xui_method_ptrcall(m_linetool,	add_item			)(linebox2);
	xui_method_ptrcall(m_linetool,	add_separate		)();
	xui_method_ptrcall(m_linetool,	add_item			)(linebox3);

	m_filter	= new xui_dropbox(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_filter,	xm_selection		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_filterselection);
	xui_method_ptrcall(m_filter,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_filter,	set_drawcolor		)(true);
	xui_method_ptrcall(m_filter,	set_backcolor		)(xui_button::default_backcolor);
	xui_method_ptrcall(m_filter,	set_corner			)(3);
	xui_method_ptrcall(m_filter,	set_borderrt		)(xui_rect2d<s32>(4, 4, 0, 4));
	xui_method_ptrcall(m_filter,	set_readonly		)(true);
	xui_method_ptrcall(m_filter,	add_item			)(L"All");
	xui_method_ptrcall(m_filter,	add_item			)(L"Texture");
	xui_method_ptrcall(m_filter,	add_item			)(L"Module");
	xui_method_ptrcall(m_filter,	add_item			)(L"Sprite");
	xui_method_ptrcall(m_filter,	add_item			)(L"Action");
	xui_method_ptrcall(m_filter,	add_item			)(L"Particle");
	xui_method_ptrcall(m_filter,	add_item			)(L"Controller");
	xui_method_ptrcall(m_filter,	add_item			)(L"Json");
	xui_method_ptrcall(m_filter,	ini_dropbox			)(0);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_headperform);
	xui_method_ptrcall(m_head,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor		)(false);
	xui_method_ptrcall(m_head,		set_borderrt		)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_head,		add_child			)(m_create);
	xui_method_ptrcall(m_head,		add_child			)(m_search);
	xui_method_ptrcall(m_head,		add_child			)(m_clear );
	xui_method_ptrcall(m_head,		add_child			)(m_filter);
	xui_method_ptrcall(m_head,		add_child			)(m_linetool);

	xui_menu* menu2	= xui_menu::create(80);
	m_showfind		= menu2->add_item(NULL, L"Show in Finder");
	m_loadtype		= menu2->add_item(NULL, L"Modify All LoadType");
	m_freetype		= menu2->add_item(NULL, L"Modify All FreeType");
	menu2->add_separate();
	m_property		= menu2->add_item(NULL, L"Property");
	xui_menu* menu3 = xui_menu::create(80);
	m_auto			= menu3->add_item(NULL, L"Auto");
	m_never			= menu3->add_item(NULL, L"Never");
	m_immediate		= menu3->add_item(NULL, L"Immediate");
	xui_menu* menu4 = xui_menu::create(80);
	m_on			= menu4->add_item(NULL, L"On");
	m_off			= menu4->add_item(NULL, L"Off");
	xui_method_ptrcall(m_loadtype,	set_submenu			)(menu3);
	xui_method_ptrcall(m_freetype,	set_submenu			)(menu4);
	xui_method_ptrcall(m_showfind,	xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_showfindclick);
	xui_method_ptrcall(m_property,	xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_propertyclick);
	xui_method_ptrcall(m_auto,		xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_freetypeclick);
	xui_method_ptrcall(m_never,		xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_freetypeclick);
	xui_method_ptrcall(m_immediate, xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_freetypeclick);
	xui_method_ptrcall(m_on,		xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_loadtypeclick);
	xui_method_ptrcall(m_off,		xm_click			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_loadtypeclick);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_pathview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_pathview,	xm_selectedchange	) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_pathviewselection);
	xui_method_ptrcall(m_pathview,	ini_component		)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_pathview,	set_acceptdrag		)(false);
	xui_method_ptrcall(m_pathview,	set_allowmulti		)(false);
	xui_method_ptrcall(m_pathview,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_pathview,	set_contextmenu		)(menu2);

	m_fileview	= new onity_fileview;
	xui_method_ptrcall(m_fileview,	xm_fileviewnodeclick) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_fileviewnodeclick);
	xui_method_ptrcall(m_fileview,	xm_fileviewdoubleclk) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_fileviewdoubleclk);
	xui_method_ptrcall(m_fileview,	xm_fileviewassetdrag) += new xui_method_member<xui_method_dragdrop, onity_project>(this, &onity_project::on_fileviewassetdrag);
	xui_method_ptrcall(m_fileview,	ini_component		)(0, 0, DOCKSTYLE_F);

	m_pathpane	= new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_pathpane,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_pathpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_pathpane,	set_borderrt		)(xui_rect2d<s32>(8, 0, 8, 0));
	xui_method_ptrcall(m_pathpane,	set_hscrollauto		)(false);

	m_sizeroll	= new xui_slider(xui_vector<s32>(64, 16), FLOWSTYLE_H, ARROWDRAW_NONE);
	xui_method_ptrcall(m_sizeroll,	set_backcolor		)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_sizeroll,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_sizeroll,	ini_component		)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_sizeroll,	ini_scroll			)(60, 0);
	xui_method_ptrcall(m_sizeroll,	xm_scroll			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_sizerollscroll);

	m_status	= new xui_drawer(xui_vector<s32>(200, 20)); 
	xui_method_ptrcall(m_status,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_status,	set_textoffset		)(xui_vector<s32>(2, 0));

	m_toolpane	= new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_toolpane,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_toolpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_toolpane,	set_borderrt		)(xui_rect2d<s32>(4, 2, 24, 2));
	xui_method_ptrcall(m_toolpane,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_toolpane,	add_child			)(m_sizeroll);
	xui_method_ptrcall(m_toolpane,	add_child			)(m_status);

	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_WE);

	m_fill		= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fill,		xm_perform			) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_fillperform);
	xui_method_ptrcall(m_fill,		xm_renderelse		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_fillrenderelse);
	xui_method_ptrcall(m_fill,		ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fill,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_fill,		set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_fill,		set_drawcolor		)(false);
	xui_method_ptrcall(m_fill,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_fill,		add_child			)(m_pathview);
	xui_method_ptrcall(m_fill,		add_child			)(m_pathpane);
	xui_method_ptrcall(m_fill,		add_child			)(m_toolpane);
	xui_method_ptrcall(m_fill,		add_child			)(m_fileview);
	xui_method_ptrcall(m_fill,		add_child			)(m_sizectrl);
	add_pagectrl(m_head);
	add_pagectrl(m_fill);
}

/*
//destructor
*/
xui_delete_explain(onity_project)( void )
{
	xui_timermgr::get_ins()->del_timer(m_timer);
}

/*
//method
*/
xui_method_explain(onity_project, ini_pathtree,				void			)( void )
{
	std::vector<std::wstring> pathvec = xui_global::get_path(L"");
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		std::wstring  full = pathvec[i];
		xui_treenode* node = m_pathview->add_upmostnode(i, new onity_pathdata(full, NULL, NULL));
		onity_pathdata::new_leafnode(node);
	}
}
xui_method_explain(onity_project, get_pathfile,				void			)( const std::wstring& suff, xui_proproot_vec& filevec )
{
	std::vector<xui_treenode*> nodevec = m_pathview->get_entirenode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		onity_pathdata* pathdata = (onity_pathdata*)nodevec[i]->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		const xui_proproot_vec& vec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(*itor);
			if (suff.length() == 0 || onity_filedata::get_suff(propfile->get_fullname()) == suff)
				filevec.push_back((*itor));
		}
	}
}
xui_method_explain(onity_project, loc_filenode,				void			)( const std::wstring& path, const std::wstring& file, u32 id )
{
	m_search->set_text(L"");

	onity_proppath* viewpath = NULL;
	onity_propfile* viewfile = NULL;
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		onity_pathdata* pathdata = (onity_pathdata*)pathvec[i]->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		if (proppath->get_fullname() != path)
			continue;

		const xui_proproot_vec& vec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(*itor);
			if (onity_filedata::get_safe(propfile->get_fullname()) == file)
			{
				viewpath = proppath;
				viewfile = propfile;
				break;
			}
		}

		if (viewfile)
		{
			xui_method_ptrcall(m_pathview, set_selectednode	)(pathnode, true);
			xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
			break;
		}
	}

	xui_treenode* viewnode = NULL;
	std::vector<xui_treenode*> filevec = m_fileview->get_lineview()->get_upmostnodearray();
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		xui_treenode*	filenode = filevec[i];
		onity_filedata* filedata = (onity_filedata*)filenode->get_linkdata();
		onity_propfile* propfile = dynamic_cast<onity_propfile*>(filedata->get_prop());
		if (propfile == viewfile)
		{
			viewnode = filenode;
			const std::vector<xui_treenode*>& vec = filenode->get_leafnodearray();
			if (vec.size() > 0)
			{
				for (std::vector<xui_treenode*>::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
				{
					xui_treenode*		treenode = (*itor);
					onity_treedata*		treedata = (onity_treedata*)treenode->get_linkdata();
					onity_prop2dsasset* propleaf = dynamic_cast<onity_prop2dsasset*>(treedata->get_prop());
					if (propleaf->get_assetid() == id)
					{
						viewnode = treenode;
						break;
					}
				}

				if (m_sizeroll->get_value() == 0)
					m_sizeroll->set_value(10);
			}

			break;
		}
	}

	if (viewnode)
	{
		m_fileview->set_locknode(viewnode);

		xui_treeview*   lineview = m_fileview->get_lineview();
		onity_tileview* tileview = m_fileview->get_tileview();
		xui_method_ptrcall(lineview, set_nodevisible)(viewnode);
		xui_method_ptrcall(tileview, set_tilevisible)(viewnode);
	}
}

/*
//notify
*/
xui_method_explain(onity_project, ntf_load,					void			)( onity_propfile* propfile )
{
	std::vector<std::string> itemvec = NPStringUtil::Split(xui_global::unicode_to_ascii(m_search->get_text()), ';');
	std::wstring			 filekey = (itemvec.size() > 0) ? xui_global::ascii_to_unicode(itemvec[0]) : L"";
	std::string				 leafkey = (itemvec.size() > 1) ? itemvec[1] : "";

	xui_treeview* lineview = m_fileview->get_lineview();
	const std::vector<xui_treenode*>& nodes = lineview->get_upmostnodearray();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode*   node = nodes[i];
		onity_filedata* data = (onity_filedata*)node->get_linkdata();
		if (data->get_prop() == propfile)
		{
			node->del_leafnodeall();

			onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(propfile);
			if (prop2dsres)
			{
				std::vector<xui_proproot*> subprop = prop2dsres->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_prop2dsasset* propasset = dynamic_cast<onity_prop2dsasset*>(subprop[isub]);
					NP2DSAsset* asset = propasset->get_asset();
					std::stringstream keytext;
					keytext << asset->GetKey();
					if (leafkey.length() == 0 || leafkey == keytext.str() || asset->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_2dsassetdata(propasset->get_resicon(), propasset));
						++isubindex;
					}
				}
			}

			onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(propfile);
			if (propjsones)
			{
				std::vector<xui_proproot*> subprop = propjsones->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_propjsonestemp* proptemp = dynamic_cast<onity_propjsonestemp*>(subprop[isub]);
					Omiga::EntityTemplate* temp = proptemp->get_template();
					if (leafkey.length() == 0 || temp->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_jsonestempdata(onity_resource::icon_entity, proptemp));
						++isubindex;
					}
				}
			}
		}
	}

	if (m_copy->get_data() == propfile ||
		m_move->get_data() == propfile)
	{
		m_copy->set_data(NULL);
		m_move->set_data(NULL);
		m_menuprop.clear();
		refresh_linetool();
	}

}
xui_method_explain(onity_project, ntf_rename,				void			)( const std::wstring& last, const std::wstring& curr )
{
	//lineview
	std::vector<xui_treenode*> nodes = m_fileview->get_lineview()->get_entirenode();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode*   node = nodes[i];
		onity_filedata* data = (onity_filedata*)node->get_linkdata();
		std::wstring    full = data->get_full();
		int npos = full.find(last);
		if (npos != -1)
		{
			full.replace(npos, last.length(), curr);
			data->set_full(full);
		}
	}

	//pathpane
	const std::vector<xui_control*>& child = m_pathpane->get_children();
	for (u32 i = 0; i < child.size(); ++i)
	{
		if (xui_equal_kindof(xui_toggle, child[i]))
			continue;

		xui_drawer*		drawer	= xui_dynamic_cast(xui_drawer, child[i]);
		xui_treenode*   node	= (xui_treenode*)drawer->get_data();
		onity_pathdata* data	= (onity_pathdata*)node->get_linkdata();
		if (data->get_full() == curr)
		{
			xui_method_ptrcall(drawer, set_text		)(onity_filedata::get_safe(curr));
			xui_method_ptrcall(drawer, set_renderw	)(xui_convas::get_ins()->calc_size(drawer->get_text(), drawer->get_textfont(), 0, true).w);
		}
	}

	//status
	onity_propfile* propfile = (onity_propfile*)m_status->get_data();
	if (propfile && propfile->get_fullname() == curr)
		m_status->set_text(onity_filedata::get_file(curr));
}
xui_method_explain(onity_project, ntf_modify,				void			)( const std::wstring& path, const std::wstring& file )
{
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		if (proppath->get_fullname() != path)
			continue;

		const xui_proproot_vec& filevec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = filevec.begin(); itor != filevec.end(); ++itor)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(*itor);
			if (onity_filedata::get_safe(propfile->get_fullname()) == file)
			{
				propfile->ntf_modify();
				break;
			}
		}
	}
}

/*
//event
*/
xui_method_explain(onity_project, on_timertick,				void			)( xui_component* sender, xui_method_args&	   args )
{
	const xui_notify_vec& vec = xui_global::get_fwatch();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		std::wstring path = onity_filedata::get_path(vec[i].srcpath);
		std::wstring file = onity_filedata::get_file(vec[i].srcpath);
		path = path.substr(0, path.length()-1);
		file = onity_filedata::get_safe(file);
		ntf_modify(path, file);
	}

	xui_global::del_fwatch();
}
xui_method_explain(onity_project, on_clearclick,			void			)( xui_component* sender, xui_method_args&     args )
{
	m_search->set_text(L"");
}
xui_method_explain(onity_project, on_searchtextchanged,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_method_ptrcall(m_clear,		set_visible	)(m_search->get_text().length() >  0);
	xui_method_ptrcall(m_pathpane,	set_enable	)(m_search->get_text().length() == 0);
	xui_method_ptrcall(m_fill,		refresh		)();
	if (m_fileview->get_tileview()->get_viewfile())
	{
		m_fileview->get_tileview()->set_viewtext(m_search->get_text());
	}
	else
	{
		refresh_fileview();
		if (m_search->get_text().length() == 0)
		{
			std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
			if (nodevec.size() > 0)
			{
				xui_treenode*   pathnode = nodevec.front();
				onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
				onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
				onity_propfile* viewfile = proppath->get_viewfile();
				if (viewfile && viewfile->get_linkdata())
				{
					m_fileview->get_lineview()->set_allowmulti(true);
					m_fileview->get_tileview()->set_viewfile(viewfile->get_linkdata()->get_node());
					m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_fileroll());
				}
				else
				{
					m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_pathroll());
				}
				refresh_tileview();
			}
		}
	}

	refresh_linetool();
}
xui_method_explain(onity_project, on_filterselection,		void			)( xui_component* sender, xui_method_args&	   args )
{
	if (m_search->get_text().length() > 0)
	{
		refresh_fileview();
	}
}
xui_method_explain(onity_project, on_headperform,			void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search,	on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search,	on_perform_x)(m_create->get_renderx()+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,		on_perform_x)(m_create->get_renderx()+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
	xui_method_ptrcall(m_filter,	on_perform_x)(m_search->get_renderx()+m_search->get_renderw()+6);
	xui_method_ptrcall(m_linetool,	on_perform_x)(m_filter->get_renderx()+m_filter->get_renderw()+6);
}
xui_method_explain(onity_project, on_fillperform,			void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt = m_pathview->get_renderrt()+m_pathview->get_renderpt();
	m_sizectrl->on_perform_h(rt.get_h());
	m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
	m_sizectrl->on_perform_y(rt.ay);
}
xui_method_explain(onity_project, on_fillrenderelse,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt;
	rt = m_pathview->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_pathpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_toolpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour::black);
}
xui_method_explain(onity_project, on_sizectrlmousemove,		void			)( xui_component* sender, xui_method_mouse&    args )
{
	if (m_sizectrl->has_catch())
	{
		s32 delta = xui_desktop::get_ins()->get_mousemove().x;
		s32 width = m_pathview->get_renderw() + delta;
		width = xui_max(width, 100);
		width = xui_min(width, m_fill->get_renderw()/2);
		m_pathview->set_renderw(width);		
	}
}
xui_method_explain(onity_project, on_pathviewselection,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_method_ptrcall(m_search, ini_drawer	)(L"");
	xui_method_ptrcall(m_clear,	 set_visible)(false);
	refresh_fileview();
	refresh_pathpane();

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   pathnode = nodevec.front();
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		proppath->set_viewfile(NULL);
		m_sizeroll->set_value(proppath->get_pathroll());

		if (m_histroy.empty() || m_histroy[m_curridx] != pathnode)
		{
			m_histroy.insert(m_histroy.begin()+m_curridx, pathnode);
			if (m_curridx > 0)
			{
				m_histroy.erase(m_histroy.begin(), m_histroy.begin()+m_curridx);
				m_curridx = 0;
			}
			else
			{
				static const u32 max_stack = 64;
				if (m_histroy.size() > max_stack)
					m_histroy.erase(m_histroy.begin()+max_stack);
			}
		}

		refresh_linetool();
	}
}
xui_method_explain(onity_project, on_fileviewnodeclick,		void			)( xui_component* sender, xui_method_mouse&    args )
{
	xui_proproot_vec propvec;
	std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_proppath* prop = dynamic_cast<onity_proppath*>(data->get_prop());
		if (prop == NULL)
			propvec.push_back(data->get_prop());

		if (node->get_rootnode() == NULL)
		{
			onity_filedata* filedata = (onity_filedata*)node->get_linkdata();
			xui_method_ptrcall(m_status, set_icon		)(filedata->get_icon(0));
			xui_method_ptrcall(m_status, set_text		)(filedata->get_file());
			xui_method_ptrcall(m_status, set_data		)(filedata->get_prop());
			xui_method_ptrcall(m_status, set_iconsize	)(xui_vector<s32>(16));
		}
	}

	refresh_linetool();

	if (propvec.size() > 0)
	{
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(propvec);
	}
}
xui_method_explain(onity_project, on_fileviewdoubleclk,		void			)( xui_component* sender, xui_method_mouse&	   args )
{
	if (args.mouse == MB_L)
	{
		std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
		if (nodevec.size() > 0)
		{
			xui_treenode* node = nodevec.front();
			if (node->get_rootnode() == NULL && node->get_data())
			{
				xui_treenode* pathnode = (xui_treenode*)node->get_data();
				xui_method_ptrcall(m_pathview, set_selectednode	)(pathnode, true);
				xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
			}
			else
			{
				onity_treedata* data = (onity_treedata*)node->get_linkdata();
				onity_filedata* file = dynamic_cast<onity_filedata*>(data);
				onity_propfile* prop = dynamic_cast<onity_propfile*>(data->get_prop());
				if (file)
				{
					std::wstring suff = file->get_suff();
					if (suff == L".controller")
					{
						onity_animator*  page = onity_mainform::get_ptr()->get_animator();
						onity_mainform::get_ptr()->set_pageshow(page);
						page->set_editprop((onity_propcontroller*)file->get_prop());
					}
					else
					if (suff == L".npCourse")
					{
						onity_hierarchy* page = onity_mainform::get_ptr()->get_hierarchy();
						onity_mainform::get_ptr()->set_pageshow(page);
						//page->set_editprop((onity_propcourse*    )file->get_prop());
					}
					else
					if (suff == L".npModule" ||
						suff == L".npSprite" ||
						suff == L".npAction" ||
						suff == L".json")
					{
						m_fileview->get_lineview()->non_selectednode();
						m_fileview->get_lineview()->set_allowmulti(true);
						m_fileview->get_tileview()->set_viewfile(node);

						if (m_search->get_text().length() == 0)
						{
							xui_treenode*   pathnode = m_pathview->get_selectednode().front();
							onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
							onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
							proppath->set_viewfile(prop);
							refresh_linetool();
							m_sizeroll->set_value(proppath->get_fileroll());
						}
						else
						{
							if (m_sizeroll->get_value() == 0)
								m_sizeroll->set_value(10);
						}
					}
				}
				else
				{
					onity_propactor* prop = dynamic_cast<onity_propactor*>(data->get_prop());
					if (prop)
					{
						onity_timeline* timeline = onity_mainform::get_ptr()->get_timeline();
						onity_mainform::get_ptr()->set_pageshow(timeline);
						timeline->set_editprop(prop);
					}
				}
			}
		}
	}
}
xui_method_explain(onity_project, on_fileviewassetdrag,		void			)( xui_component* sender, xui_method_dragdrop& args )
{
	std::string dragtype;
	void*       dragdata = NULL;

	std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   node = nodevec.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		onity_propfile* propfile = dynamic_cast<onity_propfile*>(prop);
		onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(prop);
		if (propfile)
		{
			dragtype  = propfile->get_dragtype();
			dragdata  = propfile->get_dragdata();
		}
		if (propleaf)
		{
			dragtype  = propleaf->get_dragtype();
			dragdata  = propleaf->get_dragdata();
		}
	}

	if (dragdata != NULL && dragtype.length() > 0)
	{
		args.drag  = sender;
		args.type  = dragtype;
		args.data  = dragdata;
		args.allow = true;
	}
}
xui_method_explain(onity_project, on_folderclick,			void			)( xui_component* sender, xui_method_args&     args )
{
	m_search->set_text(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	std::wstring path = xui_global::get_workpath();
	if (nodevec.size() > 0)
	{
		xui_treenode*   node = nodevec.front();
		onity_pathdata* data = (onity_pathdata*)node->get_linkdata();
		path = data->get_full();
	}

	std::wstringstream temp;
	temp << path.c_str();
	temp << L"/New Folder";
	if (xui_global::has_path(temp.str()))
	{
		s32 number  = 0;
		while (true)
		{
			temp.str(L"");
			temp << path.c_str();
			temp << L"/New Folder ";
			temp << number;
			if (xui_global::has_path(temp.str()) == false)
				break;

			++number;
		}
	}

	if (xui_global::add_path(temp.str()) == false)
		return;

	onity_pathdata* pathdata = new onity_pathdata(temp.str(), NULL, NULL);
	if (nodevec.size() > 0)
	{
		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* rootnode = nodevec.front();
		xui_treenode* pathnode = rootnode->add_leafnode(0, pathdata);
		xui_treenode* linenode = lineview->add_upmostnode(0, new onity_pathdata(pathdata->get_full(), pathdata->get_prop(), pathnode));
		linenode->set_data(pathnode);
		rootnode->set_expanded(true);

		lineview->refresh();
		linenode->set_edittext(0);
	}
	else
	{
		xui_treenode* pathnode = m_pathview->add_upmostnode(0, pathdata);
		m_pathview->refresh();
		pathnode->set_edittext(0);
	}
}
xui_method_explain(onity_project, on_controllerclick,		void			)( xui_component* sender, xui_method_args&	   args )
{
	m_search->set_text(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   pathnode = nodevec.front();
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		std::wstringstream temp;
		temp << pathdata->get_full().c_str();
		temp << L"/New Animation State.controller";
		if (xui_global::has_file(temp.str()))
		{
			s32 number  = 0;
			while (true)
			{
				temp.str(L"");
				temp << pathdata->get_full().c_str();
				temp << L"/New Animation Controller ";
				temp << number;
				temp << ".controller";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		onity_propcontroller* propfile = new onity_propcontroller(temp.str());
		xui_method_ptrcall(proppath, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new onity_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(onity_project, on_particleclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	m_search->set_text(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   pathnode = nodevec.front();
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		std::wstringstream temp;
		temp << pathdata->get_full().c_str();
		temp << L"/New Particle.particle";
		if (xui_global::has_file(temp.str()))
		{
			s32 number  = 0;
			while (true)
			{
				temp.str(L"");
				temp << pathdata->get_full().c_str();
				temp << L"/New Particle ";
				temp << number;
				temp << ".particle";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		onity_propparticle* propfile = new onity_propparticle(temp.str());
		xui_method_ptrcall(proppath, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new onity_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(onity_project, on_courseclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	m_search->set_text(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   pathnode = nodevec.front();
		onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		std::wstringstream temp;
		temp << pathdata->get_full().c_str();
		temp << L"/New Level.npCourse";
		if (xui_global::has_file(temp.str()))
		{
			s32 number  = 0;
			while (true)
			{
				temp.str(L"");
				temp << pathdata->get_full().c_str();
				temp << L"/New Level";
				temp << number;
				temp << ".npCourse";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		onity_propcourse* propfile = new onity_propcourse(temp.str());
		xui_method_ptrcall(proppath, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new onity_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(onity_project, on_pathitemclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_treenode* pathnode = (xui_treenode*)sender->get_data();
	if (pathnode->was_selected())
	{
		xui_treenode* viewfile = m_fileview->get_tileview()->get_viewfile();
		if (viewfile)
		{
			viewfile->set_expanded(false);

			onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
			onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
			m_fileview->get_tileview()->set_viewfile(NULL);
			m_fileview->get_lineview()->set_allowmulti(false);
			m_fileview->get_lineview()->set_selectednode(viewfile, true);

			m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_pathroll());
			refresh_tileview();
			refresh_linetool();
		}
	}
	else
	{
		xui_method_ptrcall(m_pathview, set_selectednode	)(pathnode, true);
		xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
	}
}
xui_method_explain(onity_project, on_pathtogglerender,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_toggle*     toggle = xui_dynamic_cast(xui_toggle, sender);
	xui_colour      color  = toggle->get_rendercolor() * toggle->get_vertexcolor();
	xui_rect2d<s32> rt     = toggle->get_renderrtins() + toggle->get_screenpt();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
}
xui_method_explain(onity_project, on_pathtoggleclick,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);
	if (toggle->was_push())
	{
		xui_menu* menu = toggle->get_menu();
		if (menu)
		{
			std::vector<xui_menuitem*> vec = menu->get_itemall();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				xui_menuitem*	item = vec[i];
				xui_treenode*   node = (xui_treenode*)vec[i]->get_data();
				onity_pathdata* data = (onity_pathdata*)node->get_linkdata();
				xui_method_ptrcall(item, set_text)(data->get_text(0));
			}

			menu->refresh();
		}
	}
}
xui_method_explain(onity_project, on_sizerollscroll,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

	s32 value = m_sizeroll->get_value();
	if (value < 10 && (value > 0 || m_fileview->get_tileview()->get_viewfile() != NULL))
	{
		value = 10;
		m_sizeroll->ini_scroll(m_sizeroll->get_range(), value);
	}

	refresh_tileview();

	if (m_search->get_text().length() == 0)
	{
		std::vector<xui_treenode*> pathvec = m_pathview->get_selectednode();
		if (pathvec.size() > 0)
		{
			xui_treenode*   pathnode = pathvec.front();
			onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
			onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

			if (m_fileview->get_tileview()->get_viewfile()) proppath->set_fileroll(value);
			else											proppath->set_pathroll(value);
		}
	}
}

xui_method_explain(onity_project, on_showfindclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	if (vec.size() > 0)
	{
		onity_pathdata* pathdata = (onity_pathdata*)vec.front()->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		xui_global::set_showfind(proppath->get_fullname());
	}
}
xui_method_explain(onity_project, on_propertyclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	if (vec.size() > 0)
	{
		onity_pathdata* pathdata = (onity_pathdata*)vec.front()->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(proppath);
	}
}
xui_method_explain(onity_project, on_freetypeclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	npu32 style = 0;
	if		(sender == m_auto)		style = FS_AUTO;
	else if (sender == m_never)		style = FS_NEVER;
	else if (sender == m_immediate) style = FS_IMMEDIATE;
	else
	{}

	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_pathdata* pathdata = (onity_pathdata*)vec[i]->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		std::string     pathname = xui_global::unicode_to_ascii(proppath->get_fullname()) + "/";
		set_freetype(META_MODULE, pathname, style);
		set_freetype(META_SPRITE, pathname, style);
		set_freetype(META_ACTION, pathname, style);
	}
}
xui_method_explain(onity_project, on_loadtypeclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	bool flag = (sender == m_on);
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_pathdata* pathdata = (onity_pathdata*)vec[i]->get_linkdata();
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
		std::string     pathname = xui_global::unicode_to_ascii(proppath->get_fullname()) + "/";
		set_loadtype(META_MODULE, pathname, flag);
		set_loadtype(META_SPRITE, pathname, flag);
		set_loadtype(META_ACTION, pathname, flag);
	}
}
xui_method_explain(onity_project, on_linetoolclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_treenode*   pathnode = m_histroy[m_curridx];
	onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
	onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();

	xui_proproot_vec propvec;
	std::vector<xui_treenode*> selectednode = m_fileview->get_lineview()->get_selectednode();
	for (u32 i = 0; i < selectednode.size(); ++i)
	{
		onity_treedata* data = dynamic_cast<onity_treedata*>(selectednode[i]->get_linkdata());
		onity_propleaf* prop = dynamic_cast<onity_propleaf*>(data->get_prop());
		if (prop)
			propvec.push_back(prop);
	}

	if (sender == m_add)
	{
		add_propleaf();
	}
	else
	if (sender == m_del)
	{
		del_propleaf(propvec);
	}
	else
	if (sender == m_copy)
	{
		onity_treedata*  filedata = dynamic_cast<onity_treedata*>(viewfile->get_linkdata());
		m_copy->set_data(filedata->get_prop());
		m_move->set_data(NULL);
		m_menuprop = propvec;
	}
	else
	if (sender == m_move)
	{
		onity_treedata*  filedata = dynamic_cast<onity_treedata*>(viewfile->get_linkdata());
		m_move->set_data(filedata->get_prop());
		m_copy->set_data(NULL);
		m_menuprop = propvec;
	}
	else
	if (sender == m_paste)
	{
		pst_propleaf();
	}
	else
	if (sender == m_backpath && viewfile)
	{
		viewfile->set_expanded(false);
		m_fileview->get_tileview()->set_viewfile(NULL);
		m_fileview->get_lineview()->set_allowmulti(false);
		m_fileview->get_lineview()->set_selectednode(viewfile, true);
		m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_pathroll());
		refresh_tileview();
	}
	else
	if (sender == m_forepath && viewfile == NULL && proppath->get_viewfile())
	{
		if (proppath->get_viewfile()->get_linkdata() == NULL)
			return;

		m_fileview->get_lineview()->non_selectednode();
		m_fileview->get_lineview()->set_allowmulti(true);
		m_fileview->get_tileview()->set_viewfile(proppath->get_viewfile()->get_linkdata()->get_node());
		m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_fileroll());
		refresh_tileview();
	}
	else
	{
		if (sender == m_backpath) ++m_curridx;
		if (sender == m_forepath) --m_curridx;

		if (m_curridx <= (s32)m_histroy.size()-1)
		{
			pathnode = m_histroy[m_curridx];
			pathdata = (onity_pathdata*)pathnode->get_linkdata();
			proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());

			xui_method_ptrcall(m_pathview, ini_selectednode	)(pathnode, true);
			xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
			refresh_fileview();
			refresh_pathpane();

			onity_propfile* viewfile = proppath->get_viewfile();
			if (viewfile && viewfile->get_linkdata())
			{
				m_fileview->get_lineview()->set_allowmulti(true);
				m_fileview->get_tileview()->set_viewfile(viewfile->get_linkdata()->get_node());
				m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_fileroll());
			}
			else
			{
				m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_pathroll());
			}
			refresh_tileview();
		}
	}

	refresh_linetool();
}

/*
//method
*/
xui_method_explain(onity_project, refresh_fileview,			void			)( void )
{
	xui_method_ptrcall(m_status,	set_data)(NULL);
	xui_method_ptrcall(m_fileview,	clear	)();

	xui_treeview*   lineview = m_fileview->get_lineview();
	onity_tileview*	tileview = m_fileview->get_tileview();

	u32 index = 0;
	std::vector<xui_proproot*> filevec;
	if (m_search->get_text().length() == 0)
	{
		std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
		if (nodevec.size() > 0)
		{
			xui_treenode*   rootnode = nodevec.front();
			onity_pathdata* rootdata = (onity_pathdata*)rootnode->get_linkdata();
			onity_proppath* rootprop = dynamic_cast<onity_proppath*>(rootdata->get_prop());

			const std::vector<xui_treenode*>& pathvec = rootnode->get_leafnodearray();
			filevec = rootprop->get_fileprop();
			for (u32 i = 0; i < pathvec.size(); ++i,++index)
			{
				onity_pathdata* data = (onity_pathdata*)pathvec[i]->get_linkdata();
				xui_treenode*   node = lineview->add_upmostnode(index, new onity_pathdata(data->get_full(), data->get_prop(), pathvec[i]));
				node->set_data(pathvec[i]);
			}
		}
	}
	else
	{
		get_pathfile(convert_filesuff(), filevec);
	}

	std::vector<std::string> itemvec = NPStringUtil::Split(xui_global::unicode_to_ascii(m_search->get_text()), ';');
	std::wstring			 filekey = (itemvec.size() > 0) ? xui_global::ascii_to_unicode(itemvec[0]) : L"";
	std::string				 leafkey = (itemvec.size() > 1) ? itemvec[1] : "";

	for (u32 i = 0; i < filevec.size(); ++i)
	{
		onity_propfile* prop = dynamic_cast<onity_propfile*>(filevec[i]);
		if (filekey.length() == 0 || onity_filedata::get_file(prop->get_fullname()).find(filekey) != -1)
		{
			xui_treenode* node = lineview->add_upmostnode(index, new onity_filedata(prop->get_fileicon(), prop->get_fullname(), prop));
			onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(prop);
			if (prop2dsres)
			{
				if (leafkey.length() > 0)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = prop2dsres->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_prop2dsasset* propasset = dynamic_cast<onity_prop2dsasset*>(subprop[isub]);
					NP2DSAsset* asset = propasset->get_asset();
					std::stringstream keytext;
					keytext << asset->GetKey();
					if (leafkey.length() == 0 || leafkey == keytext.str() || asset->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_2dsassetdata(propasset->get_resicon(), propasset));
						++isubindex;
					}
				}
			}

			onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(prop);
			if (propjsones)
			{
				if (leafkey.length() > 0)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = propjsones->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					onity_propjsonestemp* proptemp = dynamic_cast<onity_propjsonestemp*>(subprop[isub]);
					Omiga::EntityTemplate* temp = proptemp->get_template();
					if (leafkey.length() == 0 || temp->GetName().find(leafkey) != -1)
					{
						node->add_leafnode(isubindex, new onity_jsonestempdata(onity_resource::icon_entity, proptemp));
						++isubindex;
					}
				}
			}

			++index;
		}
	}

	lineview->set_allowmulti(false);
	lineview->refresh();
	tileview->get_drawview()->refresh();
}
xui_method_explain(onity_project, refresh_pathpane,			void			)( void )
{
	xui_method_ptrcall(m_pathpane, del_children	)();
	xui_method_ptrcall(m_pathpane, set_enable	)(true);

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size())
	{
		xui_treenode* node = nodevec.front();
		xui_treenode* root = node;
		std::vector<xui_treenode*> pathvec;
		while (root)
		{
			pathvec.push_back(root);
			root = root->get_rootnode();
		}

		xui_vecptr_delloop(pathvec)
		{
			xui_treenode*    pathnode = pathvec[i];
			onity_pathdata*  pathdata = (onity_pathdata*)pathnode->get_linkdata();
			xui_button*        button = new xui_button(xui_vector<s32>(0, 20));
			xui_method_ptrcall(button, ini_component	)(0, 0, DOCKSTYLE_L);
			xui_method_ptrcall(button, ini_drawer		)(pathdata->get_text(0));
			xui_method_ptrcall(button, set_backcolor	)(xui_colour::transparent);
			xui_method_ptrcall(button, set_drawcolor	)(true);
			xui_method_ptrcall(button, set_textcolor	)(xui_colour(1.0f, 0.8f));
			xui_method_ptrcall(button, set_data			)(pathnode);
			xui_method_ptrcall(button, on_perform_w		)(xui_convas::get_ins()->calc_size(button->get_text(), button->get_textfont(), 0, true).w);
			xui_method_ptrcall(button, xm_buttonclick	) += new xui_method_member<xui_method_args, onity_project>(this, &onity_project::on_pathitemclick);
			m_pathpane->add_child(button);

			if (pathnode->get_leafnodecount() == 0)
				continue;

			xui_menu* menu = xui_menu::create(80);
			const std::vector<xui_treenode*>& leafvec = pathnode->get_leafnodearray();
			for (u32 ileaf = 0; ileaf < leafvec.size(); ++ileaf)
			{
				xui_treenode*    leafnode = leafvec[ileaf];
				onity_pathdata*  leafdata = (onity_pathdata*)leafnode->get_linkdata();
				xui_menuitem*    menuitem = menu->add_item(NULL, leafdata->get_text(0));
				menuitem->xm_click	   += new xui_method_member<xui_method_args, onity_project>(this, &onity_project::on_pathitemclick);
				xui_method_ptrcall(menuitem, set_data)(leafnode);
				xui_method_ptrcall(menuitem, set_flag)(leafnode->was_selected());
			}

			xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16, 20), TOGGLE_BUTTON);
			toggle->xm_renderself  += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_pathtogglerender);
			toggle->xm_toggleclick += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_pathtoggleclick);
			xui_method_ptrcall(toggle, ini_component)(0, 0, DOCKSTYLE_L);
			xui_method_ptrcall(toggle, set_data		)(pathnode);
			xui_method_ptrcall(toggle, set_menu		)(menu);
			m_pathpane->add_child(toggle);
		}
	}
}
xui_method_explain(onity_project, refresh_linetool,			void			)( void )
{
	bool backenable = false;
	bool foreenable = false;
	if (m_search->get_text().length() == 0)
	{
		if (m_curridx < (s32)m_histroy.size()-1 || m_fileview->get_tileview()->get_viewfile() != NULL)
			backenable = true;
		if (m_curridx > 0)
			foreenable = true;
		std::vector<xui_treenode*> pathvec = m_pathview->get_selectednode();
		if (pathvec.size() > 0)
		{
			xui_treenode*   pathnode = pathvec.front();
			onity_pathdata* pathdata = (onity_pathdata*)pathnode->get_linkdata();
			onity_proppath* proppath = dynamic_cast<onity_proppath*>(pathdata->get_prop());
			if (m_fileview->get_tileview()->get_viewfile() == NULL && proppath->get_viewfile() != NULL)
				foreenable = true;
		}
	}

	std::vector<xui_treenode*> selectednode = m_fileview->get_lineview()->get_selectednode();
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();
	onity_filedata* filedata = viewfile == NULL ? NULL : dynamic_cast<onity_filedata*>(viewfile->get_linkdata());
	xui_method_ptrcall(m_backpath,	set_enable)(backenable);
	xui_method_ptrcall(m_forepath,	set_enable)(foreenable);
	xui_method_ptrcall(m_add,		set_enable)(viewfile != NULL);
	xui_method_ptrcall(m_del,		set_enable)(viewfile != NULL && selectednode.size() > 0);
	xui_method_ptrcall(m_copy,		set_enable)(viewfile != NULL && selectednode.size() > 0);
	xui_method_ptrcall(m_move,		set_enable)(viewfile != NULL && selectednode.size() > 0 && filedata->get_suff() != L".json");
	xui_method_ptrcall(m_paste,		set_enable)(viewfile != NULL && m_search->get_text().length() == 0 && m_menuprop.size() > 0);
}
xui_method_explain(onity_project, convert_filesuff,			std::wstring	)( void )
{
	u32 index = m_filter->get_selectedindex();
	switch (index)
	{
	case FILTER_TEXTURE:	return L".png";
	case FILTER_MODULE:		return L".npModule";
	case FILTER_SPRITE:		return L".npSprite";
	case FILTER_ACTION:		return L".npAction";
	case FILTER_COURSE:		return L".npCourse";
	case FILTER_PARTICLE:	return L".particle";
	case FILTER_CONTROLLER:	return L".controller";
	case FILTER_JSON:		return L".json";
	}

	return L"";
}
xui_method_explain(onity_project, refresh_tileview,			void			)( void )
{
	s32 value = m_sizeroll->get_value();
	xui_treeview*      lineview = m_fileview->get_lineview();
	onity_tileview*	   tileview = m_fileview->get_tileview();
	bool               lineshow = m_fileview->get_lineview()->was_visible();
	bool               tileshow = m_fileview->get_tileview()->was_visible();
	xui_method_ptrcall(lineview, set_visible	)(value == 0);
	xui_method_ptrcall(tileview, set_visible	)(value >  0);
	xui_method_ptrcall(tileview, set_tilesize	)(xui_max(10, value) + 40);
	m_fileview->refresh();

	std::vector<xui_treenode*> leafvec = lineview->get_selectednode();
	if (leafvec.size() > 0)
	{
		if (lineview->was_visible() && lineshow == false)
			lineview->set_nodevisible(leafvec.front());
		if (tileview->was_visible() && tileshow == false)
			tileview->set_tilevisible(leafvec.front());
	}
}
xui_method_explain(onity_project, set_freetype,				void			)( u08 type, const std::string& pathname, u32 style )
{
	NP2DSAssetFileMgr* file_mgr = NULL;
	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	for (npu32 i = 0; i < file_mgr->GetFileCount(); ++i)
	{
		std::string temppath = file_mgr->GetFilePH(i);
		std::string tempfile = file_mgr->GetFileFN(i);
		int index = temppath.find(pathname);
		if (index != -1)
		{
			NP2DSAssetFile*   file = NULL;
			switch (type)
			{
			case META_MODULE: file = NP2DSImageFileMgr::GetIns()->GetFile(i); break;
			case META_SPRITE: file = NP2DSFrameFileMgr::GetIns()->GetFile(i); break;
			case META_ACTION: file = NP2DSActorFileMgr::GetIns()->GetFile(i); break;
			}

			for (npu16 index = 0; index < file->GetAssetCount(); ++index)
			{
				npu32 id = file->GetAssetID(index);
				NP2DSAsset* asset = file->GetAsset(id);
				if (asset)
					asset->SetFree(style);
			}

			file->SaveXml(temppath+tempfile);
		}
	}
}
xui_method_explain(onity_project, set_loadtype,				void			)( u08 type, const std::string& pathname, bool flag )
{
	NP2DSAssetFileMgr* file_mgr = NULL;
	switch (type)
	{
	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
	}

	for (npu32 i = 0; i < file_mgr->GetFileCount(); ++i)
	{
		std::string temppath = file_mgr->GetFilePH(i);
		std::string tempfile = file_mgr->GetFileFN(i);
		int index = temppath.find(pathname);
		if (index != -1)
		{
			NP2DSAssetFile*   file = NULL;
			switch (type)
			{
			case META_MODULE: file = NP2DSImageFileMgr::GetIns()->GetFile(i); break;
			case META_SPRITE: file = NP2DSFrameFileMgr::GetIns()->GetFile(i); break;
			case META_ACTION: file = NP2DSActorFileMgr::GetIns()->GetFile(i); break;
			}

			file->SetEntireLoad(flag);
			file->SaveXml(temppath+tempfile);
		}
	}
}
xui_method_explain(onity_project, add_propleaf,				void			)( void )
{
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();
	onity_treedata* filedata = dynamic_cast<onity_treedata*>(viewfile->get_linkdata());
	onity_propfile* propfile = dynamic_cast<onity_propfile*>(filedata->get_prop());
	xui_proproot*   propleaf = NULL;
	xui_treedata*   linkdata = NULL;

	onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(propfile);
	if (prop2dsres)
	{
		propleaf = prop2dsres->add_subprop();
		onity_prop2dsasset* propasset = dynamic_cast<onity_prop2dsasset*>(propleaf);
		linkdata = new onity_2dsassetdata(propasset->get_resicon(), propasset);
	}

	onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(propfile);
	if (propjsones)
	{
		propleaf = propjsones->add_subprop();
		linkdata = new onity_jsonestempdata(onity_resource::icon_entity, propleaf);
	}

	if (linkdata)
	{
		u32 index = viewfile->get_leafnodecount();
		xui_treenode*   node     = viewfile->add_leafnode(index, linkdata);
		xui_treeview*   lineview = m_fileview->get_lineview();
		onity_tileview* tileview = m_fileview->get_tileview();
		xui_method_ptrcall(lineview, non_selectednode	)();
		xui_method_ptrcall(lineview, set_selectednode	)(node, true);
		xui_method_ptrcall(lineview, set_nodevisible	)(node);
		xui_method_ptrcall(tileview, set_tilevisible	)(node);

		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(propleaf);
	}
}
xui_method_explain(onity_project, del_propleaf,				void			)( const xui_proproot_vec& propvec )
{
	for (u32 i = 0; i < propvec.size(); ++i)
	{
		onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(propvec[i]);
		onity_propfile* propfile = propleaf->get_propfile();
		xui_treedata*   linkdata = propleaf->get_linkdata();
		xui_treedata*	filedata = propfile->get_linkdata();
		if (filedata && linkdata)
		{
			filedata->get_node()->del_leafnode(linkdata->get_node());
		}

		onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(propfile);
		if (prop2dsres)
			prop2dsres->del_subprop(propleaf);

		onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(propfile);
		if (propjsones)
			propjsones->del_subprop(propleaf);
	}
}
xui_method_explain(onity_project, pst_propleaf,				void			)( void )
{
	xui_treenode*   dstnode = m_fileview->get_tileview()->get_viewfile();
	onity_treedata* dstdata = dynamic_cast<onity_treedata*>(dstnode->get_linkdata());
	onity_propfile* dstfile = dynamic_cast<onity_propfile*>(dstdata->get_prop());
	onity_propleaf* srcprop = dynamic_cast<onity_propleaf*>(m_menuprop.front());
	onity_propfile* srcfile = srcprop->get_propfile();
	std::wstring    dstfull = dstfile->get_fullname();
	std::wstring    srcfull = srcfile->get_fullname();
	if (onity_filedata::get_suff(dstfull) == onity_filedata::get_suff(srcfull))
	{
		std::vector<onity_treedata*> vec;
		onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(dstfile);
		if (prop2dsres)
		{
			for (u32 i = 0; i < m_menuprop.size(); ++i)
			{
				onity_prop2dsasset* srcleaf = dynamic_cast<onity_prop2dsasset*>(m_menuprop[i]);
				onity_prop2dsasset* dstleaf = dynamic_cast<onity_prop2dsasset*>(prop2dsres->add_subprop(srcleaf));
				u32 index = dstnode->get_leafnodecount();
				onity_treedata* linkdata = new onity_2dsassetdata(dstleaf->get_resicon(), dstleaf);
				dstnode->add_leafnode(index, linkdata);
				vec.push_back(linkdata);
			}
		}

		onity_propjsones* propjsones = dynamic_cast<onity_propjsones*>(dstfile);
		if (propjsones)
		{
			for (u32 i = 0; i < m_menuprop.size(); ++i)
			{
				onity_propjsonestemp* srcleaf = dynamic_cast<onity_propjsonestemp*>(m_menuprop[i]);
				onity_propjsonestemp* dstleaf = dynamic_cast<onity_propjsonestemp*>(propjsones->add_subprop(srcleaf));
				u32 index = dstnode->get_leafnodecount();
				onity_treedata* linkdata = new onity_jsonestempdata(onity_resource::icon_entity, dstleaf);
				dstnode->add_leafnode(index, linkdata);
				vec.push_back(linkdata);
			}
		}

		if (m_move->get_data())
		{
			m_move->set_data(NULL);
			del_propleaf(m_menuprop);
			m_menuprop.clear();
		}

		if (vec.size() > 0)
		{
			std::vector<xui_treenode*> nodes;
			for (u32 i = 0; i < vec.size(); ++i)
				nodes.push_back(vec[i]->get_node());

			xui_treeview*   lineview = m_fileview->get_lineview();
			onity_tileview* tileview = m_fileview->get_tileview();
			xui_method_ptrcall(lineview, non_selectednode	)();
			xui_method_ptrcall(lineview, set_selectednode	)(nodes);
			xui_method_ptrcall(lineview, set_nodevisible	)(nodes.front());
			xui_method_ptrcall(tileview, set_tilevisible	)(nodes.front());

			std::vector<xui_proproot*> props;
			for (u32 i = 0; i < vec.size(); ++i)
				props.push_back(vec[i]->get_prop());
			
			onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
			inspector->set_proproot(props);
		}
	}
}