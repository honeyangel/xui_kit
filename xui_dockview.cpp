#include "xui_desktop.h"
#include "xui_dockpage.h"
#include "xui_dockview.h"

xui_implement_rtti(xui_dockview, xui_control);

/*
//static
*/
xui_method_explain(xui_dockview, create,				xui_dockview*	)( void )
{
	xui_dockview* dockview = new xui_dockview(xui_vector<s32>(0));
	dockview->ini_component(0, 0, DOCKSTYLE_F);
	return dockview;
}

/*
//constructor
*/
xui_create_explain(xui_dockview)( const xui_vector<s32>& size )
: xui_control(size)
{
	m_showpage = NULL;
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
	}
	else
	{
		xui_rect2d<s32> rt = get_freerect();
		xui_dockview* view = new xui_dockview(xui_vector<s32>(rt.get_sz()/3));
		xui_method_ptrcall(view, ini_component	)(0, 0, dockstyle);
		xui_method_ptrcall(view, add_dockpage	)(page, DOCKSTYLE_F);
		add_dockctrl(view);
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

	invalid();

	if (m_pagelist.empty() && m_viewlist.empty())
	{
		xui_dockview* view = xui_dynamic_cast(xui_dockview, m_parent);
		if (view)
			view->del_dockview(this);
	}
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

	if (m_pagelist.empty() && m_viewlist.empty())
	{
		xui_dockview* view = xui_dynamic_cast(xui_dockview, m_parent);
		if (view)
			view->del_dockview(this);
	}
}

/*
//callback
*/
xui_method_explain(xui_dockview, on_invalid,			void			)( xui_method_args& args )
{
	xui_control::on_invalid(args);
	if (m_pagelist.empty() && m_viewlist.size() > 0)
	{
		if (m_dockstyle == DOCKSTYLE_L ||
			m_dockstyle == DOCKSTYLE_R)
		{
			s32 w = 0;
			xui_vecptr_addloop(m_viewlist)
			{
				w += m_viewlist[i]->get_renderw();
			}
			set_renderw(w);
		}
		if (m_dockstyle == DOCKSTYLE_T ||
			m_dockstyle == DOCKSTYLE_B)
		{
			s32 h = 0;
			xui_vecptr_addloop(m_viewlist)
			{
				h += m_viewlist[i]->get_renderh();
			}
			set_renderh(h);
		}
	}
}
xui_method_explain(xui_dockview, on_perform,			void			)( xui_method_args& args )
{
	xui_control::on_perform(args);

	if (m_pagelist.size() > 0)
	{
		xui_rect2d<s32> freert = get_freerect();
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

}
xui_method_explain(xui_dockview, on_sizectrltopdraw,	void			)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_dockview, on_menuctrlrenderself, void			)( xui_component* sender, xui_method_args& args )
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