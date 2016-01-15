#include "xui_convas.h"
#include "xui_toolbar.h"
#include "xui_separate.h"

xui_implement_rtti(xui_separate, xui_component);

/*
//constructor
*/
xui_create_explain(xui_separate)( const xui_vector<s32>& size, xui_component* parent )
: xui_component(size, parent)
{}

/*
//callback
*/
xui_method_explain(xui_separate, on_renderback,	void)( xui_method_args& args )
{
	xui_rect2d<s32> rt = get_renderrtabs();
	u08 flow = get_flow();
	if (flow == FLOWSTYLE_H)
	{
		rt.oft_x(rt.get_w()/2-1);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,   rt.ay),   xui_vector<s32>(rt.ax,   rt.by),   xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax+1, rt.ay),   xui_vector<s32>(rt.ax+1, rt.by),   xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
	}
	if (flow == FLOWSTYLE_V)
	{
		rt.oft_y(rt.get_h()/2-1);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,   rt.ay),   xui_vector<s32>(rt.bx,   rt.ay),   xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,   rt.ay+1), xui_vector<s32>(rt.bx,   rt.ay+1), xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
	}
}

xui_method_explain(xui_separate, get_flow,		u08	)( void )
{
	xui_toolbar* toolbar = xui_dynamic_cast(xui_toolbar, m_parent);
	if (toolbar)
		return toolbar->get_flow();

	return FLOWSTYLE_H;
}