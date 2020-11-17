#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_menu.h"
#include "xui_window.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_menuitem.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"

xui_implement_rtti(xui_dockview, xui_control)

xui_dockview* xui_dockview::create( void )
{
	xui_dockview* dockview = new xui_dockview(xui_vector<s32>(0), k_dockstyle_f);
	return dockview;
}

xui_dockview::xui_dockview( const xui_vector<s32>& size, u08 dockstyle )
: xui_control(size)
{
	m_dockstyle = dockstyle;
	m_portions	= 0.0f;
	m_showpage	= NULL;
	m_viewmenu	= xui_menu::create(80);
	xui_menuitem* menuitem = m_viewmenu->add_item(NULL, L"Close");
	menuitem->xm_click			+= new xui_method_member<xui_method_args,  xui_dockview>(this, &xui_dockview::on_viewmenucloseclick);

	m_menuctrl = new xui_toggle(xui_vector<s32>(24), k_toggle_button);
	m_menuctrl->set_parent(this);
	m_menuctrl->set_menu(m_viewmenu);
	m_menuctrl->ini_component(true, false);
	m_menuctrl->xm_renderself	+= new xui_method_member<xui_method_args,  xui_dockview>(this, &xui_dockview::on_menuctrlrenderself);
	m_widgetvec.push_back(m_menuctrl);

	u08 cursor = k_cursor_default;
	switch (dockstyle)
	{
	case k_dockstyle_l:
	case k_dockstyle_r:
		cursor = k_cursor_we;
		break;
	case k_dockstyle_t:
	case k_dockstyle_b:
		cursor = k_cursor_ns;
		break;
	}
	m_sizectrl = new xui_component(xui_vector<s32>(6));
	m_sizectrl->set_parent(this);
	m_sizectrl->set_cursor(cursor);
	m_sizectrl->ini_component(true, dockstyle != k_dockstyle_f);
	m_sizectrl->xm_mousemove    += new xui_method_member<xui_method_mouse,  xui_dockview>(this, &xui_dockview::on_sizectrlmousemove);
	m_sizectrl->xm_topdraw      += new xui_method_member<xui_method_args,   xui_dockview>(this, &xui_dockview::on_sizectrltopdraw);
	m_sizectrl->xm_renderself   += new xui_method_member<xui_method_args,   xui_dockview>(this, &xui_dockview::on_sizectrlrenderself);

	m_widgetvec.push_back(m_sizectrl);
}

bool xui_dockview::was_pageshow( xui_dockpage* page )
{
	xui_window* window = page->get_window();
	if (window)
		return window->was_visible();

	return false;
}

void xui_dockview::set_pageshow( xui_dockpage* page, bool flag )
{
	xui_window* window = page->get_window();
	if (flag)
	{
		if (window)
		{
			window->set_visible(true);
		}
		else
		{
			add_dockpage(page, page->get_initdock(), false, true);
		}
	}
	else
	{
		if (window == m_parent && window->get_owner() && m_pagelist.size() == 1 && m_viewlist.empty())
		{
			window->set_visible(false);
		}
		else
		{
			del_dockpage(page, page->was_autofree());
		}
	}
}

const std::vector<xui_dockpage*>& xui_dockview::get_pagelist( void ) const
{
	return m_pagelist;
}

const std::vector<xui_dockview*>& xui_dockview::get_viewlist( void ) const
{
	return m_viewlist;
}

xui_dockpage* xui_dockview::get_showpage( void )
{
	return m_showpage;
}

void xui_dockview::set_showpage( xui_dockpage* page )
{
	if (m_showpage != page)
	{
		m_showpage  = page;

		xui_method_args args;
		xm_pagechanged(this, args);
	}
}

bool xui_dockview::has_dockpage( xui_dockpage* page )
{
	for (u32 i = 0; i < m_pagelist.size(); ++i)
	{
		if (m_pagelist[i] == page)
			return true;
	}

	return false;
}

xui_rect2d<s32> xui_dockview::get_freerect( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	for (u32 i = 0; i < m_viewlist.size(); ++i)
	{
		switch (m_viewlist[i]->get_dockstyle())
		{
		case k_dockstyle_l: rt.ax += m_viewlist[i]->get_renderw(); break;
		case k_dockstyle_t: rt.ay += m_viewlist[i]->get_renderh(); break;
		case k_dockstyle_r: rt.bx -= m_viewlist[i]->get_renderw(); break;
		case k_dockstyle_b: rt.by -= m_viewlist[i]->get_renderh(); break;
		}
	}

	return rt;
}

