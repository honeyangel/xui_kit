#include "xui_scroll.h"
#include "xui_container.h"

/*
//constructor
*/
xui_create_explain(xui_container)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_control(name, rect)
{
	m_type		   += "container";
	m_vscroll		= NULL;
	m_hscroll		= NULL;
	m_vscrollshow	= false;
	m_hscrollshow	= false;
	m_vscrollauto	= true;
	m_hscrollauto	= true;
	update_scroll();
}

/*
//destructor
*/
xui_delete_explain(xui_container)( void )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		delete m_ascrollitem[i];
	}
}

/*
//show
*/
xui_method_explain(xui_container, get_vscroll,		xui_scroll*		)( void ) const
{
	return m_vscroll;
}
xui_method_explain(xui_container, get_hscroll,		xui_scroll*		)( void ) const
{
	return m_hscroll;
}
xui_method_explain(xui_container, was_vscrollshow,	bool			)( void ) const
{
	return m_vscrollshow;
}
xui_method_explain(xui_container, set_vscrollshow,	void			)( bool flag )
{
	if (m_vscrollshow != flag)
	{
		m_vscrollshow  = flag;
		update_scroll();
	}
}
xui_method_explain(xui_container, was_hscrollshow,	bool			)( void ) const
{
	return m_hscrollshow;
}
xui_method_explain(xui_container, set_hscrollshow,	void			)( bool flag )
{
	if (m_hscrollshow != flag)
	{
		m_hscrollshow  = flag;
		update_scroll();
	}
}

/*
//rectangle
*/
xui_method_explain(xui_container, get_renderrtins,	xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	rt.bx -= ((m_vscroll == NULL) ? 0 : m_vscroll->get_renderw());
	rt.by -= ((m_hscroll == NULL) ? 0 : m_hscroll->get_renderh());
	return rt;
}

/*
//virtual
*/
xui_method_explain(xui_container, choose_else,		xui_componet*	)( const xui_vector<s32>& pt )
{
	xui_componet* componet = xui_control::choose_else(pt);
	if (componet == NULL)
	{
		xui_vecptr_addloop(m_ascrollitem)
		{
			if (componet = m_ascrollitem[i]->choose(pt))
				return componet;
		}
	}

	return componet;
}
xui_method_explain(xui_container, update_else,		void			)( f32 delta )
{
	xui_control::update_else(delta);
	xui_vecptr_addloop(m_ascrollitem)
	{
		if (m_ascrollitem[i]->was_enable())
			m_ascrollitem[i]->update(delta);
	}
}
xui_method_explain(xui_container, render_else,		void			)( void )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		if (m_ascrollitem[i]->was_visible())
			m_ascrollitem[i]->render();
	}
	xui_control::render_else();
}

/*
//callback
*/
xui_method_explain(xui_container, on_perform,		void			)( xui_method_args&  args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//vscroll
	if (m_vscroll)
	{
		pt.x = rt.bx;
		pt.y = rt.ay;
		sz.h = rt.get_sz().h;
		sz.w = m_vscroll->get_renderw();
		m_vscroll->set_renderpt(pt, false);
		m_vscroll->set_rendersz(sz, false);
	}
	//hscroll
	if (m_hscroll)
	{
		pt.x = rt.ax;
		pt.y = rt.by;
		sz.w = rt.get_sz().w;
		sz.h = m_hscroll->get_renderh();
		m_hscroll->set_renderpt(pt, false);
		m_hscroll->set_rendersz(sz, false);
	}
}
xui_method_explain(xui_container, on_setrendersz,	void			)( xui_method_args&  args )
{
	xui_control::on_setrendersz(args);
	update_scroll();
}
xui_method_explain(xui_container, on_setclientsz,	void			)( xui_method_args&  args )
{
	xui_control::on_setclientsz(args);
	update_scroll();
}
xui_method_explain(xui_container, on_setborderrt,	void			)( xui_method_args&  args )
{
	xui_control::on_setborderrt(args);
	update_scroll();
}
xui_method_explain(xui_container, on_mousewheel,	void			)( xui_method_mouse& args )
{
	xui_control::on_mousewheel(args);
	if (m_vscroll)
	{
		m_vscroll->set_value(m_vscroll->get_value()+args.wheel);
		args.handle = true;
	}
	else
	if (m_hscroll)
	{
		m_hscroll->set_value(m_hscroll->get_value()+args.wheel);
		args.handle = true;
	}
}

