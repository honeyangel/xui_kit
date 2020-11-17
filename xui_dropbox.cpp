#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_window.h"
#include "xui_toggle.h"
#include "xui_scroll.h"
#include "xui_droplist.h"
#include "xui_listitem.h"
#include "xui_itemtag.h"
#include "xui_dropbox.h"

xui_implement_rtti(xui_dropbox, xui_textbox)

xui_dropbox* xui_dropbox::create( s32 width )
{
	xui_dropbox* dropbox = new xui_dropbox(xui_vector<s32>(width, 24));
	dropbox->set_backcolor(xui_colour(1.0f, 0.20f));
	dropbox->set_drawcolor(true);
	dropbox->set_sidestyle(k_sidestyle_s);
	dropbox->set_corner(3);
	dropbox->set_borderrt(xui_rect2d<s32>(4, 4, 0, 4));
	dropbox->set_readonly(true);

	return dropbox;
}

xui_dropbox* xui_dropbox::create( s32 width, xui_bitmap* icon )
{
	xui_dropbox* dropbox = new xui_dropbox(xui_vector<s32>(width, 24), true);
	dropbox->set_backcolor(xui_colour(1.0f, 0.20f));
	dropbox->set_drawcolor(true);
	dropbox->set_sidestyle(k_sidestyle_s);
	dropbox->set_corner(3);
	dropbox->set_borderrt(xui_rect2d<s32>(4, 4, 0, 4));
	dropbox->set_icon(icon);
	dropbox->set_textoffset(xui_vector<s32>(4, 0));

	return dropbox;
}

xui_dropbox::xui_dropbox( const xui_vector<s32>& size, bool itemicon )
: xui_textbox(size)
{
	m_maxdrop	= 8;
	m_selitem   = NULL;
	m_droptog   = new xui_toggle	(xui_vector<s32>(24), k_toggle_button);
	m_droptog->xm_mousedown		 += new xui_method_member<xui_method_mouse, xui_dropbox>(this, &xui_dropbox::on_droptogmousedown);
	m_droptog->xm_renderself	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droptogrenderself);
	m_droptog->xm_nonfocus		 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_dropallnonfocus);
	m_droptog->set_parent(this);
	m_droptog->set_backcolor(xui_colour::k_white);
	m_droptog->set_movecolor(xui_button::k_default_downcolor);
	m_droptog->ini_component(0, 0, k_dockstyle_r);
	m_widgetvec.push_back(m_droptog);

	m_droplst   = new xui_droplist	(xui_vector<s32>(0), true);
	m_droplst->xm_nonfocus		 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_dropallnonfocus);
	m_droplst->xm_selectedchange += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstselection);
	m_droplst->xm_mousedown		 += new xui_method_member<xui_method_mouse, xui_dropbox>(this, &xui_dropbox::on_dropallmousedown);
	m_droplst->xm_keybddown		 += new xui_method_member<xui_method_keybd, xui_dropbox>(this, &xui_dropbox::on_dropallkeybddown);
	m_droplst->xm_setclientsz	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstsetclientsz);
	m_droplst->xm_setrendersz	 += new xui_method_member<xui_method_args,  xui_dropbox>(this, &xui_dropbox::on_droplstsetrendersz);
	m_droplst->set_sidestyle(k_sidestyle_s);
	m_droplst->set_borderrt(xui_rect2d<s32>(4));
	m_droplst->set_corner(3);
	m_droplst->set_iconsize(itemicon ? xui_vector<s32>(16) : xui_vector<s32>(0));

	refresh();
}

xui_dropbox::~xui_dropbox( void )
{
	xui_desktop::get_ins()->move_recycle(m_droplst);
	for (u32 i = 0; i < m_itemvec.size(); ++i)
		delete m_itemvec[i];
}

