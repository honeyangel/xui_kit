#include "xui_desktop.h"
#include "xui_treeview.h"
#include "xui_treegrid.h"

xui_implement_rtti(xui_treegrid, xui_control);

/*
//constructor
*/
xui_create_explain(xui_treegrid)( u32 index, xui_component* parent )
: xui_control(xui_vector<s32>(0), parent)
{
	m_index	 = index;
	m_cursor = CURSOR_WE;
}

/*
//callback
*/
xui_method_explain(xui_treegrid, on_mousemove,	void)( xui_method_mouse& args )
{
	xui_component::on_mousemove(args);
	if (has_catch())
	{
		s32 delta = g_desktop->get_mousecurr().x - g_desktop->get_mouselast().x;

		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		treeview->set_columnsize(m_index, treeview->get_columninfo(m_index).size+delta);
	}
}