#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_timedata.h"
#include "xui_treenode.h"
#include "xui_timeview.h"
#include "xui_timehead.h"
#include "xui_timeline.h"

/*
//constructor
*/
xui_create_explain(xui_timeline)( xui_timeview* timeview, xui_timedata* linkdata )
: xui_control("", xui_rect2d<s32>(0))
{
	m_type     += "timeline";
	m_border	= xui_rect2d<s32>(5, 2, 0, 2);
	m_parent    = timeview;
	m_backcolor = xui_colour(0.0f);
	m_downrela  = xui_vector<s32>(0);
	m_currrela	= xui_vector<s32>(0);
	m_linkdata	= linkdata;
	m_linkdata->set_line(this);
}

/*
//method
*/
xui_method_explain(xui_timeline, get_linkdata,		xui_timedata*				)( void )
{
	return m_linkdata;
}

/*
//selected frame
*/
xui_method_explain(xui_timeline, was_selafter,		bool						)( s32 frame ) const
{
	for (u32 i = 0; i < m_selframe.size(); ++i)
	{
		if (frame > m_selframe[i])
			return true;
	}

	return false;
}
xui_method_explain(xui_timeline, has_selframe,		bool						)( s32 frame ) const
{
	return std::find(m_selframe.begin(), m_selframe.end(), frame) != m_selframe.end();
}
xui_method_explain(xui_timeline, has_selframe,		bool						)( void ) const
{
	return m_selframe.size() > 0;
}
xui_method_explain(xui_timeline, get_selframe,		const std::vector<s32>&		)( void ) const
{
	return m_selframe;
}
xui_method_explain(xui_timeline, set_selframe,		void						)( const std::vector<s32>& selframe )
{
	m_selframe = selframe;
}
xui_method_explain(xui_timeline, add_selframe,		bool						)( s32 start, s32 final )
{
	bool hasframe = false;

	std::vector<s32> allframe = m_linkdata->get_allframe();
	for (u32 i = 0; i < allframe.size(); ++i)
	{
		if (allframe[i] < start || allframe[i] > final)
			continue;

		hasframe = true;
		if (std::find(m_selframe.begin(), m_selframe.end(), allframe[i]) == m_selframe.end())
			m_selframe.push_back(allframe[i]);
	}

	return hasframe;
}
xui_method_explain(xui_timeline, del_selframe,		void						)( s32 start, s32 final )
{
	for (s32 i = (s32)m_selframe.size()-1; i >= 0; --i)
	{
		if (m_selframe[i] >= start && m_selframe[i] <= final)
			m_selframe.erase(m_selframe.begin()+i);
	}
}

/*
//timeline
*/
xui_method_explain(xui_timeline, get_rootline,		xui_timeline*				)( void )
{
	return (xui_timeline*)m_linkdata->get_node()->get_rootnode()->get_data();
}
xui_method_explain(xui_timeline, get_timelinetotal, void						)( std::vector<xui_timeline*>& lines, bool total )
{
	std::vector<xui_treenode*> nodes;
	m_linkdata->get_node()->get_leafnodetotal(nodes, total);
	xui_vecptr_addloop(nodes)
	{
		lines.push_back((xui_timeline*)nodes[i]->get_data());
	}
}
xui_method_explain(xui_timeline, get_timelinecount, u32							)( void ) const
{
	return m_linkdata->get_node()->get_leafnodecount();
}
xui_method_explain(xui_timeline, get_timelinearray, std::vector<xui_timeline*>	)( void ) const
{
	std::vector<xui_timeline*> lines;
	for (u32 i = 0; i < m_linkdata->get_node()->get_leafnodecount(); ++i)
	{
		lines.push_back((xui_timeline*)m_linkdata->get_node()->get_leafnode(i)->get_data());
	}

	return lines;
}
xui_method_explain(xui_timeline, get_timeline,		xui_timeline*				)( u32 index )
{
	return (xui_timeline*)m_linkdata->get_node()->get_leafnode(index)->get_data();
}
xui_method_explain(xui_timeline, add_timeline,		xui_timeline*				)( u32 index, xui_timedata* data )
{
	xui_timeview* view = (xui_timeview*)m_parent;
	xui_treenode* node = m_linkdata->get_node()->add_leafnode(index, data);
	xui_timeline* line = view->create_line(node, data);

	return line;
}
xui_method_explain(xui_timeline, del_timeline,		void						)( xui_timeline* line )
{
	xui_timeview* view = (xui_timeview*)m_parent;
	xui_treenode* node = line->get_linkdata()->get_node();
	m_linkdata->get_node()->del_leafnode(node);
	view->delete_line(line);
}

