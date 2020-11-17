#include "xui_canvas.h"
#include "xui_toolbar.h"
#include "xui_separate.h"

xui_implement_rtti(xui_separate, xui_component)

xui_separate::xui_separate( const xui_vector<s32>& size, u08 flow )
: xui_component(size)
{
	m_flow = flow;
}

void xui_separate::on_renderback( xui_method_args& args )
{
	xui_rect2d<s32> rt = get_renderrtabs();
    switch (m_flow)
    {
    case k_flowstyle_h:
        rt.oft_x(rt.get_w() / 2 - 1);
        xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay),     xui_vector<s32>(rt.ax, rt.by),      xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
        xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax + 1, rt.ay), xui_vector<s32>(rt.ax + 1, rt.by),  xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
        break;
    case k_flowstyle_v:
        rt.oft_y(rt.get_h() / 2 - 1);
        xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay),     xui_vector<s32>(rt.bx, rt.ay),      xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
        xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay + 1), xui_vector<s32>(rt.bx, rt.ay + 1),  xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
        break;
    }
}