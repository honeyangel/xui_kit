#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_timeview.h"
#include "xui_timeline.h"
#include "xui_timedata.h"
#include "xui_timehead.h"

xui_implement_rtti(xui_timehead, xui_control)

xui_timehead::xui_timehead( xui_timeview* timeview )
: xui_control(xui_vector<s32>(0))
{
	m_parent	= timeview;
	m_border	= xui_rect2d<s32>(5, 2, 0, 2);
	m_downrela  = xui_vector<s32>(0);
	m_currrela  = xui_vector<s32>(0);
}

void xui_timehead::on_mousedown( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == k_mb_left)
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);

		m_downrela    = args.point - get_screenpt();
		m_downrela.x -= m_border.ax;

		s32 frame = hit_frame(m_downrela.x);
		s32 inner = xui_abs(m_downrela.x - frame*timeview->get_keyspace());
		xui_keyframe_map allframe = timeview->get_allframe();
		if (allframe.find(frame) != allframe.end() && inner < 5)
		{
			timeview->set_dragtime(frame);
			timeview->set_dragmode(args.shift ? k_timedrag_select_and_after : k_timedrag_select);

			xui_keyframe_map::iterator itor = allframe.find(frame);
			if (timeview->was_selectedall((*itor).second, (*itor).first) == false)
			{
				if (args.ctrl == false)
					timeview->non_selectedline();

				timeview->set_selectedline(frame, frame, true);
			}
			else
			if (args.ctrl)
			{
				timeview->set_selectedline(frame, frame, false);
			}
		}
		else
		{
			if (args.ctrl == false)
				timeview->non_selectedline();
		}
	}
}

void xui_timehead::on_mousemove( xui_method_mouse& args )
{
	xui_control::on_mousemove(args);
	if (has_catch())
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
		m_currrela    = args.point - get_screenpt();
		m_currrela.x -= m_border.ax;

		s32 downframe = hit_frame(m_downrela.x);
		s32 currframe = hit_frame(m_currrela.x);
		s32 downinner = xui_abs(m_downrela.x - downframe*timeview->get_keyspace());

		xui_keyframe_map allframe = timeview->get_allframe();
		if (allframe.find(downframe) != allframe.end() && downinner < 5)
		{
			timeview->set_droptime(currframe);
		}
		else
		{
			s32 h = get_renderh();
			s32 currdist  = m_currrela.y - h;
			s32 downlayer = -1;
			s32 currlayer = currdist < 0 ? -1 : (currdist/h);
			timeview->set_rectshow(
				xui_min(downlayer, currlayer),
				xui_max(downlayer, currlayer),
				xui_min(downframe, currframe),
				xui_max(downframe, currframe));
		}
	}
}

void xui_timehead::on_mouserise( xui_method_mouse& args )
{
	xui_control::on_mouserise(args);
	if (args.mouse == k_mb_left)
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);

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
			xui_keyframe_map allframe = timeview->get_allframe();
			if (allframe.find(downframe) != allframe.end() && downinner < 5)
			{
				xui_method_args other_args;
				timeview->xm_selecteddrag(timeview, other_args);
				timeview->invalid();
			}
			else
			{
				timeview->set_rectrise();
			}
		}

		timeview->set_dragmode(k_timedrag_none);
		timeview->set_dragtime(-1);
		timeview->set_droptime(-1);
	}
}

void xui_timehead::on_renderself( xui_method_args& args )
{
	xui_control::on_renderself(args);

	struct keyframe_drawdata
	{
		s32			keyframe;
		xui_colour	keycolor;
		bool		selected;

		static bool			compare( const keyframe_drawdata& data1, const keyframe_drawdata& data2 )
		{
			return data1.keyframe < data2.keyframe;
		}
		static xui_colour	collect( const std::vector<xui_timeline*>& lines, bool only_selected = false, s32 frame = -1 )
		{
			xui_colour color(0.0f);
			for (u32 i = 0; i < lines.size(); ++i)
			{
				if (only_selected && lines[i]->has_selframe(frame) == false)
					continue;

				if (color == xui_colour(0.0f))
				{
					color  = lines[i]->get_linkdata()->get_keycolor();
				}
				else
				if (color != lines[i]->get_linkdata()->get_keycolor())
				{
					color  = xui_colour(1.0f);
					break;
				}
			}

			return color;
		}
	};
	std::vector<keyframe_drawdata> drawdata;

	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	xui_keyframe_map allframe = timeview->get_allframe();
	s32 allfirst = timeview->get_selectedallfirst();
	for (xui_keyframe_map::iterator itor = allframe.begin(); itor != allframe.end(); ++itor)
	{
		keyframe_drawdata data;
		data.keyframe = (*itor).first;
		data.keycolor = keyframe_drawdata::collect((*itor).second);
		data.selected = timeview->was_selectedall((*itor).second, (*itor).first);
		if (timeview->get_droptime() != -1)
		{
			s32 delta_time = timeview->get_droptime() - timeview->get_dragtime();
			if (data.selected)
			{
				data.keyframe += delta_time;
			}
			else
			{
				if (timeview->has_selectedline((*itor).second, (*itor).first))
				{
					//add other
					keyframe_drawdata other_data;
					other_data.keyframe = data.keyframe + delta_time;
					other_data.keycolor = keyframe_drawdata::collect((*itor).second, true, (*itor).first);
					other_data.selected = false;
					drawdata.push_back(other_data);
				}
			}
		}

		drawdata.push_back(data);
	}

	if (drawdata.empty())
		return;

	xui_colour   color = get_vertexcolor();
	xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();

	s32 hoverframe = -1;
	if (was_hover())
	{
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr() - get_screenpt();
		pt.x -= m_border.ax;
		s32 frame = hit_frame(pt.x);
		s32 inner = xui_abs(pt.x - frame*timeview->get_keyspace());
		if (inner < 5)
			hoverframe = frame;
	}

	std::sort(drawdata.begin(), drawdata.end(), keyframe_drawdata::compare);
	for (u32 i = 0; i < drawdata.size(); ++i)
	{
		s32 frame = drawdata[i].keyframe;

		xui_rect2d<s32> temp = rt;
		temp.oft_x(timeview->get_keyspace()*frame - 2);
		temp.set_w(4);
		xui_canvas::get_ins()->fill_rectangle(temp, color*drawdata[i].keycolor);

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
		xui_canvas::get_ins()->draw_rectangle(temp, color*sidecolor);
	}
}

s32 xui_timehead::hit_frame( s32 x )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	s32 space = timeview->get_keyspace();
	s32 frame = x / space;
	if (x % space > space/2)
		frame++;

	return frame;
}