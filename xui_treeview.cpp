#include "xui_scroll.h"
#include "xui_drawer.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_treegrid.h"
#include "xui_treeview.h"

xui_implement_rtti(xui_treeview, xui_container)

const xui_colour xui_treeview::k_lighttrace_color = xui_colour(0.5f, 127.0f/255.0f);
const xui_colour xui_treeview::k_nodeselect_color = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

xui_treeview* xui_treeview::create( const std::vector<xui_treecolumn>& columninfo )
{
	xui_vector<s32> size(0, 200);
	for (u32 i = 0; i < columninfo.size(); ++i)
		size.w += columninfo[i].maxsize;

	xui_treeview* treeview = new xui_treeview(size, columninfo);
	treeview->set_sidestyle(k_sidestyle_s);
	treeview->set_corner(3);
	treeview->set_borderrt(xui_rect2d<s32>(0, 4, 0, 4));

	return treeview;
}

xui_treeview::xui_treeview( const xui_vector<s32>& size, const std::vector<xui_treecolumn>& columninfo, s32 lineheight, u08 plusrender, bool rendergrid, bool renderhead, bool lighttrace )
: xui_container(size)
{
	m_columninfo = columninfo;
	m_plusrender = plusrender;
	m_rendergrid = rendergrid;
	m_renderhead = renderhead;
	m_lighttrace = lighttrace;
	m_lineheight = lineheight;
	m_nodeindent = 10;
	m_allowmulti = false;
	m_acceptdrag = true;
	m_columnsort = 0;
	m_sortcolumn = 0;
	m_mousecatch = NULL;
	m_mousehover = NULL;
	m_allowplace = 0;

	for (u32 i = 0; i < m_columninfo.size();   ++i)
	{
		xui_drawer* drawer = new xui_drawer(xui_vector<s32>(0));
		drawer->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_treeview>(this, &xui_treeview::on_headclick);
		drawer->xm_renderself += new xui_method_member<xui_method_args,  xui_treeview>(this, &xui_treeview::on_headrenderself);
		drawer->set_parent	 (this);
		drawer->set_borderrt (m_columninfo[i].borderrt);
		drawer->set_icon	 (m_columninfo[i].icon);
		drawer->set_text	 (m_columninfo[i].name);
		drawer->set_textfont (m_columninfo[i].textfont);
		drawer->set_textdraw (m_columninfo[i].textdraw);
		drawer->set_textalign(m_columninfo[i].textalign);
		drawer->set_iconalign(m_columninfo[i].iconalign);
		m_columnhead.push_back(drawer);
	}
	for (u32 i = 0; i < m_columninfo.size()-1; ++i)
	{
		m_columngrid.push_back(new xui_treegrid(i, this));
	}

	refresh();
}

xui_treeview::~xui_treeview( void )
{
	for (u32 i = 0; i < m_columngrid.size(); ++i)
		delete m_columngrid[i];
	for (u32 i = 0; i < m_columnhead.size(); ++i)
		delete m_columnhead[i];
}

s32 xui_treeview::get_lineheight( void ) const
{
	return m_lineheight;
}

void xui_treeview::set_lineheight( s32 height )
{
	if (m_lineheight != height)
	{
		m_lineheight  = height;
		invalid();
	}
}

s32 xui_treeview::get_nodeindent( void ) const
{
	return m_nodeindent;
}

void xui_treeview::set_nodeindent( s32 indent )
{
	if (m_nodeindent != indent)
	{
		m_nodeindent  = indent;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			m_ascrollitem[i]->refresh();
		}
	}
}

bool xui_treeview::was_lighttrace( void ) const
{
	return m_lighttrace;
}

u08 xui_treeview::get_plusrender( void ) const
{
	return m_plusrender;
}

bool xui_treeview::was_allowmulti( void ) const
{
	return m_allowmulti;
}

void xui_treeview::set_allowmulti( bool flag )
{
	m_allowmulti = flag;
}

bool xui_treeview::was_acceptdrag( void ) const
{
	return m_acceptdrag;
}

void xui_treeview::set_acceptdrag( bool flag )
{
	m_acceptdrag = flag;
}

u08 xui_treeview::get_allowplace( void ) const
{
	return m_allowplace;
}