/*
//override
*/
xui_method_explain(xui_timeline, on_mousedown,		void						)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	xui_timeview* timeview = (xui_timeview*)m_parent;
	if (args.mouse == MB_L)
	{
		m_downrela    = args.point - get_screenpt();
		m_downrela.x -= m_border.ax;

		s32 frame = hit_frame(m_downrela.x);
		s32 inner = xui_abs(m_downrela.x - frame*timeview->get_keyspace());
		if (m_linkdata->has_keyframe(frame) && inner < 5)
		{
			std::vector<s32>::iterator itor = std::find(
				m_selframe.begin(), 
				m_selframe.end(), 
				frame);

			if (itor == m_selframe.end())
			{
				if (args.ctrl == false)
					timeview->non_selectedline();

				m_selframe.push_back(frame);
				timeview->set_dragmode(args.shift ? TIMEDRAG_SELECT_AND_AFTER : TIMEDRAG_SELECT);
				timeview->set_dragtime(frame);

				xui_method_args other_args;
				timeview->xm_selectedchange(timeview, other_args);
			}
			else
			if (args.ctrl)
			{
				m_selframe.erase(itor);

				xui_method_args other_args;
				timeview->xm_selectedchange(timeview, other_args);
			}
		}
		else
		{
			if (args.ctrl == false)
				timeview->non_selectedline();
		}
	}
}
xui_method_explain(xui_timeline, on_mousemove,		void						)( xui_method_mouse& args )
{
	xui_control::on_mousemove(args);
	if (g_desktop->get_catchctrl() == this)
	{
		xui_timeview* timeview = (xui_timeview*)m_parent;
		xui_vector<s32> pt = get_screenpt();

		m_currrela    = args.point - pt;
		m_currrela.x -= m_border.ax;
		s32 downframe = hit_frame(m_downrela.x);
		s32 currframe = hit_frame(m_currrela.x);
		s32 downinner = xui_abs(m_downrela.x - downframe*timeview->get_keyspace());

		if (m_linkdata->has_keyframe(downframe) && downinner < 5)
		{
			timeview->set_droptime(currframe);
		}
		else
		{
			s32 dist = pt.y - timeview->get_timehead()->get_screenpt().y - timeview->get_timehead()->get_renderh();
			s32 downdist = m_downrela.y + dist;
			s32 currdist = m_currrela.y + dist;

			s32 h = get_renderh();
			s32 downlayer = downdist / h;
			s32 currlayer = currdist < 0 ? -1 : (currdist/h);
			timeview->set_rectshow(
				xui_min(downlayer, currlayer),
				xui_max(downlayer, currlayer),
				xui_min(downframe, currframe),
				xui_max(downframe, currframe));
		}
	}
}
xui_method_explain(xui_timeline, on_mouserise,		void						)( xui_method_mouse& args )
{
	xui_control::on_mouserise(args);
	xui_timeview* timeview = (xui_timeview*)m_parent;
	if (args.mouse == MB_L)
	{
		m_currrela    = args.point - get_screenpt();
		m_currrela.x -= m_border.ax;
		s32 downframe = hit_frame(m_downrela.x);
		s32 currframe = hit_frame(m_currrela.x);
		s32 downinner = xui_abs(m_downrela.x - downframe*timeview->get_keyspace());

		if (downframe == currframe)
		{
			timeview->set_curframe(currframe);
		}
		else
		{
			if (m_linkdata->has_keyframe(downframe) && downinner < 5)
			{
				xui_method_args other_args;
				timeview->xm_selecteddrag(timeview, other_args);
			}
			else
			{
				timeview->set_rectrise();
			}
		}

		timeview->set_dragmode(TIMEDRAG_NONE);
		timeview->set_dragtime(-1);
		timeview->set_droptime(-1);
	}
}
xui_method_explain(xui_timeline, on_renderself,		void						)( xui_method_args&  args )
{
	xui_control::on_renderself(args);

	struct keystyle_drawdata
	{
		s32			keyframe;
		u08			keystyle;
		bool		selected;

		static bool compare( const keystyle_drawdata& data1, const keystyle_drawdata& data2 )
		{
			return data1.keyframe < data2.keyframe;
		}
	};
	std::vector<keystyle_drawdata> drawdata;

	xui_timeview* timeview = (xui_timeview*)m_parent;
	s32 allfirst = timeview->get_selectedallfirst();
	xui_keystyle_map allstyle = m_linkdata->get_allstyle();
	for (xui_keystyle_map::iterator itor = allstyle.begin(); itor != allstyle.end(); ++itor)
	{
		keystyle_drawdata data;
		data.keyframe = (*itor).first;
		data.keystyle = (*itor).second;
		data.selected = std::find(m_selframe.begin(), m_selframe.end(), data.keyframe) != m_selframe.end();
		if (timeview->get_droptime() != -1)
		{
			s32 delta_time = timeview->get_droptime() - timeview->get_dragtime();
			if (data.selected)
			{
				data.keyframe += delta_time;
			}
			else
			if (timeview->get_dragmode() == TIMEDRAG_SELECT_AND_AFTER && allfirst != -1 && data.keyframe > allfirst)
			{
				data.keyframe += delta_time;
			}
			else
			if (timeview->get_dragmode() == TIMEDRAG_SELECT_AND_AFTER && allfirst == -1 && was_selafter(data.keyframe))
			{
				data.keyframe += delta_time;
			}
		}

		drawdata.push_back(data);
	}

	if (drawdata.empty())
		return;

	xui_colour   color = get_vertexcolor();
	xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();

	s32 hoverframe = -1;
	if (g_desktop->get_hoverctrl() == this || 
		g_desktop->get_hoverctrl() == timeview->get_timehead())
	{
		xui_vector<s32> pt = g_desktop->get_mousecurr() - get_screenpt();
		pt.x -= m_border.ax;
		s32 frame = hit_frame(pt.x);
		s32 inner = xui_abs(pt.x - frame*timeview->get_keyspace());
		if (inner < 5)
			hoverframe = frame;
	}

	std::sort(drawdata.begin(), drawdata.end(), keystyle_drawdata::compare);
	for (u32 i = 0; i < drawdata.size(); ++i)
	{
		s32 frame = drawdata[i].keyframe;

		xui_rect2d<s32> temp = rt;
		temp.oft_x(timeview->get_keyspace()*frame - 2);
		temp.set_w(4);

		g_convas->fill_rectangle(temp, color*m_linkdata->get_keycolor());

		xui_colour sidecolor(1.0f, 0.0f, 0.0f, 0.0f);
		if (drawdata[i].selected)
		{
			sidecolor = xui_colour(1.0f, 0.0f, 1.0f, 1.0f);
		}
		else
		if (frame == hoverframe || timeview->was_rectmove(this, frame))
		{
			sidecolor = xui_colour(1.0f, 0.0f, 1.0f, 1.0f);
		}

		g_convas->draw_rectangle(temp, color*sidecolor);
	}

	for (u32 i = 0; i < drawdata.size()-1; ++i)
	{
		if (drawdata[i].keystyle == KS_STATIC)
			continue;

		s32 currframe = drawdata[i  ].keyframe;
		s32 nextframe = drawdata[i+1].keyframe;
		if (currframe == nextframe)
			continue;

		xui_colour linecolor = drawdata[i].selected
			? color * xui_colour(1.0f, 0.0f, 1.0f, 1.0f)
			: color * xui_colour(1.0f, 0.7f, 0.7f, 0.7f);

		xui_vector<s32> p1 = rt.get_pt() + xui_vector<s32>(timeview->get_keyspace()*currframe+3, rt.get_sz().h/2);
		xui_vector<s32> p2 = rt.get_pt() + xui_vector<s32>(timeview->get_keyspace()*nextframe-3, rt.get_sz().h/2);
		if (drawdata[i].keystyle == KS_BEZIER && p2.x-p1.x > 16)
		{
			xui_rect2d<s32> temp(0, 0, 12, 12);
			temp.set_pt(p1);
			g_convas->draw_arc(temp, linecolor, -180.0f, 90.0f, 1.0f);
			p1.x += 6;
			p2.x -= 6;
			g_convas->draw_line(p1, p2, linecolor);
			temp.set_pt(xui_vector<s32>(p2.x-6, p2.y-12));
			g_convas->draw_arc(temp, linecolor,    0.0f, 90.0f, 1.0f);
		}
		else
		{
			g_convas->draw_line(p1, p2, linecolor);
		}
	}
}

/*
//method
*/
xui_method_explain(xui_timeline, hit_frame,			s32							)( s32 x )
{
	xui_timeview* timeview = (xui_timeview*)m_parent;
	s32 space = timeview->get_keyspace();
	s32 frame = x / space;
	if (x % space > space/2)
		frame++;

	return frame;
}