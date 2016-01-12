#include "xui_scroll.h"
#include "xui_slider.h"
#include "xui_desktop.h"
#include "xui_timetool.h"
#include "xui_timegrad.h"
#include "xui_timedata.h"
#include "xui_timeline.h"
#include "xui_timehead.h"
#include "xui_timerect.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_timeview.h"

const s32 GRAD_LAYOUT[7][3] = 
{
	{65,   1, 1},
	{30,   2, 2},
	{15,   5, 5},
	{ 8,  10, 5},
	{ 4,  20, 5},
	{ 2,  50, 5},
	{ 1, 100, 5}
};

/*
//constructor
*/
xui_create_explain(xui_timeview)( const std::string& name, const xui_rect2d<s32>& rect, s32 treesize, s32 lineheight, u32 lineflag, xui_bitmap** flagicon )
: xui_container(name, rect)
{
	m_type     += "timeview";
	m_keyspace = 15;
	m_keylarge =  5;
	m_keysmall =  1;
	m_curframe = -1;
	m_dragmode =  0;
	m_droptime = -1;
	m_dragtime = -1;

	m_timetool = new xui_timetool();
	xui_method_ptrcall(m_timetool, set_parent		)(this);
	xui_method_ptrcall(m_timetool, set_backcolor	)(xui_colour(0.0f));
	m_widgetvec.push_back(m_timetool);
	m_timegrad = new xui_timegrad();
	m_timegrad->xm_updateself	+= new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	xui_method_ptrcall(m_timegrad, set_parent		)(this);
	xui_method_ptrcall(m_timegrad, set_backcolor	)(xui_colour(0.0f));
	m_widgetvec.push_back(m_timegrad);
	m_timerect = new xui_timerect();
	m_timerect->xm_updateself	+= new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timerectdraghorz);
	xui_method_ptrcall(m_timerect, set_parent		)(this);
	xui_method_ptrcall(m_timerect, set_backcolor	)(xui_colour(0.2f, 0.0f, 1.0f, 1.0f));
	xui_method_ptrcall(m_timerect, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_timerect, set_sidecolor	)(xui_colour(1.0f, 0.0f, 0.9f, 0.9f));
	m_widgetvec.push_back(m_timerect);
	m_timehead = new xui_timehead();
	m_timehead->xm_updateself	+= new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	m_timehead->xm_updateself	+= new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timeviewdragvert);
	xui_method_ptrcall(m_timehead, set_parent		)(this);
	xui_method_ptrcall(m_timehead, set_backcolor	)(xui_colour(0.0f));
	m_widgetvec.push_back(m_timehead);
	m_spaceset = new xui_slider("", xui_rect2d<s32>(0, 0, 150, 10), FLOWSTYLE_H);
	m_spaceset->xm_scroll		+= new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_spacesetscroll);
	xui_method_ptrcall(m_spaceset, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_spaceset, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_spaceset, set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_spaceset, set_alignhorz	)(ALIGNHORZ_L);
	xui_method_ptrcall(m_spaceset, set_alignvert	)(ALIGNVERT_B);
	xui_method_ptrcall(m_spaceset, set_parent		)(this);
	xui_method_ptrcall(m_spaceset, set_range		)(100);
	xui_method_ptrcall(m_spaceset, set_value		)(15);
	m_widgetvec.push_back(m_spaceset);

	std::vector<xui_treecolumn> columninfo;
	for (u32 i = 0; i < lineflag; ++i)
	{
		xui_treecolumn info(TREECOLUMN_MAIN, 16, L"", flagicon[i]);
		info.minsize = 16;
		info.maxsize = 16;
		columninfo.push_back(info);
	}
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, treesize-16*lineflag, L""));

	m_timetree = new xui_treeview("", xui_rect2d<s32>(0, 0, treesize, 100), columninfo);
	m_timetree->xm_setclientsz += new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timetreesetclientsz);
	m_timetree->xm_invalid     += new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timetreeinvalid);
	xui_method_ptrcall(m_timetree, set_parent		)(this);
	xui_method_ptrcall(m_timetree, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_timetree, set_lineheight	)(lineheight);
	xui_method_ptrcall(m_timetree, set_lighttrace	)(false);
	xui_method_ptrcall(m_timetree, set_rendergrid	)(false);
	xui_method_ptrcall(m_timetree, set_plusrender	)(PLUSRENDER_SYMBOL);
	m_widgetvec.push_back(m_timetree);

	refresh();
}

