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
#include "onity_project.h"

xui_implement_rtti(onity_project, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_project)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_T|AREALIMIT_B, 200, DOCKSTYLE_B)
{
	ini_namectrl(xui_bitmap::create("icon/project.png"), L"Project");

	xui_menu*  menu = xui_menu::create(80);
	m_folder = menu->add_item(NULL, L"Folder");

	m_create = new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_create,	ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_create,	set_corner			)(3);
	xui_method_ptrcall(m_create,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_create,	set_drawcolor		)(true);
	xui_method_ptrcall(m_create,	set_textalign		)(TA_LC);
	xui_method_ptrcall(m_create,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_create,	ini_drawer			)(L"Create");
	xui_method_ptrcall(m_create,	set_menu			)(menu);

	m_search = new xui_textbox(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_search,	xm_textchanged		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_searchtextchanged);
	xui_method_ptrcall(m_search,	xm_textenter		) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_search,	ini_drawer			)(xui_bitmap::create("icon/search.png"));
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TA_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);
	xui_method_ptrcall(m_search,	set_hintdraw		)(xui_family_render(xui_colour::gray));

	m_clear = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_clear,		xm_click			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_clearclick);
	xui_method_ptrcall(m_clear,		ini_component		)(true, false);
	xui_method_ptrcall(m_clear,		ini_component		)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_clear,		set_corner			)(8);
	xui_method_ptrcall(m_clear,		set_drawcolor		)(true);
	xui_method_ptrcall(m_clear,		ini_drawer			)(xui_bitmap::create("icon/clear.png"));

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
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0));
	m_pathview  = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_pathview,	xm_selectedchange	) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_treeviewselection);
	xui_method_ptrcall(m_pathview,	ini_component		)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_pathview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_pathview,	set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_pathview,	set_hscrollauto		)(false);

	m_lineview = new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_lineview,	xm_selectedchange	) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_treeviewselection);
	xui_method_ptrcall(m_lineview,	xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse, onity_project>(this, &onity_project::on_lineviewdclick);
	xui_method_ptrcall(m_lineview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_lineview,	set_drawcolor		)(false);
	xui_method_ptrcall(m_lineview,	set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_lineview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_lineview,	set_hscrollauto		)(false);

	m_pathpane = new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_pathpane,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_pathpane,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_pathpane,	set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_pathpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_pathpane,	set_hscrollauto		)(false);

	m_toolpane = new xui_panel(xui_vector<s32>(20));
	xui_method_ptrcall(m_toolpane,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_toolpane,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_toolpane,	set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_toolpane,	set_drawcolor		)(false);
	xui_method_ptrcall(m_toolpane,	set_hscrollauto		)(false);

	m_sizectrl = new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse, onity_project>(this, &onity_project::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_WE);

	m_fill = new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_fill,		xm_perform			) += new xui_method_member<xui_method_args,  onity_project>(this, &onity_project::on_fillperform);
	xui_method_ptrcall(m_fill,		ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fill,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_fill,		set_sidecolor		)(xui_colour::black);
	xui_method_ptrcall(m_fill,		set_drawcolor		)(false);
	xui_method_ptrcall(m_fill,		set_hscrollauto		)(false);
	xui_method_ptrcall(m_fill,		add_child			)(m_pathview);
	xui_method_ptrcall(m_fill,		add_child			)(m_pathpane);
	xui_method_ptrcall(m_fill,		add_child			)(m_toolpane);
	xui_method_ptrcall(m_fill,		add_child			)(m_lineview);
	xui_method_ptrcall(m_fill,		add_child			)(m_sizectrl);
	add_pagectrl(m_head);
	add_pagectrl(m_fill);
}

/*
//callback
*/
//DEBUG
class onity_line_data : public xui_treedata
{
public:
	/*
	//constructor
	*/
	onity_line_data( const std::wstring& full )
	: xui_treedata(full, NULL)
	{}

