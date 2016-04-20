#include "NPFileName.h"
#include "NP2DSStateCtrl.h"
#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"

#include "xui_bitmap.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_toggle.h"
#include "xui_button.h"
#include "xui_textbox.h"
#include "xui_panel.h"
#include "xui_treeview.h"
#include "xui_listview.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_slider.h"
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
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_mainform.h"
#include "onity_animator.h"
#include "onity_project.h"

xui_implement_rtti(onity_project, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_project)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(onity_resource::icon_project, L"Project");

	xui_menu* menu	= xui_menu::create(80);
	m_folder		= menu->add_item(onity_resource::icon_folder,	L"Folder");
	menu->add_separate();
	m_controller	= menu->add_item(onity_resource::icon_animator, L"Animation Controller");
	xui_method_ptrcall(m_folder,	xm_click			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_folderclick);
	xui_method_ptrcall(m_controller,xm_click			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_controllerclick);

	m_create = new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu);

	m_search = new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_searchtextenter);
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

	m_clear = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_buttonclick		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(onity_resource::icon_clear);

	m_head  = new xui_panel(xui_vector<s32>(28));
	xui_method_ptrcall(m_head,		xm_perform			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_headperform);
	xui_method_ptrcall(m_head,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,		set_drawcolor		)(false);
	xui_method_ptrcall(m_head,		set_borderrt		)(xui_rect2d<s32>(8, 4, 8, 4));
	xui_method_ptrcall(m_head,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_head,		add_child			)(m_create);
	xui_method_ptrcall(m_head,		add_child			)(m_search);
	xui_method_ptrcall(m_head,		add_child			)(m_clear );

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_pathview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_pathview,	xm_selectedchange	) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_pathviewselection);
	xui_method_ptrcall(m_pathview,	ini_component		)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_pathview,	set_hscrollauto		)(false);

	m_fileview = new onity_fileview;
	xui_method_ptrcall(m_fileview,	xm_lineviewselection) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_fileviewselection);
	xui_method_ptrcall(m_fileview,	xm_lineviewdoubleclk) += new xui_method_member<xui_method_mouse, onity_project>(this, &onity_project::on_fileviewdoubleclk);
	xui_method_ptrcall(m_fileview,	ini_component		)(0, 0, DOCKSTYLE_F);

	m_pathpane = new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_pathpane,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_pathpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_pathpane,	set_borderrt		)(xui_rect2d<s32>(8, 0, 8, 0));
	xui_method_ptrcall(m_pathpane,	set_hscrollauto		)(false);

	m_slider = new xui_slider(xui_vector<s32>(64, 16), FLOWSTYLE_H, ARROWDRAW_NONE);
	xui_method_ptrcall(m_slider,	set_backcolor		)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_slider,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_slider,	ini_component		)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_slider,	ini_scroll			)(60, 0);
	xui_method_ptrcall(m_slider,	xm_scroll			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_sliderscroll);

	m_status = new xui_drawer(xui_vector<s32>(200, 20)); 
	xui_method_ptrcall(m_status,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_status,	set_textoffset		)(xui_vector<s32>(2, 0));

	m_toolpane = new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_toolpane,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_toolpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_toolpane,	set_borderrt		)(xui_rect2d<s32>(4, 2, 24, 2));
	xui_method_ptrcall(m_toolpane,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_toolpane,	add_child			)(m_slider);
	xui_method_ptrcall(m_toolpane,	add_child			)(m_status);

	m_sizectrl = new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse, onity_project>(this, &onity_project::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_WE);

	m_fill = new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fill,		xm_perform			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_fillperform);
	xui_method_ptrcall(m_fill,		xm_renderelse		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_fillrenderelse);
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
//method
*/
xui_method_explain(onity_project, ini_pathtree,				void)( void )
{
	std::vector<std::wstring> pathvec = xui_global::get_path(L"");
	for (u32 i = 0; i < pathvec.size(); ++i)
	{
		std::wstring  full = pathvec[i];
		xui_treenode* node = m_pathview->add_upmostnode(i, new onity_pathdata(full, NULL, NULL));
		onity_pathdata::new_leafnode(node);
	}
}

/*
//notify
*/
xui_method_explain(onity_project, ntf_load,					void)( onity_propfile* propfile )
{
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
				for (u32 isub = 0; isub < subprop.size(); ++isub)
				{
					node->add_leafnode(isub, new onity_2dsassetdata(subprop[isub]));
				}
			}
		}
	}
}
xui_method_explain(onity_project, ntf_rename,				void)( const std::wstring& last, const std::wstring& curr )
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
	if (propfile && propfile->get_full() == curr)
		m_status->set_text(onity_filedata::get_file(curr));
}