/*
//control
*/
xui_method_explain(xui_timeview, get_timegrad,				xui_timegrad*				)( void )
{
	return m_timegrad;
}
xui_method_explain(xui_timeview, get_timetool,				xui_timetool*				)( void )
{
	return m_timetool;
}
xui_method_explain(xui_timeview, get_timehead,				xui_timehead*				)( void )
{
	return m_timehead;
}
xui_method_explain(xui_timeview, get_timetree,				xui_treeview*				)( void )
{
	return m_timetree;
}

/*
//property
*/
xui_method_explain(xui_timeview, add_toolitem,				void						)( xui_componet*		item )
{
	m_timetool->add_item(item);
}
xui_method_explain(xui_timeview, set_headtext,				void						)( const std::wstring&	text )
{
	u32 index = m_timetree->get_columninfocount()-1;
	m_timetree->set_columntext(index, text);
}
xui_method_explain(xui_timeview, set_gradfont,				void						)( const xui_family&	font )
{
	m_timegrad->set_font(font);
}
xui_method_explain(xui_timeview, set_gradtextcolor,			void						)( const xui_colour&	textcolor )
{
	m_timegrad->set_textcolor(textcolor);
}

/*
//method
*/
xui_method_explain(xui_timeview, get_allframe,				const xui_keyframe_map&		)( void ) const
{
	return m_allframe;
}
xui_method_explain(xui_timeview, get_curframe,				s32							)( void ) const
{
	return m_curframe;
}
xui_method_explain(xui_timeview, set_curframe,				void						)( s32 frame )
{
	if (m_curframe != xui_max(frame, 0))
	{
		m_curframe  = xui_max(frame, 0);

		xui_method_args args;
		xm_curframechange(this, args);
	}
}
xui_method_explain(xui_timeview, get_keyspace,				s32							)( void) const
{
	return m_keyspace;
}
xui_method_explain(xui_timeview, get_keylarge,				s32							)( void ) const
{
	return m_keylarge;
}
xui_method_explain(xui_timeview, get_keysmall,				s32							)( void ) const
{
	return m_keysmall;
}
xui_method_explain(xui_timeview, set_keyspace,				void						)( s32 space )
{
	if (m_keyspace != xui_max(space, 1))
	{
		m_keyspace  = xui_max(space, 1);
		for (s32 i = 0; i < 7; ++i)
		{
			if (m_keyspace >= GRAD_LAYOUT[i][0])
			{
				m_keylarge  = GRAD_LAYOUT[i][1];
				m_keysmall  = m_keylarge / GRAD_LAYOUT[i][2];
				break;
			}
		}

		refresh();
	}
}

/*
//drag
*/
xui_method_explain(xui_timeview, get_droptime,				s32							)( void ) const
{
	return m_droptime;
}
xui_method_explain(xui_timeview, set_droptime,				void						)( s32 time )
{
	m_droptime = time;
}
xui_method_explain(xui_timeview, get_dragmode,				u08							)( void ) const
{
	return m_dragmode;
}
xui_method_explain(xui_timeview, set_dragmode,				void						)( u08 mode )
{
	m_dragmode = mode;
}
xui_method_explain(xui_timeview, get_dragtime,				s32							)( void ) const
{
	return m_dragtime;
}
xui_method_explain(xui_timeview, set_dragtime,				void						)( s32 time )
{
	m_dragtime = time;
}