void xui_treeview::set_allowplace( u08 place )
{
	m_allowplace = place;
}

u32 xui_treeview::get_columninfocount( void ) const
{
	return m_columninfo.size();
}

const std::vector<xui_treecolumn>& xui_treeview::get_columninfoarray( void ) const
{
	return m_columninfo;
}

const xui_treecolumn& xui_treeview::get_columninfo( u32 index ) const
{
	return m_columninfo[index];
}

void xui_treeview::set_columnsize( u32 index, s32 size )
{
	size = xui_min(m_columninfo[index].maxsize, size);
	size = xui_max(m_columninfo[index].minsize, size);
	if (m_columninfo[index].size != size)
	{
		m_columninfo[index].size  = size;
		refresh();
	}
}

void xui_treeview::set_columntext( u32 index, const std::wstring& text )
{
	m_columninfo[index].name = text;
	m_columnhead[index]->set_text(text);
}

u08 xui_treeview::get_columnsort( void ) const
{
	return m_columnsort;
}

u32 xui_treeview::get_sortcolumn( void ) const
{
	return m_sortcolumn;
}

void xui_treeview::non_columnsort( void )
{
	if (m_columnsort != k_treesort_none)
	{
		m_columnsort  = k_treesort_none;
		m_upmostpart.clear();

		for (u32 i = 0; i < m_upmostnode.size(); ++i)
		{
			m_upmostnode[i]->non_leafsort();
		}

		invalid();
	}
}

void xui_treeview::set_columnsort( u32 index, u08 sort )
{
	if (m_searchtext.length() > 0 || sort == k_treesort_none)
		return;

	if (index < m_columninfo.size())
	{
		if (m_columninfo[index].type == k_treecolumn_icon ||
			m_columninfo[index].type == k_treecolumn_bool ||
			m_columninfo[index].sort == 0)
			return;
	}

	if (m_sortcolumn != index || m_columnsort != sort)
	{
		m_sortcolumn  = index;
		m_columnsort  = sort;

		if (m_upmostpart.empty())
			m_upmostpart = m_upmostnode;

		std::sort(
			m_upmostpart.begin(),
			m_upmostpart.end(),
			xui_treenode::compare);

		for (u32 i = 0; i < m_upmostpart.size(); ++i)
		{
			m_upmostpart[i]->set_leafsort();
		}

		invalid();
	}
}

const std::wstring& xui_treeview::get_searchtext( void ) const
{
	return m_searchtext;
}

void xui_treeview::set_searchtext( const std::wstring& text )
{
	non_columnsort();

	if (m_searchtext != text)
	{
		m_searchtext  = text;
		if (m_searchtext.length() == 0)
		{
			m_upmostpart.clear();
			for (u32 i = 0; i < m_upmostnode.size(); ++i)
			{
				m_upmostnode[i]->non_findtext();
			}
		}
		else
		{
			u32 mainIndex = 0;
			for (u32 i = 0; i < m_columninfo.size(); ++i)
			{
				if (m_columninfo[i].type == k_treecolumn_main)
				{
					mainIndex = i;
					break;
				}
			}

			m_upmostpart.clear();
			for (u32 i = 0; i < m_upmostnode.size(); ++i)
			{
				if (m_upmostnode[i]->has_findtext(mainIndex, m_searchtext))
					m_upmostpart.push_back(m_upmostnode[i]);
			}
		}

		invalid();
	}
}

void xui_treeview::ini_selectednode( xui_treenode* node, bool selected )
{
	if (node->was_selected() != selected)
	{
		if (selected && m_allowmulti == false)
			non_selectednode(false);

		node->set_selected(selected);
		if (selected)
		{
			m_selectnode.push_back(node);
		}
		else
		{
			std::vector<xui_treenode*>::iterator itor = std::find(
				m_selectnode.begin(),
				m_selectnode.end(),
				node);
			if (itor != m_selectnode.end())
				m_selectnode.erase(itor);
		}
	}
}

