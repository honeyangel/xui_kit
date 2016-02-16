#include "xui_convas.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_menuitem.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"

xui_implement_rtti(xui_dockview, xui_control);

/*
//static
*/
xui_method_explain(xui_dockview, create,				xui_dockview*	)( void )
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
xui_method_explain(xui_dockview, get_showpage,			xui_dockpage*	)( void )
{
	return m_showpage;
}
xui_method_explain(xui_dockview, set_showpage,			void			)( xui_dockpage* page )
{
	m_showpage = page;
}
xui_method_explain(xui_dockview, get_freerect,			xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vecptr_addloop(m_viewlist)
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
xui_method_explain(xui_dockview, get_namerect,			xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt(0);
	if (m_pagelist.size() > 0)
	{
		s32 w = xui_min(80, (get_freerect().get_w()-32)/m_pagelist.size());
		rt.set_x(2);
		rt.set_y(2);
		rt.set_h(24);
		rt.set_w(w*m_pagelist.size());
	}

	return rt;
}
xui_method_explain(xui_dockview, get_portions,			f32				)( void ) const
{
	return m_portions;
}
xui_method_explain(xui_dockview, cal_portions,			void			)( void )
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
xui_method_explain(xui_dockview, use_portions,			void			)( void )
{
	f32 portions = m_portions;
	//TODO
	m_portions = portions;

	xui_vecptr_addloop(m_viewlist)
	{
		m_viewlist[i]->use_portions();
	}
}

/*
//page
*/
xui_method_explain(xui_dockview, add_dockpage,			void			)( xui_dockpage* page, u08 dockstyle )
{
	if (page->get_parent())
		return;

	if (dockstyle == DOCKSTYLE_F)
	{
		if (m_showpage == NULL)
			m_showpage  = page;

		add_dockctrl(page);
		m_pagelist.push_back(page);
		m_menuctrl->set_visible(m_pagelist.size() > 0);
	}
	else
	{
		xui_rect2d<s32> rt = get_freerect();
		xui_rect2d<s32> bd = page->get_borderrt();
		xui_vector<s32> sz;
		sz.w = (rt.get_w()-bd.ax-bd.bx)   /3 + bd.ax + bd.bx;
		sz.h = (rt.get_h()-bd.ay-bd.by-24)/3 + bd.ay + bd.by + 24;
		xui_dockview* view = new xui_dockview(sz, dockstyle);
		add_dockctrl(view);
		xui_method_ptrcall(view, cal_portions)();
		xui_method_ptrcall(view, add_dockpage)(page, DOCKSTYLE_F);
		m_viewlist.push_back(view);
	}

	invalid();
}
xui_method_explain(xui_dockview, del_dockpage,			void			)( xui_dockpage* page )
{
	del_dockctrl(page);
	xui_vecptr_addloop(m_pagelist)
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
	invalid();
}
xui_method_explain(xui_dockview, del_dockview,			void			)( xui_dockview* view )
{
	del_dockctrl(view);
	xui_vecptr_addloop(m_viewlist)
	{
		if (view == m_viewlist[i])
		{
			m_viewlist.erase(m_viewlist.begin()+i);
			break;
		}
	}
	xui_desktop::get_ins()->move_recycle(view);

	invalid();
}
xui_method_explain(xui_dockview, mov_dockview,			void			)( std::vector<xui_dockview*>& viewlist, xui_dockview* rootview )
{
	xui_vecptr_addloop(viewlist)
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
//callback
*/
xui_method_explain(xui_dockview, on_setrendersz,		void			)( xui_method_args& args )
{
	xui_control::on_setrendersz(args);
	cal_portions();
}
xui_method_explain(xui_dockview, on_invalid,			void			)( xui_method_args& args )
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
	}
	else
	{
		use_portions();
	}
}
xui_method_explain(xui_dockview, on_perform,			void			)( xui_method_args& args )
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
		xui_rect2d<s32> namert = get_namerect();
		s32 x = namert.get_x();
		s32 y = namert.get_y();
		s32 w = namert.get_w() / m_pagelist.size();
		xui_vecptr_addloop(m_pagelist)
		{
			xui_dockpage* page = m_pagelist[i];
			xui_method_ptrcall(page, on_perform_pt	)(freert.get_pt());
			xui_method_ptrcall(page, on_perform_sz	)(freert.get_sz());
			xui_method_ptrcall(page, mov_namectrl	)(x, y, w);
			x += w;
		}
	}
}

/*
//event
*/
xui_method_explain(xui_dockview, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
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

		xui_rect2d<s32> freert = get_freerect();
		if (delta.w < 0 && freert.get_w()+delta.w < 60)
			delta.w = xui_min(0, 60-freert.get_w());
		if (delta.h < 0 && freert.get_h()+delta.h < 60)
			delta.h = xui_min(0, 60-freert.get_h());

		xui_dockview* rootview = xui_dynamic_cast(xui_dockview, m_parent);
		if (rootview)
		{
			freert = rootview->get_freerect();
			if (delta.w > 0 && freert.get_w()-delta.w < 60)
				delta.w = freert.get_w()-60;
			if (delta.h > 0 && freert.get_h()-delta.h < 60)
				delta.h = freert.get_h()-60;
		}

		if (delta.w != 0 || delta.h != 0)
			set_rendersz(get_rendersz()+delta);
	}
}
xui_method_explain(xui_dockview, on_sizectrltopdraw,	void			)( xui_component* sender, xui_method_args& args )
{
	//xui_rect2d<s32> rt = sender->get_renderrtabs();
	//xui_convas::get_ins()->fill_rectangle(rt, xui_colour(0.8f, 0.0f));
}
xui_method_explain(xui_dockview, on_menuctrlrenderself, void			)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(xui_dockview, on_viewmenucloseclick,	void			)( xui_component* sender, xui_method_args& args )
{

}

/*
//method
*/
xui_method_explain(xui_dockview, add_dockctrl,			void			)( xui_component* comp )
{
	comp->set_parent(this);
	m_widgetvec.push_back(comp);
}
xui_method_explain(xui_dockview, del_dockctrl,			void			)( xui_component* comp )
{
	comp->set_parent(NULL);
	xui_vecptr_addloop(m_widgetvec)
	{
		if (comp == m_widgetvec[i])
		{
			m_widgetvec.erase(m_widgetvec.begin()+i);
			break;
		}
	}
}