#include "xui_desktop.h"
#include "xui_textbox.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_treedata.h"
#include "xui_treeview.h"
#include "xui_treenode.h"

xui_implement_rtti(xui_treenode, xui_control);

/*
//constructor
*/
xui_create_explain(xui_treenode)( xui_treedata* linkdata, xui_treeview* treeview )
: xui_control(xui_vector<s32>(0))
{
	m_parent	= treeview;
	m_drawcolor = true;
	m_selected	= false;
	m_rootnode	= NULL;
	m_holdtime  = -1;
	m_linkdata	= linkdata;
	m_linkdata->set_node(this);

	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		xui_drawer* drawer = NULL;
		if (columninfo.type == TREECOLUMN_BOOL)
		{
			if (columninfo.boolmode == TOGGLE_CIRCLE)	drawer = xui_toggle::circle();
			else										drawer = xui_toggle::create();
			xui_method_ptrcall(drawer, set_parent		)(this);
		}
		else
		{
			drawer = new xui_drawer(xui_vector<s32>(0));
			xui_method_ptrcall(drawer, set_parent		)(this);
			xui_method_ptrcall(drawer, set_borderrt		)(columninfo.borderrt);
			xui_method_ptrcall(drawer, set_textfont		)(columninfo.textfont);
			xui_method_ptrcall(drawer, set_textdraw		)(columninfo.textdraw);
			xui_method_ptrcall(drawer, set_iconsize		)(columninfo.type == TREECOLUMN_MAIN ? xui_vector<s32>(16)   : xui_vector<s32>(0));
			xui_method_ptrcall(drawer, set_iconalign	)(columninfo.type == TREECOLUMN_MAIN ? IMAGE_FRONT_TEXT		 : columninfo.iconalign);
			xui_method_ptrcall(drawer, set_textalign	)(columninfo.type == TREECOLUMN_MAIN ? TA_LC				 : columninfo.textalign);
			xui_method_ptrcall(drawer, set_textoffset	)(columninfo.type == TREECOLUMN_MAIN ? xui_vector<s32>(2, 0) : xui_vector<s32>(0));
		}
		m_widgetvec.push_back(drawer);
	}

	m_edittext = new xui_textbox(xui_vector<s32>(0));
	xui_method_ptrcall(m_edittext, ini_component)(true, false);
	xui_method_ptrcall(m_edittext, set_parent	)(this);
	xui_method_ptrcall(m_edittext, set_backcolor)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_edittext, set_drawcolor)(true);
	xui_method_ptrcall(m_edittext, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_edittext, set_sidecolor)(xui_colour::black);
	m_edittext->xm_nonfocus  += new xui_method_member<xui_method_args,  xui_treenode>(this, &xui_treenode::on_textnonfocus);
	m_edittext->xm_keybddown += new xui_method_member<xui_method_keybd, xui_treenode>(this, &xui_treenode::on_textkeybddown);
	m_widgetvec.push_back(m_edittext);

	m_treeplus = new xui_plusctrl(treeview->get_plusrender(), false);
	xui_method_ptrcall(m_treeplus, set_parent	)(this);
	m_treeplus->xm_expand	 += new xui_method_member<xui_method_args,	xui_treenode>(this, &xui_treenode::on_nodeexpand);
	m_widgetvec.push_back(m_treeplus);

	use_linkdata();
}

/*
//static compare
*/
xui_method_explain(xui_treenode, compare,			bool								)( xui_treenode* node1, xui_treenode* node2 )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, node1->get_parent());
	u32 index = treeview->get_sortcolumn();
	std::wstring text1 = (node1->get_linkdata() == NULL) ? L"" : node1->get_linkdata()->get_text(index);
	std::wstring text2 = (node2->get_linkdata() == NULL) ? L"" : node2->get_linkdata()->get_text(index);
	if (treeview->get_columninfo(index).sort == SORTTYPE_NUMBER)
	{
		std::wstringstream t1; t1 << text1.c_str();
		std::wstringstream t2; t2 << text2.c_str();
		f32 numb1; t1 >> numb1;
		f32 numb2; t2 >> numb2;
		if (treeview->get_columnsort() == TREESORT_INCREMENT)
			return numb1 < numb2;
		else
			return numb2 < numb1;
	}
	else
	{
		if (treeview->get_columnsort() == TREESORT_INCREMENT)
			return text1 < text2;
		else
			return text2 < text1;
	}
}