void xui_treeview::set_selectednode( xui_treenode* node, bool selected )
{
	if (node->was_selected() != selected)
	{
		if (selected && m_allowmulti == false)
			non_selectednode(false);

		node->set_selected(selected);
		if (selected)
		{
			m_selectnode.push_back(node);
		}
		else
		{
			std::vector<xui_treenode*>::iterator itor = std::find(
				m_selectnode.begin(),
				m_selectnode.end(),
				node);
			if (itor != m_selectnode.end())
				m_selectnode.erase(itor);
		}

		xui_method_args args;
		xm_selectedchange(this, args);
	}
}

void xui_treeview::set_selectednode( const std::vector<xui_treenode*>& nodes )
{
	if (m_allowmulti == false)
		return;

	bool selectedchange = false;
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i]->was_selected() == false)
		{
			selectedchange = true;
			nodes[i]->set_selected(true);
			m_selectnode.push_back(nodes[i]);
		}
	}

	if (selectedchange)
	{
		xui_method_args args;
		xm_selectedchange(this, args);
	}
}

void xui_treeview::non_selectednode( bool firemethod )
{
	if (m_selectnode.size() > 0)
	{
		for (u32 i = 0; i < m_selectnode.size(); ++i)
		{
			m_selectnode[i]->set_selected(false);
		}
		m_selectnode.clear();

		if (firemethod)
		{
			xui_method_args args;
			xm_selectedchange(this, args);
		}
	}
}

const std::vector<xui_treenode*>& xui_treeview::get_selectednode( void ) const
{
	return m_selectnode;
}

std::vector<xui_treenode*> xui_treeview::get_entirenode( bool total )
{
	std::vector<xui_treenode*> nodes;
	if (m_searchtext.length() > 0 || m_columnsort != k_treesort_none)
	{
		for (u32 i = 0; i < m_upmostpart.size(); ++i)
			m_upmostpart[i]->get_leafnodetotal(nodes, total);
	}
	else
	{
		for (u32 i = 0; i < m_upmostnode.size(); ++i)
			m_upmostnode[i]->get_leafnodetotal(nodes, total);
	}

	return nodes;
}

u32 xui_treeview::get_upmostnodecount( void ) const
{
	return m_upmostnode.size();
}

const std::vector<xui_treenode*>& xui_treeview::get_upmostnodearray( void ) const
{
	return m_upmostnode;
}

u32 xui_treeview::get_upmostnodeindex( xui_treenode* node ) const
{
	for (u32 i = 0; i < m_upmostnode.size(); ++i)
	{
		if (m_upmostnode[i] == node)
			return i;
	}

	return -1;
}

void xui_treeview::set_upmostnodeindex( xui_treenode* node, u32 index )
{
	std::vector<xui_treenode*>::iterator itor = std::find(m_upmostnode.begin(), m_upmostnode.end(), node);
	if (itor != m_upmostnode.end())
	{
		m_upmostnode.erase(itor);
		m_upmostnode.insert(m_upmostnode.begin()+index, node);
		refresh();
	}
}

xui_treenode* xui_treeview::get_upmostnode( u32 index )
{
	return m_upmostnode[index];
}

xui_treenode* xui_treeview::add_upmostnode( u32 index, xui_treedata* data )
{
	xui_treenode* node = create_node(data);
	m_upmostnode.insert(m_upmostnode.begin()+index, node);
	invalid();
	return node;
}

void xui_treeview::del_upmostnode( xui_treenode* node )
{
	bool exist = false;
	std::vector<xui_treenode*>::iterator itor;
	itor = std::find(m_upmostpart.begin(), m_upmostpart.end(), node);
	if (itor != m_upmostpart.end())
	{
		exist = true;
		m_upmostpart.erase(itor);
	}
	itor = std::find(m_upmostnode.begin(), m_upmostnode.end(), node);
	if (itor != m_upmostnode.end())
	{
		exist = true;
		m_upmostnode.erase(itor);
	}

	if (exist)
		delete_node(node);

	invalid();
}

void xui_treeview::del_upmostnodeall( void )
{
	m_upmostnode.clear();
	m_upmostpart.clear();
	m_selectnode.clear();

	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		m_ascrollitem[i]->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(m_ascrollitem[i]);
	}
	m_ascrollitem.clear();
	refresh();
}

