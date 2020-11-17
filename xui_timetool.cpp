#include "xui_timer.h"
#include "xui_timermgr.h"
#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_timeview.h"
#include "xui_timetool.h"

xui_implement_rtti(xui_timetool, xui_toolbar)

xui_timetool::xui_timetool( xui_timeview* timeview )
: xui_toolbar(xui_vector<s32>(0, 40))
{
	m_grap		= 8;
	m_border	= xui_rect2d<s32>(8, 8, 0, 8);
	m_parent	= timeview;

	//plus
	m_plus = new xui_button(xui_vector<s32>(24));
	m_plus->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_plus->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	m_plus->set_parent(this);
	m_plus->set_corner(3);
	m_plus->set_drawcolor(true);
	m_plus->set_sidestyle(k_sidestyle_s);
	m_widgetvec.push_back(m_plus);

	//prev
	m_head = new xui_button(xui_vector<s32>(24));
	m_head->set_drawcolor(true);
	m_head->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_head->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	m_prev = new xui_button(xui_vector<s32>(24));
	m_prev->set_drawcolor(true);
	m_prev->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_prev->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);

	xui_linebox* prevline = new xui_linebox(xui_vector<s32>(48, 24), k_flowstyle_h);
	prevline->set_parent(this);
	prevline->set_sidestyle(k_sidestyle_s);
	prevline->set_corner(3);
	prevline->add_linectrl(m_head);
	prevline->add_linectrl(m_prev);
	m_widgetvec.push_back(prevline);


	//play
	m_back = new xui_toggle(xui_vector<s32>(24), k_toggle_button);
	m_back->set_drawcolor(true);
	m_back->xm_toggleclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_back->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	m_play = new xui_toggle(xui_vector<s32>(24), k_toggle_button);
	m_play->set_drawcolor(true);
	m_play->xm_toggleclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_play->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);

	xui_linebox* playline = new xui_linebox(xui_vector<s32>(48, 24), k_flowstyle_h);
	playline->set_parent(this);
	playline->set_sidestyle(k_sidestyle_s);
	playline->set_corner(3);
	playline->add_linectrl(m_back);
	playline->add_linectrl(m_play);
	m_widgetvec.push_back(playline);


	//next
	m_next = new xui_button(xui_vector<s32>(24));
	m_next->set_drawcolor(true);
	m_next->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_next->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	m_tail = new xui_button(xui_vector<s32>(24));
	m_tail->set_drawcolor(true);
	m_tail->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_tail->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);

	xui_linebox* nextline = new xui_linebox(xui_vector<s32>(48, 24), k_flowstyle_h);
	nextline->set_parent(this);
	nextline->set_sidestyle(k_sidestyle_s);
	nextline->set_corner(3);
	nextline->add_linectrl(m_next);
	nextline->add_linectrl(m_tail);
	m_widgetvec.push_back(nextline);


	//loop
	m_loop = new xui_toggle(xui_vector<s32>(24), k_toggle_button);
	m_loop->set_parent(this);
	m_loop->set_corner(3);
	m_loop->set_drawcolor(true);
	m_loop->set_sidestyle(k_sidestyle_s);
	m_loop->xm_toggleclick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_loop->xm_renderself	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	m_widgetvec.push_back(m_loop);


	//timer
	m_playtimer = xui_timermgr::get_ins()->add_timer(this, 1.0f/30.0f, NULL);
	m_playtimer->set_enable(false);
	m_playtimer->xm_tick	 += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_timertick);

	refresh();
}

xui_timetool::~xui_timetool( void )
{
	xui_timermgr::get_ins()->del_timer(m_playtimer);
}

xui_timer* xui_timetool::get_playtimer( void )
{
	return m_playtimer;
}

void xui_timetool::on_buttonclick( xui_component* sender, xui_method_args& args )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	xui_keyframe_map allframe = timeview->get_allframe();
	if (allframe.size() == 0)
		return;

	if (sender == m_plus)
	{
		timeview->xm_addlayer(timeview, args);
	}
	else
	if (sender == m_head) 
	{
		xui_keyframe_map::iterator itor = allframe.begin();
		timeview->set_curframe((*itor).first);
	}
	else 
	if (sender == m_prev)
	{
		for (xui_keyframe_map::reverse_iterator itor = allframe.rbegin(); itor != allframe.rend(); ++itor)
		{
			if ((*itor).first < timeview->get_curframe())
			{
				timeview->set_curframe((*itor).first);
				break;
			}
		}
	}
	else
	if (sender == m_next)
	{
		for (xui_keyframe_map::iterator itor = allframe.begin(); itor != allframe.end(); ++itor)
		{
			if ((*itor).first > timeview->get_curframe())
			{
				timeview->set_curframe((*itor).first);
				break;
			}
		}
	}
	else
	if (sender == m_tail)
	{
		xui_keyframe_map::reverse_iterator itor = allframe.rbegin();
		timeview->set_curframe((*itor).first);
	}
}

void xui_timetool::on_toggleclick( xui_component* sender, xui_method_args& args )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	xui_keyframe_map allframe = timeview->get_allframe();

	if (sender == m_play)
	{
		m_back->ini_toggle(false);
		m_playtimer->set_enable(m_play->was_push());
		xui_keyframe_map::reverse_iterator itor = allframe.rbegin();
		if (m_play->was_push() && timeview->get_curframe() >  (*itor).first)
			timeview->set_curframe(0);
	}
	else 
	if (sender == m_back)
	{
		m_play->ini_toggle(false);
		m_playtimer->set_enable(m_back->was_push());
		xui_keyframe_map::reverse_iterator itor = allframe.rbegin();
		if (m_play->was_push() && timeview->get_curframe() == 0)
			timeview->set_curframe((*itor).first);
	}
	else 
	if (sender == m_loop)
	{}
}

