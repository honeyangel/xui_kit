#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_scroll.h"
#include "xui_listitem.h"
#include "xui_listview.h"

xui_implement_rtti(xui_listview, xui_container)

const xui_colour xui_listview::k_default_movecolor = xui_colour(0.5f, 127.0f/255.0f);
const xui_colour xui_listview::k_default_tickcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

xui_listview* xui_listview::create( const xui_vector<s32>& size, bool drawtick )
{
	xui_listview* listview = new xui_listview(size, drawtick);
	listview->set_sidestyle(k_sidestyle_s);
	listview->set_borderrt(xui_rect2d<s32>(4));
	listview->set_corner(3);
	listview->set_iconsize(xui_vector<s32>(16));
    listview->set_iconoffset(xui_vector<s32>(4, 0));
	return listview;
}

xui_listview::xui_listview( const xui_vector<s32>& size, bool drawtick )
: xui_container(size)
{
	m_drawtick		= drawtick;
	m_iconsize		= xui_vector<s32>(0);
	m_textalign		= k_textalign_lc;
	m_iconalign		= k_image_front_text;
	m_movecolor		= k_default_movecolor;
	m_tickcolor		= k_default_tickcolor;
	m_itemborder	= xui_rect2d<s32>(2);
	m_iconoffset	= xui_vector<s32>(0);
	m_textoffset	= xui_vector<s32>(4, 0);
	m_lineheight	= 20;
	m_allowmulti	= false;
}

bool xui_listview::was_allowmulti( void ) const
{
	return m_allowmulti;
}

void xui_listview::set_allowmulti( bool flag )
{
	m_allowmulti = flag;
}

const xui_vector<s32>& xui_listview::get_iconsize( void ) const
{
	return m_iconsize;
}

void xui_listview::set_iconsize( const xui_vector<s32>& iconsize )
{
	if (m_iconsize != iconsize)
	{
		m_iconsize  = iconsize;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconsize(m_iconsize);
		}

		invalid();
	}
}

const xui_family& xui_listview::get_textfont( void ) const
{
	return m_textfont;
}

void xui_listview::set_textfont( const xui_family& textfont )
{
	if (m_textfont != textfont)
	{
		m_textfont  = textfont;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textfont(m_textfont);
		}
	}
}

const xui_family_render& xui_listview::get_textdraw( void ) const
{
	return m_textdraw;
}

void xui_listview::set_textdraw( const xui_family_render& textdraw )
{
	if (m_textdraw != textdraw)
	{
		m_textdraw  = textdraw;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textdraw(m_textdraw);
		}
	}
}

u08 xui_listview::get_textalign( void ) const
{
	return m_textalign;
}

void xui_listview::set_textalign( u08 textalign )
{
	if (m_textalign != textalign)
	{
		m_textalign  = textalign;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textalign(m_textalign);
		}
	}
}

u08 xui_listview::get_iconalign( void ) const
{
	return m_iconalign;
}

void xui_listview::set_iconalign( u08 iconalign )
{
	if (m_iconalign != iconalign)
	{
		m_iconalign  = iconalign;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconalign(m_iconalign);
		}
	}
}

const xui_colour& xui_listview::get_movecolor( void ) const
{
	return m_movecolor;
}

void xui_listview::set_movecolor( const xui_colour& movecolor )
{
	m_movecolor = movecolor;
}

const xui_colour& xui_listview::get_tickcolor( void ) const
{
	return m_tickcolor;
}

void xui_listview::set_tickcolor( const xui_colour& tickcolor )
{
	m_tickcolor = tickcolor;
}

const xui_rect2d<s32>& xui_listview::get_itemborder( void ) const
{
	return m_itemborder;
}

void xui_listview::set_itemborder( const xui_rect2d<s32>& itemborder )
{
	if (m_itemborder != itemborder)
	{
		m_itemborder  = itemborder;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_borderrt(m_itemborder);
		}
	}
}

const xui_vector<s32>& xui_listview::get_iconoffset( void ) const
{
	return m_iconoffset;
}

void xui_listview::set_iconoffset( const xui_vector<s32>& iconoffset )
{
	if (m_iconoffset != iconoffset)
	{
		m_iconoffset  = iconoffset;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconoffset(m_iconoffset);
		}

		invalid();
	}
}

const xui_vector<s32>& xui_listview::get_textoffset( void ) const
{
	return m_textoffset;
}

void xui_listview::set_textoffset( const xui_vector<s32>& textoffset )
{
	if (m_textoffset != textoffset)
	{
		m_textoffset  = textoffset;
		for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textoffset(m_textoffset);
		}

		invalid();
	}
}

s32 xui_listview::get_lineheight( void ) const
{
	return m_lineheight;
}

void xui_listview::set_lineheight( s32 height )
{
	if (m_lineheight != height)
	{
		m_lineheight  = height;
		invalid();
	}
}