void xui_treeview::set_nodevisible( xui_treenode* node )
{
	xui_treenode* root = node->get_rootnode();
	while (root)
	{
		root->set_expanded(true);
		root = root->get_rootnode();
	}
	refresh();

	if (m_vscroll)
	{
		std::vector<xui_treenode*> nodes = get_entirenode(false);
		for (u32 i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i] == node)
			{
				s32 maxvalue = i * m_lineheight;
				s32 minvalue = maxvalue - get_renderrtins().get_sz().h + m_lineheight;
				if (m_vscroll->get_value() < minvalue)
					m_vscroll->set_value(minvalue);
				if (m_vscroll->get_value() > maxvalue)
					m_vscroll->set_value(maxvalue);

				break;
			}
		}
	}
}

xui_rect2d<s32> xui_treeview::get_renderrtins( void ) const
{
	xui_rect2d<s32> rt = xui_container::get_renderrtins();
	if (m_renderhead)
		rt.ay += m_lineheight;

	return rt;
}

xui_treenode* xui_treeview::choose_node( const xui_vector<s32>& pt )
{
	xui_rect2d<s32> rt = get_renderrtins();
	if (rt.was_inside(pt))
	{
		xui_vector<s32> scrollpt;
		scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
		scrollpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
		xui_vector<s32> relative = pt + scrollpt;
		std::vector<xui_treenode*> alltreenodes = get_entirenode(false);
		for (u32 i = 0; i < alltreenodes.size(); ++i)
		{
			xui_treenode* node = alltreenodes[i];
			if (node->get_renderrt().was_inside(relative-node->get_renderpt()))
				return node;
		}
	}

	return NULL;
}

xui_component* xui_treeview::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = xui_control::choose_else(pt);
	if (m_render.was_inside(pt))
	{
		xui_vector<s32> scrollpt;
		scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
		scrollpt.y = 0;
		xui_vector<s32> relative = pt - m_render.get_pt() + scrollpt;
		if (component == NULL && m_renderhead && m_rendergrid)
		{
			for (s32 i = (s32)m_columngrid.size()-1; i >= 0; --i)
			{
				if (component = m_columngrid[i]->choose(relative))
					return component;
			}
		}
		if (component == NULL && m_renderhead)
		{
			for (s32 i = (s32)m_columnhead.size()-1; i >= 0; --i)
			{
				if (component = m_columnhead[i]->choose(relative))
					return component;
			}
		}
	}
	xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
	if (rt.was_inside(pt))
	{
		xui_vector<s32> scrollpt;
		scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
		scrollpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
		xui_vector<s32> relative = pt - m_render.get_pt() + scrollpt;
		if (component == NULL)
		{
			std::vector<xui_treenode*> alltreenodes = get_entirenode(false);
			for (u32 i = 0; i < alltreenodes.size(); ++i)
			{
				if (component = alltreenodes[i]->choose(relative))
					return component;
			}
		}
	}

	return component;
}

void xui_treeview::update_else( f32 delta )
{
	xui_control::update_else(delta);
	std::vector<xui_treenode*> alltreenodes = get_entirenode(false);
	for (u32 i = 0; i < alltreenodes.size(); ++i)
	{
		if (alltreenodes[i]->was_visible())
			alltreenodes[i]->update(delta);
	}
	for (u32 i = 0; i < m_columngrid.size(); ++i)
	{
		if (m_columngrid[i]->was_visible())
			m_columngrid[i]->update(delta);
	}
	for (u32 i = 0; i < m_columnhead.size(); ++i)
	{
		if (m_columnhead[i]->was_visible())
			m_columnhead[i]->update(delta);
	}
}

