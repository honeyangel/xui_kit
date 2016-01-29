#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_border.h"
#include "xui_scroll.h"
#include "xui_panel.h"

xui_implement_rtti(xui_panel, xui_container);

/*
//constructor
*/
xui_create_explain(xui_panel)( const xui_vector<s32>& size )
: xui_container(size)
{
	m_backcolor = xui_colour(1.0f, 0.15f);
	//m_resizevec.push_back(new xui_border(xui_vector<s32>(5), this, RESIZEFLAG_L));
	//m_resizevec.push_back(new xui_border(xui_vector<s32>(5), this, RESIZEFLAG_R));
	//m_resizevec.push_back(new xui_border(xui_vector<s32>(5), this, RESIZEFLAG_T));
	//m_resizevec.push_back(new xui_border(xui_vector<s32>(5), this, RESIZEFLAG_B));

	refresh();
}

/*
//destructor
*/
xui_delete_explain(xui_panel)( void )
{
	for (u32 i = 0; i < m_resizevec.size(); ++i)
		delete m_resizevec[i];

	del_children();
}

/*
//child
*/
xui_method_explain(xui_panel, get_children,		const std::vector<xui_control*>&	)( void ) const
{
	return m_childctrl;
}
xui_method_explain(xui_panel, get_childcount,	u32									)( void ) const
{
	return m_childctrl.size();
}
xui_method_explain(xui_panel, get_childindex,	u32									)( xui_control* child ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i] == child)
			return i;
	}

	return -1;
}
xui_method_explain(xui_panel, set_childindex,	void								)( xui_control* child, u32 index )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_childctrl.begin(), 
		m_childctrl.end(), 
		child);

	if (itor != m_childctrl.end())
	{
		m_childctrl.erase(itor);
		m_childctrl.insert(m_childctrl.begin()+index, child);
	}
}
xui_method_explain(xui_panel, was_child,		bool								)( xui_control* child ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i] == child)
			return true;
	}

	return false;
}
xui_method_explain(xui_panel, get_child,		xui_control*						)( u32 index ) const
{
	return m_childctrl[index];
}
xui_method_explain(xui_panel, get_child,		xui_control*						)( const std::string& name ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->get_name() == name)
			return m_childctrl[i];
	}

	return NULL;
}
xui_method_explain(xui_panel, was_descendant,	bool								)( xui_control* child ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i] == child)
			return true;

		xui_panel* panel = xui_dynamic_cast(xui_panel, m_childctrl[i]);
		if (panel && panel->was_descendant(child))
			return true;
	}

	return false;
}
xui_method_explain(xui_panel, get_descendant,	xui_control*						)( const std::string& name ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->get_name() == name)
			return m_childctrl[i];

		xui_panel* panel = xui_dynamic_cast(xui_panel, m_childctrl[i]);
		if (panel)
		{
			xui_control* child = NULL;
			if (child = panel->get_descendant(name))
				return child;
		}
	}

	return NULL;
}
xui_method_explain(xui_panel, add_child,		void								)( xui_control* child )
{
	if (child == NULL || was_child(child) || child->get_parent())
		return;

	//设值父控件
	child->set_parent(this);
	m_childctrl.push_back(child);

	//
	xui_vector<s32> pt;
	pt.x = m_hscroll == NULL ? 0 : m_hscroll->get_value();
	pt.y = m_vscroll == NULL ? 0 : m_vscroll->get_value();
	child->set_scrollpt(pt);

	//事件
	xui_method_args	  args;
	args.wparam = (void*)child;
	on_addchild(	  args);
	xm_addchild(this, args);
}
xui_method_explain(xui_panel, del_child,		void								)( xui_control* child, bool destroy )
{
	if (child == NULL)
		return;

	std::vector<xui_control*>::iterator itor = std::find(
		m_childctrl.begin(), 
		m_childctrl.end(), 
		child);

	if (itor == m_childctrl.end())
		return;

	//移除事件
	xui_method_args   args;
	args.wparam = (void*)child;
	on_delchild(	  args);
	xm_delchild(this, args);

	//移除控件
	child->set_parent(NULL);
	m_childctrl.erase(itor);

	//销毁控件
	if (destroy)
		xui_desktop::get_ins()->move_recycle(this);
}
xui_method_explain(xui_panel, del_children,		void								)( void )
{
	std::vector<xui_control*> temp = m_childctrl;
	xui_vecptr_addloop(temp)
		del_child(temp[i]);
}