	virtual xui_bitmap*	 get_icon( u32 index )
	{
		int npos = m_text.find_last_of(L'.');
		if (npos != -1)
			return xui_bitmap::create("icon/file.png");
		else
			return xui_bitmap::create("icon/folder.png");
	}
	virtual std::wstring get_text( u32 index )
	{
		std::wstring temp = m_text;

		int npos  = temp.find_last_of(L'/');
		if (npos == -1)
			npos  = temp.find_last_of(L'\\');
		if (npos != -1)
			temp.erase(0, npos+1);

			npos  = temp.find_last_of(L'.');
		if (npos != -1)
			temp.erase(npos);

		return temp;
	}
};
class onity_path_data : public xui_treedata
{
public:
	/*
	//constructor
	*/
	onity_path_data( const std::wstring& full )
	: xui_treedata(full, xui_bitmap::create("icon/folder.png"))
	{}

	const std::wstring&	 get_full( void ) const
	{
		return m_text;
	}
	virtual std::wstring get_text( u32 index )
	{
		std::wstring temp = m_text;

		int npos  = temp.find_last_of(L'/');
		if (npos == -1)
			npos  = temp.find_last_of(L'\\');
		if (npos != -1)
			temp.erase(0, npos+1);

		return temp;
	}

	static void create_subnode( xui_treenode* root )
	{
		onity_path_data* data = (onity_path_data*)root->get_linkdata();
		u32 index = 0;
		std::vector<std::wstring> pathvec = xui_global::get_path(data->get_full());
		for (u32 i = 0; i < pathvec.size(); ++i,++index)
		{
			std::wstring  full = data->get_full()+L"/"+pathvec[i];
			xui_treenode* node = root->add_leafnode(index, new onity_path_data(full));
			onity_path_data::create_subnode(node);
		}
	}
};

xui_method_explain(onity_project, on_load,					void)( xui_method_args& args )
{
	xui_dockpage::on_load(args);
	 
	std::wstring workpath = L"D:/BreezeGame/art/res";//xui_global::get_workpath();
	u32 index = 0;
	std::vector<std::wstring> pathvec = xui_global::get_path(workpath);
	for (u32 i = 0; i < pathvec.size(); ++i,++index)
	{
		std::wstring  full = workpath+L"/"+pathvec[i];
		xui_treenode* node = m_pathview->add_upmostnode(index, new onity_path_data(full));
		onity_path_data::create_subnode(node);
	}
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
xui_method_explain(onity_project, on_treeviewselection,		void)( xui_component* sender, xui_method_args&  args )
{
	if (sender == m_pathview)
	{
		m_lineview->del_upmostnodeall();
		std::vector<xui_treenode*> nodevec = m_pathview->get_selectednode();
		if (nodevec.size() > 0)
		{
			u32 index = 0;
			xui_treenode* root = nodevec.front();
			std::vector<xui_treenode*> pathvec = root->get_leafnodearray();
			for (u32 i = 0; i < pathvec.size(); ++i,++index)
			{
				onity_path_data* data = (onity_path_data*)pathvec[i]->get_linkdata();
				xui_treenode*    node = m_lineview->add_upmostnode(index, new onity_line_data(data->get_full()));
				node->set_data(pathvec[i]);
			}

			onity_path_data* rootdata = (onity_path_data*)root->get_linkdata();
			std::vector<std::wstring> filevec = xui_global::get_file(rootdata->get_full());
			for (u32 i = 0; i < filevec.size(); ++i,++index)
			{
				m_lineview->add_upmostnode(index, new onity_line_data(rootdata->get_full()+filevec[i]));
			}
		}
		m_lineview->refresh();
	}
}
xui_method_explain(onity_project, on_lineviewdclick,		void)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_treenode* node = m_lineview->choose_node(m_lineview->get_renderpt(args.point));
		if (node && node->get_data())
		{
			xui_treenode* pathnode = (xui_treenode*)node->get_data();
			xui_method_ptrcall(m_pathview, set_selectednode	)(pathnode, true);
			xui_method_ptrcall(m_pathview, set_nodevisible	)(pathnode);
		}
	}
}