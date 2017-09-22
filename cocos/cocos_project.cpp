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
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_pathdata.h"
#include "cocos_likedata.h"
#include "cocos_filedata.h"
#include "cocos_fileview.h"
#include "cocos_tileview.h"
#include "cocos_propfile.h"
#include "cocos_propleaf.h"
#include "cocos_proppath.h"
#include "cocos_proplike.h"
#include "cocos_propatlas.h"
#include "cocos_propframe.h"
#include "cocos_propparticle.h"
#include "cocos_proptexture.h"
#include "cocos_propmaterial.h"
#include "cocos_propvertshader.h"
#include "cocos_propfragshader.h"
#include "cocos_propttf.h"
#include "cocos_propfnt.h"
#include "cocos_propspineatlas.h"
#include "cocos_propspine.h"
#include "cocos_propcsd.h"
#include "cocos_framedata.h"
#include "cocos_resource.h"
#include "cocos_renderview.h"
#include "cocos_mainform.h"
#include "cocos_project.h"

xui_implement_rtti(cocos_project, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_project)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
, m_root(NULL)
, m_like(NULL)
, m_curridx(0)
{
	ini_namectrl(cocos_resource::icon_project, L"Project");

	xui_menu* menu1	= xui_menu::create(80);
	m_folder		= menu1->add_item(cocos_resource::icon_folder,	 L"Folder");
	menu1->add_separate();
	m_particle		= menu1->add_item(cocos_resource::icon_particle, L"Particle");
	m_material		= menu1->add_item(cocos_resource::icon_animator, L"Material");
	m_scene			= menu1->add_item(cocos_resource::icon_scene,	 L"Scene");
	xui_method_ptrcall(m_folder,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_folderclick);
	xui_method_ptrcall(m_particle,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_particleclick);
	xui_method_ptrcall(m_material,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_materialclick);
	xui_method_ptrcall(m_scene,		xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_sceneclick);
	m_timer			= xui_timermgr::get_ins()->add_timer(this, 1.0f, NULL);
	xui_method_ptrcall(m_timer,		xm_tick				) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_timertick);
	m_timer_save	= xui_timermgr::get_ins()->add_timer(this, 30.f, NULL);
	xui_method_ptrcall(m_timer_save,xm_tick				) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_timersavetick);

	m_create		= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu1);

	m_search		= new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_searchtextchanged);
	xui_method_ptrcall(m_search,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer			)(cocos_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);
	xui_method_ptrcall(m_search,	set_hintdraw		)(xui_family_render(xui_colour::gray));

	m_clear			= new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(cocos_resource::icon_clear);

	m_backpath		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_backpath,	ini_drawer			)(cocos_resource::icon_left);
	xui_method_ptrcall(m_backpath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_backpath,	set_enable			)(false);
	xui_method_ptrcall(m_backpath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_backpath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_forepath		= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_forepath,	ini_drawer			)(cocos_resource::icon_right);
	xui_method_ptrcall(m_forepath,	set_drawcolor		)(true);
	xui_method_ptrcall(m_forepath,	set_enable			)(false);
	xui_method_ptrcall(m_forepath,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_forepath,	xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_add			= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_add,		ini_drawer			)(cocos_resource::icon_add);
	xui_method_ptrcall(m_add,		set_drawcolor		)(true);
	xui_method_ptrcall(m_add,		set_enable			)(false);
	xui_method_ptrcall(m_add,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_add,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_del			= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_del,		ini_drawer			)(cocos_resource::icon_remove);
	xui_method_ptrcall(m_del,		set_drawcolor		)(true);
	xui_method_ptrcall(m_del,		set_enable			)(false);
	xui_method_ptrcall(m_del,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_del,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_copy			= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_copy,		ini_drawer			)(cocos_resource::icon_copy);
	xui_method_ptrcall(m_copy,		set_drawcolor		)(true);
	xui_method_ptrcall(m_copy,		set_enable			)(false);
	xui_method_ptrcall(m_copy,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_copy,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_move			= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_move,		ini_drawer			)(cocos_resource::icon_cut);
	xui_method_ptrcall(m_move,		set_drawcolor		)(true);
	xui_method_ptrcall(m_move,		set_enable			)(false);
	xui_method_ptrcall(m_move,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_move,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_paste			= new xui_button(xui_vector<s32>(20));
	xui_method_ptrcall(m_paste,		ini_drawer			)(cocos_resource::icon_paste);
	xui_method_ptrcall(m_paste,		set_drawcolor		)(true);
	xui_method_ptrcall(m_paste,		set_enable			)(false);
	xui_method_ptrcall(m_paste,		set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_paste,		xm_buttonclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

	m_favorite		= new xui_toggle(xui_vector<s32>(20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_favorite,	ini_drawer			)(cocos_resource::icon_favorite);
	xui_method_ptrcall(m_favorite,	ini_toggle			)(false);
	xui_method_ptrcall(m_favorite,	set_drawcolor		)(true);
	xui_method_ptrcall(m_favorite,	set_enable			)(false);
	xui_method_ptrcall(m_favorite,	set_corner			)(3);
	xui_method_ptrcall(m_favorite,	set_iconalign		)(IMAGE_C);
	xui_method_ptrcall(m_favorite,	xm_toggleclick		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_linetoolclick);

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
	xui_method_ptrcall(m_linetool,	add_separate		)();
	xui_method_ptrcall(m_linetool,	add_item			)(m_favorite);

	m_filter	= new xui_dropbox(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_filter,	xm_selection		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_filterselection);
	xui_method_ptrcall(m_filter,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_filter,	set_drawcolor		)(true);
	xui_method_ptrcall(m_filter,	set_backcolor		)(xui_button::default_backcolor);
	xui_method_ptrcall(m_filter,	set_corner			)(3);
	xui_method_ptrcall(m_filter,	set_borderrt		)(xui_rect2d<s32>(4, 4, 0, 4));
	xui_method_ptrcall(m_filter,	set_readonly		)(true);
	xui_method_ptrcall(m_filter,	set_maxdrop			)(10);
	xui_method_ptrcall(m_filter,	add_item			)(L"All");
	xui_method_ptrcall(m_filter,	add_item			)(L"Atlas");
	xui_method_ptrcall(m_filter,	add_item			)(L"Texture");
	xui_method_ptrcall(m_filter,	add_item			)(L"Particle");
	xui_method_ptrcall(m_filter,	add_item			)(L"Material");
	xui_method_ptrcall(m_filter,	add_item			)(L"Vert Shader");
	xui_method_ptrcall(m_filter,	add_item			)(L"Frag Shader");
	xui_method_ptrcall(m_filter,	add_item			)(L"TTF");
	xui_method_ptrcall(m_filter,	add_item			)(L"FNT");
	xui_method_ptrcall(m_filter,	add_item			)(L"Spine Atlas");
	xui_method_ptrcall(m_filter,	add_item			)(L"Spine");
	xui_method_ptrcall(m_filter,	add_item			)(L"CSD");
	xui_method_ptrcall(m_filter,	ini_dropbox			)(0);

	m_head		= new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_headperform);
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
	m_pathload		= menu2->add_item(NULL, L"Refresh");
	m_showfind		= menu2->add_item(NULL, L"Show in Finder");
	menu2->add_separate();
	m_property		= menu2->add_item(NULL, L"Property");
	xui_method_ptrcall(m_pathload,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_pathloadclick);
	xui_method_ptrcall(m_showfind,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_showfindclick);
	xui_method_ptrcall(m_property,	xm_click			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_propertyclick);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_pathview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_pathview,	xm_selectedchange	) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_pathviewselection);
	xui_method_ptrcall(m_pathview,	ini_component		)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_pathview,	set_acceptdrag		)(false);
	xui_method_ptrcall(m_pathview,	set_allowmulti		)(false);
	xui_method_ptrcall(m_pathview,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_pathview,	set_contextmenu		)(menu2);

	m_fileview	= new cocos_fileview;
	xui_method_ptrcall(m_fileview,	xm_fileviewnodeclick) += new xui_method_member<xui_method_mouse,	cocos_project>(this, &cocos_project::on_fileviewnodeclick);
	xui_method_ptrcall(m_fileview,	xm_fileviewdoubleclk) += new xui_method_member<xui_method_mouse,	cocos_project>(this, &cocos_project::on_fileviewdoubleclk);
	xui_method_ptrcall(m_fileview,	xm_fileviewassetdrag) += new xui_method_member<xui_method_dragdrop, cocos_project>(this, &cocos_project::on_fileviewassetdrag);
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
	xui_method_ptrcall(m_sizeroll,	xm_scroll			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_sizerollscroll);

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
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	cocos_project>(this, &cocos_project::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_WE);

	m_fill		= new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fill,		xm_perform			) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_fillperform);
	xui_method_ptrcall(m_fill,		xm_renderelse		) += new xui_method_member<xui_method_args,		cocos_project>(this, &cocos_project::on_fillrenderelse);
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
xui_delete_explain(cocos_project)( void )
{
	delete m_root;
	delete m_like;
	xui_timermgr::get_ins()->del_timer(m_timer);
	xui_timermgr::get_ins()->del_timer(m_timer_save);
}

