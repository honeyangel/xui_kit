#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_toggle.h"
#include "xui_scroll.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_itemtag.h"
#include "xui_dropbox.h"

xui_implement_rtti(xui_dropbox, xui_textbox);

/*
//static
*/
xui_method_explain(xui_dropbox, create,					xui_dropbox*	)( s32 width )
{
	xui_dropbox* dropbox = new xui_dropbox(xui_vector<s32>(width, 24));
	xui_method_ptrcall(dropbox, set_backcolor	)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(dropbox, set_drawcolor	)(true);
	xui_method_ptrcall(dropbox, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(dropbox, set_corner		)(3);
	xui_method_ptrcall(dropbox, set_borderrt	)(xui_rect2d<s32>(4, 4, 0, 4));
	xui_method_ptrcall(dropbox, set_readonly	)(true);

	return dropbox;
}
xui_method_explain(xui_dropbox, create,					xui_dropbox*	)( s32 width, xui_bitmap* icon )
{
	xui_dropbox* dropbox = new xui_dropbox(xui_vector<s32>(width, 24), true);
	xui_method_ptrcall(dropbox, set_backcolor	)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(dropbox, set_drawcolor	)(true);
	xui_method_ptrcall(dropbox, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(dropbox, set_corner		)(3);
	xui_method_ptrcall(dropbox, set_borderrt	)(xui_rect2d<s32>(4, 4, 0, 4));
	xui_method_ptrcall(dropbox, set_icon		)(icon);
	xui_method_ptrcall(dropbox, set_textoffset	)(xui_vector<s32>(4, 0));

	return dropbox;
}

/*
//constructor
*/
xui_create_explain(xui_dropbox)( const xui_vector<s32>& size, bool itemicon )
: xui_textbox(size)
{
	m_maxdrop	= 8;
	m_selitem   = NULL;
	m_droptog   = new xui_toggle	(xui_vector<s32>(24), TOGGLE_BUTTON);
	m_droptog->xm_mousedown		 += new xui_method_member<xui_method_mouse, xui_dropbox>(this, &xui_dropbox::on_droptogmousedown);
	m_droptog->xm_renderself	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droptogrenderself);
	m_droptog->xm_nonfocus		 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_dropallnonfocus);
	xui_method_ptrcall(m_droptog, set_parent	)(this);
	xui_method_ptrcall(m_droptog, ini_component	)(0, 0, DOCKSTYLE_R);
	m_widgetvec.push_back(m_droptog);

	m_droplst   = new xui_listview	(xui_vector<s32>(0), false);
	m_droplst->xm_nonfocus		 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_dropallnonfocus);
	m_droplst->xm_selectedchange += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstselection);
	m_droplst->xm_mousedown		 += new xui_method_member<xui_method_mouse, xui_dropbox>(this, &xui_dropbox::on_dropallmousedown);
	m_droplst->xm_keybddown		 += new xui_method_member<xui_method_keybd, xui_dropbox>(this, &xui_dropbox::on_dropallkeybddown);
	m_droplst->xm_setclientsz	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstsetclientsz);
	m_droplst->xm_setrendersz	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstsetrendersz);
	xui_method_ptrcall(m_droplst, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_droplst, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_droplst, set_corner	)(3);
	xui_method_ptrcall(m_droplst, set_iconsize	)(itemicon ? xui_vector<s32>(16) : xui_vector<s32>(0));

	refresh();
}

/*
//destructor
*/
xui_delete_explain(xui_dropbox)( void )
{
	delete m_droplst;
}

/*
//init
*/
xui_method_explain(xui_dropbox, ini_dropbox,			void			)( u32 selectedindex )
{
	if (selectedindex == -1)
	{
		m_selitem	= NULL;
		m_text		= L"";
	}
	else
	{
		m_selitem	= m_itemvec[selectedindex];
		m_text		= m_selitem->get_text();
	}

	set_caretindex(0);
}

/*
//prop
*/
xui_method_explain(xui_dropbox, get_maxdrop,			u32				)( void ) const
{
	return m_maxdrop;
}
xui_method_explain(xui_dropbox, set_maxdrop,			void			)( u32 count )
{
	m_maxdrop = count;
}