/*
//rect
*/
xui_method_explain(xui_timeview, was_rectmove,				bool						)( xui_componet* layer, s32 frame )
{
	if (m_timerect->was_visible() == false)
		return false;

	xui_rect2d<s32> range = m_timerect->get_range();
	if (range.ay == -1)
	{
		return (frame >= range.ax && frame <= range.bx);
	}
	else
	{
		s32 index = -1;
		std::vector<xui_timeline*> lines = get_timelinetotal(false);
		xui_vecptr_addloop(lines)
		{
			if (lines[i] == layer)
			{
				index = i;
				break;
			}
		}

		return (index >= range.ay && index <= range.by &&
				frame >= range.ax && frame <= range.bx);
	}
}
xui_method_explain(xui_timeview, set_rectshow,				void						)( s32 startlayer, s32 finallayer, s32 startframe, s32 finalframe )
{
	m_timerect->set_range(startlayer, finallayer, startframe, finalframe);
}
xui_method_explain(xui_timeview, set_rectrise,				void						)( void )
{
	bool hasframe = false;

	xui_rect2d<s32> range = m_timerect->get_range();
	if (range.ay == -1)
	{
		for (xui_keyframe_map::iterator itor = m_allframe.begin(); itor != m_allframe.end(); ++itor)
		{
			if ((*itor).first >= range.ax && (*itor).first <= range.bx)
			{
				hasframe = true;
				break;
			}
		}

		set_selectedline(range.ax, range.bx, true);
	}
	else
	{
		std::vector<xui_timeline*> lines = get_timelinetotal(false);
		for (s32 i = 0; i < (s32)lines.size(); ++i)
		{
			if (i < range.ay || i > range.by)
				continue;

			if (lines[i]->add_selframe(range.ax, range.bx))
			{
				hasframe = true;
			}
		}
	}

	if (hasframe)
	{
		m_timerect->req_focus();
	}
	else
	{
		m_timerect->set_visible(false);
	}
}

/*
//selected
*/
xui_method_explain(xui_timeview, get_selectedallfirst,		s32							)( void ) const
{
	for (xui_keyframe_map::const_iterator itor = m_allframe.begin(); itor != m_allframe.end(); ++itor)
	{
		if (was_selectedall((*itor).second, (*itor).first))
			return (*itor).first;
	}

	return -1;
}
xui_method_explain(xui_timeview, was_selectedall,			bool						)( const std::vector<xui_timeline*>& lines, s32 frame ) const
{
	for (u32 i = 0; i < lines.size(); ++i)
	{
		if (lines[i]->has_selframe(frame) == false)
			return false;
	}

	return true;
}
xui_method_explain(xui_timeview, has_selectedline,			bool						)( const std::vector<xui_timeline*>& lines, s32 frame ) const
{
	for (u32 i = 0; i < lines.size(); ++i)
	{
		if (lines[i]->has_selframe(frame))
			return true;
	}

	return false;
}
xui_method_explain(xui_timeview, set_selectedline,			void						)( s32 start, s32 final, bool selected )
{
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vecptr_addloop(nodes)
	{
		xui_timeline*	line = (xui_timeline*)nodes[i]->get_data();
		if (selected)	line->add_selframe(start, final);
		else			line->del_selframe(start, final);
	}

	xui_method_args args;
	xm_selectedchange(this, args);
}
xui_method_explain(xui_timeview, non_selectedline,			void						)( bool fireMethod )
{
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode();
	xui_vecptr_addloop(nodes)
	{
		xui_timeline* line = (xui_timeline*)nodes[i]->get_data();
		line->set_selframe(std::vector<s32>());
	}

	if (fireMethod)
	{
		xui_method_args args;
		xm_selectedchange(this, args);
	}
}
xui_method_explain(xui_timeview, get_selectedline,			std::vector<xui_timeline*>	)( void )
{
	std::vector<xui_timeline*> lines;
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode();
	xui_vecptr_addloop(nodes)
	{
		xui_timeline* line = (xui_timeline*)nodes[i]->get_data();
		if (line->has_selframe())
			lines.push_back(line);
	}

	return lines;
}

