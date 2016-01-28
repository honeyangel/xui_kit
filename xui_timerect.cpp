#include "xui_desktop.h"
#include "xui_timeview.h"
#include "xui_timehead.h"
#include "xui_treeview.h"
#include "xui_timerect.h"

xui_implement_rtti(xui_timerect, xui_control);

/*
//constructor
*/
xui_create_explain(xui_timerect)( xui_timeview* timeview )
: xui_control(xui_vector<s32>(0))
{
	m_parent		= timeview;
	m_cursor		= CURSOR_HAND;
	m_visible		= false;
	m_backcolor		= xui_colour(0.5f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	m_drawcolor		= true;
	m_sidestyle		= SIDESTYLE_S;
	m_sidecolor		= xui_colour(1.0f, 0.0f, 0.9f, 0.9f);
	m_downrela		= xui_vector<s32>(0);
	m_currrela		= xui_vector<s32>(0);
	m_startlayer	= 0;
	m_finallayer	= 0;
	m_startframe	= 0;
	m_finalframe	= 0;
}

/*
//method
*/
xui_method_explain(xui_timerect, get_range,		xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> result;
	result.ax = m_startframe;
	result.bx = m_finalframe;
	result.ay = m_startlayer;
	result.by = m_finallayer;
	return result;
}
xui_method_explain(xui_timerect, set_range,		void			)( s32 startlayer, s32 finallayer, s32 startframe, s32 finalframe )
{
	m_startlayer = startlayer;
	m_finallayer = finallayer;
	m_startframe = startframe;
	m_finalframe = finalframe;

	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	xui_timehead* timehead = timeview->get_timehead();
	xui_rect2d<s32> rt;
	rt.ax = timehead->get_renderx() + timehead->get_borderrt().ax + startframe*timeview->get_keyspace();
	rt.bx = timehead->get_renderx() + timehead->get_borderrt().ax + finalframe*timeview->get_keyspace();
	rt.ay = timehead->get_rendery() + timehead->get_renderh()     + startlayer*timeview->get_timetree()->get_lineheight();
	rt.by = timehead->get_rendery() + timehead->get_renderh()     + finallayer*timeview->get_timetree()->get_lineheight() + timeview->get_timetree()->get_lineheight();
	on_perform_pt(rt.get_pt());
	on_perform_sz(rt.get_sz());
	set_visible  (rt.was_valid());
}
xui_method_explain(xui_timerect, set_rangemove,	void			)( s32 delta )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	xui_timehead* timehead = timeview->get_timehead();
	xui_rect2d<s32> rt;
	rt.ax = timehead->get_renderx() + timehead->get_borderrt().ax + m_startframe*timeview->get_keyspace() + delta*timeview->get_keyspace();
	rt.bx = timehead->get_renderx() + timehead->get_borderrt().ax + m_finalframe*timeview->get_keyspace() + delta*timeview->get_keyspace();
	rt.ay = timehead->get_rendery() + timehead->get_renderh()     + m_startlayer*timeview->get_timetree()->get_lineheight();
	rt.by = timehead->get_rendery() + timehead->get_renderh()     + m_finallayer*timeview->get_timetree()->get_lineheight() + timeview->get_timetree()->get_lineheight();
	on_perform_pt(rt.get_pt());
	on_perform_sz(rt.get_sz());
}

/*
//callback
*/
xui_method_explain(xui_timerect, on_nonfocus,	void			)( xui_method_args&  args )
{
	xui_control::on_nonfocus(args);
	set_visible(false);
}
xui_method_explain(xui_timerect, on_mousedown,	void			)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);

		m_downrela    = args.point - timeview->get_timehead()->get_screenpt();
		m_downrela.x -= timeview->get_timehead()->get_borderrt().ax;

		s32 frame = hit_frame(m_downrela.x);
		timeview->set_dragmode(args.shift ? TIMEDRAG_SELECT_AND_AFTER : TIMEDRAG_SELECT);
		timeview->set_dragtime(frame);
	}
}
xui_method_explain(xui_timerect, on_mousemove,	void			)( xui_method_mouse& args )
{
	xui_control::on_mousemove(args);
	if (has_catch())
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
		xui_timehead* timehead = timeview->get_timehead();

		m_currrela    = args.point - timehead->get_screenpt();
		m_currrela.x -= timehead->get_borderrt().ax;

		s32 downframe = hit_frame(m_downrela.x);
		s32 currframe = hit_frame(m_currrela.x);
		timeview->set_droptime(currframe);

		set_rangemove(currframe-downframe);
	}
}
xui_method_explain(xui_timerect, on_mouserise,	void			)( xui_method_mouse& args )
{
	xui_control::on_mouserise(args);
	if (args.mouse == MB_L)
	{
		xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);

		m_currrela    = args.point - timeview->get_timehead()->get_screenpt();
		m_currrela.x -= timeview->get_timehead()->get_borderrt().ax;
		s32 downframe = hit_frame(m_downrela.x);
		s32 currframe = hit_frame(m_currrela.x);

		if (downframe != currframe)
		{
			xui_method_args other_args;
			timeview->xm_selecteddrag(timeview, other_args);
			set_visible(false);
		}

		timeview->set_dragmode(TIMEDRAG_NONE);
		timeview->set_dragtime(-1);
		timeview->set_droptime(-1);
	}
}

/*
//method
*/
xui_method_explain(xui_timerect, hit_frame,			s32			)( s32 x )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	s32 space = timeview->get_keyspace();
	s32 frame = x / space;
	if (x % space > space/2)
		frame++;

	return frame;
}