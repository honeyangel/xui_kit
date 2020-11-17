#include "xui_desktop.h"
#include "xui_canvas.h"
#include "xui_linebox.h"

xui_implement_rtti(xui_linebox, xui_control)

xui_linebox* xui_linebox::create( void )
{
	xui_linebox* linebox = new xui_linebox(xui_vector<s32>(0, 24));
	linebox->set_sidestyle(k_sidestyle_s);
	linebox->set_corner(3);
	return linebox;
}

xui_linebox::xui_linebox( const xui_vector<s32>& size, u08 flow )
: xui_control(size)
{
	m_flow = flow;
}

u08 xui_linebox::get_flow( void ) const
{
	return m_flow;
}

void xui_linebox::set_flow( u08 flow )
{
	if (m_flow != flow)
	{
		m_flow  = flow;
		invalid();
	}
}

void xui_linebox::add_linectrl( xui_control* ctrl )
{
	if (ctrl->get_parent())
		return;

	ctrl->set_parent(this);
	ctrl->set_sidestyle(k_sidestyle_n);
	m_widgetvec.push_back(ctrl);
	invalid();
}

void xui_linebox::del_linectrl( xui_control* ctrl )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		ctrl);

	if (itor == m_widgetvec.end())
		return;

	ctrl->set_parent(NULL);
	xui_desktop::get_ins()->move_recycle(ctrl);
	m_widgetvec.erase(itor);
	invalid();
}

xui_rect2d<s32> xui_linebox::get_cornerrt( xui_component* component ) const
{
	xui_rect2d<s32> corner(0);
	if (m_widgetvec.size())
	{
		if (m_widgetvec.front() == component)
		{
			switch (m_flow)
			{
			case k_flowstyle_h:
				corner.value[0] = m_corner;
				corner.value[3] = m_corner;
				break;
			case k_flowstyle_v:
				corner.value[0] = m_corner;
				corner.value[1] = m_corner;
				break;
			}
		}

		if (m_widgetvec.back () == component)
		{
			switch (m_flow)
			{
			case k_flowstyle_h:
				corner.value[1] = m_corner;
				corner.value[2] = m_corner;
				break;
			case k_flowstyle_v:
				corner.value[2] = m_corner;
				corner.value[3] = m_corner;
				break;
			}
		}
	}

	return corner;
}

void xui_linebox::render_else( void )
{
	xui_control::render_else();

	if (m_sidestyle)
	{
		xui_rect2d<s32> cornerrt = xui_control::get_cornerrt();
		xui_rect2d<s32> renderrt = get_renderrtabs();
		xui_colour      color    = get_vertexcolor();

		renderrt.bx -= 1;
		renderrt.by -= 1;
		xui_colour side_color = m_sidecolor * color;

		if (m_widgetvec.size() > 0)
		{
			xui_vector<s32> p1 = renderrt.get_pt(); 
			xui_vector<s32> p2 = p1 + xui_vector<s32>(0, renderrt.get_h());
			for (u32 i = 0; i < m_widgetvec.size()-1; ++i)
			{
				s32 w = m_widgetvec[i]->get_renderw();
				p1 += xui_vector<s32>(w, 0);
				p2 += xui_vector<s32>(w, 0);
				xui_canvas::get_ins()->draw_line(p1, p2, side_color);
			}
		}
	}
}

void xui_linebox::on_invalid( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_vector<s32> sz = (m_flow == k_flowstyle_h)
			? xui_vector<s32>(0, get_renderh())
			: xui_vector<s32>(get_renderw(), 0);

		for (u32 i = 0;  i < m_widgetvec.size(); ++i)
		{
			switch (m_flow)
			{
			case k_flowstyle_h:
				sz.w += m_widgetvec[i]->get_renderw();
				break;
			case k_flowstyle_v:
				sz.h += m_widgetvec[i]->get_renderh();
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

void xui_linebox::on_perform( xui_method_args& args )
{
	xui_vector<s32> pt(0);
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		xui_component* comp = m_widgetvec[i];
		comp->on_perform_pt(pt);

		switch (m_flow)
		{
		case k_flowstyle_h:
			comp->on_perform_h(get_renderh());
			pt.x += comp->get_renderw();
			break;
		case k_flowstyle_v:
			comp->on_perform_w(get_renderw());
			pt.y += comp->get_renderh();
			break;
		}
	}
}
