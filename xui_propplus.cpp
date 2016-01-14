#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_propplus.h"

/*
//constructor
*/
xui_propplus::xui_propplus( void )
: xui_component("", xui_rect2d<s32>(0))
{
	m_type     += "propplus";
	m_visible	= false;
	m_expanded	= true;
}

/*
//method
*/
xui_method_explain(xui_propplus, was_expanded,	bool)( void ) const
{
	return m_expanded;
}
xui_method_explain(xui_propplus, set_expanded,	void)( bool flag )
{
	if (m_expanded != flag)
	{
		m_expanded  = flag;

		if (m_parent)
			m_parent->refresh();
	}
}

/*
//callback
*/
xui_method_explain(xui_propplus, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_component::on_mousedown(args);
	set_expanded(!m_expanded);
}
xui_method_explain(xui_propplus, on_renderself, void)( xui_method_args&  args )
{
	xui_component::on_renderself(args);

	bool shape = m_expanded;//true-Ö±½Ç false-Èý½Ç
	bool solid = m_expanded;

	xui_colour color = get_vertexcolor();
	if (g_desktop->get_hoverctrl() == this)
	{
		color *= xui_colour(1.0f, 0.0f, 0.6f, 1.0f);
	}

	xui_vector<s32> pt = get_screenpt() + xui_vector<s32>(get_renderw()/2, get_renderh()/2);
	xui_vector<s32> poly[4];
	if (shape)
	{
		poly[0] = xui_vector<s32>(pt.x-6, pt.y-3);
		poly[1] = xui_vector<s32>(pt.x+6, pt.y-3);
		poly[2] = xui_vector<s32>(pt.x,   pt.y+3);
		poly[3] = poly[0];
	}
	else
	{
		poly[0] = xui_vector<s32>(pt.x-3, pt.y-6);
		poly[1] = xui_vector<s32>(pt.x+3, pt.y);
		poly[2] = xui_vector<s32>(pt.x-3, pt.y+6);
		poly[3] = poly[0];
	}

	//g_convas->draw_path(poly, 4, color);
	g_convas->fill_poly(poly, 3, color);
}