/*
//line
*/
xui_method_explain(xui_timeview, get_timelinetotal,			std::vector<xui_timeline*>	)( bool total )
{
	std::vector<xui_timeline*> lines;
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(total);
	xui_vecptr_addloop(nodes)
	{
		lines.push_back((xui_timeline*)nodes[i]->get_data());
	}

	return lines;
}
xui_method_explain(xui_timeview, get_timelinecount,			u32							)( void ) const
{
	return m_timetree->get_upmostnodecount();
}
xui_method_explain(xui_timeview, get_timelinearray,			std::vector<xui_timeline*>	)( void ) const
{
	std::vector<xui_timeline*> lines;
	std::vector<xui_treenode*> nodes = m_timetree->get_upmostnodearray();
	xui_vecptr_addloop(nodes)
	{
		lines.push_back((xui_timeline*)nodes[i]->get_data());
	}

	return lines;
}
xui_method_explain(xui_timeview, get_timeline,				xui_timeline*				)( u32 index )
{
	xui_treenode* node = m_timetree->get_upmostnode(index);
	return (xui_timeline*)node->get_data();
}
xui_method_explain(xui_timeview, add_timeline,				xui_timeline*				)( u32 index, xui_timedata* data )
{
	xui_treenode* node = m_timetree->add_upmostnode(index, data);
	return create_line(node, data);
}
xui_method_explain(xui_timeview, del_timeline,				void						)( xui_timeline* line )
{
	xui_treenode* node = line->get_linkdata()->get_node();
	m_timetree->del_upmostnode(node);
	delete_line(line);
}

/*
//rectangle
*/
xui_method_explain(xui_timeview, get_renderrtins,			xui_rect2d<s32>				)( void ) const
{
	xui_rect2d<s32> rt = xui_container::get_renderrtins();
	rt.ax += m_timetree->get_renderw();
	rt.ay += m_timegrad->get_renderh();
	return rt;
}

/*
//virtual
*/
xui_method_explain(xui_timeview, choose_else,				xui_componet*				)( const xui_vector<s32>& pt )
{
	xui_componet* componet = xui_control::choose_else(pt);
	if (componet == NULL)
	{
		std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
		xui_vecptr_addloop(nodes)
		{
			xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
			if (componet = timeline->choose(pt))
				return componet;
		}
	}

	return componet;
}
xui_method_explain(xui_timeview, update_else,				void						)( f32 delta )
{
	xui_control::update_else(delta);

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vecptr_addloop(nodes)
	{
		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		timeline->update(delta);
	}
}
xui_method_explain(xui_timeview, render_else,				void						)( void )
{
	xui_control::render_else();
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vecptr_addloop(nodes)
	{
		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		timeline->render();
	}
}