void xui_treeview::render_else( void )
{
	xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());

	xui_canvas::get_ins()->set_cliprect(currrect);
	std::vector<xui_treenode*> alltreenodes = get_entirenode(false);
	for (u32 i = 0; i < alltreenodes.size(); ++i)
	{
		if (currrect.get_inter(alltreenodes[i]->get_renderrtabs()).was_valid())
			alltreenodes[i]->render();
	}
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_rect2d<s32> rt = get_renderrtabs();
	rt.ax += m_corner;
	rt.bx -= m_corner;
	xui_canvas::get_ins()->set_cliprect(cliprect.get_inter(rt));
	if (m_renderhead)
	{
		for (u32 i = 0; i < m_columnhead.size(); ++i)
		{
			m_columnhead[i]->render();
		}
	}

	xui_colour vertexcolor = get_vertexcolor();
	xui_vector<s32> p1;
	xui_vector<s32> p2;
	if (m_rendergrid)
	{
		rt = (m_vscroll == NULL) ? get_renderrt() : get_renderrtins();
		for (u32 i = 0; i < m_columngrid.size(); ++i)
		{
			p1 = m_columngrid[i]->get_screenpt();
			p2 = p1 + xui_vector<s32>(0, rt.by);
			xui_canvas::get_ins()->draw_line(p1, p2, m_sidecolor*vertexcolor);
		}
	}
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_control::render_else();

	if (m_renderhead && m_rendergrid)
	{
		xui_vector<s32> pt = get_screenpt();
		p1.x = pt.x;
		p1.y = pt.y + m_border.ay + m_lineheight - 1;
		p2.x = pt.x + get_renderw();
		p2.y = pt.y + m_border.ay + m_lineheight - 1;
		xui_canvas::get_ins()->draw_line(p1, p2, m_sidecolor*vertexcolor);
	}

	if (m_allowplace != k_treedrop_notallow)
	{
		xui_treenode* hovernode = choose_node(get_renderpt(xui_desktop::get_ins()->get_mousecurr()));
		if (hovernode)
		{
			xui_rect2d<s32> rt = hovernode->get_renderrtabs().get_inter(get_renderrtins()+get_screenpt());
			if (m_allowplace == k_treedrop_inner)
			{
				xui_canvas::get_ins()->draw_rectangle(rt, xui_colour::k_black, 2);
			}
			else
			{
				xui_vector<s32> p1;
				xui_vector<s32> p2;
				switch (m_allowplace)
				{
				case k_treedrop_front:
					p1 = xui_vector<s32>(rt.ax, rt.ay);
					p2 = xui_vector<s32>(rt.bx, rt.ay);
					break;
				case k_treedrop_after:
					p1 = xui_vector<s32>(rt.ax, rt.ay+m_lineheight);
					p2 = xui_vector<s32>(rt.bx, rt.ay+m_lineheight);
					break;
				}

				rt  = xui_rect2d<s32>(p1.x+1, p1.y-1, p2.x-1, p1.y+1);
				xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_black);
				p1 += xui_vector<s32>(4, 0);
				xui_canvas::get_ins()->fill_triangle(p1, 3, k_triangle_right, xui_colour::k_black);
				p2 -= xui_vector<s32>(3, 0);
				xui_canvas::get_ins()->fill_triangle(p2, 3, k_triangle_left,  xui_colour::k_black);
			}
		}
	}
}

void xui_treeview::on_invalid( xui_method_args&	args )
{
	s32 width = 0;
	for (u32 i = 0; i < m_columninfo.size(); ++i)
		width += m_columninfo[i].size;

	std::vector<xui_treenode*> nodes = get_entirenode(false);
	xui_vector<s32> sz(width, m_lineheight * nodes.size());
	xui_rect2d<s32> rt = get_renderrtins();
	sz.w = xui_max(sz.w, rt.get_w());
	sz.h = xui_max(sz.h, rt.get_h());

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
	}
}

void xui_treeview::on_perform( xui_method_args&	args )
{
	xui_container::on_perform(args);

	s32 width = 0;
	for (u32 i = 0; i < m_columninfo.size(); ++i)
	{
		width += m_columninfo[i].size;
	}
	//fill view
	xui_rect2d<s32> rt = get_renderrtins();
	if (width < rt.get_w() && width > 0)
	{
		for (u32 i = 0; i < m_columninfo.size(); ++i)
		{
			if (m_columninfo[i].type == k_treecolumn_main)
			{
				m_columninfo[i].size += (rt.get_w() - width);
				break;
			}
		}

		width = rt.get_w();
	}

	xui_vector<s32> pt;
	xui_vector<s32> sz;

	//head
	pt.x = m_border.ax;
	pt.y = m_border.ay;
	for (u32 i = 0; i < m_columnhead.size(); ++i)
	{
		sz.w  = m_columninfo[i].size;
		sz.h  = m_lineheight;
		m_columnhead[i]->on_perform_pt(pt);
		m_columnhead[i]->on_perform_sz(sz);
		pt.x += sz.w;
	}

	//grid
	pt.x = m_border.ax;
	pt.y = 0;
	for (u32 i = 0; i < m_columngrid.size(); ++i)
	{
		pt.x += m_columninfo[i].size;
		sz.w  = 4;
		sz.h  = m_border.ay + m_lineheight;
		m_columngrid[i]->on_perform_pt(pt);
		m_columngrid[i]->on_perform_sz(sz);
	}

	//node
	std::vector<xui_treenode*> alltreenodes = get_entirenode(false);
	for (u32 i = 0; i < alltreenodes.size(); ++i)
	{
		pt.x = m_border.ax;
		pt.y = m_border.ay + i*m_lineheight + (m_renderhead ? m_lineheight : 0);
		sz.w = width;
		sz.h = m_lineheight;
		alltreenodes[i]->on_perform_pt(pt);
		alltreenodes[i]->on_perform_sz(sz);
	}
}