void xui_listview::set_selecteditem( xui_listitem* item, bool selected )
{
	if (item->was_selected() != selected)
	{
		item->set_selected(selected);
		if (selected && m_allowmulti == false)
		{
			for (u32 i = 0; i < m_ascrollitem.size(); ++i)
			{
				xui_listitem* temp = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
				if (temp != item)
					temp->set_selected(false);
			}
		}

		xui_method_args args;
		xm_selectedchange(this, args);
	}
}

void xui_listview::set_selecteditem( u32 start, u32 final )
{
	if (m_allowmulti == false)
		return;

	bool selectedchange = false;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (i >= start && i <= final)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			if (item->was_selected() == false)
			{
				item->set_selected(true);
				selectedchange = true;
			}
		}
	}

	if (selectedchange)
	{
		xui_method_args args;
		xm_selectedchange(this, args);
	}
}

void xui_listview::non_selecteditem( bool firemethod )
{
	bool selectedchange = false;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
		if (item->was_selected())
		{
			item->set_selected(false);
			selectedchange = true;
		}
	}

	if (selectedchange && firemethod)
	{
		xui_method_args args;
		xm_selectedchange(this, args);
	}
}

std::vector<xui_listitem*> xui_listview::get_selecteditem( void )
{
	std::vector<xui_listitem*> selecteditem;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
		if (item->was_selected())
			selecteditem.push_back(item);
	}

	return selecteditem;
}

std::vector<u32> xui_listview::get_selecteditemindex( void )
{
	std::vector<u32> selectedindex;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
		if (item->was_selected())
			selectedindex.push_back(i);
	}

	return selectedindex;
}

u32 xui_listview::get_itemcount( void ) const
{
	return m_ascrollitem.size();
}

u32 xui_listview::get_itemindex( xui_listitem* item )
{
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (m_ascrollitem[i] == item)
			return i;
	}

	return -1;
}

xui_listitem* xui_listview::get_item( u32 index )
{
	return xui_dynamic_cast(xui_listitem, m_ascrollitem[index]);
}

void xui_listview::add_item( xui_listitem* item, u32 index )
{
	item->set_parent(this);
	item->set_iconalign(m_iconalign);
	item->set_textalign(m_textalign);
	item->set_iconoffset(m_iconoffset);
	item->set_textoffset(m_textoffset);
	item->set_borderrt(m_itemborder);
	m_ascrollitem.insert(m_ascrollitem.begin()+index, item);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	item->set_scrollpt(pt);
	invalid();
}

xui_listitem* xui_listview::add_item( const std::wstring& text )
{
	return add_item(NULL, text);
}

xui_listitem* xui_listview::add_item( xui_bitmap* icon, const std::wstring& text )
{
	xui_listitem* item = new xui_listitem(m_itemborder, m_drawtick);
	item->ini_drawer(icon, m_iconsize);
	item->ini_drawer(text, m_textfont, m_textdraw);
	item->set_parent(this);
	item->set_iconalign(m_iconalign);
	item->set_textalign(m_textalign);
	item->set_iconoffset(m_iconoffset);
	item->set_textoffset(m_textoffset);
	m_ascrollitem.push_back(item);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	item->set_scrollpt(pt);
	invalid();

	return item;
}

void xui_listview::del_item( xui_listitem* item, bool destroy )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_ascrollitem.begin(),
		m_ascrollitem.end(),
		item);

	if (itor != m_ascrollitem.end())
	{
		item->set_parent(NULL);
		m_ascrollitem.erase(itor);
		invalid();

		if (destroy)
			xui_desktop::get_ins()->move_recycle(item);
	}
}

void xui_listview::del_itemall( void )
{
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		m_ascrollitem[i]->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(m_ascrollitem[i]);
	}
	m_ascrollitem.clear();
	invalid();
}

void xui_listview::set_itemvisible( xui_listitem* item )
{
	if (m_vscroll)
	{
		u32 index = get_itemindex(item);
		s32 maxvalue = index * m_lineheight;
		s32 minvalue = maxvalue - get_renderrtins().get_h() + m_lineheight;
		if (m_vscroll->get_value() < minvalue)
			m_vscroll->set_value(minvalue);
		if (m_vscroll->get_value() > maxvalue)
			m_vscroll->set_value(maxvalue);
	}
}

void xui_listview::on_invalid( xui_method_args& args )
{
	s32 maxwidth = 0;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
		maxwidth = xui_max(maxwidth, item->get_maxwidth());
	}

	xui_vector<s32> sz(maxwidth, m_lineheight * m_ascrollitem.size());
	xui_rect2d<s32> rt = get_renderrtins();
	sz.w = xui_max(sz.w, rt.get_sz().w);
	sz.h = xui_max(sz.h, rt.get_sz().h);

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
	}
}

void xui_listview::on_perform( xui_method_args& args )
{
	xui_container::on_perform(args);

	xui_vector<s32> pt;
	xui_vector<s32> sz;
	pt.x = m_border.ax;
	pt.y = m_border.ay;
	sz.w = get_clientw();
	sz.h = m_lineheight;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		m_ascrollitem[i]->on_perform_pt(pt);
		m_ascrollitem[i]->on_perform_sz(sz);
		pt.y += m_lineheight;
	}
}