xui_rect2d<s32> xui_dockview::get_namerect( void ) const
{
	xui_rect2d<s32> rt(0);
	if (m_pagelist.size() > 0)
	{
		s32 maxtotal = get_freerect().get_w()-32;
		s32 total  = 0;
		for (u32 i = 0; i < m_pagelist.size(); ++i)
		{
			total += m_pagelist[i]->get_namesize();
		}

		rt.set_x(2);
		rt.set_y(2);
		rt.set_h(24);
		rt.set_w(xui_min(total, maxtotal));
	}

	return rt;
}

f32 xui_dockview::get_portions( void ) const
{
	return m_portions;
}

void xui_dockview::cal_portions( void )
{
	if (m_parent)
	{
		switch (m_dockstyle)
		{
		case k_dockstyle_l:
		case k_dockstyle_r:
			m_portions = (f32)get_renderw() / (f32)m_parent->get_renderw();
			break;
		case k_dockstyle_t:
		case k_dockstyle_b:
			m_portions = (f32)get_renderh() / (f32)m_parent->get_renderh();
			break;
		}
	}
}

void xui_dockview::use_portions( void )
{
	xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
	if (rootview)
	{
		f32 portions = m_portions;
		switch (m_dockstyle)
		{
		case k_dockstyle_l:
		case k_dockstyle_r:
			set_renderw((s32)((f32)rootview->get_renderw()*m_portions));
			break;
		case k_dockstyle_t:
		case k_dockstyle_b:
			set_renderh((s32)((f32)rootview->get_renderh()*m_portions));
			break;
		}
		m_portions = portions;
	}

	for (u32 i = 0; i < m_viewlist.size(); ++i)
	{
		m_viewlist[i]->use_portions();
	}
}

xui_vector<s32> xui_dockview::get_minlimit( void )
{
	xui_vector<s32> minlimit(0);
	switch (m_dockstyle)
	{
	case k_dockstyle_l:
	case k_dockstyle_r:
		{
			minlimit.h = get_renderh();
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.w = xui_max(minlimit.w, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_t ||
					view->get_dockstyle() == k_dockstyle_b)
					minlimit.w  = xui_max(minlimit.w, view->get_minlimit().w);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_l ||
					view->get_dockstyle() == k_dockstyle_r)
					minlimit.w += xui_max(view->get_renderw(), view->get_minlimit().w);
			}
		}
		break;
	case k_dockstyle_t:
	case k_dockstyle_b:
		{
			minlimit.w = get_renderw();
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.h = xui_max(minlimit.h, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_l ||
					view->get_dockstyle() == k_dockstyle_r)
					minlimit.h  = xui_max(minlimit.h, view->get_minlimit().h);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_t ||
					view->get_dockstyle() == k_dockstyle_b)
					minlimit.h += xui_max(view->get_renderh(), view->get_minlimit().h);
			}
		}
		break;
	case k_dockstyle_f:
		{
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.w = xui_max(minlimit.w, m_pagelist[i]->get_minlimit());
				minlimit.h = xui_max(minlimit.h, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_l ||
					view->get_dockstyle() == k_dockstyle_r)
					minlimit.w += xui_max(view->get_renderw(), view->get_minlimit().w);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == k_dockstyle_t ||
					view->get_dockstyle() == k_dockstyle_b)
					minlimit.h += xui_max(view->get_renderh(), view->get_minlimit().h);
			}
		}
		break;
	}

	return minlimit;
}

xui_vector<s32> xui_dockview::get_maxlimit( void )
{
	xui_vector<s32> maxlimit = get_rendersz();
	xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
	if (rootview)
	{
		xui_rect2d<s32> freerect = rootview->get_freerect();
		s32 minlimit = 0;
		const std::vector<xui_dockpage*>& pagelist = rootview->get_pagelist();
		for (u32 i = 0; i < pagelist.size(); ++i)
		{
			minlimit = xui_max(minlimit, pagelist[i]->get_minlimit());
		}
		switch (m_dockstyle)
		{
		case k_dockstyle_l:
		case k_dockstyle_r:
			maxlimit.w += freerect.get_w()-minlimit;
			break;
		case k_dockstyle_t:
		case k_dockstyle_b:
			maxlimit.h += freerect.get_h()-minlimit;
			break;
		}
	}

	return maxlimit;
}