/*
//event
*/
xui_method_explain(onity_project, on_clearclick,			void)( xui_component* sender, xui_method_args&  args )
{
	m_search->set_text(L"");
}
xui_method_explain(onity_project, on_searchtextchanged,		void)( xui_component* sender, xui_method_args&  args )
{
	m_clear->ini_component(true, m_search->get_text().length() > 0);
	//TODO
}
xui_method_explain(onity_project, on_searchtextenter,		void)( xui_component* sender, xui_method_args&  args )
{
	//TODO
}
xui_method_explain(onity_project, on_headperform,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_head->get_renderrtins();
	s32 w = rt.get_w()-m_create->get_renderw()-6;
	xui_method_ptrcall(m_search, on_perform_w)(xui_min(w, 150));
	xui_method_ptrcall(m_search, on_perform_x)(rt.ax+m_create->get_renderw()+6);
	xui_method_ptrcall(m_clear,  on_perform_x)(rt.ax+m_create->get_renderw()+6+m_search->get_renderw()-m_clear->get_renderw()-2);
}
xui_method_explain(onity_project, on_fillperform,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_pathview->get_renderrt()+m_pathview->get_renderpt();
	m_sizectrl->on_perform_h(rt.get_h());
	m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
	m_sizectrl->on_perform_y(rt.ay);
}
xui_method_explain(onity_project, on_fillrenderelse,		void)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt;
	rt = m_pathview->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_pathpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx, rt.by), xui_colour::black);
	rt = m_toolpane->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour::black);
}
xui_method_explain(onity_project, on_sizectrlmousemove,		void)( xui_component* sender, xui_method_mouse& args )
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
xui_method_explain(onity_project, on_pathviewselection,		void)( xui_component* sender, xui_method_args&  args )
{
	refresh_lineview();
	refresh_pathpane();
}
xui_method_explain(onity_project, on_fileviewselection,		void)( xui_component* sender, xui_method_args&  args )
{
	std::vector<xui_treenode*> nodevec = m_fileview->get_lineview()->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   node = nodevec.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(data->get_prop());

		if (node->get_rootnode() == NULL)
		{
			onity_filedata* filedata = (onity_filedata*)node->get_linkdata();
			xui_method_ptrcall(m_status, set_icon)(filedata->get_icon(0));
			xui_method_ptrcall(m_status, set_text)(filedata->get_file());
			xui_method_ptrcall(m_status, set_data)(filedata->get_prop());
		}
	}
}
xui_method_explain(onity_project, on_fileviewdoubleclk,		void)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_treenode* node = m_fileview->get_lineview()->choose_node(m_fileview->get_renderpt(args.point));
		if (node && node->get_rootnode() == NULL)
		{
			if (node->get_data())
			{
				xui_treenode* pathnode = (xui_treenode*)node->get_data();
				xui_method_ptrcall(m_pathview, set_selectednode	)(pathnode, true);
				xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
			}
			else
			{
				onity_filedata* file = (onity_filedata*)node->get_linkdata();
				std::wstring    suff = file->get_suff();
				if (suff == L".controller")
				{
					onity_animator* animator = onity_mainform::get_ptr()->get_animator();
					onity_mainform::get_ptr()->set_pageshow(animator);
					animator->set_editprop((onity_propcontroller*)file->get_prop());
				}
				else
				if (suff == L".npModule" ||
					suff == L".npSprite" ||
					suff == L".npAction")
				{
					m_slider->set_data((void*)m_slider->get_value());
					if (m_slider->get_value() == 0)
						m_slider->set_value(10);

					onity_tileview* tileview = m_fileview->get_tileview();
					tileview->set_viewfile(node);
				}
			}
		}
	}
}
xui_method_explain(onity_project, on_folderclick,			void)( xui_component* sender, xui_method_args&  args )
{
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
xui_method_explain(onity_project, on_controllerclick,		void)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treenode*   node = nodevec.front();
		onity_pathdata* data = (onity_pathdata*)node->get_linkdata();
		std::wstring    path = data->get_full();

		std::wstringstream temp;
		temp << path.c_str();
		temp << L"/New Animation State.controller";
		if (xui_global::has_file(temp.str()))
		{
			s32 number  = 0;
			while (true)
			{
				temp.str(L"");
				temp << path.c_str();
				temp << L"/New Animation Controller ";
				temp << number;
				temp << ".controller";
				if (xui_global::has_file(temp.str()) == false)
					break;

				++number;
			}
		}

		onity_propcontroller* prop = new onity_propcontroller(temp.str());
		xui_method_ptrcall(data, add_leaf	)(prop);
		xui_method_ptrcall(prop, save		)();

		xui_treeview* lineview = m_fileview->get_lineview();
		xui_treenode* linenode = lineview->add_upmostnode(0, new onity_filedata(prop->get_full(), prop));
		lineview->refresh();
		linenode->set_edittext(0);
	}
}
xui_method_explain(onity_project, on_pathitemclick,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_treenode* node = (xui_treenode*)sender->get_data();
	if (node->was_selected())
	{
		onity_tileview* tileview = m_fileview->get_tileview();
		if (tileview->get_viewfile())
		{
			tileview->set_viewfile(NULL);

			s32 value = (s32)m_slider->get_data();
			m_slider->set_value(value);
			m_slider->set_data (NULL);

			if (tileview->was_visible())
			{
				xui_treeview* lineview = m_fileview->get_lineview();
				std::vector<xui_treenode*> nodes = lineview->get_selectednode();
				if (nodes.size() > 0)
				{
					tileview->set_tilevisible(nodes.front());
				}
			}
		}
	}
	else
	{
		xui_method_ptrcall(m_pathview, set_selectednode	)(node, true);
		xui_method_ptrcall(m_pathview, set_nodevisible	)(node);
	}
}
xui_method_explain(onity_project, on_pathtogglerender,		void)( xui_component* sender, xui_method_args&  args )
{
	xui_toggle*     toggle = xui_dynamic_cast(xui_toggle, sender);
	xui_colour      color  = toggle->get_rendercolor() * toggle->get_vertexcolor();
	xui_rect2d<s32> rt     = toggle->get_renderrtins() + toggle->get_screenpt();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
}
xui_method_explain(onity_project, on_pathtoggleclick,		void)( xui_component* sender, xui_method_args&  args )
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
xui_method_explain(onity_project, on_sliderscroll,			void)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

	s32 value = m_slider->get_value();
	if (value < 10 && value > 0)
	{
		value = 10;
		m_slider->ini_scroll(m_slider->get_range(), value);
	}

	xui_treeview*      lineview = m_fileview->get_lineview();
	onity_tileview*	   tileview = m_fileview->get_tileview();
	xui_method_ptrcall(lineview, set_visible	)(value == 0);
	xui_method_ptrcall(tileview, set_visible	)(value >  0);
	xui_method_ptrcall(tileview, set_tilesize	)(xui_max(10, value) + 40);
	m_fileview->refresh();
}

