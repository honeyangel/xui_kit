#include "xui_timer.h"
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

xui_implement_rtti(xui_timeview, xui_container);

/*
//static
*/
xui_method_explain(xui_timeview, create, xui_timeview*)( s32 width, const std::vector<xui_treecolumn>& columninfo )
{
	xui_timeview* timeview = new xui_timeview(xui_vector<s32>(width, 300), columninfo);
	xui_method_ptrcall(timeview, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(timeview, set_borderrt	)(xui_rect2d<s32>(4));
	return timeview;
}

/*
//constructor
*/
xui_create_explain(xui_timeview)( const xui_vector<s32>& size, const std::vector<xui_treecolumn>& columninfo, s32 lineheight )
: xui_container(size)
{
	m_hscrollshow	= true;
	m_keyspace		= 10;
	m_keylarge		=  5;
	m_keysmall		=  1;
	m_curframe		= -1;
	m_dragmode		=  0;
	m_droptime		= -1;
	m_dragtime		= -1;

	m_timetool = new xui_timetool(this);
	m_timegrad = new xui_timegrad(this);
	m_timegrad->xm_updateself		+= new xui_method_member<xui_method_update,			xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	m_widgetvec.push_back(m_timetool);
	m_widgetvec.push_back(m_timegrad);

	m_ksslider = new xui_slider(xui_vector<s32>(100, 16), FLOWSTYLE_H, ARROWDRAW_PLUSANDMINUS);
	m_ksslider->xm_scroll			+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_kssliderscroll);
	xui_method_ptrcall(m_ksslider, set_parent	)(this);
	xui_method_ptrcall(m_ksslider, set_backcolor)(xui_colour::darkgray);
	xui_method_ptrcall(m_ksslider, set_corner	)(3);
	xui_method_ptrcall(m_ksslider, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_ksslider, ini_scroll	)(100, 15);
	m_widgetvec.push_back(m_ksslider);

	m_fpstring = new xui_drawer(xui_vector<s32>(32, 16));
	xui_method_ptrcall(m_fpstring, set_parent	)(this);
	xui_method_ptrcall(m_fpstring, ini_drawer	)(L"fps:");
	m_fpnumber = new xui_drawer(xui_vector<s32>(24, 16));
	m_fpnumber->xm_mousemove		+= new xui_method_member<xui_method_mouse,			xui_timeview>(this, &xui_timeview::on_fpnumbermousemove);
	m_fpnumber->xm_renderself		+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_fpnumberrenderself);
	xui_method_ptrcall(m_fpnumber, set_parent	)(this);
	xui_method_ptrcall(m_fpnumber, set_cursor	)(CURSOR_WE);
	xui_method_ptrcall(m_fpnumber, set_textalign)(TEXTALIGN_CC);
	xui_method_ptrcall(m_fpnumber, ini_drawer	)(L"30");
	m_widgetvec.push_back(m_fpstring);
	m_widgetvec.push_back(m_fpnumber);

	std::vector<xui_treecolumn> columntemp = columninfo;
	columntemp.push_back(xui_treecolumn(TREECOLUMN_TEXT,  24, L"", NULL, 0));
	s32 treesize = 0;
	for (u32 i = 0; i < columntemp.size(); ++i)
		treesize += columntemp[i].size;

	m_timetree = new xui_treeview(xui_vector<s32>(treesize, 0), columntemp, lineheight, PLUSRENDER_SYMBOL, false, true, false);
	xui_method_ptrcall(m_timetree, set_parent	)(this);
	xui_method_ptrcall(m_timetree, set_drawcolor)(false);
	xui_method_ptrcall(m_timetree, set_borderrt	)(xui_rect2d<s32>(4, 0, 8, 0));
	m_timetree->xm_nonfocus			+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_timetreenonfocus);
	m_timetree->xm_mouseclick		+= new xui_method_member<xui_method_mouse,			xui_timeview>(this, &xui_timeview::on_timetreemouseclick);
	m_timetree->xm_setclientsz		+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_timetreesetclientsz);
	m_timetree->xm_invalid			+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_timetreeinvalid);
	m_timetree->xm_updateself		+= new xui_method_member<xui_method_update,			xui_timeview>(this, &xui_timeview::on_timetreeupdateself);
	m_timetree->xm_treedragover		+= new xui_method_member<xui_method_treedragdrop,	xui_timeview>(this, &xui_timeview::on_timetreeoverdrop);
	m_timetree->xm_treedragdrop		+= new xui_method_member<xui_method_treedragdrop,	xui_timeview>(this, &xui_timeview::on_timetreeoverdrop);
	//m_widgetvec.push_back(m_timetree);

	m_tldelete = new xui_button(xui_vector<s32>(16));
	m_tldelete->xm_buttonclick		+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_tldeleteclick);
	m_tldelete->xm_renderself		+= new xui_method_member<xui_method_args,			xui_timeview>(this, &xui_timeview::on_tldeleterenderself);
	xui_method_ptrcall(m_tldelete, set_parent	)(this);
	xui_method_ptrcall(m_tldelete, set_backcolor)(xui_colour::white);
	xui_method_ptrcall(m_tldelete, set_movecolor)(xui_button::default_downcolor);
	xui_method_ptrcall(m_tldelete, ini_component)(true, false);
	//m_widgetvec.push_back(m_tldelete);

	m_timerect = new xui_timerect(this);
	m_timehead = new xui_timehead(this);
	m_timerect->xm_updateself		+= new xui_method_member<xui_method_update,			xui_timeview>(this, &xui_timeview::on_timerectdraghorz);
	m_timehead->xm_updateself		+= new xui_method_member<xui_method_update,			xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	m_timehead->xm_updateself		+= new xui_method_member<xui_method_update,			xui_timeview>(this, &xui_timeview::on_timeviewdragvert);
	m_timehead->xm_mouserise		+= new xui_method_member<xui_method_mouse,			xui_timeview>(this, &xui_timeview::on_timelinemouseclick);
	m_timehead->xm_keybddown		+= new xui_method_member<xui_method_keybd,			xui_timeview>(this, &xui_timeview::on_timelinekeybddown);
	m_timerect->xm_keybddown		+= new xui_method_member<xui_method_keybd,			xui_timeview>(this, &xui_timeview::on_timelinekeybddown);
	m_ascrollitem.push_back(m_timerect);
	m_ascrollitem.push_back(m_timehead);

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
xui_method_explain(xui_timeview, add_toolitem,				void						)( xui_component* item )
{
	m_timetool->add_item(item);
}
xui_method_explain(xui_timeview, set_headtext,				void						)( const std::wstring& text )
{
	u32 index = m_timetree->get_columninfocount()-1;
	m_timetree->set_columntext(index, text);
}
xui_method_explain(xui_timeview, set_gradfont,				void						)( const xui_family& font )
{
	m_timegrad->set_textfont(font);
}
xui_method_explain(xui_timeview, set_graddraw,				void						)( const xui_family_render&	draw )
{
	m_timegrad->set_textdraw(draw);
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
xui_method_explain(xui_timeview, was_rectmove,				bool						)( xui_component* layer, s32 frame )
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
		for (u32 i = 0; i < lines.size(); ++i)
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
		bool selframe = false;
		std::vector<xui_timeline*> lines = get_timelinetotal(false);
		for (s32 i = 0; i < (s32)lines.size(); ++i)
		{
			if (i < range.ay || i > range.by)
				continue;

			if (lines[i]->add_selframe(range.ax, range.bx))
			{
				hasframe = true;
				selframe = true;
			}
		}

		if (selframe)
		{
			xui_method_args			args;
			xm_selectedchange(this, args);
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
	for (u32 i = 0; i < nodes.size(); ++i)
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
	for (u32 i = 0; i < nodes.size(); ++i)
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
	for (u32 i = 0; i < nodes.size(); ++i)
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
	for (u32 i = 0; i < nodes.size(); ++i)
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
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		lines.push_back((xui_timeline*)nodes[i]->get_data());
	}

	return lines;
}
xui_method_explain(xui_timeview, get_timelineindex,			u32							)( xui_timeline* line )
{
	xui_timedata* data = line->get_linkdata();
	return m_timetree->get_upmostnodeindex(data->get_node());
}
xui_method_explain(xui_timeview, set_timelineindex,			void						)( xui_timeline* line, u32 index )
{
	xui_timedata* data = line->get_linkdata();
	m_timetree->set_upmostnodeindex(data->get_node(), index);
	refresh();
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
xui_method_explain(xui_timeview, del_timelineall,			void						)( void )
{
	m_timetree->del_upmostnodeall();
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (m_ascrollitem[i] == m_timerect ||
			m_ascrollitem[i] == m_timehead)
			continue;

		m_ascrollitem[i]->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(m_ascrollitem[i]);
	}

	m_ascrollitem.clear();
	m_ascrollitem.push_back(m_timerect);
	m_ascrollitem.push_back(m_timehead);

	invalid();
}
xui_method_explain(xui_timeview, set_timelinevisible,		void								)( xui_timeline* line )
{
	xui_timedata* data = line->get_linkdata();
	xui_treenode* node = data->get_node();
	xui_treenode* root = node->get_rootnode();
	while (root)
	{
		root->set_expanded(true);
		root = root->get_rootnode();
	}
	refresh();

	if (m_vscroll)
	{
		std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
		for (u32 i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i] == node)
			{
				s32 maxvalue = (i+1) * m_timetree->get_lineheight();
				s32 minvalue = maxvalue - get_renderrtins().get_sz().h + m_timetree->get_lineheight();
				if (m_vscroll->get_value() < minvalue)
					m_vscroll->set_value(minvalue);
				if (m_vscroll->get_value() > maxvalue)
					m_vscroll->set_value(maxvalue);

				break;
			}
		}
	}
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
xui_method_explain(xui_timeview, choose_else,				xui_component*				)( const xui_vector<s32>& pt )
{
	xui_component* component = xui_control::choose_else(pt);
	if (component == NULL)
	{
		xui_rect2d<s32> rt = get_renderrt() + m_render.get_pt();
		if (rt.was_inside(pt))
		{
			xui_vector<s32> relative;
			s32 scrollx = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
			s32 scrolly = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
			relative = pt - m_render.get_pt() + xui_vector<s32>(	  0, scrolly);
			if (m_tldelete->choose(relative))
				return m_tldelete;
			if (component = m_timetree->choose(relative))
				return component;

			relative = pt - m_render.get_pt() + xui_vector<s32>(scrollx, scrolly);
			if (m_timerect->choose(relative))
				return m_timerect;
			if (m_timehead->choose(relative))
				return m_timehead;

			std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
			for (s32 i = (s32)nodes.size()-1; i >= 0; --i)
			{
				xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
				if (component = timeline->choose(relative))
					return component;
			}
		}
	}

	return component;
}
xui_method_explain(xui_timeview, update_else,				void						)( f32 delta )
{
	xui_control::update_else(delta);

	if (m_timehead->was_enable() && m_timehead->was_visible())
		m_timehead->update(delta);
	if (m_timerect->was_enable() && m_timerect->was_visible())
		m_timerect->update(delta);
	if (m_timetree->was_enable() && m_timetree->was_visible())
		m_timetree->update(delta);
	if (m_tldelete->was_enable() && m_tldelete->was_visible())
		m_tldelete->update(delta);

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		if (timeline->was_enable() && timeline->was_visible())
			timeline->update(delta);
	}
}
xui_method_explain(xui_timeview, render_else,				void						)( void )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();

	xui_rect2d<s32> temprect = xui_container::get_renderrtins() + get_screenpt();
	temprect.ay += m_timegrad->get_renderh();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(temprect));
	if (m_timetree->was_visible())
		m_timetree->render();
	if (m_tldelete->was_visible())
		m_tldelete->render();
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
	xui_convas::get_ins()->set_cliprect(currrect);
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		if (currrect.get_inter(timeline->get_renderrtabs()).was_valid())
			timeline->render();
	}
	if (m_timehead->was_visible())
		m_timehead->render();
	if (m_timerect->was_visible())
		m_timerect->render();
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_control::render_else();
}