void xui_dockview::add_dockpage( xui_dockpage* page, u08 dockstyle, bool autosize, bool merge )
{
	if (page->get_parent())
		return;

	if (dockstyle == k_dockstyle_f)
	{
		if (page->has_dockarea(m_dockstyle) == false)
			return;

		add_dockctrl(page);
		m_pagelist.push_back(page);
		m_menuctrl->set_visible(m_pagelist.size() > 0);

		if (m_showpage == NULL)
			set_showpage(page);
	}
	else
	{
		if (page->has_dockarea(dockstyle) == false)
			return;

		xui_dockview* view = NULL;
		for (u32 i = 0; i < m_viewlist.size(); ++i)
		{
			if (m_viewlist[i]->get_dockstyle() == dockstyle)
			{
				view = m_viewlist[i];
				break;
			}
		}
		if (view == NULL || merge == false)
		{
			xui_rect2d<s32> rt = get_freerect();
			xui_rect2d<s32> bd = page->get_borderrt();
			xui_vector<s32> sz = page->get_rendersz();
			if (autosize)
			{
				sz.w = (rt.get_w()-bd.ax-bd.bx)   /3 + bd.ax + bd.bx;
				sz.h = (rt.get_h()-bd.ay-bd.by-24)/3 + bd.ay + bd.by + 24;
			}

			view = new xui_dockview(sz, dockstyle);
			add_dockctrl(view);
			m_viewlist.push_back(view);
			view->cal_portions();
		}

		view->add_dockpage(page, k_dockstyle_f);
	}

	invalid();
}

void xui_dockview::del_dockpage( xui_dockpage* page, bool destroy )
{
	del_dockctrl(page);
	for (u32 i = 0; i < m_pagelist.size(); ++i)
	{
		if (page == m_pagelist[i])
		{
			m_pagelist.erase(m_pagelist.begin()+i);
			break;
		}
	}

	if (m_showpage == page)
		set_showpage(m_pagelist.size() > 0 ? m_pagelist.front() : NULL);

	if (destroy)
		xui_desktop::get_ins()->move_recycle(page);

	m_menuctrl->set_visible(m_pagelist.size() > 0);
	refresh();
}

void xui_dockview::del_dockview( xui_dockview* view )
{
	del_dockctrl(view);
	for (u32 i = 0; i < m_viewlist.size(); ++i)
	{
		if (view == m_viewlist[i])
		{
			m_viewlist.erase(m_viewlist.begin()+i);
			break;
		}
	}
	xui_desktop::get_ins()->move_recycle(view);

	refresh();
}

void xui_dockview::mov_dockview( std::vector<xui_dockview*>& viewlist, xui_dockview* rootview )
{
	for (u32 i = 0; i < viewlist.size(); ++i)
	{
		xui_dockview* view = viewlist[i];
		view->set_parent(this);
		view->ini_component(0, 0, rootview->get_dockstyle());
		view->cal_portions();
		m_viewlist.push_back(view);

		for (u32 i = m_widgetvec.size()-1; i >= 0; --i)
		{
			if (m_widgetvec[i] == rootview)
			{
				m_widgetvec.insert(m_widgetvec.begin()+i, view);
				break;
			}
		}
	}

	viewlist.clear();
	invalid();
}

void xui_dockview::save_config( FILE* file, get_pagename func, u32 indent )
{
	std::string space; 
	for (u32 i = 0; i < indent; ++i) 
		space += " ";

	char buffer[256];
	u32 viewcount = m_viewlist.size();
	u32 pagecount = m_pagelist.size();
	sprintf(buffer, "%sdockstyle=%d\n", space.c_str(), m_dockstyle);	fwrite(buffer, 1, strlen(buffer), file);
	sprintf(buffer, "%sportions=%f\n",  space.c_str(), m_portions);		fwrite(buffer, 1, strlen(buffer), file);
	sprintf(buffer, "%sviewcount=%d\n",	space.c_str(), viewcount);		fwrite(buffer, 1, strlen(buffer), file);
	for (u32 i = 0; i < viewcount; ++i)
	{
		xui_dockview* view = m_viewlist[i];
		view->save_config(file, func, indent+4);
	}

	sprintf(buffer, "%spagecount=%d\n",	space.c_str(), pagecount);		fwrite(buffer, 1, strlen(buffer), file);
	for (u32 i = 0; i < m_pagelist.size(); ++i)
	{
		std::string pagename = func(m_pagelist[i]);
		if (pagename.length() > 0)
		{
			sprintf(buffer, "%s%s\n", space.c_str(), pagename.c_str());
			fwrite(buffer, 1, strlen(buffer), file);
		}
	}
}

