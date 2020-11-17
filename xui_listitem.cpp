#include "xui_bitmap.h"
#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_listview.h"
#include "xui_listitem.h"

xui_implement_rtti(xui_listitem, xui_drawer)

xui_listitem::xui_listitem( const xui_rect2d<s32>& border, bool drawtick )
: xui_drawer(xui_vector<s32>(0))
{
	m_drawcolor = true;
	m_border	= border;
	m_selected	= false;
	m_drawtick	= drawtick;
}

bool xui_listitem::was_selected( void ) const
{
	return m_selected;
}

void xui_listitem::set_selected( bool flag )
{
	m_selected  = flag;
}

s32 xui_listitem::get_maxwidth( void )
{
	s32 width = m_border.ax + m_border.bx;

	if (m_drawtick)
	{
		width += 16;
	}
	width += m_iconoffset.x;
	width += m_iconsize.w;
	width += m_textoffset.x;
	if (m_text.length() > 0 && m_singleline)
	{
		width += xui_canvas::get_ins()->calc_size(m_text, m_textfont, 0, true).w;
	}

	return width;
}

xui_rect2d<s32> xui_listitem::get_renderrtins( void ) const
{
	xui_rect2d<s32> rt = xui_drawer::get_renderrtins();
	if (m_drawtick)
		rt.ax += 16;

	return rt;
}

xui_colour xui_listitem::get_rendercolor( void ) const
{
	xui_listview* listview = xui_dynamic_cast(xui_listview, m_parent);
	if		(m_drawtick == false && m_selected)	return listview->get_tickcolor();
	else if (was_hover())						return listview->get_movecolor();
	else										return m_backcolor;
}

void xui_listitem::on_setborderrt( xui_method_args& args )
{
	xui_drawer::on_setborderrt(args);
	m_parent->invalid();
}

void xui_listitem::on_textchanged( xui_method_args& args )
{
	xui_drawer::on_textchanged(args);
	m_parent->invalid();
}

void xui_listitem::on_iconchanged( xui_method_args& args )
{
	xui_drawer::on_iconchanged(args);
	m_parent->invalid();
}

void xui_listitem::on_fontchanged( xui_method_args& args )
{
	xui_drawer::on_fontchanged(args);
	m_parent->invalid();
}

void xui_listitem::on_renderself( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);
	if (m_drawtick && m_selected)
	{
		xui_colour      color  = get_vertexcolor();
		xui_rect2d<s32> rt     = get_renderrtabs();
		xui_vector<s32> center = xui_vector<s32>(rt.ax+m_border.ax+8, rt.ay+rt.get_h()/2);
		xui_canvas::get_ins()->draw_tick(center, 8, color*xui_colour(1.0f));
	}
}

void xui_listitem::on_mousedown( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);
	if (args.mouse == k_mb_left)
	{
		xui_listview* listview = xui_dynamic_cast(xui_listview, m_parent);
		if (args.ctrl  == false &&
			args.shift == false &&
			m_selected == false)
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
}

void xui_listitem::on_keybddown( xui_method_keybd& args )
{
	xui_drawer::on_keybddown(args);
	if (args.kcode == k_key_uarrow ||
		args.kcode == k_key_darrow)
	{
		xui_listview* listview = xui_dynamic_cast(xui_listview, m_parent);
		std::vector<u32> selectedindex = listview->get_selecteditemindex();
		if (selectedindex.empty())
			return;

		u32 index;
		if (args.kcode == k_key_uarrow) index = selectedindex.front()-1;
		if (args.kcode == k_key_darrow) index = selectedindex.back ()+1;
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