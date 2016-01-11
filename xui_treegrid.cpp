#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_treeview.h"
#include "xui_treegrid.h"

/*
//constructor
*/
xui_create_explain(xui_treegrid)( u32 index, xui_treeview* treeview )
: xui_control("", xui_rect2d<s32>(0))
{
	m_type	   += "treegrid";
	m_index		= index;
	m_parent	= treeview;
	m_cursor	= CURSOR_WE;
	m_backcolor = xui_colour(0.0f);
}

/*
//callback
*/
xui_method_explain(xui_treegrid, on_mousemove,	void)( xui_method_mouse& args )
{
	xui_componet::on_mousemove(args);
	if (g_desktop->get_catchctrl() == this)
	{
		s32 delta = g_desktop->get_mousecurr().x - g_desktop->get_mouselast().x;

		xui_treeview* treeview = (xui_treeview*)m_parent;
		treeview->set_columnsize(m_index, treeview->get_columninfo(m_index).size+delta);
	}
}