void xui_dropbox::ini_dropbox( u32 selectedindex )
{
	if (selectedindex == -1 || selectedindex >= m_itemvec.size())
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

u32 xui_dropbox::get_maxdrop( void ) const
{
	return m_maxdrop;
}

void xui_dropbox::set_maxdrop( u32 count )
{
	m_maxdrop = count;
}

u32 xui_dropbox::get_itemcount( void ) const
{
	return m_itemvec.size();
}

xui_itemtag* xui_dropbox::get_item( u32 index )
{
	return m_itemvec[index];
}

void xui_dropbox::add_item( const std::wstring& text )
{
	add_item(new xui_itemtag(text));
}

void xui_dropbox::add_item( xui_itemtag* item )
{
	m_itemvec.push_back(item);
}

void xui_dropbox::del_item( xui_itemtag* item )
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

void xui_dropbox::del_itemall( void )
{
	m_itemvec.clear();
	m_droplst->del_itemall();
	non_selecteditem();
	set_droplisthide();
}

u32 xui_dropbox::get_selectedindex( void )
{
	for (u32 i = 0; i < m_itemvec.size(); ++i)
	{
		if (m_selitem == m_itemvec[i])
			return i;
	}

	return -1;
}

xui_itemtag* xui_dropbox::get_selecteditem( void )
{
	return m_selitem;
}

void xui_dropbox::set_selecteditem( xui_itemtag* item )
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

void xui_dropbox::non_selecteditem( void )
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

xui_rect2d<s32> xui_dropbox::get_renderrtins( void ) const
{
	xui_rect2d<s32> rt = xui_textbox::get_renderrtins();
	rt.bx -= m_droptog->get_renderw();
	return rt;
}

void xui_dropbox::on_keybddown( xui_method_keybd& args )
{
	xui_textbox::on_keybddown(args);
	if (args.kcode == k_key_enter)
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

	if (args.kcode == k_key_darrow)
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

void xui_dropbox::on_mousedown( xui_method_mouse& args )
{
	xui_textbox::on_mousedown(args);
	if (args.mouse == k_mb_left)
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

void xui_dropbox::on_nonfocus( xui_method_args&  args )
{
	xui_textbox::on_nonfocus(args);
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || (focusctrl->was_ancestor(this) == false && focusctrl->was_ancestor(m_droplst) == false))
	{
		set_droplisthide();
	}
}

void xui_dropbox::on_getfocus( xui_method_args&  args )
{
	xui_textbox::on_getfocus(args);
	if (m_readonly == false)
	{
		set_droplistshow(m_text);
	}
}

void xui_dropbox::on_textchanged( xui_method_args&  args )
{
	xui_textbox::on_textchanged(args);
	if (m_readonly == false)
	{
		set_droplistshow(m_text);
	}
}

void xui_dropbox::on_perform( xui_method_args&	 args )
{
	perform_dockstyle(get_renderrt(), m_widgetvec);
}

void xui_dropbox::on_droptogmousedown( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == k_mb_left)
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

void xui_dropbox::on_droptogrenderself( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt		= m_droptog->get_renderrtabs();
	xui_colour		color   = m_droptog->get_rendercolor() * get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_canvas::get_ins()->fill_triangle(center, 3, k_triangle_down,  color);
}

void xui_dropbox::on_droplstselection( xui_component* sender, xui_method_args&  args )
{
	std::vector<xui_listitem*> items = m_droplst->get_selecteditem();
	if (items.size() > 0)
	{
		set_selecteditem((xui_itemtag*)items[0]->get_data());
	}
}

void xui_dropbox::on_dropallnonfocus( xui_component* sender, xui_method_args&  args )
{
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || (focusctrl->was_ancestor(this) == false && focusctrl->was_ancestor(m_droplst) == false))
	{
		set_droplisthide();
	}
}

void xui_dropbox::on_dropallkeybddown( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == k_key_enter)
	{
		set_droplisthide();
	}
}

void xui_dropbox::on_dropallmousedown( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == k_mb_left)
	{
		set_droplisthide();
	}
}

void xui_dropbox::on_droplstsetclientsz( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> sz = m_droplst->get_clientsz();
	xui_rect2d<s32> rt = m_droplst->get_borderrt();
	sz.w += rt.ax;
	sz.w += rt.bx;
	if (m_maxdrop < m_droplst->get_itemcount())
		sz.w += xui_scroll::k_default_size;

	sz.w  = xui_max(sz.w, get_renderw());
	sz.h  = xui_min(m_maxdrop, m_droplst->get_itemcount()) * m_droplst->get_lineheight();
	sz.h += rt.ay;
	sz.h += rt.by;

	m_droplst->set_rendersz(sz);
}

void xui_dropbox::on_droplstsetrendersz( xui_component* sender, xui_method_args&  args )
{
	xui_window* window = get_window();

	xui_rect2d<s32> rt = window->get_renderrtabs();
	xui_vector<s32> sz = m_droplst->get_rendersz();
	xui_vector<s32> pt;
	pt.x  = rt.bx - get_screenpt().x - sz.w ;
	pt.x  = xui_min(pt.x, 0);
	pt.y  = get_renderh();

	if (get_renderrtabs().by + m_droplst->get_renderh() < rt.by)
		pt.y = get_renderh();
	else
		pt.y = -m_droplst->get_renderh();

	m_droplst->set_renderpt(pt+get_screenpt());
}

void xui_dropbox::set_droplistshow( const std::wstring& text )
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
			xui_listitem* item = m_droplst->add_item(L"");
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

	if (m_droplst->get_itemcount() > 0)
	{
		m_droptog->set_push(true);
		m_droplst->refresh();
		xui_window* window = get_window();
		xui_desktop::get_ins()->set_floatctrl(window, m_droplst);
	}
}

void xui_dropbox::set_droplisthide( void )
{
	m_droptog->set_push(false);
	xui_desktop::get_ins()->set_floatctrl(NULL, NULL);
}