#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "cocos_recent.h"

xui_implement_rtti(cocos_recent, xui_window);

/*
//constructor
*/
xui_create_explain(cocos_recent)( void )
: xui_window(xui_vector<s32>(480, 640), true)
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_drawer	= new xui_drawer(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_drawer, ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_drawer, set_text			)(L"Select recently path");

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, true));
	m_recent	= new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_NORMAL, false, false);
	xui_method_ptrcall(m_recent, xm_mousedoubleclick) += new xui_method_member<xui_method_mouse, cocos_recent>(this, &cocos_recent::on_recentdoubleclick);
	xui_method_ptrcall(m_recent, ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_recent, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_recent, set_drawcolor		)(true);
	xui_method_ptrcall(m_recent, set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_recent, set_hscrollauto	)(false);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept, xm_buttonclick		) += new xui_method_member<xui_method_args, cocos_recent>(this, &cocos_recent::on_buttonclick);
	xui_method_ptrcall(m_accept, ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_accept, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept, set_corner			)(3);
	xui_method_ptrcall(m_accept, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept, set_drawcolor		)(true);
	xui_method_ptrcall(m_accept, set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept, set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept, ini_drawer			)(L"Open");

	m_browse	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_browse, xm_buttonclick		) += new xui_method_member<xui_method_args, cocos_recent>(this, &cocos_recent::on_buttonclick);
	xui_method_ptrcall(m_browse, ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_browse, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_browse, set_corner			)(3);
	xui_method_ptrcall(m_browse, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_browse, set_drawcolor		)(true);
	xui_method_ptrcall(m_browse, set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_browse, set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_browse, ini_drawer			)(L"Open Other ...");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 32));
	xui_method_ptrcall(m_bottom, ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom, set_borderrt		)(xui_rect2d<s32>(0, 8, 0, 0));
	xui_method_ptrcall(m_bottom, add_child			)(m_accept);
	xui_method_ptrcall(m_bottom, add_child			)(m_browse);

	add_child(m_drawer);
	add_child(m_bottom);
	add_child(m_recent);

	load_config();
}

/*
//method
*/
xui_method_explain(cocos_recent, get_selectpath,		std::wstring)( void )
{
	std::wstring path;

	std::vector<xui_treenode*> vec = m_recent->get_selectednode();
	if (vec.size() > 0)
	{
		xui_treenode* selectednode = vec.front();
		xui_treedata* selecteddata = selectednode->get_linkdata();
		path = selecteddata->get_text(0);
	}

	return path;
}

/*
//callback
*/
xui_method_explain(cocos_recent, on_accept,				void		)( xui_component* sender, xui_method_args&  args )
{
	save_config();
	xui_window::on_accept(sender, args);
}

/*
//event
*/
xui_method_explain(cocos_recent, on_recentdoubleclick,	void		)( xui_component* sender, xui_method_mouse& args )
{
	if (m_recent->get_selectednode().empty())
		return;

	xui_method_args other_args;
	on_accept(this, other_args);
}
xui_method_explain(cocos_recent, on_buttonclick,		void		)( xui_component* sender, xui_method_args&  args )
{
	if (sender == m_browse)
	{
		std::wstring path = xui_global::get_openpath();
		if (path.length() > 0)
		{
			xui_treenode* curr = NULL;
			for (u32 i = 0; i < m_recent->get_upmostnodecount(); ++i)
			{
				xui_treenode* node = m_recent->get_upmostnode(i);
				xui_treedata* data = node->get_linkdata();
				if (data->get_text(0) == path)
				{
					curr = node;
					break;
				}
			}

			if (curr == NULL)
				curr = m_recent->add_upmostnode(0, new xui_treedata(path));

			m_recent->set_selectednode(curr, true);
		}
	}

	if (m_recent->get_selectednode().empty())
		return;

	xui_method_args other_args;
	on_accept(this, other_args);
}

/*
//method
*/
xui_method_explain(cocos_recent, load_config,			void		)( void )
{
	FILE* file = fopen("cocos_editor.recent", "r");
	if (file)
	{
		u32 index = 0;
		while (feof(file) == false)
		{
			std::string line = xui_global::get_fileline(file);
			if (line.length() == 0)
				continue;

			m_recent->add_upmostnode(index, new xui_treedata(xui_global::ascii_to_unicode(line)));
			++index;
		}

		fclose(file);
	}
}
xui_method_explain(cocos_recent, save_config,			void		)( void )
{
	FILE* file = fopen("cocos_editor.recent", "w");
	if (file)
	{
		std::string line = xui_global::unicode_to_ascii(get_selectpath()) + "\n";
		fwrite((void*)line.c_str(), 1, line.length(), file);
		for (u32 i = 0; i < m_recent->get_upmostnodecount(); ++i)
		{
			xui_treenode* node = m_recent->get_upmostnode(i);
			if (node->was_selected())
				continue;

			xui_treedata* data = node->get_linkdata();
			line = xui_global::unicode_to_ascii(data->get_text(0)) + "\n";
			fwrite((void*)line.c_str(), 1, line.length(), file);
		}

		fclose(file);
	}
}