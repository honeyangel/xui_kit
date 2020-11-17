#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_textbox.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_treedata.h"
#include "xui_treeview.h"
#include "xui_treenode.h"

xui_implement_rtti(xui_treenode, xui_control)

xui_treenode::xui_treenode( xui_treedata* linkdata, xui_treeview* treeview )
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
		if (columninfo.type == k_treecolumn_bool)
		{
			xui_toggle* toggle = NULL;
			if (columninfo.boolmode == k_toggle_circle)	toggle = xui_toggle::circle();
			else										toggle = xui_toggle::create();
			toggle->set_parent(this);
			toggle->xm_toggleclick += new xui_method_member<xui_method_args, xui_treenode>(this, &xui_treenode::on_toggleclick);
			drawer = toggle;
		}
		else
		{
			drawer = new xui_drawer(xui_vector<s32>(0));
			drawer->set_parent    (this);
			drawer->set_borderrt  (columninfo.borderrt);
			drawer->set_textfont  (columninfo.textfont);
			drawer->set_textdraw  (columninfo.textdraw);
			drawer->set_iconsize  (columninfo.type == k_treecolumn_main ? xui_vector<s32>(16)   : xui_vector<s32>(0));
			drawer->set_iconalign (columninfo.type == k_treecolumn_main ? k_image_front_text	: columninfo.iconalign);
			drawer->set_textalign (columninfo.type == k_treecolumn_main ? k_textalign_lc	    : columninfo.textalign);
			drawer->set_textoffset(columninfo.type == k_treecolumn_main ? xui_vector<s32>(2, 0) : xui_vector<s32>(0));
		}
		m_widgetvec.push_back(drawer);
	}

	m_edittext = new xui_textbox(xui_vector<s32>(0));
	m_edittext->ini_component(true, false);
	m_edittext->set_parent(this);
	m_edittext->set_backcolor(xui_colour(1.0f, 0.2f));
	m_edittext->set_drawcolor(true);
	m_edittext->set_sidestyle(k_sidestyle_s);
	m_edittext->set_sidecolor(xui_colour::k_black);
	m_edittext->xm_nonfocus  += new xui_method_member<xui_method_args,  xui_treenode>(this, &xui_treenode::on_textnonfocus);
	m_edittext->xm_keybddown += new xui_method_member<xui_method_keybd, xui_treenode>(this, &xui_treenode::on_textkeybddown);
	m_widgetvec.push_back(m_edittext);

	m_treeplus = new xui_plusctrl(treeview->get_plusrender(), false);
	m_treeplus->set_parent(this);
	m_treeplus->xm_expand	 += new xui_method_member<xui_method_args,	xui_treenode>(this, &xui_treenode::on_nodeexpand);
	m_widgetvec.push_back(m_treeplus);

	use_linkdata();
}

xui_treenode::~xui_treenode( void )
{
	delete m_linkdata;
}

bool xui_treenode::compare( xui_treenode* node1, xui_treenode* node2 )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, node1->get_parent());
	u32 index = treeview->get_sortcolumn();
	std::wstring text1 = (node1->get_linkdata() == NULL) ? L"" : node1->get_linkdata()->get_text(index);
	std::wstring text2 = (node2->get_linkdata() == NULL) ? L"" : node2->get_linkdata()->get_text(index);
	if (treeview->get_columninfo(index).sort == k_sorttype_number)
	{
		std::wstringstream t1; t1 << text1.c_str();
		std::wstringstream t2; t2 << text2.c_str();
		f32 numb1; t1 >> numb1;
		f32 numb2; t2 >> numb2;
		if (treeview->get_columnsort() == k_treesort_increment)
			return numb1 < numb2;
		else
			return numb2 < numb1;
	}
	else
	{
		if (treeview->get_columnsort() == k_treesort_increment)
			return text1 < text2;
		else
			return text2 < text1;
	}
}

bool xui_treenode::was_selected( void ) const
{
	return m_selected;
}

void xui_treenode::set_selected( bool flag )
{
	if (m_selected != flag)
	{
		m_selected  = flag;
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		m_treeplus->set_onlyside(/*treeview->was_lighttrace() && */m_selected);
		use_linkdata();
	}
}

bool xui_treenode::was_expanded( void ) const
{
	return m_treeplus->was_expanded();
}

void xui_treenode::set_expanded( bool flag, bool recursion )
{
	m_treeplus->set_expanded(flag);
	if (recursion)
	{
		for (u32 i = 0; i < m_leafnode.size(); ++i)
		{
			m_leafnode[i]->set_expanded(flag, recursion);
		}
	}
}

s32 xui_treenode::get_currnodedepth( void )
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

