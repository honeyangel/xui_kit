#include "xui_canvas.h"
#include "xui_plusctrl.h"

xui_implement_rtti(xui_plusctrl, xui_button)

xui_plusctrl::xui_plusctrl( u08 drawmode, bool expanded )
: xui_button(xui_vector<s32>(16))
{
	m_backcolor	= xui_colour(1.0f,   0.8f);
	m_movecolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	m_downcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);
	m_visible	= false;
	m_onlyside  = false;
	m_drawmode	= drawmode;
	m_expanded	= expanded;
}

bool xui_plusctrl::was_expanded( void ) const
{
	return m_expanded;
}

void xui_plusctrl::set_expanded( bool flag )
{
	if (m_expanded != flag)
	{
		m_expanded  = flag;

		xui_method_args args;
		xm_expand(this, args);
	}
}

void xui_plusctrl::set_onlyside( bool flag )
{
	m_onlyside = flag;
}

xui_colour xui_plusctrl::get_rendercolor( void ) const
{
	if (m_onlyside)
		return m_backcolor;

	return xui_button::get_rendercolor();
}

void xui_plusctrl::on_mousedown( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == k_mb_left)
		set_expanded(!m_expanded);
}

void xui_plusctrl::on_renderself( xui_method_args& args )
{
	xui_button::on_renderself(args);

	xui_rect2d<s32> rt		= get_renderrtins() + get_screenpt();
	xui_colour		color   = get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (m_drawmode == k_plusrender_symbol)
	{
        if (m_onlyside && was_hover())
            xui_canvas::get_ins()->draw_rectangle(xui_rect2d<s32>(
                center.x-4,
                center.y-1,
                center.x+4,
                center.y+1), color);
        else
		    xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			    center.x-4,
			    center.y-1,
			    center.x+4,
			    center.y+1), color);

		if (m_expanded == false)
        {
            if (m_onlyside && was_hover())
                xui_canvas::get_ins()->draw_rectangle(xui_rect2d<s32>(
                    center.x-1,
                    center.y-4,
                    center.x+1,
                    center.y+4), color);
            else
                xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
                    center.x-1,
                    center.y-4,
                    center.x+1,
                    center.y+4), color);
        }
	}
	else
	{
		if (m_onlyside && was_hover())
		{
			if (m_expanded) xui_canvas::get_ins()->draw_triangle(center, 3, k_triangle_down,  color);
			else			xui_canvas::get_ins()->draw_triangle(center, 3, k_triangle_right, color);
		}
		else
		{
			if (m_expanded) xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_down,  color);
			else			xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_right, color);
		}
	}
}