#include "xui_timer.h"
#include "xui_convas.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_timeview.h"
#include "xui_timetool.h"

/*
//constructor
*/
xui_create_explain(xui_timetool)( void )
: xui_toolbar("timetool", xui_rect2d<s32>(0, 0, 40, 40))
{
	m_type  += "timetool";
	m_grap   = 8;
	m_border = xui_rect2d<s32>(8);

	//prev
	m_head = new xui_button("", xui_rect2d<s32>(0, 0, 24, 24));
	m_head->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_head->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	xui_method_ptrcall(m_head,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_head,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_head,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_prev = new xui_button("", xui_rect2d<s32>(0, 0, 24, 24));
	m_prev->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_prev->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	xui_method_ptrcall(m_prev,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_prev,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_prev,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));

	xui_linebox* prevline = new xui_linebox("", xui_rect2d<s32>(0, 0, 48, 24));
	xui_method_ptrcall(prevline, set_corner		)(5);
	xui_method_ptrcall(prevline, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(prevline, add_linectrl	)(m_head);
	xui_method_ptrcall(prevline, add_linectrl	)(m_prev);
	add_item(prevline);


	//play
	m_back = new xui_toggle("", xui_rect2d<s32>(0, 0, 24, 24));
	m_back->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_back->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	xui_method_ptrcall(m_back,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_back,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_back,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_play = new xui_toggle("", xui_rect2d<s32>(0, 0, 24, 24));
	m_play->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_play->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	xui_method_ptrcall(m_play,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_play,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_play,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));

	xui_linebox* playline = new xui_linebox("", xui_rect2d<s32>(0, 0, 48, 24));
	xui_method_ptrcall(playline, set_corner		)(5);
	xui_method_ptrcall(playline, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(playline, add_linectrl	)(m_back);
	xui_method_ptrcall(playline, add_linectrl	)(m_play);
	add_item(playline);


	//next
	m_next = new xui_button("", xui_rect2d<s32>(0, 0, 24, 24));
	m_next->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_next->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	xui_method_ptrcall(m_next,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_next,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_next,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_tail = new xui_button("", xui_rect2d<s32>(0, 0, 24, 24));
	m_tail->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonclick);
	m_tail->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_buttonrenderself);
	xui_method_ptrcall(m_tail,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_tail,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_tail,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));

	xui_linebox* nextline = new xui_linebox("", xui_rect2d<s32>(0, 0, 48, 24));
	xui_method_ptrcall(nextline, set_corner		)(5);
	xui_method_ptrcall(nextline, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(nextline, add_linectrl	)(m_next);
	xui_method_ptrcall(nextline, add_linectrl	)(m_tail);
	add_item(nextline);


	//loop
	m_loop = new xui_toggle("", xui_rect2d<s32>(0, 0, 24, 24));
	m_loop->xm_click		+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_toggleclick);
	m_loop->xm_renderself	+= new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_togglerenderself);
	xui_method_ptrcall(m_loop,	 set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_loop,	 set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_loop,	 set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_loop,	 set_corner		)(5);
	add_item(m_loop);

	//timer
	m_playtimer = new xui_timer(this, 1.0f/60.0f);
	m_playtimer->set_enable(false);
	m_playtimer->xm_tick += new xui_method_member<xui_method_args, xui_timetool>(this, &xui_timetool::on_timertick);

	refresh();
}