/*
//method
*/
xui_method_explain(onity_project, refresh_lineview,			void)( void )
{
	m_status->set_data(NULL);

	m_fileview->clear();
	std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
	if (nodevec.size() > 0)
	{
		xui_treeview*   lineview = m_fileview->get_lineview();
		onity_tileview*	tileview = m_fileview->get_tileview();

		u32 index = 0;
		xui_treenode*   rootnode = nodevec.front();
		onity_pathdata* rootdata = (onity_pathdata*)rootnode->get_linkdata();

		const std::vector<xui_treenode*>& pathvec = rootnode->get_leafnodearray();
		for (u32 i = 0; i < pathvec.size(); ++i,++index)
		{
			onity_pathdata* data = (onity_pathdata*)pathvec[i]->get_linkdata();
			xui_treenode*   node = lineview->add_upmostnode(index, new onity_pathdata(data->get_full(), data->get_prop(), pathvec[i]));
			node->set_data(pathvec[i]);
		}

		const std::vector<xui_proproot*>& propvec = rootdata->get_leaf();
		for (u32 i = 0; i < propvec.size(); ++i,++index)
		{
			onity_propfile* prop = (onity_propfile*)propvec[i];
			xui_treenode*   node = lineview->add_upmostnode(index, new onity_filedata(prop->get_full(), prop));

			onity_prop2dsres* prop2dsres = dynamic_cast<onity_prop2dsres*>(prop);
			if (prop2dsres)
			{
				std::vector<xui_proproot*> subprop = prop2dsres->get_subprop();
				for (u32 isub = 0; isub < subprop.size(); ++isub)
				{
					node->add_leafnode(isub, new onity_2dsassetdata(subprop[isub]));
				}
			}
		}

		lineview->refresh();
		tileview->get_drawview()->refresh();
	}
}
xui_method_explain(onity_project, refresh_pathpane,			void)( void )
{
	m_pathpane->del_children();

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
//xui_method_explain(onity_project, refresh_pathmeta, void)( u08 type, const std::string& lastpath, const std::string& currpath )
//{
//	NP2DSAssetFileMgr* file_mgr = NULL;
//	switch (type)
//	{
//	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
//	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
//	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
//	}
//
//	for (npu32 i = 0; i < file_mgr->GetFileCount(); ++i)
//	{
//		std::string temppath = file_mgr->GetFilePH(i);
//		int index = temppath.find(lastpath);
//		if (index != -1)
//		{
//			temppath.replace(index, lastpath.length(), currpath);
//			file_mgr->SetFilePH(i, temppath);
//			onity_pathdata::save_meta(type, i);
//		}
//	}
//}
//xui_method_explain(onity_project, refresh_filemeta, void)( u08 type, const std::string& lastfull, const std::string& currfull )
//{
//	NP2DSAssetFileMgr* file_mgr = NULL;
//	switch (type)
//	{
//	case META_MODULE:  file_mgr = NP2DSImageFileMgr::GetIns(); break;
//	case META_SPRITE:  file_mgr = NP2DSFrameFileMgr::GetIns(); break;
//	case META_ACTION:  file_mgr = NP2DSActorFileMgr::GetIns(); break;
//	}
//
//	std::string pathname = NPFileNameHelper::PathName(lastfull);
//	std::string filename = NPFileNameHelper::FileName(lastfull);
//	npu32 id = file_mgr->GetFileID(pathname, filename);
//	if (id != -1)
//	{
//		file_mgr->SetFileFN(id, NPFileNameHelper::FileName(currfull));
//		onity_pathdata::save_meta(type, id);
//	}
//}