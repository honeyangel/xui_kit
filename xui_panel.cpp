#include "xui_desktop.h"
#include "xui_border.h"
#include "xui_scroll.h"
#include "xui_panel.h"

/*
//constructor
*/
xui_create_explain(xui_panel)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_container(name, rect)
{
	m_type = "panel";

	xui_rect2d<s32> rt(0, 0, 5, 5);
	m_widgetvec.push_back(new xui_border("", rt, RESIZEFLAG_L, this));
	m_widgetvec.push_back(new xui_border("", rt, RESIZEFLAG_R, this));
	m_widgetvec.push_back(new xui_border("", rt, RESIZEFLAG_T, this));
	m_widgetvec.push_back(new xui_border("", rt, RESIZEFLAG_B, this));
	perform();
}

/*
//destructor
*/
xui_delete_explain(xui_panel)( void )
{
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

		if (m_childctrl[i]->get_type().find("panel") != -1)
		{
			xui_panel* panel = (xui_panel*)m_childctrl[i];
			if (panel->was_descendant(child))
				return true;
		}
	}

	return false;
}

xui_method_explain(xui_panel, get_descendant,	xui_control*						)( const std::string& name ) const
{
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->get_name() == name)
			return m_childctrl[i];

		if (m_childctrl[i]->get_type().find("panel") != -1)
		{
			xui_panel* panel = (xui_panel*)m_childctrl[i];

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
		g_desktop->move_recycle(this);
}

xui_method_explain(xui_panel, del_children,		void								)( void )
{
	std::vector<xui_control*> temp = m_childctrl;
	xui_vecptr_addloop(temp)
		del_child(temp[i]);
}

/*
//mov
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
		if (m_childctrl[i]->get_type().find("panel") != -1)
		{
			xui_panel* panel = (xui_panel*)m_childctrl[i];
			panel->realign();
		}
	}

	refresh();
}

/*
//virtual
*/
xui_method_explain(xui_panel, choose_else,		xui_component*						)( const xui_vector<s32>& pt )
{
	xui_component* componet = xui_container::choose_else(pt);
	if (componet == NULL)
	{
		xui_vecptr_addloop(m_childctrl)
		{
			if (componet = m_childctrl[i]->choose(pt))
				return componet;
		}
	}

	return componet;
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
	xui_vecptr_addloop(m_childctrl)
	{
		if (m_childctrl[i]->was_visible())
			m_childctrl[i]->render();
	}

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
		std::vector<xui_component*> tempVec;
		xui_vecptr_addloop(m_childctrl)
		{
			tempVec.push_back(m_childctrl[i]);
		}

		xui_rect2d<s32> rt = get_clientrt();
		rt.oft_x(m_border.ax);
		rt.oft_y(m_border.ay);

		perform_alignhorz(rt, tempVec);
		perform_alignvert(rt, tempVec);
		perform_dockstyle(rt, tempVec);
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