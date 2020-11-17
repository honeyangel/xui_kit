#include "xui_desktop.h"
#include "xui_canvas.h"
#include "xui_border.h"
#include "xui_scroll.h"
#include "xui_panel.h"

xui_implement_rtti(xui_panel, xui_container)

xui_panel::xui_panel( const xui_vector<s32>& size )
: xui_container(size)
{
	m_backcolor = xui_colour(1.0f, 0.15f);
	refresh();
}

xui_panel::~xui_panel( void )
{
	del_children();
}

const std::vector<xui_control*>& xui_panel::get_children( void ) const
{
	return m_childctrl;
}

u32 xui_panel::get_childcount( void ) const
{
	return m_childctrl.size();
}

u32 xui_panel::get_childindex( xui_control* child ) const
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i] == child)
			return i;
	}

	return -1;
}

void xui_panel::set_childindex( xui_control* child, u32 index )
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

bool xui_panel::was_child( xui_control* child ) const
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i] == child)
			return true;
	}

	return false;
}

xui_control* xui_panel::get_child( u32 index ) const
{
	return m_childctrl[index];
}

xui_control* xui_panel::get_child( const std::string& name ) const
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i]->get_name() == name)
			return m_childctrl[i];
	}

	return NULL;
}

bool xui_panel::was_descendant( xui_control* child ) const
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i] == child)
			return true;

		xui_panel* panel = xui_dynamic_cast(xui_panel, m_childctrl[i]);
		if (panel && panel->was_descendant(child))
			return true;
	}

	return false;
}

xui_control* xui_panel::get_descendant( const std::string& name ) const
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
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

void xui_panel::add_child( xui_control* child )
{
	if (child == NULL || was_child(child) || child->get_parent())
		return;

	child->set_parent(this);
	m_childctrl.push_back(child);

	xui_vector<s32> pt;
	pt.x = m_hscroll == NULL ? 0 : m_hscroll->get_value();
	pt.y = m_vscroll == NULL ? 0 : m_vscroll->get_value();
	child->set_scrollpt(pt);

	xui_method_args	  args;
	args.wparam = (void*)child;
	on_addchild(	  args);
	xm_addchild(this, args);
}

void xui_panel::del_child( xui_control* child, bool destroy )
{
	if (child == NULL)
		return;

	std::vector<xui_control*>::iterator itor = std::find(
		m_childctrl.begin(), 
		m_childctrl.end(), 
		child);

	if (itor == m_childctrl.end())
		return;

	xui_method_args   args;
	args.wparam = (void*)child;
	on_delchild(	  args);
	xm_delchild(this, args);

	child->set_parent(NULL);
	m_childctrl.erase(itor);

	if (destroy)
		xui_desktop::get_ins()->move_recycle(child);
}

void xui_panel::del_children( void )
{
	std::vector<xui_control*> temp = m_childctrl;
	for (u32 i = 0; i < temp.size(); ++i)
	{
		del_child(temp[i]);
	}
}

void xui_panel::movfore( xui_control* child )
{
	u32 index = get_childindex(child);
	if (index < get_childcount()-1)
		set_childindex(child, index+1);
}

void xui_panel::movback( xui_control* child )
{
	u32 index = get_childindex(child);
	if (index > 0)
		set_childindex(child, index-1);
}

void xui_panel::realign( void )
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_panel* panel = xui_dynamic_cast(xui_panel, m_childctrl[i]);
		if (panel)
			panel->realign();
	}

	refresh();
}

xui_component* xui_panel::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = xui_container::choose_else(pt);
	if (component == NULL)
	{
		xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
		if (rt.was_inside(pt))
		{
			xui_vector<s32> scrollpt;
			scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
			scrollpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
			xui_vector<s32> relative = pt - m_render.get_pt() + scrollpt;
			for (s32 i = (s32)m_childctrl.size()-1; i >= 0; --i)
			{
				if (component = m_childctrl[i]->choose(relative))
					return component;
			}
		}
	}

	return component;
}

void xui_panel::update_else( f32 delta )
{
	xui_container::update_else(delta);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i]->was_visible())
			m_childctrl[i]->update(delta);
	}
}

void xui_panel::render_else( void )
{
	xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
	xui_canvas::get_ins()->set_cliprect(currrect);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i]->was_visible() && currrect.get_inter(m_childctrl[i]->get_renderrtabs()).was_valid())
			m_childctrl[i]->render();
	}
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_container::render_else();
}

void xui_panel::on_addchild( xui_method_args& args )
{
	invalid();
}

void xui_panel::on_delchild( xui_method_args& args )
{
	invalid();
}

void xui_panel::on_perform( xui_method_args& args )
{
	xui_container::on_perform(args);
	if (m_childctrl.size() > 0)
	{
		std::vector<xui_component*> vec;
		for (u32 i = 0; i < m_childctrl.size(); ++i)
		{
			vec.push_back(m_childctrl[i]);
		}

		xui_rect2d<s32> rt = xui_rect2d<s32>(xui_vector<s32>(m_border.ax, m_border.ay), get_clientsz());
		perform_alignhorz(rt, vec);
		perform_alignvert(rt, vec);
		perform_dockstyle(rt, vec);
	}
}

void xui_panel::on_invalid( xui_method_args& args )
{
	xui_vector<s32> sz(0);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		if (m_childctrl[i]->get_dockstyle() == k_dockstyle_f ||
			m_childctrl[i]->get_dockstyle() == k_dockstyle_u)
			continue;

		if (m_childctrl[i]->get_dockstyle() == k_dockstyle_l ||
			m_childctrl[i]->get_dockstyle() == k_dockstyle_r ||
			m_childctrl[i]->get_alignhorz() != k_alignhorz_n)
			sz.w = xui_max(sz.w, m_childctrl[i]->get_renderw());
		else
		if (m_childctrl[i]->get_dockstyle() == k_dockstyle_n)
			sz.w = xui_max(sz.w, m_childctrl[i]->get_renderx() - m_border.ax + m_childctrl[i]->get_renderw());

		if (m_childctrl[i]->get_dockstyle() == k_dockstyle_t ||
			m_childctrl[i]->get_dockstyle() == k_dockstyle_b ||
			m_childctrl[i]->get_alignvert() != k_alignvert_n)
			sz.h = xui_max(sz.h, m_childctrl[i]->get_renderh());
		else
		if (m_childctrl[i]->get_dockstyle() == k_dockstyle_n)
			sz.h = xui_max(sz.h, m_childctrl[i]->get_rendery() - m_border.ay + m_childctrl[i]->get_renderh());
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

void xui_panel::on_vertvalue( xui_method_args& args )
{
	xui_container::on_vertvalue(args);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		m_childctrl[i]->set_scrolly(m_vscroll->get_value());
	}
}

void xui_panel::on_horzvalue( xui_method_args& args )
{
	xui_container::on_horzvalue(args);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		m_childctrl[i]->set_scrollx(m_hscroll->get_value());
	}
}