/*
//override
*/
xui_method_explain(xui_timeview, on_invalid,				void						)( xui_method_args& args )
{
	m_allframe.clear();
	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	for (u32 i = 0; i < nodes.size(); ++i)
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
	sz.w = xui_max(m_keyspace*finalframe+10,	rt.get_w());
	sz.h = xui_max(m_timetree->get_renderh(),	rt.get_h());

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
		update_scroll();
	}
}
xui_method_explain(xui_timeview, on_perform,				void						)( xui_method_args& args )
{
	xui_control::on_perform(args);

	//vscroll
	if (m_vscroll)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		m_vscroll->on_perform_x(rt.bx);
		m_vscroll->on_perform_y(rt.ay);
		m_vscroll->on_perform_h(rt.get_h());
	}
	//hscroll
	if (m_hscroll)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		rt.ax += m_timegrad->get_borderrt().ax;
		m_hscroll->on_perform_x(rt.ax);
		m_hscroll->on_perform_y(rt.by);
		m_hscroll->on_perform_w(rt.get_w());
	}

	xui_vector<s32> pt(0);
	xui_vector<s32> sz(0);
	pt.x  = m_border.ax;
	pt.y  = m_border.ay;
	m_timetool->on_perform_pt(pt);

	pt.x  = m_border.ax;
	pt.y  = m_border.ay + m_timetool->get_renderh();
	sz.w  = xui_max(m_timetool->get_renderw(), m_timetree->get_renderw());
	sz.h  = m_timetree->get_renderh();
	m_timetree->on_perform_pt(pt);
	m_timetree->on_perform_sz(sz);

	pt.x  = m_border.ax + m_timetree->get_renderw();
	pt.y  = m_border.ay;
	sz.w  = get_renderrtins().get_w();
	sz.h  = m_timegrad->get_renderh();
	m_timegrad->on_perform_pt(pt);
	m_timegrad->on_perform_sz(sz);

	pt.y  = m_border.ay + m_timegrad->get_renderh();
	sz.w  = get_clientw();
	sz.h  = m_timetree->get_lineheight();
	m_timehead->on_perform_pt(pt);
	m_timehead->on_perform_sz(sz);

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		pt.y += m_timetree->get_lineheight();

		xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
		timeline->on_perform_pt(pt);
		timeline->on_perform_sz(sz);
	}

	pt.x  = m_border.ax;
	pt.y  = get_renderh() - m_border.by - m_ksslider->get_renderh();
	m_ksslider->on_perform_pt(pt);

	pt.x += m_ksslider->get_renderw() + 40;
	m_fpstring->on_perform_pt(pt);
	pt.x += m_fpstring->get_renderw();
	m_fpnumber->on_perform_pt(pt);
}
xui_method_explain(xui_timeview, on_vertvalue,				void						)( xui_method_args& args )
{
	xui_container::on_vertvalue(args);
	m_timetree->set_scrolly(m_vscroll->get_value());
	m_tldelete->set_scrolly(m_vscroll->get_value());
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

	xui_rect2d<s32> temprt = xui_container::get_renderrtins() + get_screenpt();
	temprt.ay += m_timegrad->get_renderh();
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();

	std::vector<xui_treenode*> nodevec = m_timetree->get_entirenode(false);
	u32 selectedindex = -1;
	for (u32 i = 0; i < nodevec.size(); ++i)
	{
		if (nodevec[i]->was_selected())
		{
			selectedindex = i;
			break;
		}
	}

	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(temprt));
	s32 line_start = (m_vscroll == NULL) ? 0 : m_vscroll->get_value()/m_timetree->get_lineheight();
	s32 line_final = line_start + get_renderrtins().get_h() / m_timetree->get_lineheight() + 1;
	for (s32 i = line_start; i <= line_final; ++i)
	{
		xui_colour  finalcolor;
		if (i == 0) finalcolor = xui_colour(1.0f, 100.0f/255.0f);
		else        finalcolor = (i == selectedindex+1) ? xui_treeview::nodeselect_color : xui_colour(1.0f,  80.0f/255.0f);

		rt.set_pt(xui_vector<s32>(pt.x, pt.y+i*m_timetree->get_lineheight()+1));
		xui_convas::get_ins()->fill_rectangle(rt, color*finalcolor);
	}

	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtins()+get_screenpt()));
	pt = m_timegrad->get_screenpt() + xui_vector<s32>(m_timegrad->get_borderrt().ax, m_timegrad->get_renderh());
	s32 value = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	s32 grad_start = value / m_keyspace;
	s32 grad_final = grad_start + m_timegrad->get_renderw() / m_keyspace + 1;
	for (s32 i = xui_max(0, grad_start); i <= grad_final; ++i)
	{
		if (i%m_keylarge == 0 || i == m_curframe)
		{
			xui_vector<s32> p1 = pt + xui_vector<s32>(i*m_keyspace-value, 0);
			xui_vector<s32> p2 = p1 + xui_vector<s32>(0, get_renderrtins().get_h());
			xui_colour finalcolor = (i == m_curframe)
				? color * xui_colour(1.0f, 0.0f, 1.0f, 1.0f)
				: color * m_backcolor;

			xui_convas::get_ins()->draw_line(p1, p2, finalcolor);
		}
	}

	if (m_allframe.size() > 0)
	{
		xui_keyframe_map::reverse_iterator itor = m_allframe.rbegin();
		s32 grad = (*itor).first;
		xui_vector<s32> p1 = pt + xui_vector<s32>(grad*m_keyspace-value, 0);
		xui_vector<s32> p2 = p1 + xui_vector<s32>(0, get_renderrtins().get_h());
		xui_convas::get_ins()->draw_line(p1, p2, color*xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
	}

	xui_convas::get_ins()->set_cliprect(cliprect);
}