/*
//method
*/
xui_method_explain(cocos_project, ini_pathtree,				void			)( void )
{
	m_root = new cocos_proppath(cocos_resource::icon_folder, L"");
	xui_treenode* node = m_pathview->add_upmostnode(0, new cocos_pathdata(cocos_resource::icon_folder, L"", m_root));
	m_root->ini_treeprop(node);
	m_like = new cocos_proplike;
	m_pathview->add_upmostnode(0, new cocos_likedata(m_like));
}
xui_method_explain(cocos_project, get_pathfile,				void			)( s32 type, xui_proproot_vec& filevec )
{
	std::vector<xui_treenode*> nodevec = m_pathview->get_entirenode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		cocos_pathdata* pathdata = (cocos_pathdata*)nodevec[i]->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
		if (proppath == m_like)
			continue;

		const xui_proproot_vec& vec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (type == FILTER_ALL)
				filevec.push_back(propfile);
			if (type == FILTER_ATLAS		&& dynamic_cast<cocos_propatlas*		>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_TEXTURE		&& dynamic_cast<cocos_proptexture*		>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_PARTICLE		&& dynamic_cast<cocos_propparticle*		>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_MATERIAL		&& dynamic_cast<cocos_propmaterial*		>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_VERTSHADER	&& dynamic_cast<cocos_propvertshader*	>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_FRAGSHADER	&& dynamic_cast<cocos_propfragshader*	>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_TTF			&& dynamic_cast<cocos_propttf*			>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_FNT			&& dynamic_cast<cocos_propfnt*			>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_SPINEATLAS	&& dynamic_cast<cocos_propspineatlas*	>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_SPINE		&& dynamic_cast<cocos_propspine*		>(propfile))
				filevec.push_back(propfile);
			if (type == FILTER_CSD			&& dynamic_cast<cocos_propcsd*			>(propfile))
				filevec.push_back(propfile);
		}
	}
}
xui_method_explain(cocos_project, get_pathfile,				cocos_propfile*	)( const std::wstring& path, const std::wstring& file )
{
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		cocos_pathdata* pathdata = (cocos_pathdata*)pathvec[i]->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());

		if (proppath->get_fullname() != path)
			continue;

		const xui_proproot_vec& vec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (cocos_filedata::get_file(propfile->get_fullname()) == file)
				return propfile;
		}
	}

	return NULL;
}
xui_method_explain(cocos_project, loc_filenode,				void			)( const std::wstring& path, const std::wstring& file, u32 id )
{
	std::wstringstream text;
	text << id;
	loc_filenode(path, file, text.str());
}
xui_method_explain(cocos_project, loc_filenode,				void			)( const std::wstring& path, const std::wstring& file, const std::wstring& name )
{
	xui_method_ptrcall(m_filter, ini_dropbox)(0);
	xui_method_ptrcall(m_search, set_text	)(L"");

	if (m_fileview->get_tileview()->get_viewfile() != NULL)
	{
		xui_method_args args;
		on_linetoolclick(m_backpath, args);
	}

	cocos_proppath* viewpath = NULL;
	cocos_propfile* viewfile = NULL;
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		cocos_pathdata* pathdata = (cocos_pathdata*)pathvec[i]->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());

		if (proppath == m_like ||
			proppath->get_fullname() != path)
			continue;

		const xui_proproot_vec& vec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (cocos_filedata::get_file(propfile->get_fullname()) == file)
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
		cocos_filedata* filedata = (cocos_filedata*)filenode->get_linkdata();
		cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(filedata->get_prop());
		if (propfile == viewfile)
		{
			viewnode  = filenode;
			const std::vector<xui_treenode*>& vec = filenode->get_leafnodearray();
			if (vec.size() > 0)
			{
				for (std::vector<xui_treenode*>::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
				{
					xui_treenode*		treenode = (*itor);
					cocos_treedata*		treedata = (cocos_treedata*)treenode->get_linkdata();
					if (treedata->get_text(0) == name)
					{
						viewnode = treenode;
						break;
					}
					else
					{
						//cocos_prop2dsasset* propleaf = dynamic_cast<cocos_prop2dsasset*>(treedata->get_prop());
						//if (propleaf)
						//{
						//	std::wstringstream text;
						//	text << propleaf->get_assetid();
						//	if (text.str() == name)
						//	{
						//		viewnode = treenode;
						//		break;
						//	}
						//}
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
		cocos_tileview* tileview = m_fileview->get_tileview();
		xui_method_ptrcall(lineview, set_nodevisible)(viewnode);
		xui_method_ptrcall(tileview, set_tilevisible)(viewnode);
	}
}
xui_method_explain(cocos_project, get_pathview,				xui_treeview*	)( void )
{
	return m_pathview;
}
xui_method_explain(cocos_project, get_fileview,				cocos_fileview*	)( void )
{
	return m_fileview;
}

/*
//notify
*/
xui_method_explain(cocos_project, ntf_load,					void			)( cocos_propfile* propfile )
{
	std::wstring  keytext  = xui_global::get_upper(m_search->get_text());
	xui_treeview* lineview = m_fileview->get_lineview();
	const std::vector<xui_treenode*>& nodes = lineview->get_upmostnodearray();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode*   node = nodes[i];
		cocos_filedata* data = (cocos_filedata*)node->get_linkdata();
		if (data->get_prop() == propfile)
		{
			node->del_leafnodeall();

			cocos_propatlas* propatlas = dynamic_cast<cocos_propatlas*>(propfile);
			if (propatlas)
			{
				std::vector<xui_proproot*> subprop = propatlas->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					cocos_propframe* propframe = dynamic_cast<cocos_propframe*>(subprop[isub]);
					std::wstring framename = xui_global::ascii_to_unicode(propframe->get_framename());
					if (keytext.length() == 0 || xui_global::get_upper(framename).find(keytext) != -1)
					{
						node->add_leafnode(isubindex, new cocos_framedata(propframe->get_frameicon(), propframe));
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
xui_method_explain(cocos_project, ntf_rename,				void			)( const std::wstring& last, const std::wstring& curr )
{
	//lineview
	std::vector<xui_treenode*> nodes = m_fileview->get_lineview()->get_entirenode();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_treenode*   node = nodes[i];
		cocos_filedata* data = (cocos_filedata*)node->get_linkdata();
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
		cocos_pathdata* data	= (cocos_pathdata*)node->get_linkdata();
		if (data->get_full() == curr)
		{
			xui_method_ptrcall(drawer, set_text		)(cocos_filedata::get_safe(curr));
			xui_method_ptrcall(drawer, set_renderw	)(xui_convas::get_ins()->calc_size(drawer->get_text(), drawer->get_textfont(), 0, true).w);
		}
	}

	//status
	cocos_propfile* propfile = (cocos_propfile*)m_status->get_data();
	if (propfile && propfile->get_fullname() == curr)
		m_status->set_text(cocos_filedata::get_file(curr));
}
xui_method_explain(cocos_project, ntf_modify,				void			)( const std::wstring& path, const std::wstring& file )
{
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
		if (proppath == m_like ||
			proppath->get_fullname() != path)
			continue;

		const xui_proproot_vec& filevec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = filevec.begin(); itor != filevec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (cocos_filedata::get_file(propfile->get_fullname()) == file)
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
xui_method_explain(cocos_project, on_timertick,				void			)( xui_component* sender, xui_method_args&	   args )
{
	const xui_notify_vec& vec = xui_global::get_fwatch();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		std::wstring path = cocos_filedata::get_path(vec[i].srcpath);
		std::wstring file = cocos_filedata::get_file(vec[i].srcpath);
		path = path.substr(0, path.length()-1);
		//file = cocos_filedata::get_safe(file);
		ntf_modify(path, file);
	}

	xui_global::del_fwatch();
}
xui_method_explain(cocos_project, on_timersavetick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> pathvec = m_pathview->get_entirenode();
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		xui_treenode*	pathnode = pathvec[i];
		cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
		if (proppath == m_like)
			continue;

		const xui_proproot_vec& filevec = proppath->get_fileprop();
		for (xui_proproot_vec::const_iterator itor = filevec.begin(); itor != filevec.end(); ++itor)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(*itor);
			if (propfile->was_modify())
			{
				propfile->auto_save();
			}
		}
	}
}
xui_method_explain(cocos_project, on_clearclick,			void			)( xui_component* sender, xui_method_args&     args )
{
	m_search->set_text(L"");
}
xui_method_explain(cocos_project, on_searchtextchanged,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_method_ptrcall(m_clear,		set_visible	)(m_search->get_text().length() >  0);
	xui_method_ptrcall(m_pathpane,	set_enable	)(m_search->get_text().length() == 0);
	xui_method_ptrcall(m_fill,		refresh		)();
	if (m_fileview->get_tileview()->get_viewfile())
	{
		m_fileview->get_tileview()->set_viewtext(xui_global::get_upper(m_search->get_text()));
	}
	else
	{
		refresh_fileview();
		if (m_search->get_text().length() == 0)
		{
			xui_treeview*	lineview = m_fileview->get_lineview();
			cocos_tileview*	tileview = m_fileview->get_tileview();

			std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
			if (nodevec.size() > 0)
			{
				xui_treenode*   pathnode = nodevec.front();
				cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
				cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
				cocos_propfile* viewfile = proppath->get_viewfile();
				if (viewfile && viewfile->get_linkdata(lineview))
				{
					lineview->set_allowmulti(true);
					tileview->set_viewfile(viewfile->get_linkdata(lineview)->get_node(), xui_global::get_upper(m_search->get_text()));
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
xui_method_explain(cocos_project, on_filterselection,		void			)( xui_component* sender, xui_method_args&	   args )
{
	if (m_search->get_text().length() > 0)
	{
		refresh_fileview();
	}
}
xui_method_explain(cocos_project, on_headperform,			void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search,	on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search,	on_perform_x)(m_create->get_renderx()+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,		on_perform_x)(m_create->get_renderx()+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
	xui_method_ptrcall(m_filter,	on_perform_x)(m_search->get_renderx()+m_search->get_renderw()+6);
	xui_method_ptrcall(m_linetool,	on_perform_x)(m_filter->get_renderx()+m_filter->get_renderw()+6);
}
xui_method_explain(cocos_project, on_fillperform,			void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt = m_pathview->get_renderrt()+m_pathview->get_renderpt();
	m_sizectrl->on_perform_h(rt.get_h());
	m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
	m_sizectrl->on_perform_y(rt.ay);
}
xui_method_explain(cocos_project, on_fillrenderelse,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_rect2d<s32> rt;
	rt = m_pathview->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_pathpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_toolpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour::black);
}
xui_method_explain(cocos_project, on_sizectrlmousemove,		void			)( xui_component* sender, xui_method_mouse&    args )
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
xui_method_explain(cocos_project, on_pathviewselection,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_method_ptrcall(m_search, ini_drawer	)(L"");
	xui_method_ptrcall(m_clear,	 set_visible)(false);
	refresh_fileview();
	refresh_pathpane();
	refresh_pathmenu();

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   pathnode = nodevec.front();
		cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
		cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
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
xui_method_explain(cocos_project, on_fileviewnodeclick,		void			)( xui_component* sender, xui_method_mouse&    args )
{
	xui_proproot_vec propvec;
	std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(data->get_prop());
		if (prop == NULL)
			propvec.push_back(data->get_prop());

		if (node->get_rootnode() == NULL)
		{
			cocos_filedata* filedata = (cocos_filedata*)node->get_linkdata();
			xui_method_ptrcall(m_status, set_icon		)(filedata->get_icon(0));
			xui_method_ptrcall(m_status, set_text		)(filedata->get_file());
			xui_method_ptrcall(m_status, set_data		)(filedata->get_prop());
			xui_method_ptrcall(m_status, set_iconsize	)(xui_vector<s32>(16));
		}
	}

	refresh_linetool();

	if (propvec.size() > 0)
	{
		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(propvec);
	}
}
xui_method_explain(cocos_project, on_fileviewdoubleclk,		void			)( xui_component* sender, xui_method_mouse&	   args )
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
				cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
				cocos_filedata* file = dynamic_cast<cocos_filedata*>(data);
				cocos_propfile* prop = dynamic_cast<cocos_propfile*>(data->get_prop());
				if (file)
				{
					//std::wstring suff = file->get_suff();
					//if (suff == L".controller")
					//{
					//	cocos_animator*  page = cocos_mainform::get_ptr()->get_animator();
					//	cocos_mainform::get_ptr()->set_pageshow(page);
					//	page->set_editprop((cocos_propcontroller*)file->get_prop());
					//}
					//else
					//if (suff == L".npCourse")
					//{
					//	cocos_hierarchy* page = cocos_mainform::get_ptr()->get_hierarchy();
					//	cocos_mainform::get_ptr()->set_pageshow(page);
					//	page->set_editprop((cocos_propcourse*    )file->get_prop());
					//}
					//else
					cocos_propatlas* propatlas = dynamic_cast<cocos_propatlas*>(prop);
					cocos_propcsd*   propcsd   = dynamic_cast<cocos_propcsd*  >(prop);
					if (propcsd)
					{
						cocos_mainform::get_ptr()->add_scene(propcsd);
					}
					else
					if (propatlas)
					{
						m_fileview->get_lineview()->non_selectednode();
						m_fileview->get_lineview()->set_allowmulti(true);
						m_fileview->get_tileview()->set_viewfile(node, xui_global::get_upper(m_search->get_text()));

						if (m_search->get_text().length() == 0)
						{
							xui_treenode*   pathnode = m_pathview->get_selectednode().front();
							cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
							cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
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
			}
		}
	}
}
xui_method_explain(cocos_project, on_fileviewassetdrag,		void			)( xui_component* sender, xui_method_dragdrop& args )
{
	static std::vector<void*>  datavec; datavec.clear();

	std::string typestr;
	std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		xui_treenode*   node = nodevec[i];
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		xui_proproot*   prop = data->get_prop();

		cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(prop);
		cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(prop);
		if (propfile)
		{
			if (typestr.length() > 0 && typestr != propfile->get_dragtype())
				break;

			typestr = propfile->get_dragtype();
			datavec.push_back(propfile->get_dragdata());
		}
		if (propleaf)
		{
			if (typestr.length() > 0 && typestr != propleaf->get_dragtype())
				break;

			typestr = propleaf->get_dragtype();
			datavec.push_back(propleaf->get_dragdata());
		}
	}

	if (datavec.size() > 0)
	{
		args.drag  = sender;
		args.allow = true;

		if (datavec.size() == 1)
		{
			args.type = typestr;
			args.data = datavec.front();
		}
		else
		{
			args.type = typestr + "Vector";
			args.data = &datavec;
		}
	}
}
xui_method_explain(cocos_project, on_folderclick,			void			)( xui_component* sender, xui_method_args&     args )
{
	xui_method_ptrcall(m_filter, ini_dropbox)(0);
	xui_method_ptrcall(m_search, set_text	)(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   rootnode = nodevec.front();
		cocos_pathdata* rootdata = (cocos_pathdata*)rootnode->get_linkdata();
		cocos_proppath* rootprop = dynamic_cast<cocos_proppath*>(rootdata->get_prop());
		if (rootprop == m_like)
			return;

		std::wstring path = rootdata->get_full();
		std::wstringstream temp;
		temp <<  path.c_str();
		temp << (path.length() > 0 ? L"/New Folder" : L"New Folder");
		if (xui_global::has_path(temp.str()))
		{
			s32 number = 0;
			while (true)
			{
				temp.str(L"");
				temp <<  path.c_str();
				temp << (path.length() > 0 ? L"/New Folder" : L"New Folder");
				temp << number;
				if (xui_global::has_path(temp.str()) == false)
					break;

				++number;
			}
		}

		if (xui_global::add_path(temp.str()) == false)
			return;

		cocos_proppath* proppath = new cocos_proppath(cocos_resource::icon_folder, temp.str());
		xui_treenode*	pathnode = rootnode->add_leafnode(0, new cocos_pathdata(cocos_resource::icon_folder, proppath->get_fullname(), proppath));
		xui_treeview*	lineview = m_fileview->get_lineview();
		xui_treenode*	linenode = lineview->add_upmostnode(0, new cocos_pathdata(cocos_resource::icon_folder, proppath->get_fullname(), proppath));
		rootprop->add_pathprop(proppath);
		linenode->set_data(pathnode);
		rootnode->set_expanded(true);

		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(cocos_project, on_particleclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_method_ptrcall(m_filter, ini_dropbox)(0);
	xui_method_ptrcall(m_search, set_text	)(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   rootnode = nodevec.front();
		cocos_pathdata* rootdata = (cocos_pathdata*)rootnode->get_linkdata();
		cocos_proppath* rootprop = dynamic_cast<cocos_proppath*>(rootdata->get_prop());
		if (rootprop == m_like)
			return;

		std::wstring path = rootdata->get_full();
		std::wstringstream temp;
		temp <<  path.c_str();
		temp << (path.length() > 0 ? L"/New Particle.plist" : L"New Particle.plist");
		if (xui_global::has_file(temp.str()))
		{
			s32 number  = 0;
			while (true)
			{
				temp.str(L"");
				temp <<  path.c_str();
				temp << (path.length() > 0 ? L"/New Particle" : L"New Particle");
				temp << number;
				temp << ".plist";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		cocos_propparticle* propfile = new cocos_propparticle(temp.str());
		xui_method_ptrcall(rootprop, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new cocos_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(cocos_project, on_materialclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_method_ptrcall(m_filter, ini_dropbox)(0);
	xui_method_ptrcall(m_search, set_text	)(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   rootnode = nodevec.front();
		cocos_pathdata* rootdata = (cocos_pathdata*)rootnode->get_linkdata();
		cocos_proppath* rootprop = dynamic_cast<cocos_proppath*>(rootdata->get_prop());
		if (rootprop == m_like)
			return;

		std::wstring path = rootdata->get_full();
		std::wstringstream temp;
		temp <<  path.c_str();
		temp << (path.length() > 0 ? L"/New Material.mat" : L"New Material.mat");
		if (xui_global::has_file(temp.str()))
		{
			s32 number = 0;
			while (true)
			{
				temp.str(L"");
				temp <<  path.c_str();
				temp << (path.length() > 0 ? L"/New Material" : L"New Material");
				temp << number;
				temp << ".mat";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		cocos_propmaterial* propfile = new cocos_propmaterial(temp.str());
		xui_method_ptrcall(rootprop, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new cocos_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(cocos_project, on_sceneclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_method_ptrcall(m_filter, ini_dropbox)(0);
	xui_method_ptrcall(m_search, set_text	)(L"");

	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   rootnode = nodevec.front();
		cocos_pathdata* rootdata = (cocos_pathdata*)rootnode->get_linkdata();
		cocos_proppath* rootprop = dynamic_cast<cocos_proppath*>(rootdata->get_prop());
		if (rootprop == m_like)
			return;

		std::wstring path = rootdata->get_full();
		std::wstringstream temp;
		temp <<  path.c_str();
		temp << (path.length() > 0 ? L"/New Scene.csd" : L"New Scene.csd");
		if (xui_global::has_file(temp.str()))
		{
			s32 number = 0;
			while (true)
			{
				temp.str(L"");
				temp <<  path.c_str();
				temp << (path.length() > 0 ? L"/New Scene" : L"New Scene");
				temp << number;
				temp << ".csd";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		cocos_propcsd* propfile = new cocos_propcsd(temp.str());
		xui_method_ptrcall(rootprop, add_fileprop)(propfile);
		xui_method_ptrcall(propfile, save		 )();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new cocos_filedata(propfile->get_fileicon(), propfile->get_fullname(), propfile));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(cocos_project, on_pathitemclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_treenode* pathnode = (xui_treenode*)sender->get_data();
	if (pathnode->was_selected())
	{
		xui_treenode* viewfile = m_fileview->get_tileview()->get_viewfile();
		if (viewfile)
		{
			viewfile->set_expanded(false);

			cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
			cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
			m_fileview->get_tileview()->set_viewfile(NULL, L"");
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
xui_method_explain(cocos_project, on_pathtogglerender,		void			)( xui_component* sender, xui_method_args&     args )
{
	xui_toggle*     toggle = xui_dynamic_cast(xui_toggle, sender);
	xui_colour      color  = toggle->get_rendercolor() * toggle->get_vertexcolor();
	xui_rect2d<s32> rt     = toggle->get_renderrtins() + toggle->get_screenpt();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
}
xui_method_explain(cocos_project, on_pathtoggleclick,		void			)( xui_component* sender, xui_method_args&     args )
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
				cocos_pathdata* data = (cocos_pathdata*)node->get_linkdata();
				xui_method_ptrcall(item, set_text)(data->get_text(0));
			}

			menu->refresh();
		}
	}
}
xui_method_explain(cocos_project, on_sizerollscroll,		void			)( xui_component* sender, xui_method_args&     args )
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
			cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
			cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());

			if (m_fileview->get_tileview()->get_viewfile()) proppath->set_fileroll(value);
			else											proppath->set_pathroll(value);
		}
	}
}

xui_method_explain(cocos_project, on_pathloadclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode*	node = vec.front();
		cocos_pathdata* data = (cocos_pathdata*)node->get_linkdata();
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(data->get_prop());
		prop->refresh();
		refresh_fileview();
	}
}
xui_method_explain(cocos_project, on_showfindclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	if (vec.size() > 0)
	{
		cocos_pathdata* data = (cocos_pathdata*)vec.front()->get_linkdata();
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(data->get_prop());
		xui_global::set_showfind(prop->get_fullname());
	}
}
xui_method_explain(cocos_project, on_propertyclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	std::vector<xui_treenode*> vec = m_pathview->get_selectednode();
	if (vec.size() > 0)
	{
		cocos_pathdata* data = (cocos_pathdata*)vec.front()->get_linkdata();
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(data->get_prop());

		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(prop);
	}
}
xui_method_explain(cocos_project, on_linetoolclick,			void			)( xui_component* sender, xui_method_args&	   args )
{
	xui_treenode*   pathnode = m_histroy[m_curridx];
	cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
	cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();

	xui_proproot_vec propvec;
	std::vector<xui_treenode*> selectednode = m_fileview->get_lineview()->get_selectednode();
	for (u32 i = 0; i < selectednode.size(); ++i)
	{
		cocos_treedata* data = dynamic_cast<cocos_treedata*>(selectednode[i]->get_linkdata());
		cocos_propleaf* prop = dynamic_cast<cocos_propleaf*>(data->get_prop());
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
		cocos_treedata*  filedata = dynamic_cast<cocos_treedata*>(viewfile->get_linkdata());
		m_copy->set_data(filedata->get_prop());
		m_move->set_data(NULL);
		m_menuprop = propvec;
	}
	else
	if (sender == m_move)
	{
		cocos_treedata*  filedata = dynamic_cast<cocos_treedata*>(viewfile->get_linkdata());
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
	if (sender == m_favorite)
	{
		cocos_treedata* filedata = NULL;
		if (viewfile)
		{
			filedata = (cocos_treedata*)viewfile->get_linkdata();
		}
		else
		if (selectednode.size() > 0)
		{
			xui_treenode* node = selectednode.front();
			if (node->get_data() == NULL)
			{
				filedata = (cocos_treedata*)node->get_linkdata();
			}
		}

		if (filedata)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(filedata->get_prop());
			if (m_favorite->was_push())
				m_like->add_fileprop(propfile);
			else
				m_like->del_fileprop(propfile);

			m_like->save();
		}
	}
	else
	if (sender == m_backpath && viewfile)
	{
		xui_treeview*   lineview = m_fileview->get_lineview();
		cocos_tileview* tileview = m_fileview->get_tileview();
		viewfile->set_expanded(false);
		tileview->set_viewfile(NULL, L"");
		lineview->set_allowmulti(false);
		lineview->set_selectednode(viewfile, true);
		m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_pathroll());
		refresh_tileview();
	}
	else
	if (sender == m_forepath && viewfile == NULL && proppath->get_viewfile())
	{
		xui_treeview*   lineview = m_fileview->get_lineview();
		cocos_tileview* tileview = m_fileview->get_tileview();

		if (proppath->get_viewfile()->get_linkdata(lineview) == NULL)
			return;

		lineview->non_selectednode();
		lineview->set_allowmulti(true);
		tileview->set_viewfile(proppath->get_viewfile()->get_linkdata(lineview)->get_node(), xui_global::get_upper(m_search->get_text()));
		m_sizeroll->ini_scroll(m_sizeroll->get_range(), proppath->get_fileroll());
		refresh_tileview();
	}
	else
	{
		xui_treeview*   lineview = m_fileview->get_lineview();
		cocos_tileview* tileview = m_fileview->get_tileview();

		if (sender == m_backpath) ++m_curridx;
		if (sender == m_forepath) --m_curridx;

		if (m_curridx <= (s32)m_histroy.size()-1)
		{
			pathnode = m_histroy[m_curridx];
			pathdata = (cocos_pathdata*)pathnode->get_linkdata();
			proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());

			xui_method_ptrcall(m_pathview, ini_selectednode	)(pathnode, true);
			xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
			refresh_fileview();
			refresh_pathpane();
			refresh_pathmenu();

			cocos_propfile* viewfile = proppath->get_viewfile();
			if (viewfile && viewfile->get_linkdata(lineview))
			{
				lineview->set_allowmulti(true);
				tileview->set_viewfile(viewfile->get_linkdata(lineview)->get_node(), xui_global::get_upper(m_search->get_text()));
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
xui_method_explain(cocos_project, refresh_fileview,			void			)( void )
{
	xui_method_ptrcall(m_status,	set_data)(NULL);
	xui_method_ptrcall(m_fileview,	clear	)();

	xui_treeview*   lineview = m_fileview->get_lineview();
	cocos_tileview*	tileview = m_fileview->get_tileview();

	u32 index = 0;
	std::vector<xui_proproot*> filevec;
	if (m_search->get_text().length() == 0)
	{
		std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
		if (nodevec.size() > 0)
		{
			xui_treenode*   rootnode = nodevec.front();
			cocos_pathdata* rootdata = (cocos_pathdata*)rootnode->get_linkdata();
			cocos_proppath* rootprop = dynamic_cast<cocos_proppath*>(rootdata->get_prop());

			const std::vector<xui_treenode*>& pathvec = rootnode->get_leafnodearray();
			filevec = rootprop->get_fileprop();
			for (u32 i = 0; i < pathvec.size(); ++i,++index)
			{
				cocos_pathdata* data = (cocos_pathdata*)pathvec[i]->get_linkdata();
				xui_treenode*   node = lineview->add_upmostnode(index, new cocos_pathdata(cocos_resource::icon_folder, data->get_full(), data->get_prop()));
				node->set_data(pathvec[i]);
			}
		}
	}
	else
	{
		get_pathfile(m_filter->get_selectedindex(), filevec);
	}

	std::wstring keytext = xui_global::get_upper(m_search->get_text());
	for (u32 i = 0; i < filevec.size(); ++i)
	{
		cocos_propfile*		prop		= dynamic_cast<cocos_propfile*>(filevec[i]);
		std::wstring		safe		= cocos_filedata::get_safe(prop->get_fullname());
		cocos_propatlas*	propatlas	= dynamic_cast<cocos_propatlas*>(prop);
		bool showroot = (keytext.length() == 0 || xui_global::get_upper(safe).find(keytext) != -1);
		bool showleaf = (keytext.length() != 0 && propatlas && propatlas->has_frame(keytext));
		if (showroot || showleaf)
		{
			xui_treenode* node = lineview->add_upmostnode(index, new cocos_filedata(prop->get_fileicon(), prop->get_fullname(), prop));
			if (propatlas)
			{
				if (showleaf)
					node->set_expanded(true);

				std::vector<xui_proproot*> subprop = propatlas->get_subprop();
				for (u32 isub = 0, isubindex = 0; isub < subprop.size(); ++isub)
				{
					cocos_propframe* propframe = dynamic_cast<cocos_propframe*>(subprop[isub]);
					std::wstring framename = xui_global::ascii_to_unicode(propframe->get_framename());
					if (showroot || xui_global::get_upper(framename).find(keytext) != -1)
					{
						node->add_leafnode(isubindex, new cocos_framedata(propframe->get_frameicon(), propframe));
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
xui_method_explain(cocos_project, refresh_pathpane,			void			)( void )
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

		for (s32 i = (s32)pathvec.size()-1; i >= 0; --i)
		{
			xui_treenode*    pathnode = pathvec[i];
			cocos_pathdata*  pathdata = (cocos_pathdata*)pathnode->get_linkdata();
			xui_button*        button = new xui_button(xui_vector<s32>(0, 20));
			xui_method_ptrcall(button, ini_component	)(0, 0, DOCKSTYLE_L);
			xui_method_ptrcall(button, ini_drawer		)(pathdata->get_text(0));
			xui_method_ptrcall(button, set_backcolor	)(xui_colour::transparent);
			xui_method_ptrcall(button, set_drawcolor	)(true);
			xui_method_ptrcall(button, set_textcolor	)(xui_colour(1.0f, 0.8f));
			xui_method_ptrcall(button, set_data			)(pathnode);
			xui_method_ptrcall(button, on_perform_w		)(xui_convas::get_ins()->calc_size(button->get_text(), button->get_textfont(), 0, true).w);
			xui_method_ptrcall(button, xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_project>(this, &cocos_project::on_pathitemclick);
			m_pathpane->add_child(button);

			if (pathnode->get_leafnodecount() == 0)
				continue;

			xui_menu* menu = xui_menu::create(80);
			const std::vector<xui_treenode*>& leafvec = pathnode->get_leafnodearray();
			for (u32 ileaf = 0; ileaf < leafvec.size(); ++ileaf)
			{
				xui_treenode*    leafnode = leafvec[ileaf];
				cocos_pathdata*  leafdata = (cocos_pathdata*)leafnode->get_linkdata();
				xui_menuitem*    menuitem = menu->add_item(NULL, leafdata->get_text(0));
				menuitem->xm_click	   += new xui_method_member<xui_method_args, cocos_project>(this, &cocos_project::on_pathitemclick);
				xui_method_ptrcall(menuitem, set_data)(leafnode);
				xui_method_ptrcall(menuitem, set_flag)(leafnode->was_selected());
			}

			xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16, 20), TOGGLE_BUTTON);
			toggle->xm_renderself  += new xui_method_member<xui_method_args,  cocos_project>(this, &cocos_project::on_pathtogglerender);
			toggle->xm_toggleclick += new xui_method_member<xui_method_args,  cocos_project>(this, &cocos_project::on_pathtoggleclick);
			xui_method_ptrcall(toggle, ini_component)(0, 0, DOCKSTYLE_L);
			xui_method_ptrcall(toggle, set_data		)(pathnode);
			xui_method_ptrcall(toggle, set_menu		)(menu);
			m_pathpane->add_child(toggle);
		}
	}
}
xui_method_explain(cocos_project, refresh_linetool,			void			)( void )
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
			cocos_pathdata* pathdata = (cocos_pathdata*)pathnode->get_linkdata();
			cocos_proppath* proppath = dynamic_cast<cocos_proppath*>(pathdata->get_prop());
			if (m_fileview->get_tileview()->get_viewfile() == NULL && proppath->get_viewfile() != NULL)
				foreenable = true;
		}
	}

	std::vector<xui_treenode*> selectednode = m_fileview->get_lineview()->get_selectednode();
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();
	cocos_filedata* filedata = (viewfile == NULL) ? NULL : dynamic_cast<cocos_filedata*>(viewfile->get_linkdata());
	xui_method_ptrcall(m_backpath,	set_enable)(backenable);
	xui_method_ptrcall(m_forepath,	set_enable)(foreenable);
	xui_method_ptrcall(m_add,		set_enable)(false && viewfile != NULL);
	xui_method_ptrcall(m_del,		set_enable)(false && viewfile != NULL && selectednode.size() > 0);
	xui_method_ptrcall(m_copy,		set_enable)(false && viewfile != NULL && selectednode.size() > 0);
	xui_method_ptrcall(m_move,		set_enable)(false && viewfile != NULL && selectednode.size() > 0 && filedata->get_suff() != L".json");
	xui_method_ptrcall(m_paste,		set_enable)(false && viewfile != NULL && m_search->get_text().length() == 0 && m_menuprop.size() > 0);

	cocos_propfile* propfile = NULL;
	if (filedata)
	{
		propfile = dynamic_cast<cocos_propfile*>(filedata->get_prop());
	}
	else
	if (selectednode.size() > 0)
	{
		xui_treenode* node = selectednode.front();
		if (node->get_data() == NULL)
		{
			cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
			propfile = dynamic_cast<cocos_propfile*>(data->get_prop());
		}
	}

	xui_method_ptrcall(m_favorite,	set_enable)(propfile != NULL);
	xui_method_ptrcall(m_favorite,	ini_toggle)(m_like->has_fileprop(propfile));
}
//xui_method_explain(cocos_project, convert_filesuff,			std::wstring	)( void )
//{
//	u32 index = m_filter->get_selectedindex();
//	switch (index)
//	{
//	case FILTER_TEXTURE:	return L".png";
//	case FILTER_PARTICLE:	return L".particle";
//	}
//
//	return L"";
//}
xui_method_explain(cocos_project, refresh_tileview,			void			)( void )
{
	s32 value = m_sizeroll->get_value();
	xui_treeview*      lineview = m_fileview->get_lineview();
	cocos_tileview*	   tileview = m_fileview->get_tileview();
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
xui_method_explain(cocos_project, refresh_pathmenu,			void			)( void )
{
	bool enable = false;
	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*	node = nodevec.front();
		cocos_treedata* data = (cocos_treedata*)node->get_linkdata();
		cocos_proppath* prop = dynamic_cast<cocos_proppath*>(data->get_prop());
		enable = (prop != m_like);
	}

	xui_method_ptrcall(m_folder,	set_enable)(enable);
	xui_method_ptrcall(m_particle,	set_enable)(enable);
	xui_method_ptrcall(m_pathload,	set_enable)(enable);
	xui_method_ptrcall(m_showfind,	set_enable)(enable);
	xui_method_ptrcall(m_property,	set_enable)(enable);
}
xui_method_explain(cocos_project, add_propleaf,				void			)( void )
{
	xui_treenode*   viewfile = m_fileview->get_tileview()->get_viewfile();
	cocos_treedata* filedata = dynamic_cast<cocos_treedata*>(viewfile->get_linkdata());
	cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(filedata->get_prop());
	xui_proproot*   propleaf = NULL;
	xui_treedata*   linkdata = NULL;

	//cocos_prop2dsres* prop2dsres = dynamic_cast<cocos_prop2dsres*>(propfile);
	//if (prop2dsres)
	//{
	//	propleaf = prop2dsres->add_subprop();
	//	cocos_prop2dsasset* propasset = dynamic_cast<cocos_prop2dsasset*>(propleaf);
	//	linkdata = new cocos_2dsassetdata(propasset->get_resicon(), propasset);
	//}

	//cocos_propjsones* propjsones = dynamic_cast<cocos_propjsones*>(propfile);
	//if (propjsones)
	//{
	//	propleaf = propjsones->add_subprop();
	//	linkdata = new cocos_jsonestempdata(cocos_resource::icon_entity, propleaf);
	//}

	if (linkdata)
	{
		u32 index = viewfile->get_leafnodecount();
		xui_treenode*   node     = viewfile->add_leafnode(index, linkdata);
		xui_treeview*   lineview = m_fileview->get_lineview();
		cocos_tileview* tileview = m_fileview->get_tileview();
		xui_method_ptrcall(lineview, non_selectednode	)();
		xui_method_ptrcall(lineview, set_selectednode	)(node, true);
		xui_method_ptrcall(lineview, set_nodevisible	)(node);
		xui_method_ptrcall(tileview, set_tilevisible	)(node);

		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(propleaf);
	}
}
xui_method_explain(cocos_project, del_propleaf,				void			)( const xui_proproot_vec& propvec )
{
	//for (u32 i = 0; i < propvec.size(); ++i)
	//{
	//	cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(propvec[i]);
	//	cocos_propfile* propfile = propleaf->get_propfile();
	//	xui_treedata*   linkdata = propleaf->get_linkdata(m_fileview->get_lineview());
	//	xui_treedata*	filedata = propfile->get_linkdata(m_fileview->get_lineview());
	//	if (filedata && linkdata)
	//	{
	//		filedata->get_node()->del_leafnode(linkdata->get_node());
	//	}

	//	cocos_prop2dsres* prop2dsres = dynamic_cast<cocos_prop2dsres*>(propfile);
	//	if (prop2dsres)
	//		prop2dsres->del_subprop(propleaf);

	//	cocos_propjsones* propjsones = dynamic_cast<cocos_propjsones*>(propfile);
	//	if (propjsones)
	//		propjsones->del_subprop(propleaf);
	//}
}
xui_method_explain(cocos_project, pst_propleaf,				void			)( void )
{
	xui_treenode*   dstnode = m_fileview->get_tileview()->get_viewfile();
	cocos_treedata* dstdata = dynamic_cast<cocos_treedata*>(dstnode->get_linkdata());
	cocos_propfile* dstfile = dynamic_cast<cocos_propfile*>(dstdata->get_prop());
	cocos_propleaf* srcprop = dynamic_cast<cocos_propleaf*>(m_menuprop.front());
	cocos_propfile* srcfile = srcprop->get_propfile();
	std::wstring    dstfull = dstfile->get_fullname();
	std::wstring    srcfull = srcfile->get_fullname();
	if (cocos_filedata::get_suff(dstfull) == cocos_filedata::get_suff(srcfull))
	{
		std::vector<cocos_treedata*> vec;
		//cocos_prop2dsres* prop2dsres = dynamic_cast<cocos_prop2dsres*>(dstfile);
		//if (prop2dsres)
		//{
		//	for (u32 i = 0; i < m_menuprop.size(); ++i)
		//	{
		//		cocos_prop2dsasset* srcleaf = dynamic_cast<cocos_prop2dsasset*>(m_menuprop[i]);
		//		cocos_prop2dsasset* dstleaf = dynamic_cast<cocos_prop2dsasset*>(prop2dsres->add_subprop(srcleaf));
		//		u32 index = dstnode->get_leafnodecount();
		//		cocos_treedata* linkdata = new cocos_2dsassetdata(dstleaf->get_resicon(), dstleaf);
		//		dstnode->add_leafnode(index, linkdata);
		//		vec.push_back(linkdata);
		//	}
		//}

		//cocos_propjsones* propjsones = dynamic_cast<cocos_propjsones*>(dstfile);
		//if (propjsones)
		//{
		//	for (u32 i = 0; i < m_menuprop.size(); ++i)
		//	{
		//		cocos_propjsonestemp* srcleaf = dynamic_cast<cocos_propjsonestemp*>(m_menuprop[i]);
		//		cocos_propjsonestemp* dstleaf = dynamic_cast<cocos_propjsonestemp*>(propjsones->add_subprop(srcleaf));
		//		u32 index = dstnode->get_leafnodecount();
		//		cocos_treedata* linkdata = new cocos_jsonestempdata(cocos_resource::icon_entity, dstleaf);
		//		dstnode->add_leafnode(index, linkdata);
		//		vec.push_back(linkdata);
		//	}
		//}

		//if (m_move->get_data())
		//{
		//	m_move->set_data(NULL);
		//	del_propleaf(m_menuprop);
		//	m_menuprop.clear();
		//}

		//if (vec.size() > 0)
		//{
		//	std::vector<xui_treenode*> nodes;
		//	for (u32 i = 0; i < vec.size(); ++i)
		//		nodes.push_back(vec[i]->get_node());

		//	xui_treeview*   lineview = m_fileview->get_lineview();
		//	cocos_tileview* tileview = m_fileview->get_tileview();
		//	xui_method_ptrcall(lineview, non_selectednode	)();
		//	xui_method_ptrcall(lineview, set_selectednode	)(nodes);
		//	xui_method_ptrcall(lineview, set_nodevisible	)(nodes.front());
		//	xui_method_ptrcall(tileview, set_tilevisible	)(nodes.front());

		//	std::vector<xui_proproot*> props;
		//	for (u32 i = 0; i < vec.size(); ++i)
		//		props.push_back(vec[i]->get_prop());
		//	
		//	cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		//	inspector->set_proproot(props);
		//}
	}
}