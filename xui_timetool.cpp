#include "xui_timer.h"
#include "xui_timermgr.h"
#include "xui_convas.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_timeview.h"
#include "xui_timetool.h"

xui_implement_rtti(xui_timetool, xui_toolbar);

/*
//constructor
*/
xui_create_explain(xui_timetool)( xui_timeview* timeview )
: xui_toolbar(xui_vector<s32>(0, 40))
{
	m_grap		= 8;
	m_border	= xui_rect2d<s32>(8, 8, 0, 8);
	m_parent	= timeview;

	//plus
	m_plus = new xui_button(xui_vector<s32>(24));
	m_plus->xm_buttonclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_plus->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	xui_method_ptrcall(m_plus,   set_parent		)(this);
	xui_method_ptrcall(m_plus,	 set_corner		)(3);
	xui_method_ptrcall(m_plus,	 set_sidestyle	)(SIDESTYLE_S);
	m_widgetvec.push_back(m_plus);

	//prev
	m_head = new xui_button(xui_vector<s32>(24));
	m_head->xm_buttonclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_head->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	m_prev = new xui_button(xui_vector<s32>(24));
	m_prev->xm_buttonclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_prev->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);

	xui_linebox* prevline = new xui_linebox(xui_vector<s32>(48, 24), FLOWSTYLE_H);
	xui_method_ptrcall(prevline, set_parent		)(this);
	xui_method_ptrcall(prevline, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(prevline, set_corner		)(3);
	xui_method_ptrcall(prevline, add_linectrl	)(m_head);
	xui_method_ptrcall(prevline, add_linectrl	)(m_prev);
	m_widgetvec.push_back(prevline);


	//play
	m_back = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	m_back->xm_toggleclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_back->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	m_play = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	m_play->xm_toggleclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_play->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);

	xui_linebox* playline = new xui_linebox(xui_vector<s32>(48, 24), FLOWSTYLE_H);
	xui_method_ptrcall(playline, set_parent		)(this);
	xui_method_ptrcall(playline, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(playline, set_corner		)(3);
	xui_method_ptrcall(playline, add_linectrl	)(m_back);
	xui_method_ptrcall(playline, add_linectrl	)(m_play);
	m_widgetvec.push_back(playline);


	//next
	m_next = new xui_button(xui_vector<s32>(24));
	m_next->xm_buttonclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_next->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	m_tail = new xui_button(xui_vector<s32>(24));
	m_tail->xm_buttonclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_tail->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);

	xui_linebox* nextline = new xui_linebox(xui_vector<s32>(48, 24), FLOWSTYLE_H);
	xui_method_ptrcall(nextline, set_parent		)(this);
	xui_method_ptrcall(nextline, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(nextline, set_corner		)(3);
	xui_method_ptrcall(nextline, add_linectrl	)(m_next);
	xui_method_ptrcall(nextline, add_linectrl	)(m_tail);
	m_widgetvec.push_back(nextline);


	//loop
	m_loop = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	m_loop->xm_toggleclick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_loop->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	xui_method_ptrcall(m_loop,   set_parent		)(this);
	xui_method_ptrcall(m_loop,	 set_corner		)(3);
	xui_method_ptrcall(m_loop,	 set_sidestyle	)(SIDESTYLE_S);
	m_widgetvec.push_back(m_loop);


	//timer
	m_playtimer = xui_timermgr::get_ins()->add_timer(this, 1.0f/30.0f, NULL);
	m_playtimer->set_enable(false);
	m_playtimer->xm_tick	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_timertick);

	refresh();
}

/*
//destructor
*/
xui_delete_explain(xui_timetool)( void )
{
	xui_timermgr::get_ins()->del_timer(m_playtimer);
}

/*
//method
*/
xui_method_explain(xui_timetool, get_playtimer,			xui_timer*	)( void )
{
	return m_playtimer;
}

