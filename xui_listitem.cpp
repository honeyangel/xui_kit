#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_listview.h"
#include "xui_listitem.h"

/*
//constructor
*/
xui_listitem::xui_listitem( const std::string& name, const xui_rect2d<s32>& rect, xui_bitmap* flagbitmap )
: xui_drawer(name, rect)
{
	m_type      += "listitem";
	m_selected   = false;
	m_flagbitmap = flagbitmap;
}

/*
//selected
*/
xui_method_explain(xui_listitem, was_selected,		bool				)( void ) const
{
	return m_selected;
}
xui_method_explain(xui_listitem, set_selected,		void				)( bool flag )
{
	m_selected  = flag;
}

/*
//method
*/
xui_method_explain(xui_listitem, get_maxwidth,		s32					)( void )
{
	s32 width = m_border.ax + m_border.bx;
	if (m_text.length() > 0)
	{
		width += g_convas->calc_size(m_text, m_font, xui_rect2d<s32>(0), true).w;
		width += xui_abs(m_textoffset.x);
	}
	if (m_icon)
	{
		width += m_icon->get_size().w;
		width += xui_abs(m_iconoffset.x);
	}

	return width;
}

/*
//override
*/
xui_method_explain(xui_listitem, get_renderrtins,	xui_rect2d<s32>		)( void ) const
{
	xui_rect2d<s32> rt = xui_drawer::get_renderrtins();
	if (m_flagbitmap)
		rt.ax += m_flagbitmap->get_size().w;

	return rt;
}
xui_method_explain(xui_listitem, get_rendercolor,	xui_colour			)( void ) const
{
	xui_listview* listview = (xui_listview*)m_parent;
	if		(m_flagbitmap == NULL && m_selected)	return listview->get_flagcolor();
	else if (g_desktop->get_hoverctrl() == this)	return listview->get_movecolor();
	else											return m_backcolor;
}

/*
//callback
*/
xui_method_explain(xui_listitem, on_textchanged,	void				)( xui_method_args&  args )
{
	xui_drawer::on_textchanged(args);
	m_parent->invalid();
}
xui_method_explain(xui_listitem, on_iconchanged,	void				)( xui_method_args&  args )
{
	xui_drawer::on_iconchanged(args);
	m_parent->invalid();
}
xui_method_explain(xui_listitem, on_fontchanged,	void				)( xui_method_args&  args )
{
	xui_drawer::on_fontchanged(args);
	m_parent->invalid();
}
xui_method_explain(xui_listitem, on_renderself,		void				)( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);
	if (m_flagbitmap)
	{
		xui_vector<s32> pt;
		pt.x = m_border.ax;
		pt.y = (get_renderh() - m_flagbitmap->get_size().h) / 2;
		g_convas->draw_image(
			m_flagbitmap,
			pt + get_screenpt(),
			get_vertexcolor());
	}
}
xui_method_explain(xui_listitem, on_mousedown,		void				)( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);

	xui_listview* listview = (xui_listview*)m_parent;
	if (args.ctrl  == false &&
		args.shift == false)
		listview->non_selecteditem(false);

	std::vector<u32> selectedindex = listview->get_selecteditemindex();
	if (args.ctrl)
	{
		listview->set_selecteditem(this, !m_selected);
	}
	else
	if (args.shift && selectedindex.size() > 0)
	{
		u32 index = listview->get_itemindex(this);
		u32 start = xui_min(index, selectedindex.front());
		u32 final = xui_max(index, selectedindex.front());
		listview->set_selecteditem(start, final);
	}
	else
	{
		listview->set_selecteditem(this, true);
	}
}
xui_method_explain(xui_listitem, on_keybddown,		void				)( xui_method_keybd& args )
{
	xui_drawer::on_keybddown(args);
	if (args.kcode == KEY_UARROW ||
		args.kcode == KEY_DARROW)
	{
		xui_listview* listview = (xui_listview*)m_parent;
		std::vector<u32> selectedindex = listview->get_selecteditemindex();
		if (selectedindex.empty())
			return;

		u32 index;
		if (args.kcode == KEY_UARROW) index = selectedindex.front()-1;
		if (args.kcode == KEY_DARROW) index = selectedindex.back ()+1;
		if (index >= listview->get_itemcount())
			return;

		xui_listitem* item = listview->get_item(index);
		if (args.shift == false || 
			listview->was_allowmulti() == false)
		{
			item->req_focus();
			listview->non_selecteditem(false);
			listview->set_itemvisible(item);
		}

		listview->set_selecteditem(item, true);
	}
}