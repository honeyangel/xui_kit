#include "xui_convas.h"
#include "xui_global.h"
#include "xui_menu.h"
#include "xui_window.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_menuitem.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"

xui_implement_rtti(xui_dockview, xui_control);

/*
//static
*/
xui_method_explain(xui_dockview, create,				xui_dockview*						)( void )
{
	xui_dockview* dockview = new xui_dockview(xui_vector<s32>(0), DOCKSTYLE_F);
	return dockview;
}

/*
//constructor
*/
xui_create_explain(xui_dockview)( const xui_vector<s32>& size, u08 dockstyle )
: xui_control(size)
{
	m_dockstyle = dockstyle;
	m_portions	= 0.0f;
	m_showpage	= NULL;
	m_viewmenu	= xui_menu::create(80);
	xui_menuitem* menuitem = m_viewmenu->add_item(NULL, L"Close");
	menuitem->xm_click			+= new xui_method_member<xui_method_args,  xui_dockview>(this, &xui_dockview::on_viewmenucloseclick);

	m_menuctrl = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_menuctrl, set_parent	)(this);
	xui_method_ptrcall(m_menuctrl, set_menu		)(m_viewmenu);
	xui_method_ptrcall(m_menuctrl, ini_component)(true, false);
	m_menuctrl->xm_renderself	+= new xui_method_member<xui_method_args,  xui_dockview>(this, &xui_dockview::on_menuctrlrenderself);
	m_widgetvec.push_back(m_menuctrl);

	u08 cursor = CURSOR_DEFAULT;
	switch (dockstyle)
	{
	case DOCKSTYLE_L:
	case DOCKSTYLE_R:
		cursor = CURSOR_WE;
		break;
	case DOCKSTYLE_T:
	case DOCKSTYLE_B:
		cursor = CURSOR_NS;
		break;
	}
	m_sizectrl	= new xui_component(xui_vector<s32>(6));
	xui_method_ptrcall(m_sizectrl, set_parent	)(this);
	xui_method_ptrcall(m_sizectrl, set_cursor	)(cursor);
	xui_method_ptrcall(m_sizectrl, ini_component)(true, dockstyle != DOCKSTYLE_F);
	m_sizectrl->xm_mousemove	+= new xui_method_member<xui_method_mouse, xui_dockview>(this, &xui_dockview::on_sizectrlmousemove);
	m_sizectrl->xm_topdraw		+= new xui_method_member<xui_method_args,  xui_dockview>(this, &xui_dockview::on_sizectrltopdraw);
	m_widgetvec.push_back(m_sizectrl);
}

