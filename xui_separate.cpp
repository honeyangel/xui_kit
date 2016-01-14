#include "xui_convas.h"
#include "xui_toolbar.h"
#include "xui_separate.h"

/*
//constructor
*/
xui_create_explain(xui_separate)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_component(name, rect)
{
	m_type   += "separate";
}

/*
//callback
*/
xui_method_explain(xui_separate, on_renderback,	void)( xui_method_args& args )
{
	xui_rect2d<s32> rt = get_renderrtabs();
	u08 flow = get_flow();
	switch (flow)
	{
	case FLOWSTYLE_H:
		rt.oft_x(rt.get_sz().w/2-1);
		rt.set_w(0);
		g_convas->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
		rt.oft_x(1);
		g_convas->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
		break;
	case FLOWSTYLE_V:
		rt.oft_y(rt.get_sz().h/2-1);
		rt.set_h(0);
		g_convas->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour(1.0f, 0.15f, 0.15f, 0.15f));
		rt.oft_y(1);
		g_convas->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.by), xui_colour(1.0f, 0.65f, 0.65f, 0.65f));
		break;
	}
}

xui_method_explain(xui_separate, get_flow,			u08	)( void )
{
	if (m_parent && m_parent->get_type().find("toolbar") != -1)
		return ((xui_toolbar*)m_parent)->get_flow();

	return FLOWSTYLE_H;
}