/*
//method
*/
xui_method_explain(xui_container, on_scroll,		void			)( xui_componet* sender, xui_method_args& args )
{
	if (sender == m_vscroll)
	{
		on_vertvalue(      args);
		xm_vertvalue(this, args);
	}
	if (sender == m_hscroll)
	{
		on_horzvalue(      args);
		xm_horzvalue(this, args);
	}
}
xui_method_explain(xui_container, on_vertvalue,		void			)( xui_method_args&  args )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		m_ascrollitem[i]->set_scrolly(m_vscroll->get_value());
	}
}
xui_method_explain(xui_container, on_horzvalue,		void			)( xui_method_args&  args )
{
	xui_vecptr_addloop(m_ascrollitem)
	{
		m_ascrollitem[i]->set_scrollx(m_hscroll->get_value());
	}
}

/*
//scroll
*/
xui_method_explain(xui_container, update_scroll,	void			)( void )
{
	xui_rect2d<s32> clientrt = get_clientrt();
	xui_rect2d<s32> renderrt = xui_control::get_renderrtins();

	bool needvscroll = false;
	bool needhscroll = false;
	if (clientrt.get_sz().h > renderrt.get_sz().h) needvscroll = true;
	if (clientrt.get_sz().w > renderrt.get_sz().w) needhscroll = true;
	if (needvscroll) renderrt.bx -= 20;
	if (needhscroll) renderrt.by -= 20;

	if (clientrt.get_sz().h > renderrt.get_sz().h)
	{
		if (m_vscrollauto)
			create_scroll(FLOWSTYLE_V);

		if (m_vscroll)
			m_vscroll->set_enable(true);
	}
	else
	{
		if (m_vscrollshow)
			create_scroll(FLOWSTYLE_V);
		else
			delete_scroll(FLOWSTYLE_V);

		if (m_vscroll)
			m_vscroll->set_enable(false);
	}

	if (clientrt.get_sz().w > renderrt.get_sz().w)
	{
		if (m_hscrollauto)
			create_scroll(FLOWSTYLE_H);

		if (m_hscroll)
			m_hscroll->set_enable(true);
	}
	else
	{
		if (m_hscrollshow)
			create_scroll(FLOWSTYLE_H);
		else
			delete_scroll(FLOWSTYLE_H);

		if (m_hscroll)
			m_hscroll->set_enable(false);
	}


	//更新滚动条位置大小和范围
	renderrt = get_renderrtins();
	if (m_vscroll)
		m_vscroll->set_range(clientrt.get_sz().h - renderrt.get_sz().h);
	if (m_hscroll)
		m_hscroll->set_range(clientrt.get_sz().w - renderrt.get_sz().w);
}
xui_method_explain(xui_container, create_scroll,	void			)( u08 style )
{
	if (style == FLOWSTYLE_H && m_hscroll)
		return;
	if (style == FLOWSTYLE_V && m_vscroll)
		return;

	xui_scroll* scroll = new xui_scroll("", xui_rect2d<s32>(0, 0, 20, 20), style);
	switch (style)
	{
	case FLOWSTYLE_H:
		m_hscroll = scroll;
		break;
	case FLOWSTYLE_V:
		m_vscroll = scroll;
		break;
	}

	scroll->xm_scroll += new xui_method_member<xui_method_args, xui_container>(this, &xui_container::on_scroll);
	scroll->set_parent(this);
	m_widgetvec.push_back(scroll);

	invalid();
}
xui_method_explain(xui_container, delete_scroll,	void			)( u08 style )
{
	if (style == FLOWSTYLE_H && m_hscroll == NULL)
		return;
	if (style == FLOWSTYLE_V && m_vscroll == NULL)
		return;

	xui_scroll* scroll = NULL;
	switch (style)
	{
	case FLOWSTYLE_H:
		   scroll = m_hscroll;
		m_hscroll = NULL;
		break;
	case FLOWSTYLE_V:
		   scroll = m_vscroll;
		m_vscroll = NULL;
		break;
	}

	std::vector<xui_componet*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		scroll);

	if (itor != m_widgetvec.end())
	{
		m_widgetvec.erase(itor);
		delete scroll;
	}

	invalid();
}