xui_treenode* xui_treenode::get_rootnode( void )
{
	return m_rootnode;
}

void xui_treenode::get_leafnodetotal( std::vector<xui_treenode*>& nodes, bool total )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	nodes.push_back(this);
	if (m_treeplus->was_expanded() || total || treeview->get_searchtext().length() > 0)
	{
		if (treeview->get_searchtext().length() > 0 || treeview->get_columnsort() != k_treesort_none)
		{
			for (u32 i = 0; i < m_leafpart.size(); ++i)
				m_leafpart[i]->get_leafnodetotal(nodes, total);
		}
		else
		{
			for (u32 i = 0; i < m_leafnode.size(); ++i)
				m_leafnode[i]->get_leafnodetotal(nodes, total);
		}
	}
}

u32 xui_treenode::get_leafnodecount( void ) const
{
	return m_leafnode.size();
}

const std::vector<xui_treenode*>& xui_treenode::get_leafnodearray( void ) const
{
	return m_leafnode;
}

u32 xui_treenode::get_leafnodeindex( xui_treenode* node ) const
{
	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		if (m_leafnode[i] == node)
			return i;
	}

	return -1;
}

xui_treenode* xui_treenode::get_leafnode( u32 index )
{
	return m_leafnode[index];
}

xui_treenode* xui_treenode::add_leafnode( u32 index, xui_treedata* data )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	xui_treenode* node = treeview->create_node(data);
	node->m_rootnode = this;
	m_leafnode.insert(m_leafnode.begin()+index, node);

	if (treeview->get_searchtext().empty() && treeview->get_columnsort() == k_treesort_none)
		m_treeplus->set_visible(true);

	treeview->invalid();
	return node;
}

void xui_treenode::del_leafnode( xui_treenode* node )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);

	bool exist = false;
	std::vector<xui_treenode*>::iterator itor;
	itor = std::find(m_leafpart.begin(), m_leafpart.end(), node);
	if (itor != m_leafpart.end())
	{
		exist = true;
		m_leafpart.erase(itor);
	}
	itor = std::find(m_leafnode.begin(), m_leafnode.end(), node);
	if (itor != m_leafnode.end())
	{
		exist = true;
		m_leafnode.erase(itor);
	}

	if (exist)
		treeview->delete_node(node);

	if (treeview->get_searchtext().empty() && treeview->get_columnsort() == k_treesort_none && m_leafnode.empty())
		m_treeplus->set_visible(false);

	treeview->invalid();
}

void xui_treenode::del_leafnodeall( void )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		treeview->delete_node(m_leafnode[i]);
	}

	m_leafnode.clear();
	m_leafpart.clear();

	treeview->invalid();
}

xui_treedata* xui_treenode::get_linkdata( void )
{
	return m_linkdata;
}

void xui_treenode::use_linkdata( void )
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		if (columninfo.type == k_treecolumn_bool)
		{
			xui_toggle* toggle = xui_dynamic_cast(xui_toggle, m_widgetvec[i]);
			toggle->ini_toggle(m_linkdata->get_flag(i));
		}
		else
		{
			xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[i]);
			xui_bitmap*			icon		= m_linkdata->get_icon(i);
			std::wstring		text		= m_linkdata->get_text(i);
			xui_family			textfont	= m_linkdata->get_textfont(i);
			xui_family_render	textdraw	= m_linkdata->get_textdraw(i);
			xui_vector<s32>		iconsize	= drawer->get_iconsize();
			drawer->ini_drawer(icon, iconsize);
			drawer->ini_drawer(text, textfont, textdraw);
		}
	}
}

void xui_treenode::ini_holdtime( void )
{
	m_holdtime = 0;
}

xui_colour xui_treenode::get_rendercolor( void ) const
{
	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	if (treeview->was_lighttrace() && m_selected)
	{
		return xui_treeview::k_nodeselect_color;
	}

	return m_backcolor;
}