void xui_dockview::load_config( FILE* file, get_pagectrl func )
{
	std::string line;

	//dockstyle
	line = xui_global::get_fileline(file);
	if (line.length() > 0)
	{
		std::string temp = line.substr(line.find_first_not_of(' '));
		sscanf(temp.c_str(), "dockstyle=%hhd", &m_dockstyle);
	}

	//portions
	line = xui_global::get_fileline(file);
	if (line.length() > 0)
	{
		std::string temp = line.substr(line.find_first_not_of(' '));
		sscanf(temp.c_str(), "portions=%f",  &m_portions);
	}

	//viewcount
	u32 viewcount = 0;
	line = xui_global::get_fileline(file);
	if (line.length() > 0)
	{
		std::string temp = line.substr(line.find_first_not_of(' '));
		sscanf(temp.c_str(), "viewcount=%d", &viewcount);
	}
	for (u32 i = 0; i < viewcount; ++i)
	{
		xui_dockview* dockview = new xui_dockview(xui_vector<s32>(0), k_dockstyle_n);
		dockview->load_config(file, func);
		add_dockctrl(dockview);
		m_viewlist.push_back(dockview);
	}

	//pagecount
	u32 pagecount = 0;
	line = xui_global::get_fileline(file);
	if (line.length() > 0)
	{
		std::string temp = line.substr(line.find_first_not_of(' '));
		sscanf(temp.c_str(), "pagecount=%d", &pagecount);
	}
	for (u32 i = 0; i < pagecount; ++i)
	{
		line = xui_global::get_fileline(file);
		if (line.empty())
			continue;

		std::string temp = line.substr(line.find_first_not_of(' '));
		xui_dockpage* dockpage = func(temp);
		if (dockpage)
		{
			add_dockpage(dockpage, k_dockstyle_f);
		}
	}

	u08 cursor = k_cursor_default;
	switch (m_dockstyle)
	{
	case k_dockstyle_l:
	case k_dockstyle_r:
		cursor = k_cursor_we;
		break;
	case k_dockstyle_t:
	case k_dockstyle_b:
		cursor = k_cursor_ns;
		break;
	}
	m_sizectrl->set_cursor(cursor);
	m_sizectrl->ini_component(true, m_dockstyle != k_dockstyle_f);
}

void xui_dockview::on_setrendersz( xui_method_args&  args )
{
	xui_control::on_setrendersz(args);
	xui_dockview* view = xui_dynamic_cast(xui_dockview, m_parent);
	if (view == NULL)
	{
		use_portions();
	}
}

void xui_dockview::on_invalid( xui_method_args&  args )
{
	xui_control::on_invalid(args);

	xui_dockview* view = xui_dynamic_cast(xui_dockview, m_parent);
	if (view)
	{
		if (m_pagelist.empty())
		{
			view->mov_dockview(m_viewlist, this);
			m_widgetvec.clear();
			m_widgetvec.push_back(m_sizectrl);
			m_widgetvec.push_back(m_menuctrl);
			view->del_dockview(this);
		}
		else
		{
			xui_vector<s32> sz = get_rendersz();
			xui_vector<s32> minlimit = get_minlimit();
			sz.w = xui_max(sz.w, minlimit.w);
			sz.h = xui_max(sz.h, minlimit.h);
			if (get_rendersz() != sz)
			{
				set_rendersz(sz);
				cal_portions();
				for (u32 i = 0; i < m_viewlist.size(); ++i)
				{
					m_viewlist[i]->cal_portions();
				}
			}
		}
	}
	else
	{
		xui_window* window = get_window();
		if (m_pagelist.empty() && m_viewlist.empty() && window->get_owner())
		{
			xui_desktop::get_ins()->del_child(window);
		}
	}
}