/*
//selected
*/
xui_method_explain(xui_treenode, was_selected,		bool								)( void ) const
{
	return m_selected;
}
xui_method_explain(xui_treenode, set_selected,		void								)( bool flag )
{
	if (m_selected != flag)
	{
		m_selected  = flag;
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		m_treeplus->set_onlyside(treeview->was_lighttrace() && m_selected);
		use_linkdata();
	}
}

xui_method_explain(xui_treenode, was_expanded,		bool								)( void ) const
{
	return m_treeplus->was_expanded();
}
xui_method_explain(xui_treenode, set_expanded,		void								)( bool flag, bool recursion )
{
	m_treeplus->set_expanded(flag);
	if (recursion)
	{
		xui_vecptr_addloop(m_leafnode)
		{
			m_leafnode[i]->set_expanded(flag, recursion);
		}
	}
}

/*
//node
*/
xui_method_explain(xui_treenode, get_currnodedepth,	s32									)( void )
{
	s32 depth = 0;
	xui_treenode* node = m_rootnode;
	while (node)
	{
		++depth;
		node = node->m_rootnode;
	}

	return depth;
}
xui_method_explain(xui_treenode, get_rootnode,		xui_treenode*						)( void )
{
	return m_rootnode;
}
xui_method_explain(xui_treenode, get_leafnodetotal, void								)( std::vector<xui_treenode*>& nodes, bool total )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	nodes.push_back(this);
	if (m_treeplus->was_expanded() || total || treeview->get_searchtext().length() > 0)
	{
		xui_vecptr_addloop(m_leafnode)
		{
			m_leafnode[i]->get_leafnodetotal(nodes, total);
		}
	}
}
xui_method_explain(xui_treenode, get_leafnodecount, u32									)( void ) const
{
	return m_leafnode.size();
}
xui_method_explain(xui_treenode, get_leafnodearray, const std::vector<xui_treenode*>&	)( void ) const
{
	return m_leafnode;
}
xui_method_explain(xui_treenode, get_leafnode,		xui_treenode*						)( u32 index )
{
	return m_leafnode[index];
}
xui_method_explain(xui_treenode, add_leafnode,		xui_treenode*						)( u32 index, xui_treedata* data )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	xui_treenode* node = treeview->create_node(data);
	node->m_rootnode = this;
	m_leafnode.insert(m_leafnode.begin()+index, node);

	m_treeplus->set_visible(true);
	return node;
}
xui_method_explain(xui_treenode, del_leafnode,		void								)( xui_treenode* node )
{
	std::vector<xui_treenode*>::iterator itor = std::find(
		m_leafnode.begin(),
		m_leafnode.end(),
		node);

	if (itor != m_leafnode.end())
	{
		m_leafnode.erase(itor);
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		treeview->delete_node(node);
	}

	m_treeplus->set_visible(m_leafnode.size() > 0);
}

/*
//data
*/
xui_method_explain(xui_treenode, get_linkdata,		xui_treedata*						)( void )
{
	return m_linkdata;
}
xui_method_explain(xui_treenode, use_linkdata,		void								)( void )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		if (columninfo.type == TREECOLUMN_BOOL)
		{
			xui_toggle* toggle = xui_dynamic_cast(xui_toggle, m_widgetvec[i]);
			xui_method_ptrcall(toggle, set_push		)(m_linkdata->get_flag(i));
		}
		else
		{
			xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[i]);
			xui_bitmap*			icon		= m_linkdata->get_icon(i);
			std::wstring		text		= m_linkdata->get_text(i);
			xui_family			textfont	= m_linkdata->get_textfont(i);
			xui_family_render	textdraw	= m_linkdata->get_textdraw(i);
			xui_vector<s32>		iconsize	= (icon == NULL) ? xui_vector<s32>(0) : icon->get_size();
			drawer->ini_drawer(icon, iconsize);
			drawer->ini_drawer(text, textfont, textdraw);
		}
	}
}