xui_component* xui_treenode::choose( const xui_vector<s32>& pt )
{
	if (m_enable && m_visible && m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();
		for (u32 i = 0; i < m_widgetvec.size(); ++i)
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

void xui_treenode::on_keybddown( xui_method_keybd& args )
{
	xui_control::on_keybddown(args);
	if (args.kcode == k_key_enter)
	{
		set_expanded(!was_expanded());
	}
	if (args.kcode == k_key_f2)
	{
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
		{
			if (treeview->get_columninfo(i).type == k_treecolumn_main &&
				treeview->get_columninfo(i).editable)
			{
				set_edittext(i);
				break;
			}
		}
	}
	if (args.kcode == k_key_uarrow ||
		args.kcode == k_key_darrow)
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
		if (args.kcode == k_key_uarrow) index = selectedindex.front()-1;
		if (args.kcode == k_key_darrow) index = selectedindex.back ()+1;
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

void xui_treenode::on_perform( xui_method_args&	args )
{
	xui_control::on_perform(args);

	xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
	s32 x = 0;
	for (u32 i = 0; i < treeview->get_columninfocount(); ++i)
	{
		const xui_treecolumn& columninfo = treeview->get_columninfo(i);
		if (columninfo.type == k_treecolumn_bool)
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

		if (columninfo.type == k_treecolumn_main)
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

void xui_treenode::on_updateself( xui_method_update& args )
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

void xui_treenode::set_edittext( u32 index )
{
	xui_drawer* drawer = xui_dynamic_cast(xui_drawer, m_widgetvec[index]);

	xui_rect2d<s32> rt = drawer->get_renderrt()+drawer->get_renderpt();
	rt.ax += drawer->get_iconoffset().x;
	rt.ax += drawer->get_iconsize().w;
	rt.ax += drawer->get_textoffset().x;

	m_edittext->set_visible   (true);
	m_edittext->set_renderpt  (rt.get_pt());
	m_edittext->set_rendersz  (rt.get_sz());
	m_edittext->set_borderrt  (drawer->get_borderrt());
	m_edittext->set_text      (drawer->get_text());
	m_edittext->set_textfont  (drawer->get_textfont());
	m_edittext->set_textdraw  (drawer->get_textdraw());
	m_edittext->set_textalign (drawer->get_textalign());
	m_edittext->set_caretindex(0, true);
	m_edittext->set_selecttext(0, drawer->get_text().length());
	m_edittext->set_data      ((void*)index);
	m_edittext->req_focus     ();

	drawer->set_text(L"");
}

void xui_treenode::set_linktext( void )
{
	u32 index = (u32)(long long)m_edittext->get_data();
	m_edittext->set_visible(false);
	std::wstring text = m_linkdata->get_text(index);
	if (m_edittext->get_text() != text)
	{
		m_linkdata->set_text(index, m_edittext->get_text());
		use_linkdata();
	}
}

void xui_treenode::set_leafsort( void )
{
	if (m_leafpart.empty())
		m_leafpart = m_leafnode;

	std::sort(
		m_leafpart.begin(),
		m_leafpart.end(),
		xui_treenode::compare);

	for (u32 i = 0; i < m_leafpart.size(); ++i)
	{
		m_leafpart[i]->set_leafsort();
	}
}

void xui_treenode::non_leafsort( void )
{
	m_leafpart.clear();

	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		m_leafnode[i]->non_leafsort();
	}
}

bool xui_treenode::has_findtext( u32 index, const std::wstring& text )
{
	m_leafpart.clear();
	m_treeplus->set_visible(false);
	bool result = (m_linkdata && xui_global::get_upper(m_linkdata->get_text(index)).find(xui_global::get_upper(text)) != -1);
	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		if (m_leafnode[i]->has_findtext(index, text))
			m_leafpart.push_back(m_leafnode[i]);
	}

	return result || (m_leafpart.size() > 0);
}

void xui_treenode::non_findtext( void )
{
	m_leafpart.clear();
	m_treeplus->set_visible(m_leafnode.size() > 0);

	for (u32 i = 0; i < m_leafnode.size(); ++i)
	{
		m_leafnode[i]->non_findtext();
	}
}

void xui_treenode::on_textnonfocus( xui_component* sender, xui_method_args& args )
{
	m_edittext->set_visible(false);
	use_linkdata();
}

void xui_treenode::on_textkeybddown( xui_component* sender, xui_method_keybd& args )
{
	switch (args.kcode)
	{
	case k_key_esc:
		m_edittext->set_visible(false);
		use_linkdata();
		break;
	case k_key_enter:
		set_linktext();
		break;
	}
}

void xui_treenode::on_nodeexpand( xui_component* sender, xui_method_args& args )
{
	m_parent->invalid();
}

void xui_treenode::on_toggleclick( xui_component* sender, xui_method_args& args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);
	u32 index = -1;
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i] == toggle)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		if (m_selected)
		{
			xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
			std::vector<xui_treenode*> nodevec = treeview->get_selectednode();
			for (u32 i = 0; i < nodevec.size(); ++i)
			{
				nodevec[i]->get_linkdata()->set_flag(index, toggle->was_push());
				nodevec[i]->use_linkdata();
			}
		}
		else
		{
			m_linkdata->set_flag(index, toggle->was_push());
			use_linkdata();
		}
	}
}
