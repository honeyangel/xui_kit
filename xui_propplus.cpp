#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_propplus.h"

xui_implement_rtti(xui_propplus, xui_component);

/*
//constructor
*/
xui_propplus::xui_propplus( xui_component* parent )
: xui_component(xui_vector<s32>(0), parent)
{
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
	if (args.mouse == MB_L)
		set_expanded(!m_expanded);
}
xui_method_explain(xui_propplus, on_renderself, void)( xui_method_args&  args )
{
	xui_component::on_renderself(args);

	xui_vector<s32> pt	  = get_screenpt() + xui_vector<s32>(get_renderw()/2, get_renderh()/2);
	xui_colour      color = get_vertexcolor();
	if (was_hover())
		color *= xui_colour(1.0f, 0.0f, 0.6f, 1.0f);

	xui_vector<s32> poly[3];
	if (m_expanded)
	{
		poly[0] = xui_vector<s32>(pt.x-6, pt.y-3);
		poly[1] = xui_vector<s32>(pt.x+6, pt.y-3);
		poly[2] = xui_vector<s32>(pt.x,   pt.y+3);
	}
	else
	{
		poly[0] = xui_vector<s32>(pt.x-3, pt.y-6);
		poly[1] = xui_vector<s32>(pt.x+3, pt.y);
		poly[2] = xui_vector<s32>(pt.x-3, pt.y+6);
	}

	xui_convas::get_ins()->fill_poly(poly, 3, color);
}