#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "xui_treeplus.h"

/*
//constructor
*/
xui_treeplus::xui_treeplus( xui_treenode* treenode )
: xui_component("", xui_rect2d<s32>(0))
{
	m_type     += "treeplus";
	m_parent	= treenode;
	m_visible	= false;
	m_expanded	= false;
}

/*
//method
*/
xui_method_explain(xui_treeplus, was_expanded,	bool)( void ) const
{
	return m_expanded;
}
xui_method_explain(xui_treeplus, set_expanded,	void)( bool flag )
{
	if (m_expanded != flag)
	{
		m_expanded  = flag;

		xui_treenode* treenode = (xui_treenode*)m_parent;
		xui_treeview* treeview = (xui_treeview*)treenode->get_parent();
		if (treeview)
			treeview->invalid();
	}
}

/*
//callback
*/
xui_method_explain(xui_treeplus, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_component::on_mousedown(args);
	set_expanded(!m_expanded);
}
xui_method_explain(xui_treeplus, on_renderself, void)( xui_method_args&  args )
{
	xui_component::on_renderself(args);
	xui_treenode* treenode = (xui_treenode*)m_parent;
	xui_treeview* treeview = (xui_treeview*)treenode->get_parent();

	bool shape = m_expanded;//true-Ö±½Ç false-Èý½Ç
	bool solid = m_expanded;

	xui_colour color = get_vertexcolor();
	if (g_desktop->get_hoverctrl() == this)
	{
		if (treenode->was_selected() == false || treeview->was_lighttrace() == false)
			color *= xui_colour(1.0f, 0.0f, 0.6f, 1.0f);
		else
			solid  = !solid;
	}

	xui_vector<s32> pt = get_screenpt() + xui_vector<s32>(get_renderw()/2, get_renderh()/2);
	if (treeview->get_plusrender() == PLUSRENDER_NORMAL)
	{
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
		//if (solid)
			g_convas->fill_poly(poly, 3, color);
	}
	else
	{
		xui_rect2d<s32> rt;
		rt.ax = pt.x-4;
		rt.ay = pt.y-1;
		rt.bx = pt.x+4;
		rt.by = pt.y+1;
		g_convas->fill_rectangle(rt, color);

		if (!shape)
		{
			rt.ax = pt.x-1;
			rt.ay = pt.y-4;
			rt.bx = pt.x+1;
			rt.by = pt.y+4;
			g_convas->fill_rectangle(rt, color);
		}
	}
}