/*
//method
*/
xui_method_explain(xui_timeview, create_line,				xui_timeline*				)( xui_treenode* node, xui_timedata* data )
{
	xui_timeline* line = new xui_timeline(data, this);
	line->xm_updateself	+= new xui_method_member<xui_method_update, xui_timeview>(this, &xui_timeview::on_timeviewdraghorz);
	line->xm_updateself	+= new xui_method_member<xui_method_update, xui_timeview>(this, &xui_timeview::on_timeviewdragvert);
	line->xm_mouserise	+= new xui_method_member<xui_method_mouse,  xui_timeview>(this, &xui_timeview::on_timelinemouseclick);
	line->xm_keybddown	+= new xui_method_member<xui_method_keybd,	xui_timeview>(this, &xui_timeview::on_timelinekeybddown);
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
		line->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(line);
	}

	invalid();
}

/*
//event
*/
xui_method_explain(xui_timeview, on_timelinekeybddown,		void						)( xui_component* sender, xui_method_keybd&			args )
{
	if (args.kcode == KEY_DELETE)
	{
		xui_method_args   del_args ;
		xm_delframe(this, del_args);

		std::vector<xui_treenode*> nodes = m_timetree->get_upmostnodearray();
		for (u32 i = 0; i < nodes.size(); ++i)
		{
			xui_timeline* timeline = (xui_timeline*)nodes[i]->get_data();
			timeline->use_linkdata();
		}

		invalid();
	}
}
xui_method_explain(xui_timeview, on_timelinemouseclick,		void						)( xui_component* sender, xui_method_mouse&			args )
{
	xm_linemouseclick(this, args);
}
xui_method_explain(xui_timeview, on_timetreenonfocus,		void						)( xui_component* sender, xui_method_args&			args )
{
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || focusctrl->was_ancestor(m_timetree) == false)
		m_tldelete->set_visible(false);
}
xui_method_explain(xui_timeview, on_timetreemouseclick,		void						)( xui_component* sender, xui_method_mouse&			args )
{
	std::vector<xui_treenode*> vec = m_timetree->get_selectednode();
	if (vec.size())
	{
		xui_treenode* selectednode = vec.front();
		xui_rect2d<s32> rt = selectednode->get_renderrtabs() + m_tldelete->get_scrollpt() - get_screenpt();
		xui_method_ptrcall(m_tldelete, on_perform_x	)(rt.bx  - m_tldelete->get_renderw() - 4);
		xui_method_ptrcall(m_tldelete, on_perform_y	)(rt.ay  + m_timetree->get_lineheight()/2 - m_tldelete->get_renderh()/2);
		xui_method_ptrcall(m_tldelete, set_visible	)(true);
	}
	else
	{
		xui_method_ptrcall(m_tldelete, set_visible	)(false);
	}
}
xui_method_explain(xui_timeview, on_timetreesetclientsz,	void						)( xui_component* sender, xui_method_args&			args )
{
	xui_vector<s32> sz = m_timetree->get_clientsz();
	xui_rect2d<s32> rt = m_timetree->get_borderrt();
	sz.w += (rt.ax+rt.bx);
	sz.h += (rt.ay+rt.by+m_timetree->get_lineheight());
	m_timetree->set_rendersz(sz);
}
xui_method_explain(xui_timeview, on_timetreeupdateself,		void						)( xui_component* sender, xui_method_update&		args )
{
	if (m_timetree->has_catch())
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		s32 lineheight = m_timetree->get_lineheight();
		if (xui_abs(pt.y-dw.y) < lineheight/2)
			return;

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+lineheight/2)
			scroll_value = -lineheight/2;
		if (pt.y < rt.by && pt.y > rt.by-lineheight/2)
			scroll_value =  lineheight/2;

		if (m_vscroll && scroll_value != 0)
		{
			m_vscroll->set_value(m_vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_timetreeoverdrop,		void						)( xui_component* sender, xui_method_treedragdrop&	args )
{
	m_tldelete->set_visible(false);
}
xui_method_explain(xui_timeview, on_timetreeinvalid,		void						)( xui_component* sender, xui_method_args&			args )
{
	s32 width = 0;
	for (u32 i = 0; i < m_timetree->get_columninfocount(); ++i)
		width += m_timetree->get_columninfo(i).size;

	std::vector<xui_treenode*> nodes = m_timetree->get_entirenode(false);
	xui_vector<s32> sz(width, m_timetree->get_lineheight() * nodes.size());
	xui_rect2d<s32> rt = m_timetree->get_renderrtins();
	sz.w = xui_max(sz.w, rt.get_w());

	if (m_timetree->get_clientsz() != sz)
	{
		m_timetree->set_clientsz(sz);
	}
	else
	{
		m_timetree->perform();
	}
}
xui_method_explain(xui_timeview, on_timeviewdraghorz,		void						)( xui_component* sender, xui_method_update&		args )
{
	if (sender->has_catch())
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.x-dw.x) < 10)
			return;

		s32 scroll_value =  0;
		if (pt.x > rt.ax && pt.x < rt.ax+10)
			scroll_value = -m_keyspace;
		if (pt.x < rt.bx && pt.x > rt.bx-10)
			scroll_value =  m_keyspace;

		if (m_hscroll && scroll_value != 0)
		{
			m_hscroll->set_value(m_hscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_timeviewdragvert,		void						)( xui_component* sender, xui_method_update&		args )
{
	if (sender->has_catch())
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.y-dw.y) < m_timetree->get_lineheight()/2)
			return;

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+10)
			scroll_value = -m_timetree->get_lineheight()/2;
		if (pt.y < rt.by && pt.y > rt.by-10)
			scroll_value =  m_timetree->get_lineheight()/2;

		if (m_vscroll && scroll_value != 0)
		{
			m_vscroll->set_value(m_vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_timerectdraghorz,		void						)( xui_component* sender, xui_method_update&		args )
{
	if (sender->has_catch())
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.x-dw.x) < 10)
			return;

		s32 scroll_value =  0;
		if (pt.x > rt.ax && pt.x < rt.ax+10)
			scroll_value = -m_keyspace;
		if (pt.x < rt.bx && pt.x > rt.bx-10)
			scroll_value =  m_keyspace;

		if (m_hscroll && scroll_value != 0)
		{
			m_hscroll->set_value(m_hscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_timeview, on_kssliderscroll,			void						)( xui_component* sender, xui_method_args&			args )
{
	set_keyspace(m_ksslider->get_value());
}
xui_method_explain(xui_timeview, on_fpnumberrenderself,		void						)( xui_component* sender, xui_method_args&			args )
{
	xui_rect2d<s32> rt = m_fpnumber->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by-2), xui_vector<s32>(rt.bx, rt.by-2), xui_colour::white);
}
xui_method_explain(xui_timeview, on_fpnumbermousemove,		void						)( xui_component* sender, xui_method_mouse&			args )
{
	if (m_fpnumber->has_catch())
	{
		xui_vector<s32> delta = xui_desktop::get_ins()->get_mousemove();

		s32 frame = 0;
	 	std::wstringstream temp(m_fpnumber->get_text().c_str());
		temp >> frame;
		frame += delta.x;
		frame  = xui_min(frame, 60);
		frame  = xui_max(frame,  0);
		std::wstringstream text;
		text << frame;
		m_fpnumber->ini_drawer(text.str());

		xui_timer* timer = m_timetool->get_playtimer();
		timer->set_interval(1.0f/(f32)frame);
	}
}
xui_method_explain(xui_timeview, on_tldeleterenderself,		void						)( xui_component* sender, xui_method_args&			args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->fill_circle(center, 8, xui_colour::gray, 0, 360);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(xui_timeview, on_tldeleteclick,			void						)( xui_component* sender, xui_method_args&			args )
{
	xm_dellayer(this, args);
}