void xui_timetool::on_buttonrenderself( xui_component* sender, xui_method_args&	args )
{
	xui_colour color = sender->get_vertexcolor() * xui_colour::k_white;
	//if (sender->has_catch() || sender->was_hover())
	//	color *= xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

	xui_rect2d<s32> rt = sender->get_renderrtabs() + (sender->has_catch() ? xui_vector<s32>(0, 1) : xui_vector<s32>(0));
	if (sender == m_plus)
	{
		xui_vector<s32> center(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
		xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-8,
			center.y-2,
			center.x+8,
			center.y+2), color);

		xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-2,
			center.y-8,
			center.x+2,
			center.y+8), color);
	}
	else
	if (sender == m_head)
	{
		rt.oft_x(4);
		rt.oft_y(6);
		rt.set_w(rt.get_w()- 8);
		rt.set_h(rt.get_h()-12);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.bx-10,	rt.ay+rt.get_h()/2);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay);
		poly[2] = xui_vector<s32>(rt.bx,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay+rt.get_h()/2);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay);
		poly[2] = xui_vector<s32>(rt.ax+10,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_h()-2);
		xui_canvas::get_ins()->fill_rectangle(rt, color);
	}
	else
	if (sender == m_prev)
	{
		rt.oft_x(5);
		rt.oft_y(5);
		rt.set_w(rt.get_w()-10);
		rt.set_h(rt.get_h()-10);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay+rt.get_h()/2);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay);
		poly[2] = xui_vector<s32>(rt.bx,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_h()-4);
		xui_canvas::get_ins()->fill_rectangle(rt, color);
	}
	else
	if (sender == m_next)
	{
		rt.oft_x(5);
		rt.oft_y(5);
		rt.set_w(rt.get_w()-10);
		rt.set_h(rt.get_h()-10);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay+rt.get_h()/2);
		poly[2] = xui_vector<s32>(rt.ax,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_w()-2);
		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_h()-4);
		xui_canvas::get_ins()->fill_rectangle(rt, color);
	}
	else
	if (sender == m_tail)
	{
		rt.oft_x(4);
		rt.oft_y(6);
		rt.set_w(rt.get_w()- 8);
		rt.set_h(rt.get_h()-12);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.bx-10,	rt.ay);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay+rt.get_h()/2);
		poly[2] = xui_vector<s32>(rt.bx-10,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay+rt.get_h()/2);
		poly[2] = xui_vector<s32>(rt.ax,	rt.by);
		xui_canvas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_w()-2);
		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_h()-2);
		xui_canvas::get_ins()->fill_rectangle(rt, color);
	}
}

void xui_timetool::on_togglerenderself( xui_component* sender, xui_method_args&	args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);

	xui_colour color = sender->get_vertexcolor() * xui_colour::k_white;
	//if (toggle->was_push() || toggle->has_catch() || toggle->was_hover())
	//	color *= xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

	xui_rect2d<s32> rt = sender->get_renderrtabs();
	rt.oft_x(4);
	rt.oft_y(4);
	rt.set_w(rt.get_w()-8);
	rt.set_h(rt.get_h()-8);

	if (sender == m_back)
	{
		if (m_back->was_push())
		{
			rt.oft_x(1);
			rt.oft_y(1);
			rt.set_w(rt.get_w()-2);
			rt.set_h(rt.get_h()-2);
			xui_canvas::get_ins()->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay+rt.get_h()/2);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay);
			poly[2] = xui_vector<s32>(rt.bx, rt.by);
			xui_canvas::get_ins()->fill_poly(poly, 3, color);
		}
	}
	else
	if (sender == m_play)
	{
		if (m_play->was_push())
		{
			rt.oft_y(1);
			rt.set_w(rt.get_w()-2);
			rt.set_h(rt.get_h()-2);
			xui_canvas::get_ins()->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay+rt.get_h()/2);
			poly[2] = xui_vector<s32>(rt.ax, rt.by);
			xui_canvas::get_ins()->fill_poly(poly, 3, color);
		}
	}
	else
	if (sender == m_loop && xui_global::s_icon_loop)
	{
		rt  =  m_loop->get_renderrtabs();
		rt += (m_loop->has_catch() ? xui_vector<s32>(0, 1) : xui_vector<s32>(0));
		xui_canvas::get_ins()->draw_image(xui_global::s_icon_loop, rt, color);
	}
}

void xui_timetool::on_timertick( xui_component* sender, xui_method_args& args )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	s32 frame = timeview->get_curframe();

	const xui_keyframe_map& allframe = timeview->get_allframe();
	if (m_play->was_push())
	{
		++frame;
		xui_keyframe_map::const_reverse_iterator itor = allframe.rbegin();
		if (frame > (*itor).first && m_loop->was_push())
			frame = 0;
	}
	else
	if (m_back->was_push())
	{
		--frame;
		xui_keyframe_map::const_iterator itor = allframe.begin();
		if (frame < (*itor).first && m_loop->was_push())
			frame = (*allframe.rbegin()).first;
	}

	timeview->set_curframe(frame);
}