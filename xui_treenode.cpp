#include "xui_desktop.h"
#include "xui_textbox.h"
#include "xui_toggle.h"
#include "xui_treedata.h"
#include "xui_treeview.h"
#include "xui_treenode.h"

xui_implement_rtti(xui_treenode, xui_control);

/*
//constructor
*/
xui_create_explain(xui_treenode)( xui_treedata* linkdata, xui_component* parent )
: xui_control(xui_vector<s32>(0), parent)
{
	m_selected	= false;
	m_rootnode	= NULL;
	m_holdtime  = -1;
	m_linkdata	= linkdata;
	m_linkdata->set_node(this);

	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, parent);
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		xui_drawer* drawer = (columninfo.type == TREECOLUMN_BOOL) 
			? new xui_toggle(xui_vector<s32>(0), TOGGLE_CIRCLE, this) 
			: new xui_drawer(xui_vector<s32>(0), this);

		xui_method_ptrcall(drawer, set_borderrt	)(columninfo.borderrt);
		xui_method_ptrcall(drawer, set_textfont	)(columninfo.textfont);
		xui_method_ptrcall(drawer, set_textdraw	)(columninfo.textdraw);
		xui_method_ptrcall(drawer, set_iconalign)(columninfo.type == TREECOLUMN_MAIN ? IMAGE_FRONT_TEXT : columninfo.iconalign);
		xui_method_ptrcall(drawer, set_textalign)(columninfo.type == TREECOLUMN_MAIN ? TA_LC            : columninfo.textalign);
		m_widgetvec.push_back(drawer);
	}

	m_edittext = new xui_textbox(xui_vector<s32>(0), this);
	xui_method_ptrcall(m_edittext, ini_component)(true, false);
	xui_method_ptrcall(m_edittext, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_edittext, set_sidecolor)(xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
	m_edittext->xm_nonfocus  += new xui_method_member<xui_method_args,  xui_treenode>(this, &xui_treenode::on_textnonfocus);
	m_edittext->xm_keybddown += new xui_method_member<xui_method_keybd, xui_treenode>(this, &xui_treenode::on_textkeybddown);
	m_widgetvec.push_back(m_edittext);

	m_treeplus = new xui_plusctrl(treeview->get_plusrender(), this);
	m_treeplus->xm_expand	+= new xui_method_member<xui_method_args,	xui_treenode>(this, &xui_treenode::on_nodeexpand);
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
	m_selected = flag;
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
			xui_method_ptrcall(drawer, set_text		)(m_linkdata->get_text(i));
			xui_method_ptrcall(drawer, set_icon		)(m_linkdata->get_icon(i));
			xui_method_ptrcall(drawer, set_textfont	)(m_linkdata->get_textfont(i));
			xui_method_ptrcall(drawer, set_textdraw )(m_linkdata->get_textdraw(i));
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
	xui_vector<s32> pt(0);
	xui_vector<s32> sz(0, treeview->get_lineheight());
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		sz.w  = columninfo.size;
		m_widgetvec[i]->on_perform_pt(pt);
		m_widgetvec[i]->on_perform_sz(sz);
		pt.x += columninfo.size;

		if (columninfo.type == TREECOLUMN_MAIN)
		{
			xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[i]);

			s32 indent = get_currnodedepth()*treeview->get_nodeindent();
			s32 offset = (treeview->get_lineheight()-m_treeplus->get_renderh()) / 2;
			xui_vector<s32> pluspt = drawer->get_renderpt() + xui_vector<s32>(offset+indent, offset);
			m_treeplus->on_perform_pt(pluspt);

			drawer->set_iconoffset(xui_vector<s32>(offset+indent+m_treeplus->get_renderw(), 0));
		}
	}
}
xui_method_explain(xui_treenode, on_topdraw,		void								)( xui_method_args&		args )
{
	//TODO
	//xui_control::on_topdraw(args);
	//xui_component* hoverctrl = g_desktop->get_hoverctrl();
	//if (hoverctrl != this && hoverctrl)
	//{
	//	if (hoverctrl->get_parent() == m_parent && xui_issub_kindof(xui_treenode, hoverctrl))
	//	{
	//		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	//		xui_vector<s32> pt = g_desktop->get_mousecurr() - hoverctrl->get_screenpt();
	//		xui_rect2d<s32> rt = hoverctrl->get_renderrtabs().get_inter(treeview->get_renderrtins()+treeview->get_screenpt());
	//		xui_vector<s32> p1;
	//		xui_vector<s32> p2;
	//		if (pt.y < get_renderh()/3)
	//		{
	//			p1 = xui_vector<s32>(rt.ax, rt.ay);
	//			p2 = xui_vector<s32>(rt.bx, rt.ay);
	//		}
	//		if (pt.y > get_renderh()*2/3)
	//		{
	//			p1 = xui_vector<s32>(rt.ax, rt.by-2);
	//			p2 = xui_vector<s32>(rt.bx, rt.by-2);
	//		}

	//		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(p1.x, p1.y, p2.x, p1.y+2), xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
	//		xui_vector<s32> path[3];
	//		path[0] = xui_vector<s32>(p1.x,   p1.y-4);
	//		path[1] = xui_vector<s32>(p1.x+5, p1.y  );
	//		path[2] = xui_vector<s32>(p1.x,   p1.y+6);
	//		xui_convas::get_ins()->fill_poly(path, 3, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
	//		path[0] = xui_vector<s32>(p2.x,   p1.y-4);
	//		path[1] = xui_vector<s32>(p2.x-5, p1.y  );
	//		path[2] = xui_vector<s32>(p2.x,   p1.y+6);
	//		xui_convas::get_ins()->fill_poly(path, 3, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
	//	}
	//}
}
xui_method_explain(xui_treenode, on_updateself,		void								)( xui_method_args&		args )
{
	xui_control::on_updateself(args);
	if (has_focus() && m_holdtime >= 0)
	{
		++m_holdtime;
		xui_vector<s32> down = g_desktop->get_mousedown();
		xui_vector<s32> curr = g_desktop->get_mousecurr();
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
	drawer->set_visible(false);

	xui_rect2d<s32> rt = drawer->get_renderrt()+drawer->get_renderpt();
	rt.ax += drawer->get_iconoffset().x;
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