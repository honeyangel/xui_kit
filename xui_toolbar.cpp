#include "xui_toolbar.h"

/*
//constructor
*/
xui_create_explain(xui_toolbar)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_control(name, rect)
{
	m_type += "toolbar";
	m_flow  = FLOWSTYLE_H;
	m_grap  = 2;
}

/*
//property
*/
xui_method_explain(xui_toolbar, get_flow,	u08	)( void ) const
{
	return m_flow;
}
xui_method_explain(xui_toolbar, set_flow,	void)( u08 flow )
{
	if (m_flow != flow)
	{
		m_flow  = flow;
		invalid();
	}
}
xui_method_explain(xui_toolbar, get_grap,	s32	)( void ) const
{
	return m_grap;
}
xui_method_explain(xui_toolbar, set_grap,	void)( s32 grap )
{
	if (m_grap != grap)
	{
		m_grap  = grap;
		invalid();
	}
}

/*
//item
*/
xui_method_explain(xui_toolbar, add_item,	void)( xui_componet* componet )
{
	if (componet->get_parent())
		return;

	componet->set_parent(this);
	m_widgetvec.push_back(componet);
	invalid();
}
xui_method_explain(xui_toolbar, del_item,	void)( xui_componet* componet )
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

/*
//callback
*/
xui_method_explain(xui_toolbar, on_invalid, void)( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_vector<s32> sz = (m_flow == FLOWSTYLE_H)
			? xui_vector<s32>(m_border.ax+m_border.bx, get_renderh())
			: xui_vector<s32>(get_renderw(), m_border.ay+m_border.by);

		for (u32 i = 0; i < m_widgetvec.size(); ++i)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				sz.w += m_widgetvec[i]->get_renderw()+m_grap;
				break;
			case FLOWSTYLE_V:
				sz.h += m_widgetvec[i]->get_renderh()+m_grap;
				break;
			}
		}

		//²¼¾Ö
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
xui_method_explain(xui_toolbar, on_perform, void)( xui_method_args& args )
{
	xui_vector<s32> pt(m_border.ax, m_border.ay);
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vecptr_addloop(m_widgetvec)
	{
		m_widgetvec[i]->set_renderpt(pt);

		switch (m_flow)
		{
		case FLOWSTYLE_H:
			m_widgetvec[i]->set_rendersz(xui_vector<s32>(m_widgetvec[i]->get_renderw(), rt.get_sz().h), false);
			pt.x += m_widgetvec[i]->get_renderw()+m_grap;
			break;
		case FLOWSTYLE_V:
			m_widgetvec[i]->set_rendersz(xui_vector<s32>(rt.get_sz().w, m_widgetvec[i]->get_renderh()), false);
			pt.y += m_widgetvec[i]->get_renderh()+m_grap;
			break;
		}
	}
}