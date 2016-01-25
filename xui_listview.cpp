#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_listitem.h"
#include "xui_listview.h"

xui_implement_rtti(xui_listview, xui_container);

/*
//static
*/
const xui_colour xui_listview::default_movecolor = xui_colour(1.0f, 127.0f/255.0f);
const xui_colour xui_listview::default_tickcolor = xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f);

xui_method_explain(xui_listview, create,				xui_listview*				)( const xui_vector<s32>& size, bool drawtick )
{
	xui_listview* listview = new xui_listview(size, drawtick);
	xui_method_ptrcall(listview, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(listview, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(listview, set_corner		)(3);
	xui_method_ptrcall(listview, set_iconsize	)(xui_vector<s32>(16));
	xui_method_ptrcall(listview, set_iconoffset	)(xui_vector<s32>(4, 0));
	return listview;
}

/*
//constructor
*/
xui_create_explain(xui_listview)( const xui_vector<s32>& size, bool drawtick, xui_component* parent )
: xui_container(size, parent)
{
	m_drawtick		= drawtick;
	m_iconsize		= xui_vector<s32>(0);
	m_textalign		= TA_LC;
	m_iconalign		= IMAGE_FRONT_TEXT;
	m_movecolor		= default_movecolor;
	m_tickcolor		= default_tickcolor;
	m_itemborder	= xui_rect2d<s32>(2);
	m_iconoffset	= xui_vector<s32>(0);
	m_textoffset	= xui_vector<s32>(4, 0);
	m_lineheight	= 20;
	m_allowmulti	= false;
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

/*
//property
*/
xui_method_explain(xui_listview, get_iconsize,			const xui_vector<s32>&		)( void ) const
{
	return m_iconsize;
}
xui_method_explain(xui_listview, set_iconsize,			void						)( const xui_vector<s32>& iconsize )
{
	if (m_iconsize != iconsize)
	{
		m_iconsize  = iconsize;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconsize(m_iconsize);
		}

		invalid();
	}
}
xui_method_explain(xui_listview, get_textfont,			const xui_family&			)( void ) const
{
	return m_textfont;
}
xui_method_explain(xui_listview, set_textfont,			void						)( const xui_family& textfont )
{
	if (m_textfont != textfont)
	{
		m_textfont  = textfont;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textfont(m_textfont);
		}
	}
}
xui_method_explain(xui_listview, get_textdraw,			const xui_family_render&	)( void ) const
{
	return m_textdraw;
}
xui_method_explain(xui_listview, set_textdraw,			void						)( const xui_family_render& textdraw )
{
	if (m_textdraw != textdraw)
	{
		m_textdraw  = textdraw;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textdraw(m_textdraw);
		}
	}
}
xui_method_explain(xui_listview, get_textalign,			u08							)( void ) const
{
	return m_textalign;
}
xui_method_explain(xui_listview, set_textalign,			void						)( u08 textalign )
{
	if (m_textalign != textalign)
	{
		m_textalign  = textalign;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textalign(m_textalign);
		}
	}
}
xui_method_explain(xui_listview, get_iconalign,			u08							)( void ) const
{
	return m_iconalign;
}
xui_method_explain(xui_listview, set_iconalign,			void						)( u08 iconalign )
{
	if (m_iconalign != iconalign)
	{
		m_iconalign  = iconalign;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconalign(m_iconalign);
		}
	}
}
xui_method_explain(xui_listview, get_movecolor,			const xui_colour&			)( void ) const
{
	return m_movecolor;
}
xui_method_explain(xui_listview, set_movecolor,			void						)( const xui_colour& movecolor )
{
	m_movecolor = movecolor;
}
xui_method_explain(xui_listview, get_tickcolor,			const xui_colour&			)( void ) const
{
	return m_tickcolor;
}
xui_method_explain(xui_listview, set_tickcolor,			void						)( const xui_colour& tickcolor )
{
	m_tickcolor = tickcolor;
}
xui_method_explain(xui_listview, get_itemborder,		const xui_rect2d<s32>&		)( void ) const
{
	return m_itemborder;
}
xui_method_explain(xui_listview, set_itemborder,		void						)( const xui_rect2d<s32>& itemborder )
{
	if (m_itemborder != itemborder)
	{
		m_itemborder  = itemborder;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_borderrt(m_itemborder);
		}
	}
}
xui_method_explain(xui_listview, get_iconoffset,		const xui_vector<s32>&		)( void ) const
{
	return m_iconoffset;
}
xui_method_explain(xui_listview, set_iconoffset,		void						)( const xui_vector<s32>& iconoffset )
{
	if (m_iconoffset != iconoffset)
	{
		m_iconoffset  = iconoffset;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_iconoffset(m_iconoffset);
		}

		invalid();
	}
}
xui_method_explain(xui_listview, get_textoffset,		const xui_vector<s32>&		)( void ) const
{
	return m_textoffset;
}
xui_method_explain(xui_listview, set_textoffset,		void						)( const xui_vector<s32>& textoffset )
{
	if (m_textoffset != textoffset)
	{
		m_textoffset  = textoffset;
		xui_vecptr_addloop(m_ascrollitem)
		{
			xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
			item->set_textoffset(m_textoffset);
		}

		invalid();
	}
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
				xui_listitem* temp = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
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
xui_method_explain(xui_listview, non_selecteditem,		void						)( bool fireMethod )
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
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
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
		xui_listitem* item = xui_dynamic_cast(xui_listitem, m_ascrollitem[i]);
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
	return xui_dynamic_cast(xui_listitem, m_ascrollitem[index]);
}
xui_method_explain(xui_listview, add_item,				xui_listitem*				)( const std::wstring& text )
{
	return add_item(text, NULL);
}
xui_method_explain(xui_listview, add_item,				xui_listitem*				)( const std::wstring& text, xui_bitmap* icon )
{
	xui_listitem* item = new xui_listitem(this, m_itemborder, m_drawtick);
	m_ascrollitem.push_back(item);
	xui_method_ptrcall(item, ini_drawer		)(icon, m_iconsize);
	xui_method_ptrcall(item, ini_drawer		)(text, m_textfont, m_textdraw);
	xui_method_ptrcall(item, set_iconalign	)(m_iconalign);
	xui_method_ptrcall(item, set_textalign	)(m_textalign);
	xui_method_ptrcall(item, set_iconoffset	)(m_iconoffset);
	xui_method_ptrcall(item, set_textoffset	)(m_textoffset);

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	xui_method_ptrcall(item, set_scrollpt	)(pt);
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
		s32 minvalue = maxvalue - get_renderrtins().get_h() + m_lineheight;
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
xui_method_explain(xui_listview, on_perform,			void						)( xui_method_args& args )
{
	xui_container::on_perform(args);

	xui_vector<s32> pt;
	xui_vector<s32> sz;
	pt.x = m_border.ax;
	pt.y = m_border.ay;
	sz.w = get_clientw();
	sz.h = m_lineheight;
	xui_vecptr_addloop(m_ascrollitem)
	{
		m_ascrollitem[i]->on_perform_pt(pt);
		m_ascrollitem[i]->on_perform_sz(sz);
		pt.y += m_lineheight;
	}
}