/*
//method
*/
xui_method_explain(xui_timetool, on_buttonclick,		void		)( xui_component* sender, xui_method_args& args )
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
			if ((*itor).first < timeview->get_curframe())
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
xui_method_explain(xui_timetool, on_toggleclick,		void		)( xui_component* sender, xui_method_args& args )
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
	{

	}
}
xui_method_explain(xui_timetool, on_buttonrenderself,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_colour color = sender->get_vertexcolor() * xui_colour(1.0f, 0.8f);
	if (sender->has_catch() || sender->was_hover())
		color *= xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

	xui_rect2d<s32> rt = sender->get_renderrtabs() + (sender->has_catch() ? xui_vector<s32>(0, 1) : xui_vector<s32>(0));
	if (sender == m_plus)
	{
		xui_vector<s32> center(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-8,
			center.y-2,
			center.x+8,
			center.y+2), color);

		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
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
		xui_convas::get_ins()->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay+rt.get_h()/2);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay);
		poly[2] = xui_vector<s32>(rt.ax+10,	rt.by);
		xui_convas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_h()-2);
		xui_convas::get_ins()->fill_rectangle(rt, color);
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
		xui_convas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_h()-4);
		xui_convas::get_ins()->fill_rectangle(rt, color);
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
		xui_convas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_w()-2);
		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_h()-4);
		xui_convas::get_ins()->fill_rectangle(rt, color);
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
		xui_convas::get_ins()->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay+rt.get_h()/2);
		poly[2] = xui_vector<s32>(rt.ax,	rt.by);
		xui_convas::get_ins()->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_w()-2);
		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_h()-2);
		xui_convas::get_ins()->fill_rectangle(rt, color);
	}
}
xui_method_explain(xui_timetool, on_togglerenderself,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_toggle* toggle = xui_dynamic_cast(xui_toggle, sender);

	xui_colour color = sender->get_vertexcolor() * xui_colour(1.0f, 0.8f);
	if (toggle->was_push() || toggle->has_catch() || toggle->was_hover())
		color *= xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

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
			xui_convas::get_ins()->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay+rt.get_h()/2);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay);
			poly[2] = xui_vector<s32>(rt.bx, rt.by);
			xui_convas::get_ins()->fill_poly(poly, 3, color);
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
			xui_convas::get_ins()->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay+rt.get_h()/2);
			poly[2] = xui_vector<s32>(rt.ax, rt.by);
			xui_convas::get_ins()->fill_poly(poly, 3, color);
		}
	}
	else
	if (sender == m_loop)
	{
		rt += (m_loop->has_catch() ? xui_vector<s32>(0, 1) : xui_vector<s32>(0));

		rt.oft_x(2);
		rt.oft_y(2);
		rt.set_w(rt.get_w()-4);
		rt.set_h(rt.get_h()-4);
		xui_convas::get_ins()->draw_arc(rt,							color, -120, 270, 1);
		xui_convas::get_ins()->draw_arc(rt+xui_vector<s32>(-1,  0),	color, -120, 270, 1);
		xui_convas::get_ins()->draw_arc(rt+xui_vector<s32>( 1,  0),	color, -120, 270, 1);
		xui_convas::get_ins()->draw_arc(rt+xui_vector<s32>( 0, -1),	color, -120, 270, 1);
		xui_convas::get_ins()->draw_arc(rt+xui_vector<s32>( 0,  1),	color, -120, 270, 1);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax-3,	rt.ay+rt.get_h()/2-2);
		poly[1] = xui_vector<s32>(rt.ax+5,	rt.ay+rt.get_h()/2);
		poly[2] = xui_vector<s32>(rt.ax-3,	rt.ay+rt.get_h()/2+6);
		xui_convas::get_ins()->fill_poly(poly, 3, color);
	}
}
xui_method_explain(xui_timetool, on_timertick,			void		)( xui_component* sender, xui_method_args& args )
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