void xui_dockview::on_perform( xui_method_args&  args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrt();
	switch (m_dockstyle)
	{
	case k_dockstyle_l:
		m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
		m_sizectrl->on_perform_h(rt.get_h());
		break;
	case k_dockstyle_r:
		m_sizectrl->on_perform_h(rt.get_h());
		break;
	case k_dockstyle_t:
		m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
		m_sizectrl->on_perform_w(rt.get_w());
		break;
	case k_dockstyle_b:
		m_sizectrl->on_perform_w(rt.get_w());
		break;
	}

	xui_rect2d<s32> freert = get_freerect();
	m_menuctrl->on_perform_x(freert.bx-2-m_menuctrl->get_renderw());
	m_menuctrl->on_perform_y(freert.ay+2);

	if (m_pagelist.size() > 0)
	{
		s32 maxtotal = freert.get_w()-32;
		s32 total  = 0;
		for (u32 i = 0; i < m_pagelist.size(); ++i)
		{
			total += m_pagelist[i]->get_namesize();
		}

		xui_rect2d<s32> namert = get_namerect();
		s32 x = 2;
		s32 y = 2;
		s32 w = maxtotal / m_pagelist.size();
		for (u32 i = 0; i < m_pagelist.size(); ++i)
		{
			xui_dockpage* page = m_pagelist[i];

			w = (total > maxtotal) ? w : page->get_namesize();
			page->on_perform_pt(freert.get_pt());
			page->on_perform_sz(freert.get_sz());
			page->mov_namectrl(x, y, w);
			x += w;
		}
	}
}

void xui_dockview::on_mousemove( xui_method_mouse& args )
{
	xui_control::on_mousemove(args);
	if (has_catch())
	{
		xui_window* window = get_window();
		if (window == m_parent && window->get_owner())
		{
			xui_vector<s32> pt = window->get_renderpt() + xui_desktop::get_ins()->get_mousemove();
			window->set_renderpt(pt);
		}
	}
}

void xui_dockview::on_sizectrlmousemove( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch())
	{
		xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

		xui_vector<s32> delta(0);
		switch (m_dockstyle)
		{
		case k_dockstyle_l: delta.w += move.x; break;
		case k_dockstyle_r: delta.w -= move.x; break;
		case k_dockstyle_t: delta.h += move.y; break;
		case k_dockstyle_b: delta.h -= move.y; break;
		}

		xui_vector<s32> sz = get_rendersz() + delta;
		xui_vector<s32> minlimit = get_minlimit();
		xui_vector<s32> maxlimit = get_maxlimit();
		sz.w = xui_max(sz.w, minlimit.w);
		sz.h = xui_max(sz.h, minlimit.h);
		sz.w = xui_min(sz.w, maxlimit.w);
		sz.h = xui_min(sz.h, maxlimit.h);

		if (get_rendersz() != sz)
		{
			set_rendersz(sz);
			cal_portions();
		}
	}
}

void xui_dockview::on_sizectrltopdraw( xui_component* sender, xui_method_args& args )
{
	//xui_rect2d<s32> rt = sender->get_renderrtabs();
	//xui_canvas::get_ins()->fill_rectangle(rt, xui_colour(0.8f, 0.0f));
}

void xui_dockview::on_menuctrlrenderself( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt     = sender->get_renderrtabs();
	xui_colour	    color  = sender->was_hover() ? xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f) : xui_button::k_default_backcolor;
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	rt.ax = center.x - 6;
	rt.ay = center.y - 4;
	rt.bx = center.x + 6;
	rt.by = center.y - 2;
	xui_canvas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_canvas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_canvas::get_ins()->fill_rectangle(rt, color);
}

void xui_dockview::on_viewmenucloseclick( xui_component* sender, xui_method_args& args )
{
	if (m_showpage)
	{
		set_pageshow(m_showpage, false);
	}
}

void xui_dockview::add_dockctrl( xui_component* comp )
{
	comp->set_parent(this);
	m_widgetvec.push_back(comp);
}

void xui_dockview::del_dockctrl( xui_component* comp )
{
	comp->set_parent(NULL);
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (comp == m_widgetvec[i])
		{
			m_widgetvec.erase(m_widgetvec.begin()+i);
			break;
		}
	}
}

void xui_dockview::on_sizectrlrenderself( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt    = sender->get_renderrtabs();
	xui_colour      color = sender->was_hover() ? xui_colour(1.0f, 42.0f / 255.0f, 135.0f / 255.0f, 190.0f / 255.0f) : xui_button::k_default_backcolor;

	switch (m_dockstyle)
	{
	case k_dockstyle_l:
		rt.ax = rt.bx -1;
		rt.ay = rt.ay +3;
		break;
	case k_dockstyle_r:
		rt.bx = rt.ax + 1;
		rt.ay = rt.ay + 3;
		break;
	case k_dockstyle_t:
		rt.ay = rt.by - 1;
		break;
	case k_dockstyle_b:
		rt.by = rt.ay + 1;
		break;
	}

	xui_canvas::get_ins()->fill_rectangle(rt, color);
}