/*
//item
*/
xui_method_explain(xui_dropbox, get_itemcount,			u32				)( void ) const
{
	return m_itemvec.size();
}
xui_method_explain(xui_dropbox, get_item,				xui_itemtag*	)( u32 index )
{
	return m_itemvec[index];
}
xui_method_explain(xui_dropbox, add_item,				void			)( const std::wstring& text )
{
	add_item(new xui_itemtag(text));
}
xui_method_explain(xui_dropbox, add_item,				void			)( xui_itemtag* item )
{
	m_itemvec.push_back(item);
}
xui_method_explain(xui_dropbox, del_item,				void			)( xui_itemtag* item )
{
	std::vector<xui_itemtag*>::iterator itor = std::find(
		m_itemvec.begin(),
		m_itemvec.end(),
		item);

	if (itor != m_itemvec.end())
	{
		m_itemvec.erase(itor);
		set_droplisthide();

		if (m_selitem == item)
		{
			non_selecteditem();
		}
	}
}
xui_method_explain(xui_dropbox, del_itemall,			void			)( void )
{
	m_itemvec.clear();

	non_selecteditem();
	set_droplisthide();
}

/*
//selected
*/
xui_method_explain(xui_dropbox, get_selectedindex,		u32				)( void )
{
	for (u32 i = 0; i < m_itemvec.size(); ++i)
	{
		if (m_selitem == m_itemvec[i])
			return i;
	}

	return -1;
}
xui_method_explain(xui_dropbox, get_selecteditem,		xui_itemtag*	)( void )
{
	return m_selitem;
}
xui_method_explain(xui_dropbox, set_selecteditem,		void			)( xui_itemtag* item )
{
	if (m_selitem != item && item)
	{
		m_selitem  = item;
		m_text	   = item->get_text();
		set_caretindex(0);

		xui_method_args    args;
		xm_selection(this, args);
	}
}
xui_method_explain(xui_dropbox, non_selecteditem,		void			)( void )
{
	if (m_selitem)
	{
		m_selitem  = NULL;
		m_text     = L"";
		set_caretindex(0);

		xui_method_args    args;
		xm_selection(this, args);
	}
}

/*
//virtual
*/
xui_method_explain(xui_dropbox, get_renderrtins,		xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt = xui_textbox::get_renderrtins();
	rt.bx -= m_droptog->get_renderw();
	return rt;
}
xui_method_explain(xui_dropbox, update,					void			)( f32 delta )
{
	xui_textbox::update(delta);
	m_droplst->update(delta);
}

/*
//callback
*/
xui_method_explain(xui_dropbox, on_keybddown,			void			)( xui_method_keybd& args )
{
	xui_textbox::on_keybddown(args);
	if (args.kcode == KEY_ENTER)
	{
		if (m_readonly == false)
		{
			for (u32 i = 0; i < m_itemvec.size(); ++i)
			{
				if (m_itemvec[i]->get_text() == m_text)
				{
					set_selecteditem(m_itemvec[i]);
					break;
				}
			}
		}

		set_droplisthide();
	}

	if (args.kcode == KEY_DARROW)
	{
		std::wstring text = m_readonly ? L"" : m_text;
		set_droplistshow(text);

		if (m_droplst->get_itemcount() > 0)
		{
			xui_listitem* item = m_droplst->get_item(0);
			item->req_focus();
			m_droplst->set_selecteditem(item, true);
		}
	}
}
xui_method_explain(xui_dropbox, on_mousedown,			void			)( xui_method_mouse& args )
{
	xui_textbox::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		if (m_readonly)
		{
			if (m_droptog->was_push())
			{
				set_droplisthide();
			}
			else
			{
				set_droplistshow(L"");
			}
		}
	}
}
xui_method_explain(xui_dropbox, on_nonfocus,			void			)( xui_method_args&  args )
{
	xui_textbox::on_nonfocus(args);
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || (focusctrl->was_ancestor(this) == false && focusctrl->was_ancestor(m_droplst) == false))
	{
		set_droplisthide();
	}
}
xui_method_explain(xui_dropbox, on_getfocus,			void			)( xui_method_args&  args )
{
	xui_textbox::on_getfocus(args);
	if (m_readonly == false)
	{
		set_droplistshow(m_text);
	}
}
xui_method_explain(xui_dropbox, on_textchanged,			void			)( xui_method_args&  args )
{
	xui_textbox::on_textchanged(args);
	if (m_readonly == false)
	{
		set_droplistshow(m_text);
	}
}
xui_method_explain(xui_dropbox, on_perform,				void			)( xui_method_args&	 args )
{
	perform_dockstyle(get_renderrt(), m_widgetvec);
}

