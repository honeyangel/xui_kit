#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_menu.h"
#include "xui_container.h"

xui_implement_rtti(xui_container, xui_control);

/*
//constructor
*/
xui_create_explain(xui_container)( const xui_vector<s32>& size )
: xui_control(size)
{
	m_drawcolor		= true;
	m_backcolor		= xui_colour(1.0f, 0.25f);
	m_vscroll		= NULL;
	m_hscroll		= NULL;
	m_vscrollshow	= false;
	m_hscrollshow	= false;
	m_vscrollauto	= true;
	m_hscrollauto	= true;
	m_contextmenu	= NULL;
	update_scroll();
}

/*
//destructor
*/
xui_delete_explain(xui_container)( void )
{
	delete m_contextmenu;
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
xui_method_explain(xui_container, was_vscrollauto,	bool			)( void ) const
{
	return m_vscrollauto;
}
xui_method_explain(xui_container, set_vscrollauto,	void			)( bool flag )
{
	if (m_vscrollauto != flag)
	{
		m_vscrollauto  = flag;
		update_scroll();
	}
}
xui_method_explain(xui_container, was_hscrollauto,	bool			)( void ) const
{
	return m_hscrollauto;
}
xui_method_explain(xui_container, set_hscrollauto,	void			)( bool flag )
{
	if (m_hscrollauto != flag)
	{
		m_hscrollauto  = flag;
		update_scroll();
	}
}

/*
//menu
*/
xui_method_explain(xui_container, get_contextmenu,	xui_menu*		)( void )
{
	return m_contextmenu;
}
xui_method_explain(xui_container, set_contextmenu,	void			)( xui_menu* menu )
{
	m_contextmenu = menu;
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
xui_method_explain(xui_container, choose_else,		xui_component*	)( const xui_vector<s32>& pt )
{
	xui_component* component = xui_control::choose_else(pt);
	xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
	if (component == NULL && rt.was_inside(pt))
	{
		xui_vector<s32> scrollpt;
		scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
		scrollpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
		xui_vector<s32> relative = pt - m_render.get_pt() + scrollpt;
		xui_vecptr_delloop(m_ascrollitem)
		{
			if (component = m_ascrollitem[i]->choose(relative))
				return component;
		}
	}

	return component;
}
xui_method_explain(xui_container, update_else,		void			)( f32 delta )
{
	xui_control::update_else(delta);
	xui_vecptr_addloop(m_ascrollitem)
	{
		if (m_ascrollitem[i]->was_enable() && m_ascrollitem[i]->was_visible())
			m_ascrollitem[i]->update(delta);
	}
	if (m_contextmenu)
		m_contextmenu->update(delta);
}
xui_method_explain(xui_container, render_else,		void			)( void )
{
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
	xui_convas::get_ins()->set_cliprect(currrect);
	xui_vecptr_addloop(m_ascrollitem)
	{
		if (m_ascrollitem[i]->was_visible() && currrect.get_inter(m_ascrollitem[i]->get_renderrtabs()).was_valid())
			m_ascrollitem[i]->render();
	}
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_control::render_else();
}

/*
//callback
*/
xui_method_explain(xui_container, on_perform,		void			)( xui_method_args&  args )
{
	xui_control::on_perform(args);
	xui_rect2d<s32> rt = get_renderrtins();

	//vscroll
	if (m_vscroll)
	{
		m_vscroll->on_perform_x(rt.bx);
		m_vscroll->on_perform_y(rt.ay);
		m_vscroll->on_perform_h(rt.get_h());
	}
	//hscroll
	if (m_hscroll)
	{
		m_hscroll->on_perform_x(rt.ax);
		m_hscroll->on_perform_y(rt.by);
		m_hscroll->on_perform_w(rt.get_w());
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
xui_method_explain(xui_container, on_mousedown,		void			)( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == MB_R && m_contextmenu)
	{
		m_contextmenu->refresh();

		xui_vector<s32> pt = args.point;
		if (pt.x + m_contextmenu->get_renderw() > xui_desktop::get_ins()->get_renderw())
			pt.x = xui_desktop::get_ins()->get_renderw() - m_contextmenu->get_renderw();
		if (pt.y + m_contextmenu->get_renderh() > xui_desktop::get_ins()->get_renderh())
			pt.y = xui_desktop::get_ins()->get_renderh() - m_contextmenu->get_renderh();

		xui_method_ptrcall(m_contextmenu, set_renderpt		)(pt);
		xui_method_ptrcall(m_contextmenu, set_showsubmenu	)(NULL);
		xui_method_ptrcall(m_contextmenu, req_focus			)();
		xui_desktop::get_ins()->set_floatctrl(m_contextmenu);
	}
}
xui_method_explain(xui_container, on_mousewheel,	void			)( xui_method_mouse& args )
{
	xui_control::on_mousewheel(args);
	if (m_vscroll)
	{
		m_vscroll->set_value(m_vscroll->get_value()-args.wheel);
		args.handle = true;
	}
	else
	if (m_hscroll)
	{
		m_hscroll->set_value(m_hscroll->get_value()-args.wheel);
		args.handle = true;
	}
}

/*
//method
*/
xui_method_explain(xui_container, on_scroll,		void			)( xui_component* sender, xui_method_args& args )
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
	xui_rect2d<s32>  clientrt = get_clientrt();
	xui_rect2d<s32>  renderrt = get_renderrtins();

	if (m_hscroll && m_hscrollshow == false) renderrt.by += xui_scroll::default_size;
	if (m_vscroll && m_vscrollshow == false) renderrt.bx += xui_scroll::default_size;
	bool needvscroll = m_vscrollshow;
	bool needhscroll = m_hscrollshow;
	if (clientrt.get_h() > renderrt.get_h() && m_vscrollauto) needvscroll = true;
	if (clientrt.get_w() > renderrt.get_w() && m_hscrollauto) needhscroll = true;
	//if (needvscroll) renderrt.bx -= xui_scroll::default_size;
	//if (needhscroll) renderrt.by -= xui_scroll::default_size;

	if (needvscroll)	create_scroll(FLOWSTYLE_V);
	else				delete_scroll(FLOWSTYLE_V);

	if (needhscroll)	create_scroll(FLOWSTYLE_H);
	else				delete_scroll(FLOWSTYLE_H);

	renderrt = get_renderrtins();
	if (m_vscroll)		xui_method_ptrcall(m_vscroll, set_enable)(clientrt.get_h() > renderrt.get_h());
	if (m_hscroll)		xui_method_ptrcall(m_hscroll, set_enable)(clientrt.get_w() > renderrt.get_w());

	//更新滚动条位置大小和范围
	if (m_vscroll)		xui_method_ptrcall(m_vscroll, set_range	)(clientrt.get_h() - renderrt.get_h());
	if (m_hscroll)		xui_method_ptrcall(m_hscroll, set_range )(clientrt.get_w() - renderrt.get_w());
}
xui_method_explain(xui_container, create_scroll,	void			)( u08 style )
{
	if (style == FLOWSTYLE_H && m_hscroll)
		return;
	if (style == FLOWSTYLE_V && m_vscroll)
		return;

	xui_scroll* scroll = xui_scroll::create(style);
	scroll->xm_scroll += new xui_method_member<xui_method_args, xui_container>(this, &xui_container::on_scroll);
	scroll->set_parent(this);
	m_widgetvec.push_back(scroll);

	switch (style)
	{
	case FLOWSTYLE_H: m_hscroll = scroll; break;
	case FLOWSTYLE_V: m_vscroll = scroll; break;
	}

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
		m_hscroll->set_value(0);
		m_hscroll = NULL;	
		break;
	case FLOWSTYLE_V: 
		scroll = m_vscroll; 
		m_vscroll->set_value(0);
		m_vscroll = NULL;	
		break;
	}

	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		scroll);

	if (itor != m_widgetvec.end())
	{
		m_widgetvec.erase(itor);
		scroll->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(scroll);
	}

	invalid();
}