void xui_treeview::on_horzvalue( xui_method_args& args )
{
	xui_container::on_horzvalue(args);
	for (u32 i = 0; i < m_columnhead.size(); ++i)
	{
		m_columnhead[i]->set_scrollx(m_hscroll->get_value());
	}
	for (u32 i = 0; i < m_columngrid.size(); ++i)
	{
		m_columngrid[i]->set_scrollx(m_hscroll->get_value());
	}
}

void xui_treeview::on_updateself( xui_method_update& args )
{
	xui_container::on_updateself(args);
	if (has_catch())
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.y-dw.y) < m_lineheight/2)
			return;

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+m_lineheight/2)
			scroll_value = -m_lineheight/2;
		if (pt.y < rt.by && pt.y > rt.by-m_lineheight/2)
			scroll_value =  m_lineheight/2;

		if (m_vscroll && scroll_value != 0)
		{
			m_vscroll->set_value(m_vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = k_mb_left;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}

void xui_treeview::on_renderself( xui_method_args& args )
{
	xui_container::on_renderself(args);
	if (m_lighttrace && was_hover())
	{
		xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
		xui_canvas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtins()+get_screenpt()));
		xui_vector<s32> pt = get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		xui_treenode* hovernode = choose_node(pt);
		if (hovernode && hovernode->was_selected() == false)
		{
			xui_rect2d<s32> rt = hovernode->get_renderrtabs();
			xui_colour   color = hovernode->get_vertexcolor();
			xui_canvas::get_ins()->fill_rectangle(rt, color*k_lighttrace_color);
		}
		xui_canvas::get_ins()->set_cliprect(cliprect);
	}
}

void xui_treeview::on_mousedoubleclick( xui_method_mouse& args )
{
	xui_container::on_mousedoubleclick(args);
	if (args.mouse == k_mb_left)
	{
		xui_treenode* node = choose_node(get_renderpt(args.point));
		if (node)
		{
			node->set_expanded(!node->was_expanded());
		}
	}
}

void xui_treeview::on_mousedown( xui_method_mouse& args )
{
	xui_container::on_mousedown(args);

	std::vector<xui_treenode*> nodes = get_entirenode(false);
	u32 selectedindex = -1;
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		if (nodes[i]->was_selected())
		{
			selectedindex = i;
			break;
		}
	}

	xui_treenode* node = choose_node(get_renderpt(args.point));
	if (node == NULL)
	{
		non_selectednode(true);
	}
	else
	{
		if (args.ctrl)
		{
			set_selectednode(node, !node->was_selected());
		}
		else
		if (args.shift && selectedindex != -1)
		{
			u32 index = -1;
			for (u32 i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i] == node)
				{
					index = i;
					break;
				}
			}
			u32 start = xui_min(index, selectedindex);
			u32 final = xui_max(index, selectedindex);
			std::vector<xui_treenode*> selectednodes;
			for (u32 i = start; i <= final && i < nodes.size(); ++i)
				selectednodes.push_back(nodes[i]);

			set_selectednode(selectednodes);
		}
		else
		if (node->was_selected() == false)
		{
			non_selectednode(false);
			set_selectednode(node, true);
		}
		else
		{
			node->ini_holdtime();
		}
	}

	if (args.mouse == k_mb_left && m_searchtext.empty() && m_columnsort == k_treesort_none)
		m_mousecatch = node;
}