/*
//override
*/
xui_method_explain(xui_timeview, on_invalid,				void						)( xui_method_args& args )
{
	m_allframe.clear();
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vecptr_addloop(nodes)
	{
		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		std::vector<s32> allframe = timeline->get_linkdata()->get_allframe();
		for (std::vector<s32>::iterator itor = allframe.begin(); itor != allframe.end(); ++itor)
		{
			s32 frame = (*itor);
			m_allframe[frame].push_back(timeline);
		}
	}

	xui_vector<s32> sz;
	xui_rect2d<s32> rt = get_renderrtins();
	s32 finalframe = m_allframe.empty() ? 0 : (*m_allframe.rbegin()).first;
	sz.w = xui_max(m_keyspace*finalframe+10,	rt.get_sz().w);
	sz.h = xui_max(m_timetree->get_renderh(),	rt.get_sz().h);

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(xui_timeview, on_perform,				void						)( xui_method_args& args )
{
	xui_container::on_perform(args);
	xui_vector<s32> pt(0);
	xui_vector<s32> sz(0);

	pt.x = m_border.ax;
	pt.y = m_border.ay;
	m_timetool->set_renderpt(pt, false);

	pt.x = m_border.ax;
	pt.y = m_border.ay + m_timetool->get_renderh();
	sz.w = xui_max(m_timetool->get_renderw(), m_timetree->get_renderw());
	sz.h = m_timetree->get_renderh();
	m_timetree->set_renderpt(pt, false);
	m_timetree->set_rendersz(sz, false);

	pt.x = m_border.ax + m_timetree->get_renderw();
	pt.y = m_border.ay;
	s32 finalframe = m_allframe.empty() ? 0 : (*m_allframe.rbegin()).first;
	sz.w = xui_max(get_renderrtins().get_sz().w, m_keyspace*finalframe+10);
	sz.h = m_timegrad->get_renderh();
	m_timegrad->set_renderpt(pt, false);
	m_timegrad->set_rendersz(sz, false);
	pt.y = m_border.ay + m_timegrad->get_renderh();
	sz.h = m_timetree->get_lineheight();
	m_timehead->set_renderpt(pt, false);
	m_timehead->set_rendersz(sz, false);

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vecptr_addloop(nodes)
	{
		pt.y += m_timetree->get_lineheight();

		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		timeline->set_renderpt(pt, false);
		timeline->set_rendersz(sz, false);
	}
}
xui_method_explain(xui_timeview, on_horzvalue,				void						)( xui_method_args& args )
{
	xui_container::on_horzvalue(args);
	m_timegrad->set_scrollx(m_hscroll->get_value());
	m_timehead->set_scrollx(m_hscroll->get_value());
	m_timerect->set_scrollx(m_hscroll->get_value());
}
xui_method_explain(xui_timeview, on_vertvalue,				void						)( xui_method_args& args )
{
	xui_container::on_vertvalue(args);
	m_timetree->set_scrolly(m_vscroll->get_value());
	m_timehead->set_scrolly(m_vscroll->get_value());
	m_timerect->set_scrolly(m_vscroll->get_value());
}
xui_method_explain(xui_timeview, on_renderself,				void						)( xui_method_args& args )
{
	xui_container::on_renderself(args);

	xui_colour      color = get_vertexcolor();
	xui_vector<s32> pt(0);
	xui_rect2d<s32> rt(0);
	pt = m_timetree->get_screenpt();
	rt.set_w(m_timetree->get_renderw() + m_timegrad->get_renderw());
	rt.set_h(m_timetree->get_lineheight() - 1);

	s32 line_start = (m_vscroll == NULL) ? 0 : m_vscroll->get_value()/m_timetree->get_lineheight();
	s32 line_final = line_start + get_renderrtins().get_sz().h / m_timetree->get_lineheight();
	for (s32 i = line_start; i <= line_final; ++i)
	{
		rt.set_pt(xui_vector<s32>(pt.x, pt.y+i*m_timetree->get_lineheight()+1));
		xui_colour finalcolor = (i == 0)
			? color * xui_colour(1.0f, 0.4f, 0.6f, 0.6f)
			: color * xui_colour(1.0f, 0.5f, 0.5f, 0.5f);

		g_convas->fill_rectangle(rt, finalcolor);
	}

	pt = m_timegrad->get_screenpt() + xui_vector<s32>(m_timegrad->get_borderrt().ax, m_timegrad->get_renderh());
	s32 grad_start = (m_hscroll == NULL) ? 0 : m_hscroll->get_value() / m_keyspace;
	s32 grad_final = grad_start + m_timegrad->get_renderw() / m_keyspace + 1;
	for (s32 i = grad_start; i <= grad_final; ++i)
	{
		if (i%m_keylarge == 0 || i == m_curframe)
		{
			xui_vector<s32> p1 = pt + xui_vector<s32>(i*m_keyspace, 0);
			xui_vector<s32> p2 = p1 + xui_vector<s32>(0, get_renderrtins().get_sz().h);
			xui_colour finalcolor = (i == m_curframe)
				? color * xui_colour(1.0f, 0.0f, 1.0f, 1.0f)
				: color * xui_colour(1.0f, 0.2f, 0.2f, 0.2f);

			g_convas->draw_line(p1, p2, finalcolor);
		}
	}

	if (m_allframe.size() > 0)
	{
		xui_keyframe_map::reverse_iterator itor = m_allframe.rbegin();
		s32 grad = (*itor).first;
		xui_vector<s32> p1 = pt + xui_vector<s32>(grad*m_keyspace, 0);
		xui_vector<s32> p2 = p1 + xui_vector<s32>(0, get_renderrtins().get_sz().h);
		g_convas->draw_line(p1, p2, color * xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
	}
}

/*
//method
*/
xui_method_explain(xui_timeview, create_line,				xui_timeline*				)( xui_treenode* node, xui_timedata* data )
{
	xui_timeline* line = new xui_timeline(this, data);
	line->xm_updateself += new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	line->xm_updateself += new xui_method_member<xui_method_args, xui_timeview>(this, &xui_timeview::on_timeviewdragvert);
	node->set_data(line);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	line->set_scrollpt(pt);

	m_ascrollitem.push_back(line);
	invalid();

	return line;
}
xui_method_explain(xui_timeview, delete_line,				void						)( xui_timeline* line )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_ascrollitem.begin(),
		m_ascrollitem.end(),
		line);

	if (itor != m_ascrollitem.end())
	{
		m_ascrollitem.erase(itor);
		delete line;
	}
}

