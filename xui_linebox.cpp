#include "xui_linebox.h"

/*
//constructor
*/
xui_create_explain(xui_linebox)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_control(name, rect)
{
	m_type += "linebox";
	m_flow  = FLOWSTYLE_H;
}

/*
//flow
*/
xui_method_explain(xui_linebox, get_flow,			u08				)( void ) const
{
	return m_flow;
}

xui_method_explain(xui_linebox, set_flow,			void			)( u08 flow )
{
	if (m_flow != flow)
	{
		m_flow  = flow;
		invalid();
	}
}

/*
//line ctrl
*/
xui_method_explain(xui_linebox, add_linectrl,		void			)( xui_componet* componet )
{
	if (componet->get_parent())
		return;

	componet->set_parent(this);
	m_widgetvec.push_back(componet);
	invalid();
}

xui_method_explain(xui_linebox, del_linectrl,		void			)( xui_componet* componet )
{
	std::vector<xui_componet*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		componet);

	if (itor == m_widgetvec.end())
		return;

	componet->set_parent(NULL);
	delete componet;
	m_widgetvec.erase(itor);
	invalid();
}

xui_method_explain(xui_linebox, get_cornerrt, xui_rect2d<s32>	)( xui_componet* componet ) const
{
	xui_rect2d<s32> corner(0);
	if (m_widgetvec.size())
	{
		if (m_widgetvec.front() == componet)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				corner.value[0] = m_corner;
				corner.value[3] = m_corner;
				break;
			case FLOWSTYLE_V:
				corner.value[0] = m_corner;
				corner.value[1] = m_corner;
				break;
			}
		}

		if (m_widgetvec.back () == componet)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				corner.value[1] = m_corner;
				corner.value[2] = m_corner;
				break;
			case FLOWSTYLE_V:
				corner.value[2] = m_corner;
				corner.value[3] = m_corner;
				break;
			}
		}
	}

	return corner;
}

/*
//callback
*/
xui_method_explain(xui_linebox, on_invalid,			void			)( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		//计算大小
		xui_vector<s32> sz = (m_flow == FLOWSTYLE_H)
			? xui_vector<s32>(0, get_renderh())
			: xui_vector<s32>(get_renderw(), 0);

		for (u32 i = 0;  i < m_widgetvec.size(); ++i)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				sz.w += m_widgetvec[i]->get_renderw();
				break;
			case FLOWSTYLE_V:
				sz.h += m_widgetvec[i]->get_renderh();
				break;
			}
		}

		//布局
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

xui_method_explain(xui_linebox, on_perform,			void			)( xui_method_args& args )
{
	xui_vector<s32> pt(0);
	xui_vecptr_addloop(m_widgetvec)
	{
		m_widgetvec[i]->set_renderpt(pt);

		switch (m_flow)
		{
		case FLOWSTYLE_H:
			m_widgetvec[i]->set_rendersz(xui_vector<s32>(m_widgetvec[i]->get_renderw(), get_renderh()), false);
			pt.x += m_widgetvec[i]->get_renderw();
			break;
		case FLOWSTYLE_V:
			m_widgetvec[i]->set_rendersz(xui_vector<s32>(get_renderw(), m_widgetvec[i]->get_renderh()), false);
			pt.y += m_widgetvec[i]->get_renderh();
			break;
		}
	}
}