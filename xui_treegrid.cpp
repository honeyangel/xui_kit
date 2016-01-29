#include "xui_desktop.h"
#include "xui_treeview.h"
#include "xui_treegrid.h"

xui_implement_rtti(xui_treegrid, xui_control);

/*
//constructor
*/
xui_create_explain(xui_treegrid)( u32 index, xui_treeview* treeview )
: xui_control(xui_vector<s32>(0))
{
	m_index	 = index;
	m_cursor = CURSOR_WE;
	m_parent = treeview;
}

/*
//callback
*/
xui_method_explain(xui_treegrid, on_mousemove,	void)( xui_method_mouse& args )
{
	xui_component::on_mousemove(args);
	if (has_catch())
	{
		xui_vector<s32> delta = xui_desktop::get_ins()->get_mousemove();
		xui_treeview* treeview = xui_dynamic_cast(xui_treeview, m_parent);
		treeview->set_columnsize(m_index, treeview->get_columninfo(m_index).size+delta.x);
	}
}