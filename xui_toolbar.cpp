#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_toolbar.h"

xui_implement_rtti(xui_toolbar, xui_control)

xui_toolbar* xui_toolbar::create( void )
{
	xui_toolbar* toolbar = new xui_toolbar(xui_vector<s32>(0, 24));
	return toolbar;
}

xui_toolbar::xui_toolbar( const xui_vector<s32>& size )
: xui_control(size)
{
	m_flow = k_flowstyle_h;
	m_grap = 2;
}

u08 xui_toolbar::get_flow( void ) const
{
	return m_flow;
}

void xui_toolbar::set_flow( u08 flow )
{
	if (m_flow != flow)
	{
		m_flow  = flow;
		invalid();
	}
}

s32 xui_toolbar::get_grap( void ) const
{
	return m_grap;
}

void xui_toolbar::set_grap( s32 grap )
{
	if (m_grap != grap)
	{
		m_grap  = grap;
		invalid();
	}
}

void xui_toolbar::add_separate( void )
{
	add_item(new xui_separate(xui_vector<s32>(8), m_flow));
}

void xui_toolbar::add_item( xui_component* component )
{
	if (component->get_parent())
		return;

	component->set_parent(this);
	m_widgetvec.push_back(component);
	invalid();
}

void xui_toolbar::del_item( xui_component* component )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		component);

	if (itor == m_widgetvec.end())
		return;

	component->set_parent(NULL);
	xui_desktop::get_ins()->move_recycle(component);
	m_widgetvec.erase(itor);
	invalid();
}

void xui_toolbar::on_invalid( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_vector<s32> sz = (m_flow == k_flowstyle_h)
			? xui_vector<s32>(m_border.ax+m_border.bx, get_renderh())
			: xui_vector<s32>(get_renderw(), m_border.ay+m_border.by);

		for (u32 i = 0; i < m_widgetvec.size(); ++i)
		{
			switch (m_flow)
			{
			case k_flowstyle_h:
				sz.w += m_widgetvec[i]->get_renderw()+m_grap;
				break;
			case k_flowstyle_v:
				sz.h += m_widgetvec[i]->get_renderh()+m_grap;
				break;
			}
		}

		if (get_rendersz() != sz)
		{
			set_rendersz(sz);
		}
		else
		{
			perform();
		}
	}
}

void xui_toolbar::on_perform( xui_method_args& args )
{
	xui_vector<s32> pt(m_border.ax, m_border.ay);
	xui_rect2d<s32> rt = get_renderrtins();
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		xui_component* comp = m_widgetvec[i];
		comp->on_perform_pt(pt);

		switch (m_flow)
		{
		case k_flowstyle_h:
			comp->on_perform_h(rt.get_h());
			pt.x += comp->get_renderw()+m_grap;
			break;
		case k_flowstyle_v:
			comp->on_perform_w(rt.get_w());
			pt.y += comp->get_renderh()+m_grap;
			break;
		}
	}
}