/*
//method
*/
xui_method_explain(xui_dockview, was_pageshow,			bool								)( xui_dockpage* page )
{
	xui_window* window = page->get_window();
	if (window)
		return window->was_visible();

	return false;
}
xui_method_explain(xui_dockview, set_pageshow,			void								)( xui_dockpage* page, bool flag )
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
			del_dockpage(page);
		}
	}
}
xui_method_explain(xui_dockview, get_pagelist,			const std::vector<xui_dockpage*>&	)( void ) const
{
	return m_pagelist;
}
xui_method_explain(xui_dockview, get_viewlist,			const std::vector<xui_dockview*>&	)( void ) const
{
	return m_viewlist;
}
xui_method_explain(xui_dockview, get_showpage,			xui_dockpage*						)( void )
{
	return m_showpage;
}
xui_method_explain(xui_dockview, set_showpage,			void								)( xui_dockpage* page )
{
	m_showpage = page;
}
xui_method_explain(xui_dockview, has_dockpage,			bool								)( xui_dockpage* page )
{
	for (u32 i = 0; i < m_pagelist.size(); ++i)
	{
		if (m_pagelist[i] == page)
			return true;
	}

	return false;
}
xui_method_explain(xui_dockview, get_freerect,			xui_rect2d<s32>						)( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	for (u32 i = 0; i < m_viewlist.size(); ++i)
	{
		switch (m_viewlist[i]->get_dockstyle())
		{
		case DOCKSTYLE_L: rt.ax += m_viewlist[i]->get_renderw(); break;
		case DOCKSTYLE_T: rt.ay += m_viewlist[i]->get_renderh(); break;
		case DOCKSTYLE_R: rt.bx -= m_viewlist[i]->get_renderw(); break;
		case DOCKSTYLE_B: rt.by -= m_viewlist[i]->get_renderh(); break;
		}
	}

	return rt;
}
xui_method_explain(xui_dockview, get_namerect,			xui_rect2d<s32>						)( void ) const
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
xui_method_explain(xui_dockview, get_portions,			f32									)( void ) const
{
	return m_portions;
}
xui_method_explain(xui_dockview, cal_portions,			void								)( void )
{
	if (m_parent)
	{
		switch (m_dockstyle)
		{
		case DOCKSTYLE_L:
		case DOCKSTYLE_R:
			m_portions = (f32)get_renderw() / (f32)m_parent->get_renderw();
			break;
		case DOCKSTYLE_T:
		case DOCKSTYLE_B:
			m_portions = (f32)get_renderh() / (f32)m_parent->get_renderh();
			break;
		}
	}
}
xui_method_explain(xui_dockview, use_portions,			void								)( void )
{
	xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
	if (rootview)
	{
		f32 portions = m_portions;
		switch (m_dockstyle)
		{
		case DOCKSTYLE_L:
		case DOCKSTYLE_R:
			set_renderw((s32)((f32)rootview->get_renderw()*m_portions));
			break;
		case DOCKSTYLE_T:
		case DOCKSTYLE_B:
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

/*
//size
*/
xui_method_explain(xui_dockview, get_minlimit,			xui_vector<s32>						)( void )
{
	xui_vector<s32> minlimit(0);
	switch (m_dockstyle)
	{
	case DOCKSTYLE_L:
	case DOCKSTYLE_R:
		{
			minlimit.h = get_renderh();
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.w = xui_max(minlimit.w, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_T ||
					view->get_dockstyle() == DOCKSTYLE_B)
					minlimit.w  = xui_max(minlimit.w, view->get_minlimit().w);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_L ||
					view->get_dockstyle() == DOCKSTYLE_R)
					minlimit.w += xui_max(view->get_renderw(), view->get_minlimit().w);
			}
		}
		break;
	case DOCKSTYLE_T:
	case DOCKSTYLE_B:
		{
			minlimit.w = get_renderw();
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.h = xui_max(minlimit.h, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_L ||
					view->get_dockstyle() == DOCKSTYLE_R)
					minlimit.h  = xui_max(minlimit.h, view->get_minlimit().h);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_T ||
					view->get_dockstyle() == DOCKSTYLE_B)
					minlimit.h += xui_max(view->get_renderh(), view->get_minlimit().h);
			}
		}
		break;
	case DOCKSTYLE_F:
		{
			for (u32 i = 0; i < m_pagelist.size(); ++i)
			{
				minlimit.w = xui_max(minlimit.w, m_pagelist[i]->get_minlimit());
				minlimit.h = xui_max(minlimit.h, m_pagelist[i]->get_minlimit());
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_L ||
					view->get_dockstyle() == DOCKSTYLE_R)
					minlimit.w += xui_max(view->get_renderw(), view->get_minlimit().w);
			}
			for (u32 i = 0; i < m_viewlist.size(); ++i)
			{
				xui_dockview* view = m_viewlist[i];
				if (view->get_dockstyle() == DOCKSTYLE_T ||
					view->get_dockstyle() == DOCKSTYLE_B)
					minlimit.h += xui_max(view->get_renderh(), view->get_minlimit().h);
			}
		}
		break;
	}

	return minlimit;
}
xui_method_explain(xui_dockview, get_maxlimit,			xui_vector<s32>						)( void )
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
		case DOCKSTYLE_L:
		case DOCKSTYLE_R:
			maxlimit.w += freerect.get_w()-minlimit;
			break;
		case DOCKSTYLE_T:
		case DOCKSTYLE_B:
			maxlimit.h += freerect.get_h()-minlimit;
			break;
		}
	}

	return maxlimit;
}

