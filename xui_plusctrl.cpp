#include "xui_convas.h"
#include "xui_plusctrl.h"

xui_implement_rtti(xui_plusctrl, xui_button);

/*
//constructor
*/
xui_create_explain(xui_plusctrl)( u08 drawmode, xui_component* parent )
: xui_button(xui_vector<s32>(16), parent)
{
	m_visible	= false;
	m_expanded	= false;
}

/*
//method
*/
xui_method_explain(xui_plusctrl, was_expanded,	bool)( void ) const
{
	return m_expanded;
}
xui_method_explain(xui_plusctrl, set_expanded,	void)( bool flag )
{
	if (m_expanded != flag)
	{
		m_expanded  = flag;

		xui_method_args args;
		xm_expand(this, args);
		//xui_treenode* treenode = xui_dynamic_cast(xui_treenode, m_parent);
		//xui_treeview* treeview = xui_dynamic_cast(xui_treeview, treenode->get_parent());
		//if (treeview)
		//	treeview->invalid();
	}
}

/*
//callback
*/
xui_method_explain(xui_plusctrl, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == MB_L)
		set_expanded(!m_expanded);
}
xui_method_explain(xui_plusctrl, on_renderself, void)( xui_method_args&  args )
{
	xui_button::on_renderself(args);

	xui_rect2d<s32> rt		= get_renderrtins() + get_screenpt();
	xui_colour		color   = get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (m_drawmode == PLUSRENDER_SYMBOL)
	{
		xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-4,
			center.y-1,
			center.x+4,
			center.y+1), color);

		if (m_expanded == false)
			xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
			center.x-1,
			center.y-4,
			center.x+1,
			center.y+4), color);
	}
	else
	{
		if (m_expanded) xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_DOWN,  color);
		else			xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_RIGHT, color);
	}
}