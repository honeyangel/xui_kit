#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_listitem.h"
#include "xui_listview.h"

/*
//constructor
*/
xui_listview::xui_listview( const std::string& name, const xui_rect2d<s32>& rect, s32 lineheight, const xui_rect2d<s32>& itemborder, const xui_vector<s32>& iconoffset, const xui_vector<s32>& textoffset, xui_bitmap* flagbitmap )
: xui_container(name, rect)
{
	m_type		+= "listview";
	m_flagbitmap = flagbitmap;
	m_lineheight = lineheight;
	m_itemborder = itemborder;
	m_iconoffset = iconoffset;
	m_textoffset = textoffset;
	m_allowmulti = false;
	m_font       = xui_family("Consolas", 16, false);
	m_textcolor  = xui_colour(1.0f, 1.0f, 0.0f, 0.0f);
	m_movecolor  = xui_colour(1.0f, 0.7f, 0.7f, 0.7f);
	m_flagcolor  = xui_colour(1.0f, 0.0f, 0.0f, 1.0f);
	m_textalign  = TA_LC;

	refresh();
}

/*
//multi
*/
xui_method_explain(xui_listview, was_allowmulti,		bool						)( void ) const
{
	return m_allowmulti;
}
xui_method_explain(xui_listview, set_allowmulti,		void						)( bool flag )
{
	m_allowmulti = flag;
}
xui_method_explain(xui_listview, get_lineheight,		s32							)( void ) const
{
	return m_lineheight;
}
xui_method_explain(xui_listview, set_lineheight,		void						)( s32 height )
{
	if (m_lineheight != height)
	{
		m_lineheight  = height;
		invalid();
	}
}

/*
//method
*/
xui_method_explain(xui_listview, get_font,				const xui_family&			)( void ) const
{
	return m_font;
}
xui_method_explain(xui_listview, set_font,				void						)( const xui_family& font )
{
	if (m_font != font)
	{
		m_font  = font;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
			item->set_font(m_font);
		}
	}
}
xui_method_explain(xui_listview, get_textcolor,			const xui_colour&			)( void ) const
{
	return m_textcolor;
}
xui_method_explain(xui_listview, set_textcolor,			void						)( const xui_colour& color )
{
	if (m_textcolor != color)
	{
		m_textcolor  = color;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
			item->set_textcolor(m_textcolor);
		}
	}
}
xui_method_explain(xui_listview, get_textalign,			u08							)( void ) const
{
	return m_textalign;
}
xui_method_explain(xui_listview, set_textalign,			void						)( u08 align )
{
	if (m_textalign != align)
	{
		m_textalign  = align;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
			item->set_textalign(m_textalign);
		}
	}
}
xui_method_explain(xui_listview, get_movecolor,			const xui_colour&			)( void ) const
{
	return m_movecolor;
}
xui_method_explain(xui_listview, set_movecolor,			void						)( const xui_colour& color )
{
	m_movecolor = color;
}
xui_method_explain(xui_listview, get_flagcolor,			const xui_colour&			)( void ) const
{
	return m_flagcolor;
}
xui_method_explain(xui_listview, set_flagcolor,			void						)( const xui_colour& color )
{
	m_flagcolor = color;
}