/*
//event
*/
xui_method_explain(xui_dropbox, on_droptogmousedown,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		if (m_droptog->was_push())
		{
			set_droplistshow(L"");
		}
		else
		{
			set_droplisthide();
		}
	}
}
xui_method_explain(xui_dropbox, on_droptogrenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt		= m_droptog->get_renderrtabs();
	xui_colour		color   = m_droptog->get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_DOWN,  color);
}
xui_method_explain(xui_dropbox, on_droplstselection,	void			)( xui_component* sender, xui_method_args&  args )
{
	std::vector<xui_listitem*> items = m_droplst->get_selecteditem();
	if (items.size() > 0)
	{
		set_selecteditem((xui_itemtag*)items[0]->get_data());
	}
}
xui_method_explain(xui_dropbox, on_dropallnonfocus,		void			)( xui_component* sender, xui_method_args&  args )
{
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || (focusctrl->was_ancestor(this) == false && focusctrl->was_ancestor(m_droplst) == false))
	{
		set_droplisthide();
	}
}
xui_method_explain(xui_dropbox, on_dropallkeybddown,	void			)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_ENTER)
	{
		set_droplisthide();
	}
}
xui_method_explain(xui_dropbox, on_dropallmousedown,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		set_droplisthide();
	}
}
xui_method_explain(xui_dropbox, on_droplstsetclientsz,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> sz = m_droplst->get_clientsz();
	xui_rect2d<s32> rt = m_droplst->get_borderrt();
	sz.w += rt.ax;
	sz.w += rt.bx;
	if (m_maxdrop < m_droplst->get_itemcount())
		sz.w += xui_scroll::default_size;

	sz.w  = xui_max(sz.w, get_renderw());
	sz.h  = xui_min(m_maxdrop, m_droplst->get_itemcount()) * m_droplst->get_lineheight();
	sz.h += rt.ay;
	sz.h += rt.by;

	m_droplst->set_rendersz(sz);
}
xui_method_explain(xui_dropbox, on_droplstsetrendersz,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> sz = m_droplst->get_rendersz();
	xui_vector<s32> pt;
	pt.x  = xui_desktop::get_ins()->get_renderw() - get_screenpt().x - sz.w ;
	pt.x  = xui_min(pt.x, 0);
	pt.y  = get_renderh();

	if (get_renderrtabs().by + m_droplst->get_renderh() < xui_desktop::get_ins()->get_renderh())
		pt.y = get_renderh();
	else
		pt.y = -m_droplst->get_renderh();

	m_droplst->set_renderpt(pt+get_screenpt());
}

/*
//method
*/
xui_method_explain(xui_dropbox, set_droplistshow,		void			)( const std::wstring& text )
{
	std::vector<xui_itemtag*> showvec;
	for (u32 i = 0; i < m_itemvec.size(); ++i)
	{
		if (text.length() == 0 || m_itemvec[i]->get_text().find(text) != -1)
			showvec.push_back(m_itemvec[i]);
	}

	if (m_droplst->get_itemcount() < showvec.size())
	{
		u32 count = showvec.size() - m_droplst->get_itemcount();
		for (u32 i = 0; i < count; ++i)
		{
			xui_listitem* item = m_droplst->add_item(L"", xui_family_render::default);
			item->xm_nonfocus  += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_dropallnonfocus);
			item->xm_keybddown += new xui_method_member<xui_method_keybd, xui_dropbox>(this, &xui_dropbox::on_dropallkeybddown);
			item->xm_mousedown += new xui_method_member<xui_method_mouse, xui_dropbox>(this, &xui_dropbox::on_dropallmousedown);
		}
	}
	if (m_droplst->get_itemcount() > showvec.size())
	{
		u32 count = m_droplst->get_itemcount() - showvec.size();
		for (u32 i = 0; i < count; ++i)
			m_droplst->del_item(m_droplst->get_item(0));
	}

	m_droplst->non_selecteditem();
	for (u32 i = 0; i < showvec.size(); ++i)
	{
		xui_listitem* item = m_droplst->get_item(i);
		item->set_text(showvec[i]->get_text());
		item->set_icon(showvec[i]->get_icon());
		item->set_data(showvec[i]);

		if (m_selitem == showvec[i])
			m_droplst->set_selecteditem(item, true);
	}

	m_droptog->set_push(true);
	m_droplst->refresh();
	xui_desktop::get_ins()->set_floatctrl(m_droplst);
}
xui_method_explain(xui_dropbox, set_droplisthide,		void			)( void )
{
	m_droptog->set_push(false);
	xui_desktop::get_ins()->set_floatctrl(NULL);
}