/*
//method
*/
xui_method_explain(xui_timetool, on_buttonclick,		void)( xui_component* sender, xui_method_args& args )
{
	xui_timeview* timeview = (xui_timeview*)m_parent;
	xui_keyframe_map allframe = timeview->get_allframe();
	if (allframe.size() == 0)
		return;

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
xui_method_explain(xui_timetool, on_toggleclick,		void)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_play)
	{
		m_back->set_check(false);
		m_playtimer->set_enable(m_play->was_check());
	}
	else 
	if (sender == m_back)
	{
		m_play->set_check(false);
		m_playtimer->set_enable(m_back->was_check());
	}
	else 
	if (sender == m_loop)
	{

	}
}
xui_method_explain(xui_timetool, on_buttonrenderself,	void)( xui_component* sender, xui_method_args& args )
{
	xui_component* catchctrl = g_desktop->get_catchctrl();

	xui_colour      color = sender->get_vertexcolor() * ((catchctrl == sender) ? xui_colour(1.0f, 0.2f, 0.2f, 0.2f) : xui_colour(1.0f));
	xui_rect2d<s32> rt    = sender->get_renderrtabs();

	if (sender == m_head)
	{
		rt.oft_x(4);
		rt.oft_y(6);
		rt.set_w(rt.get_sz().w- 8);
		rt.set_h(rt.get_sz().h-12);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.bx-10,	rt.ay+rt.get_sz().h/2);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay);
		poly[2] = xui_vector<s32>(rt.bx,	rt.by);
		g_convas->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay+rt.get_sz().h/2);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay);
		poly[2] = xui_vector<s32>(rt.ax+10,	rt.by);
		g_convas->fill_poly(poly, 3, color);

		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_sz().h-2);
		g_convas->fill_rectangle(rt, color);
	}
	else
	if (sender == m_prev)
	{
		rt.oft_x(5);
		rt.oft_y(5);
		rt.set_w(rt.get_sz().w-10);
		rt.set_h(rt.get_sz().h-10);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay+rt.get_sz().h/2);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay);
		poly[2] = xui_vector<s32>(rt.bx,	rt.by);
		g_convas->fill_poly(poly, 3, color);

		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_sz().h-4);
		g_convas->fill_rectangle(rt, color);
	}
	else
	if (sender == m_next)
	{
		rt.oft_x(5);
		rt.oft_y(5);
		rt.set_w(rt.get_sz().w-10);
		rt.set_h(rt.get_sz().h-10);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay+rt.get_sz().h/2);
		poly[2] = xui_vector<s32>(rt.ax,	rt.by);
		g_convas->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_sz().w-2);
		rt.oft_y(2);
		rt.set_w(2);
		rt.set_h(rt.get_sz().h-4);
		g_convas->fill_rectangle(rt, color);
	}
	else
	if (sender == m_tail)
	{
		rt.oft_x(4);
		rt.oft_y(6);
		rt.set_w(rt.get_sz().w- 8);
		rt.set_h(rt.get_sz().h-12);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.bx-10,	rt.ay);
		poly[1] = xui_vector<s32>(rt.bx,	rt.ay+rt.get_sz().h/2);
		poly[2] = xui_vector<s32>(rt.bx-10,	rt.by);
		g_convas->fill_poly(poly, 3, color);
		poly[0] = xui_vector<s32>(rt.ax,	rt.ay);
		poly[1] = xui_vector<s32>(rt.ax+10,	rt.ay+rt.get_sz().h/2);
		poly[2] = xui_vector<s32>(rt.ax,	rt.by);
		g_convas->fill_poly(poly, 3, color);

		rt.oft_x(rt.get_sz().w-2);
		rt.oft_y(1);
		rt.set_w(2);
		rt.set_h(rt.get_sz().h-2);
		g_convas->fill_rectangle(rt, color);
	}
}
xui_method_explain(xui_timetool, on_togglerenderself,	void)( xui_component* sender, xui_method_args& args )
{
	xui_colour      color = sender->get_vertexcolor() * (((xui_toggle*)sender)->was_check() ? xui_colour(1.0f, 0.2f, 0.2f, 0.2f) : xui_colour(1.0f));
	xui_rect2d<s32> rt    = sender->get_renderrtabs();
	rt.oft_x(4);
	rt.oft_y(4);
	rt.set_w(rt.get_sz().w-8);
	rt.set_h(rt.get_sz().h-8);

	if (sender == m_back)
	{
		if (m_back->was_check())
		{
			rt.oft_x(1);
			rt.oft_y(1);
			rt.set_w(rt.get_sz().w-2);
			rt.set_h(rt.get_sz().h-2);
			g_convas->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay+rt.get_sz().h/2);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay);
			poly[2] = xui_vector<s32>(rt.bx, rt.by);
			g_convas->fill_poly(poly, 3, color);
		}
	}
	else
	if (sender == m_play)
	{
		if (m_play->was_check())
		{
			rt.oft_y(1);
			rt.set_w(rt.get_sz().w-2);
			rt.set_h(rt.get_sz().h-2);
			g_convas->fill_round(rt, color, 3);
		}
		else
		{
			xui_vector<s32> poly[3];
			poly[0] = xui_vector<s32>(rt.ax, rt.ay);
			poly[1] = xui_vector<s32>(rt.bx, rt.ay+rt.get_sz().h/2);
			poly[2] = xui_vector<s32>(rt.ax, rt.by);
			g_convas->fill_poly(poly, 3, color);
		}
	}
	else
	if (sender == m_loop)
	{
		rt.oft_x(2);
		rt.oft_y(2);
		rt.set_w(rt.get_sz().w-4);
		rt.set_h(rt.get_sz().h-4);
		g_convas->draw_arc(rt,							color, -120, 270, 1);
		g_convas->draw_arc(rt+xui_vector<s32>(-1,  0),	color, -120, 270, 1);
		g_convas->draw_arc(rt+xui_vector<s32>( 1,  0),	color, -120, 270, 1);
		g_convas->draw_arc(rt+xui_vector<s32>( 0, -1),	color, -120, 270, 1);
		g_convas->draw_arc(rt+xui_vector<s32>( 0,  1),	color, -120, 270, 1);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax-3,	rt.ay+rt.get_sz().h/2-2);
		poly[1] = xui_vector<s32>(rt.ax+5,	rt.ay+rt.get_sz().h/2);
		poly[2] = xui_vector<s32>(rt.ax-3,	rt.ay+rt.get_sz().h/2+6);
		g_convas->fill_poly(poly, 3, color);
	}
}
xui_method_explain(xui_timetool, on_timertick,			void)( xui_component* sender, xui_method_args& args )
{
	xui_timeview* timeview = (xui_timeview*)m_parent;
	s32 frame = timeview->get_curframe();

	const xui_keyframe_map& allframe = timeview->get_allframe();
	if (m_play->was_check())
	{
		++frame;
		xui_keyframe_map::const_reverse_iterator itor = allframe.rbegin();
		if (frame > (*itor).first && m_loop->was_check())
			frame = 0;
	}
	else
	if (m_back->was_check())
	{
		--frame;
		xui_keyframe_map::const_iterator itor = allframe.begin();
		if (frame < (*itor).first && m_loop->was_check())
			frame = (*allframe.rbegin()).first;
	}

	timeview->set_curframe(frame);
}