/*
//hold
*/
xui_method_explain(xui_treenode, ini_holdtime,		void								)( void )
{
	m_holdtime = 0;
}

/*
//override
*/
xui_method_explain(xui_treenode, get_rendercolor,	xui_colour							)( void ) const
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	if (treeview->was_lighttrace() && m_selected)
	{
		return xui_treeview::nodeselect_color;
	}

	return m_backcolor;
}
xui_method_explain(xui_treenode, choose,			xui_component*						)( const xui_vector<s32>& pt )
{
	if (m_enable && m_visible && m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();
		xui_vecptr_addloop(m_widgetvec)
		{
			if (xui_equal_kindof(xui_drawer, m_widgetvec[i]))
				continue;

			xui_component* component = NULL;
			if (component = m_widgetvec[i]->choose(relative))
				return component;
		}
	}

	return NULL;
}

/*
//callback
*/
xui_method_explain(xui_treenode, on_keybddown,		void								)( xui_method_keybd&    args )
{
	xui_control::on_keybddown(args);
	if (args.kcode == KEY_ENTER)
	{
		set_expanded(!was_expanded());
	}
	if (args.kcode == KEY_F2)
	{
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
		{
			if (treeview->get_columninfo(i).type == TREECOLUMN_MAIN &&
				treeview->get_columninfo(i).editable)
			{
				set_edittext(i);
				break;
			}
		}
	}
	if (args.kcode == KEY_UARROW ||
		args.kcode == KEY_DARROW)
	{
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		std::vector<u32> selectedindex;
		std::vector<xui_treenode*> nodes = treeview->get_entirenode(false);
		for (u32 i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i]->was_selected())
				selectedindex.push_back(i);
		}
		if (selectedindex.empty())
			return;

		u32 index;
		if (args.kcode == KEY_UARROW) index = selectedindex.front()-1;
		if (args.kcode == KEY_DARROW) index = selectedindex.back ()+1;
		if (index >= nodes.size())
			return;

		xui_treenode* node = nodes[index];
		if (args.shift == false || 
			treeview->was_allowmulti() == false)
		{
			node->req_focus();
			treeview->non_selectednode(false);
			treeview->set_nodevisible(node);
		}

		treeview->set_selectednode(node, true);
	}
}
xui_method_explain(xui_treenode, on_perform,		void								)( xui_method_args&		args )
{
	xui_control::on_perform(args);

	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	s32 x = 0;
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		if (columninfo.type == TREECOLUMN_BOOL)
		{
			m_widgetvec[i]->on_perform_x(x+columninfo.size/2-m_widgetvec[i]->get_renderw()/2);
			m_widgetvec[i]->on_perform_y(treeview->get_lineheight()/2-m_widgetvec[i]->get_renderh()/2);
		}
		else
		{
			m_widgetvec[i]->on_perform_x(x);
			m_widgetvec[i]->on_perform_w(columninfo.size);
			m_widgetvec[i]->on_perform_h(treeview->get_lineheight());
		}
		x += columninfo.size;

		if (columninfo.type == TREECOLUMN_MAIN)
		{
			xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[i]);

			s32 indent = get_currnodedepth()*treeview->get_nodeindent();
			s32 offset = (treeview->get_lineheight()-m_treeplus->get_renderh()) / 2;
			xui_vector<s32> pluspt = drawer->get_renderpt() + xui_vector<s32>(indent+drawer->get_borderrt().ax, offset);
			m_treeplus->on_perform_pt(pluspt);

			drawer->set_iconoffset(xui_vector<s32>(indent+m_treeplus->get_renderw(), 0));
		}
	}
}
xui_method_explain(xui_treenode, on_updateself,		void								)( xui_method_args&		args )
{
	xui_control::on_updateself(args);
	if (has_focus() && m_holdtime >= 0)
	{
		++m_holdtime;
		xui_vector<s32> down = xui_desktop::get_ins()->get_mousedown();
		xui_vector<s32> curr = xui_desktop::get_ins()->get_mousecurr();
		if ((xui_abs(curr.x-down.x) > 8) ||
			(xui_abs(curr.y-down.y) > 8))
			m_holdtime = -1;
		if (m_holdtime > 60)
		{
			s32 x = down.x - get_screenpt().x;
			s32 w = 0;

			xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
			for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
			{
				if (x >= w && x < w + treeview->get_columninfo(i).size && treeview->get_columninfo(i).editable)
				{
					set_edittext(i);
					break;
				}

				w += treeview->get_columninfo(i).size;
			}
			m_holdtime = -1;
		}
	}
}

