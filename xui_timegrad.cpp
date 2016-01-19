#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_timeview.h"
#include "xui_timegrad.h"

xui_implement_rtti(xui_timegrad, xui_drawer);

/*
//constructor
*/
xui_create_explain(xui_timegrad)( xui_component* parent )
: xui_drawer(xui_vector<s32>(0), parent)
{
	m_border	= xui_rect2d<s32>(5, 0, 0, 0);
	m_textfont	= xui_family("Arial", 12, false);
	m_textdraw	= xui_family_render();
}

/*
//override
*/
xui_method_explain(xui_timegrad, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		xui_vector<s32> pt = args.point - get_screenpt();
		pt.x -= m_border.ax;
		set_curframe(pt.x);
	}
}
xui_method_explain(xui_timegrad, on_mousemove,	void)( xui_method_mouse& args )
{
	xui_drawer::on_mousemove(args);
	if (has_catch())
	{
		xui_vector<s32> pt = args.point - get_screenpt();
		pt.x -= m_border.ax;
		set_curframe(pt.x);
	}
}
xui_method_explain(xui_timegrad, on_renderself, void)( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	s32 space = timeview->get_keyspace();
	s32 large = timeview->get_keylarge();
	s32 small = timeview->get_keysmall();

	xui_colour      color = get_vertexcolor();
	xui_vector<s32> pt    = get_screenpt();

	s32 start = (m_scroll.x-m_border.ax) / space;
	s32 final = start + (get_renderw()-m_border.ax) / space + 1;
	for (s32 i = xui_max(start, 0); i <= final; ++i)
	{
		xui_vector<s32> p1;
		xui_vector<s32> p2;
		if (i%large == 0 || i == timeview->get_curframe())
		{
			p1 = pt + xui_vector<s32>(m_border.ax + i*space, 24);
			p2 = pt + xui_vector<s32>(m_border.ax + i*space, get_renderh());

			std::wstringstream text;
			text << i;
			xui_rect2d<s32> rt;
			rt.ax = m_border.ax + i*space-50;
			rt.bx = m_border.ax + i*space+50;
			rt.ay =  2;
			rt.by = 24;
			rt    = xui_convas::get_ins()->calc_draw(text.str(), m_textfont, rt, TA_CT, true);

			if (i != timeview->get_curframe())
			{
				xui_convas::get_ins()->draw_line(p1, p2, color*xui_colour(1.0f, 0.5f, 0.5f, 0.5f));
				xui_convas::get_ins()->draw_text(text.str(), m_textfont, rt.get_pt()+pt, m_textdraw);
			}
			else
			{
				xui_vector<s32> poly[3];
				poly[0] = xui_vector<s32>(p1.x,   p1.y  );
				poly[1] = xui_vector<s32>(p1.x-6, p1.y-6);
				poly[2] = xui_vector<s32>(p1.x+6, p1.y-6);
				xui_convas::get_ins()->draw_line(p1,  p2, color*xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
				xui_convas::get_ins()->draw_text(text.str(), m_textfont, rt.get_pt()+pt, m_textdraw);
				xui_convas::get_ins()->draw_path(poly, 3, color*xui_colour(1.0f, 0.0f, 1.0f, 1.0f));
			}
		}
		else
		if (i%small == 0 && large != small)
		{
			p1 = pt + xui_vector<s32>(m_border.ax + i*space, 32);
			p2 = pt + xui_vector<s32>(m_border.ax + i*space, get_renderh());
			xui_convas::get_ins()->draw_line(p1, p2, color*xui_colour(1.0f, 0.5f, 0.5f, 0.5f));
		}
	}
}

/*
//method
*/
xui_method_explain(xui_timegrad, set_curframe,	void)( s32 x )
{
	xui_timeview* timeview = xui_dynamic_cast(xui_timeview, m_parent);
	s32 space = timeview->get_keyspace();
	s32 frame = x / space;
	if (x % space > space/2)
		frame++;

	timeview->set_curframe(frame);
}