/*
//move
*/
xui_method_explain(xui_panel, movfore,			void								)( xui_control* child )
{
	u32 index = get_childindex(child);
	if (index < get_childcount()-1)
		set_childindex(child, index+1);
}
xui_method_explain(xui_panel, movback,			void								)( xui_control* child )
{
	u32 index = get_childindex(child);
	if (index > 0)
		set_childindex(child, index-1);
}
xui_method_explain(xui_panel, realign,			void								)( void )
{
	xui_vecptr_addloop(m_childctrl)
	{
		xui_panel* panel = xui_dynamic_cast(xui_panel, m_childctrl[i]);
		if (panel)
			panel->realign();
	}

	refresh();
}

/*
//virtual
*/
xui_method_explain(xui_panel, choose_else,		xui_component*						)( const xui_vector<s32>& pt )
{
	xui_component* component = xui_container::choose_else(pt);
	if (component == NULL)
	{
		if (m_render.was_inside(pt))
		{
			xui_vector<s32> relative = pt - m_render.get_pt();
			xui_vecptr_addloop(m_resizevec)
			{
				if (component = m_resizevec[i]->choose(relative))
					return component;
			}
		}
	}
	if (component == NULL)
	{
		xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
		if (rt.was_inside(pt))
		{
			xui_vector<s32> screenpt;
			screenpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
			screenpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
			xui_vector<s32> relative = pt - m_render.get_pt() + screenpt;
			xui_vecptr_addloop(m_childctrl)
			{
				if (component = m_childctrl[i]->choose(relative))
					return component;
			}
		}
	}

	return component;
}
xui_method_explain(xui_panel, update_else,		void								)( f32 delta )
{
	xui_container::update_else(delta);
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->was_enable())
			m_childctrl[i]->update(delta);
	}
}
xui_method_explain(xui_panel, render_else,		void								)( void )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtins()+get_screenpt()));
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->was_visible())
			m_childctrl[i]->render();
	}
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_container::render_else();
}

/*
//callback
*/
xui_method_explain(xui_panel, on_addchild,		void								)( xui_method_args& args )
{
	invalid();
}
xui_method_explain(xui_panel, on_delchild,		void								)( xui_method_args& args )
{
	invalid();
}
xui_method_explain(xui_panel, on_perform,		void								)( xui_method_args& args )
{
	xui_container::on_perform(args);
	if (m_childctrl.size() > 0)
	{
		std::vector<xui_component*> vec;
		xui_vecptr_addloop(m_childctrl)
		{
			vec.push_back(m_childctrl[i]);
		}

		xui_rect2d<s32> rt = get_clientrt();
		rt.ax += m_border.ax;
		rt.bx -= m_border.bx;
		rt.ay += m_border.ay;
		rt.by -= m_border.by;

		perform_alignhorz(rt, vec);
		perform_alignvert(rt, vec);
		perform_dockstyle(rt, vec);
	}

	if (m_resizevec.size() > 0)
	{
		xui_rect2d<s32> rt = get_renderrt();
		perform_dockstyle(rt, m_resizevec);
	}
}
xui_method_explain(xui_panel, on_invalid,		void								)( xui_method_args& args )
{
	xui_vector<s32> sz(0);
	xui_vecptr_addloop(m_childctrl)
	{
		sz.w = xui_max(sz.w, m_childctrl[i]->get_renderx() + m_childctrl[i]->get_renderw());
		sz.h = xui_max(sz.h, m_childctrl[i]->get_rendery() + m_childctrl[i]->get_renderh());
	}

	xui_rect2d<s32> rt = get_renderrtins();
	sz.w = xui_max(sz.w, rt.get_w());
	sz.h = xui_max(sz.h, rt.get_h());

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(xui_panel, on_vertvalue,		void								)( xui_method_args& args )
{
	xui_container::on_vertvalue(args);
	xui_vecptr_addloop(m_childctrl)
	{
		m_childctrl[i]->set_scrolly(m_vscroll->get_value());
	}
}
xui_method_explain(xui_panel, on_horzvalue,		void								)( xui_method_args& args )
{
	xui_container::on_horzvalue(args);
	xui_vecptr_addloop(m_childctrl)
	{
		m_childctrl[i]->set_scrollx(m_hscroll->get_value());
	}
}