/*
//method
*/
xui_method_explain(xui_treenode, set_edittext,		void								)( u32 index )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[index]);

	xui_rect2d<s32> rt = drawer->get_renderrt()+drawer->get_renderpt();
	rt.ax += drawer->get_iconoffset().x;
	rt.ax += drawer->get_iconsize().w;
	rt.ax += drawer->get_textoffset().x;

	xui_method_ptrcall(m_edittext, set_visible		)(true);
	xui_method_ptrcall(m_edittext, set_renderpt		)(rt.get_pt());
	xui_method_ptrcall(m_edittext, set_rendersz		)(rt.get_sz());
	xui_method_ptrcall(m_edittext, set_borderrt		)(drawer->get_borderrt());
	xui_method_ptrcall(m_edittext, set_text			)(drawer->get_text());
	xui_method_ptrcall(m_edittext, set_textfont		)(drawer->get_textfont());
	xui_method_ptrcall(m_edittext, set_textdraw		)(drawer->get_textdraw());
	xui_method_ptrcall(m_edittext, set_textalign	)(drawer->get_textalign());
	xui_method_ptrcall(m_edittext, set_caretindex	)(0, true);
	xui_method_ptrcall(m_edittext, set_selecttext	)(0, drawer->get_text().length());
	xui_method_ptrcall(m_edittext, set_data			)((void*)index);
	xui_method_ptrcall(m_edittext, req_focus		)();

	drawer->set_text(L"");
}
xui_method_explain(xui_treenode, set_linktext,		void								)( void )
{
	u32 index = (u32)m_edittext->get_data();
	m_edittext->set_visible(false);
	m_linkdata->set_text(index, m_edittext->get_text());
	use_linkdata();
}
xui_method_explain(xui_treenode, set_leafsort,		void								)( void )
{
	if (m_leafback.empty())
		m_leafback = m_leafnode;

	std::sort(
		m_leafnode.begin(),
		m_leafnode.end(),
		xui_treenode::compare);

	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		m_leafnode[i]->set_leafsort();
	}
}
xui_method_explain(xui_treenode, non_leafsort,		void								)( void )
{
	m_leafnode = m_leafback;
	m_leafback.clear();

	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		m_leafnode[i]->non_leafsort();
	}
}
xui_method_explain(xui_treenode, has_findtext,		bool								)( u32 index, const std::wstring& text )
{
	if (m_leafback.empty())
		m_leafback = m_leafnode;

	m_leafnode.clear();
	m_treeplus->set_visible(false);
	if (m_leafback.empty())
	{
		return (m_linkdata && m_linkdata->get_text(index).find(text) != -1);
	}
	else
	{
		for (u32 i = 0; i < m_leafback.size(); ++i)
		{
			if (m_leafback[i]->has_findtext(index, text))
				m_leafnode.push_back(m_leafback[i]);
		}

		return (m_leafnode.size() > 0);
	}
}
xui_method_explain(xui_treenode, non_findtext,		void								)( void )
{
	m_leafnode = m_leafback;
	m_leafback.clear();
	m_treeplus->set_visible(m_leafnode.size() > 0);

	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		m_leafnode[i]->non_findtext();
	}
}

/*
//event
*/
xui_method_explain(xui_treenode, on_textnonfocus,	void								)( xui_component* sender, xui_method_args&  args )
{
	set_linktext();
}
xui_method_explain(xui_treenode, on_textkeybddown,	void								)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_ENTER)
	{
		set_linktext();
	}
}
xui_method_explain(xui_treenode, on_nodeexpand,		void								)( xui_component* sender, xui_method_args&  args )
{
	m_parent->invalid();
}