void xui_treeview::on_mousemove( xui_method_mouse& args )
{
	xui_container::on_mousemove(args);

	xui_treenode* hovernode = choose_node(get_renderpt(args.point));
	if (m_mousehover != hovernode)
	{
		xui_method_args node_args;
		if (m_mousehover)
		{
			node_args.wparam = m_mousehover;
			node_args.handle = false;
			xm_treenodeleave(this, node_args);
		}

		m_mousehover  = hovernode;

		if (m_mousehover)
		{
			node_args.wparam = m_mousehover;
			node_args.handle = false;
			xm_treenodeenter(this, node_args);
		}
	}

	if (has_catch())
	{
		m_allowplace = k_treedrop_notallow;

		if (m_acceptdrag &&
			m_mousehover &&
			m_mousecatch &&
			m_mousehover != m_mousecatch)
		{
			xui_method_treedragdrop tree_args;
			xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr() - m_mousehover->get_screenpt();
			tree_args.dragnode = m_mousecatch;
			tree_args.dropnode = m_mousehover;
			if (pt.y < m_lineheight/3  ) tree_args.allowplace = k_treedrop_front;
			if (pt.y > m_lineheight*2/3) tree_args.allowplace = k_treedrop_after;
			xm_treedragover(this,   tree_args);

			m_allowplace = tree_args.allowplace;
		}
	}
}

void xui_treeview::on_mouserise( xui_method_mouse& args )
{
	xui_container::on_mouserise(args);
	if (args.mouse == k_mb_left)
	{
		if (m_allowplace != k_treedrop_notallow &&
			m_mousecatch)
		{
			xui_method_treedragdrop tree_args;
			tree_args.dragnode   = m_mousecatch;
			tree_args.dropnode   = m_mousehover;
			tree_args.allowplace = m_allowplace;
			xm_treedragdrop(this,   tree_args);
		}

		if (m_mousecatch &&
			m_mousecatch == choose_node(get_renderpt(args.point)))
		{
			m_mousecatch->req_focus();
		}

		m_mousecatch = NULL;
		m_mousehover = NULL;
		m_allowplace = k_treedrop_notallow;
	}
}

xui_treenode* xui_treeview::create_node( xui_treedata* data )
{
	xui_treenode* node = new xui_treenode(data, this);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	node->set_scrollpt(pt);
	m_ascrollitem.push_back(node);

	return node;
}

void xui_treeview::delete_node( xui_treenode* node )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_ascrollitem.begin(),
		m_ascrollitem.end(),
		node);

	if (itor != m_ascrollitem.end())
	{
		if (node->was_selected())
		{
			m_selectnode.erase(std::find(
				m_selectnode.begin(),
				m_selectnode.end(),
				node));
		}

		m_ascrollitem.erase(itor);
		node->del_leafnodeall();
		node->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(node);
	}
}

void xui_treeview::on_headclick( xui_component* sender, xui_method_mouse& args )
{
	for (u32 i = 0; i < m_columnhead.size(); ++i)
	{
		if (m_columnhead[i] == sender)
		{
			u08 sort = (m_sortcolumn == i) ? m_columnsort+1 : k_treesort_increment;
			if (sort <= k_treesort_decrement) 
				set_columnsort(i, sort);
			else
				non_columnsort();

			break;
		}
	}
}

void xui_treeview::on_headrenderself( xui_component* sender, xui_method_args& args )
{
	if (m_columnsort != k_treesort_none && sender == m_columnhead[m_sortcolumn])
	{
		xui_drawer*     drawer = xui_dynamic_cast(xui_drawer, sender);
		xui_rect2d<s32> rt     = drawer->get_rendertextrt() + drawer->get_screenpt();
		xui_vector<s32> center = xui_vector<s32>(rt.bx+10, rt.ay+rt.get_h()/2);
		xui_colour      color  = drawer->get_vertexcolor();

		u08 direction = (m_columnsort == k_treesort_increment) ? k_triangle_up : k_triangle_down;
		xui_canvas::get_ins()->fill_triangle(center, 3, direction, color*xui_colour::k_white);
		xui_canvas::get_ins()->draw_triangle(center, 3, direction, color*xui_colour::k_black);
	}
}