/*
//event
*/
xui_method_explain(xui_timeview, on_timetreesetclientsz,	void						)( xui_componet* sender, xui_method_args& args )
{
	xui_vector<s32> sz = m_timetree->get_clientsz();
	xui_rect2d<s32> rt = m_timetree->get_borderrt();
	sz.w += (rt.ax+rt.bx);
	sz.h += (rt.ay+rt.by+m_timetree->get_lineheight());
	m_timetree->set_rendersz(sz);
}
xui_method_explain(xui_timeview, on_timetreeinvalid,		void						)( xui_componet* sender, xui_method_args& args )
{
	s32 width = 0;
	for (u32 i = 0; i < m_timetree->get_columninfocount(); ++i)
		width += m_timetree->get_columninfo(i).size;

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vector<s32> sz(width, m_timetree->get_lineheight() * nodes.size());
	xui_rect2d<s32> rt = m_timetree->get_renderrtins();
	sz.w = xui_max(sz.w, rt.get_sz().w);

	if (m_timetree->get_clientsz() != sz)
	{
		m_timetree->set_clientsz(sz);
	}
	else
	{
		m_timetree->perform();
	}
}
xui_method_explain(xui_timeview, on_timeviewdraghorz,		void						)( xui_componet* sender, xui_method_args& args )
{
	xui_componet* catchctrl = g_desktop->get_catchctrl();
	if (catchctrl == sender)
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = g_desktop->get_mousecurr();

		s32 scroll_value =  0;
		if (pt.x > rt.ax && pt.x < rt.ax+10)
			scroll_value = -2;
		if (pt.x < rt.bx && pt.x > rt.bx-10)
			scroll_value =  2;

		if (m_hscroll && scroll_value != 0)
		{
			m_hscroll->set_value(m_hscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			g_desktop->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_timeviewdragvert,		void						)( xui_componet* sender, xui_method_args& args )
{
	xui_componet* catchctrl = g_desktop->get_catchctrl();
	if (catchctrl == sender)
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = g_desktop->get_mousecurr();

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+10)
			scroll_value = -2;
		if (pt.y < rt.by && pt.y > rt.by-10)
			scroll_value =  2;

		if (m_vscroll && scroll_value != 0)
		{
			m_vscroll->set_value(m_vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			g_desktop->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_timerectdraghorz,		void						)( xui_componet* sender, xui_method_args& args )
{
	xui_componet* catchctrl = g_desktop->get_catchctrl();
	if (catchctrl == m_timerect)
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = g_desktop->get_mousecurr();

		s32 scroll_value =  0;
		if (pt.x > rt.ax && pt.x < rt.ax+10)
			scroll_value = -2;
		if (pt.x < rt.bx && pt.x > rt.bx-10)
			scroll_value =  2;

		if (m_hscroll && scroll_value != 0)
		{
			m_hscroll->set_value(m_hscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			g_desktop->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_spacesetscroll,		void						)( xui_componet* sender, xui_method_args& args )
{
	s32 space = m_spaceset->get_value();
	set_keyspace(space);
}