/*
//selected
*/
xui_method_explain(xui_listview, set_selecteditem,		void						)( xui_listitem* item, bool selected )
{
	if (item->was_selected() != selected)
	{
		item->set_selected(selected);
		if (selected && m_allowmulti == false)
		{
			xui_vecptr_addloop(m_ascrollitem)
			{
				xui_listitem* temp = (xui_listitem*)m_ascrollitem[i];
				if (temp != item)
					temp->set_selected(false);
			}
		}

		xui_method_args args;
		xm_selectedchange(this, args);
	}
}
xui_method_explain(xui_listview, set_selecteditem,		void						)( u32 start, u32 final )
{
	if (m_allowmulti == false)
		return;

	bool selectedchange = false;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (i >= start && i <= final)
		{
			xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
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
xui_method_explain(xui_listview, non_selecteditem,		void						)( bool fireMethod )
{
	bool selectedchange = false;
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
		if (item->was_selected())
		{
			item->set_selected(false);
			selectedchange = true;
		}
	}

	if (selectedchange && fireMethod)
	{
		xui_method_args args;
		xm_selectedchange(this, args);
	}
}
xui_method_explain(xui_listview, get_selecteditem,		std::vector<xui_listitem*>	)( void )
{
	std::vector<xui_listitem*> selecteditem;
	xui_vecptr_addloop(m_ascrollitem)
	{
		xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
		if (item->was_selected())
			selecteditem.push_back(item);
	}

	return selecteditem;
}
xui_method_explain(xui_listview, get_selecteditemindex,	std::vector<u32>			)( void )
{
	std::vector<u32> selectedindex;
	xui_vecptr_addloop(m_ascrollitem)
	{
		xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
		if (item->was_selected())
			selectedindex.push_back(i);
	}

	return selectedindex;
}

/*
//item
*/
xui_method_explain(xui_listview, get_itemcount,			u32							)( void ) const
{
	return m_ascrollitem.size();
}
xui_method_explain(xui_listview, get_itemindex,			u32							)( xui_listitem* item )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		if (m_ascrollitem[i] == item)
			return i;
	}

	return -1;
}
xui_method_explain(xui_listview, get_item,				xui_listitem*				)( u32 index )
{
	return (xui_listitem*)m_ascrollitem[index];
}
xui_method_explain(xui_listview, add_item,				xui_listitem*				)( const std::wstring& text )
{
	return add_item(text, NULL);
}
xui_method_explain(xui_listview, add_item,				xui_listitem*				)( const std::wstring& text, xui_bitmap* icon )
{
	xui_listitem* item = new xui_listitem("item", xui_rect2d<s32>(0), m_flagbitmap);
	item->set_parent(this);
	item->set_backcolor(xui_colour(0.0f, 0.0f, 0.0f, 0.0f));
	item->set_icon(icon);
	item->set_text(text);
	item->set_font(m_font);
	item->set_iconalign(IMAGE_FRONT_TEXT);
	item->set_textalign(m_textalign);
	item->set_textcolor(m_textcolor);
	item->set_iconoffset(m_iconoffset);
	item->set_textoffset(m_textoffset);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	item->set_scrollpt(pt);

	m_ascrollitem.push_back(item);

	invalid();

	return item;
}
xui_method_explain(xui_listview, del_item,				void						)( xui_listitem* item )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_ascrollitem.begin(),
		m_ascrollitem.end(),
		item);

	if (itor != m_ascrollitem.end())
	{
		delete item;
		m_ascrollitem.erase(itor);
		invalid();
	}
}
xui_method_explain(xui_listview, del_itemall,			void						)( void )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		delete m_ascrollitem[i];
	}
	m_ascrollitem.clear();
	invalid();
}
xui_method_explain(xui_listview, set_itemvisible,		void						)( xui_listitem* item )
{
	if (m_vscroll)
	{
		u32 index = get_itemindex(item);
		s32 maxvalue = index * m_lineheight;
		s32 minvalue = maxvalue - get_renderrtins().get_sz().h + m_lineheight;
		if (m_vscroll->get_value() < minvalue)
			m_vscroll->set_value(minvalue);
		if (m_vscroll->get_value() > maxvalue)
			m_vscroll->set_value(maxvalue);
	}
}

/*
//callback
*/
xui_method_explain(xui_listview, on_invalid,			void						)( xui_method_args& args )
{
	s32 maxwidth = 0;
	xui_vecptr_addloop(m_ascrollitem)
	{
		xui_listitem* item = (xui_listitem*)m_ascrollitem[i];
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
xui_method_explain(xui_listview, on_perform,			void						)( xui_method_args& args )
{
	xui_container::on_perform(args);

	xui_vector<s32> sz = get_clientsz();
	xui_vecptr_addloop(m_ascrollitem)
	{
		m_ascrollitem[i]->set_renderpt(xui_vector<s32>(m_border.ax, m_lineheight*i), false);
		m_ascrollitem[i]->set_rendersz(xui_vector<s32>(sz.w,        m_lineheight),   false);
		m_ascrollitem[i]->set_borderrt(m_itemborder);
	}
}