/*
//page
*/
xui_method_explain(xui_dockview, add_dockpage,			void								)( xui_dockpage* page, u08 dockstyle, bool autosize, bool merge )
{
	if (page->get_parent())
		return;

	if (dockstyle == DOCKSTYLE_F)
	{
		if (page->has_dockarea(m_dockstyle) == false)
			return;

		if (m_showpage == NULL)
			m_showpage  = page;

		add_dockctrl(page);
		m_pagelist.push_back(page);
		m_menuctrl->set_visible(m_pagelist.size() > 0);
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

		view->add_dockpage(page, DOCKSTYLE_F);
	}

	invalid();
}
xui_method_explain(xui_dockview, del_dockpage,			void								)( xui_dockpage* page )
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
		m_showpage  = m_pagelist.size() > 0 ? m_pagelist.front() : NULL;

	m_menuctrl->set_visible(m_pagelist.size() > 0);
	refresh();
}
xui_method_explain(xui_dockview, del_dockview,			void								)( xui_dockview* view )
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
xui_method_explain(xui_dockview, mov_dockview,			void								)( std::vector<xui_dockview*>& viewlist, xui_dockview* rootview )
{
	for (u32 i = 0; i < viewlist.size(); ++i)
	{
		xui_dockview* view = viewlist[i];
		xui_method_ptrcall(view, set_parent		)(this);
		xui_method_ptrcall(view, ini_component	)(0, 0, rootview->get_dockstyle());
		xui_method_ptrcall(view, cal_portions	)();
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

/*
//load&save
*/
xui_method_explain(xui_dockview, save_config,			void								)( FILE* file, get_pagename func, u32 indent )
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
		sprintf(buffer, "%s%s\n", space.c_str(), pagename.c_str());
		fwrite(buffer, 1, strlen(buffer), file);
	}
}
xui_method_explain(xui_dockview, load_config,			void								)( FILE* file, get_pagectrl func )
{
	std::string line;

	//dockstyle
	line = xui_global::get_fileline(file);
	if (line.length() > 0)
	{
		std::string temp = line.substr(line.find_first_not_of(' '));
		sscanf(temp.c_str(), "dockstyle=%d", &m_dockstyle);
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
		xui_dockview* dockview = new xui_dockview(xui_vector<s32>(0), DOCKSTYLE_N);
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
			add_dockpage(dockpage, DOCKSTYLE_F);
		}
	}

	u08 cursor = CURSOR_DEFAULT;
	switch (m_dockstyle)
	{
	case DOCKSTYLE_L:
	case DOCKSTYLE_R:
		cursor = CURSOR_WE;
		break;
	case DOCKSTYLE_T:
	case DOCKSTYLE_B:
		cursor = CURSOR_NS;
		break;
	}
	xui_method_ptrcall(m_sizectrl, set_cursor	)(cursor);
	xui_method_ptrcall(m_sizectrl, ini_component)(true, m_dockstyle != DOCKSTYLE_F);
}

/*
//callback
*/
xui_method_explain(xui_dockview, on_setrendersz,		void								)( xui_method_args&  args )
{
	xui_control::on_setrendersz(args);
	xui_dockview* view = xui_dynamic_cast(xui_dockview, m_parent);
	if (view == NULL)
	{
		use_portions();
	}
}
xui_method_explain(xui_dockview, on_invalid,			void								)( xui_method_args&  args )
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
xui_method_explain(xui_dockview, on_perform,			void								)( xui_method_args&  args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrt();
	switch (m_dockstyle)
	{
	case DOCKSTYLE_L:
		m_sizectrl->on_perform_x(rt.bx-m_sizectrl->get_renderw());
		m_sizectrl->on_perform_h(rt.get_h());
		break;
	case DOCKSTYLE_R:
		m_sizectrl->on_perform_h(rt.get_h());
		break;
	case DOCKSTYLE_T:
		m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
		m_sizectrl->on_perform_w(rt.get_w());
		break;
	case DOCKSTYLE_B:
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
			xui_method_ptrcall(page, on_perform_pt	)(freert.get_pt());
			xui_method_ptrcall(page, on_perform_sz	)(freert.get_sz());
			xui_method_ptrcall(page, mov_namectrl	)(x, y, w);
			x += w;
		}
	}
}
xui_method_explain(xui_dockview, on_mousemove,			void								)( xui_method_mouse& args )
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

/*
//event
*/
xui_method_explain(xui_dockview, on_sizectrlmousemove,	void								)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch())
	{
		xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();

		xui_vector<s32> delta(0);
		switch (m_dockstyle)
		{
		case DOCKSTYLE_L: delta.w += move.x; break;
		case DOCKSTYLE_R: delta.w -= move.x; break;
		case DOCKSTYLE_T: delta.h += move.y; break;
		case DOCKSTYLE_B: delta.h -= move.y; break;
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
xui_method_explain(xui_dockview, on_sizectrltopdraw,	void								)( xui_component* sender, xui_method_args& args )
{
	//xui_rect2d<s32> rt = sender->get_renderrtabs();
	//xui_convas::get_ins()->fill_rectangle(rt, xui_colour(0.8f, 0.0f));
}
xui_method_explain(xui_dockview, on_menuctrlrenderself, void								)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt     = sender->get_renderrtabs();
	xui_colour	    color  = sender->was_hover() ? xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f) : xui_button::default_backcolor;
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	rt.ax = center.x - 6;
	rt.ay = center.y - 4;
	rt.bx = center.x + 6;
	rt.by = center.y - 2;
	xui_convas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_convas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_convas::get_ins()->fill_rectangle(rt, color);
}
xui_method_explain(xui_dockview, on_viewmenucloseclick,	void								)( xui_component* sender, xui_method_args& args )
{
	if (m_showpage)
	{
		set_pageshow(m_showpage, false);
	}
}

/*
//method
*/
xui_method_explain(xui_dockview, add_dockctrl,			void								)( xui_component* comp )
{
	comp->set_parent(this);
	m_widgetvec.push_back(comp);
}
xui_method_explain(xui_dockview, del_dockctrl,			void								)( xui_component* comp )
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