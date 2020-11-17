#include "xui_desktop.h"
#include "xui_window.h"
#include "xui_canvas.h"
#include "xui_scroll.h"
#include "xui_menu.h"
#include "xui_container.h"

xui_implement_rtti(xui_container, xui_control)

xui_container::xui_container( const xui_vector<s32>& size )
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

	xm_getfocus	+= new xui_method_member<xui_method_args, xui_container>(this, &xui_container::on_ctrlgetfocus);
}

xui_container::~xui_container( void )
{
	xui_desktop::get_ins()->move_recycle(m_contextmenu);
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		delete m_ascrollitem[i];
}

xui_scroll* xui_container::get_vscroll( void ) const
{
	return m_vscroll;
}

xui_scroll* xui_container::get_hscroll( void ) const
{
	return m_hscroll;
}

bool xui_container::was_vscrollshow( void ) const
{
	return m_vscrollshow;
}

void xui_container::set_vscrollshow( bool flag )
{
	if (m_vscrollshow != flag)
	{
		m_vscrollshow  = flag;
		update_scroll();
	}
}

bool xui_container::was_hscrollshow( void ) const
{
	return m_hscrollshow;
}

void xui_container::set_hscrollshow( bool flag )
{
	if (m_hscrollshow != flag)
	{
		m_hscrollshow  = flag;
		update_scroll();
	}
}

bool xui_container::was_vscrollauto( void ) const
{
	return m_vscrollauto;
}

void xui_container::set_vscrollauto( bool flag )
{
	if (m_vscrollauto != flag)
	{
		m_vscrollauto  = flag;
		update_scroll();
	}
}

bool xui_container::was_hscrollauto( void ) const
{
	return m_hscrollauto;
}

void xui_container::set_hscrollauto( bool flag )
{
	if (m_hscrollauto != flag)
	{
		m_hscrollauto  = flag;
		update_scroll();
	}
}

xui_menu* xui_container::get_contextmenu( void )
{
	return m_contextmenu;
}

void xui_container::set_contextmenu( xui_menu* menu )
{
	m_contextmenu = menu;
}

xui_rect2d<s32> xui_container::get_renderrtins( void ) const
{
	xui_rect2d<s32> rt = xui_control::get_renderrtins();
	rt.bx -= ((m_vscroll == NULL) ? 0 : m_vscroll->get_renderw());
	rt.by -= ((m_hscroll == NULL) ? 0 : m_hscroll->get_renderh());
	return rt;
}

xui_component* xui_container::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = xui_control::choose_else(pt);
	xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
	if (component == NULL && rt.was_inside(pt))
	{
		xui_vector<s32> scrollpt;
		scrollpt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
		scrollpt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
		xui_vector<s32> relative = pt - m_render.get_pt() + scrollpt;
		for (s32 i = (s32)m_ascrollitem.size()-1; i >= 0; --i)
		{
			if (component = m_ascrollitem[i]->choose(relative))
				return component;
		}
	}

	return component;
}

void xui_container::update_else( f32 delta )
{
	xui_control::update_else(delta);
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (m_ascrollitem[i]->was_visible())
			m_ascrollitem[i]->update(delta);
	}
}

void xui_container::render_else( void )
{
	xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtins()+get_screenpt());
	xui_canvas::get_ins()->set_cliprect(currrect);
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		if (m_ascrollitem[i]->was_visible() && currrect.get_inter(m_ascrollitem[i]->get_renderrtabs()).was_valid())
			m_ascrollitem[i]->render();
	}
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_control::render_else();
}

void xui_container::on_perform( xui_method_args&  args )
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

void xui_container::on_setrendersz( xui_method_args&  args )
{
	xui_control::on_setrendersz(args);
	update_scroll();
}

void xui_container::on_setclientsz( xui_method_args&  args )
{
	xui_control::on_setclientsz(args);
	update_scroll();
}

void xui_container::on_setborderrt( xui_method_args&  args )
{
	xui_control::on_setborderrt(args);
	update_scroll();
}

void xui_container::on_mousedown( xui_method_mouse& args )
{
	xui_control::on_mousedown(args);
	if (args.mouse == k_mb_right && m_contextmenu)
	{
		m_contextmenu->refresh();

		xui_window* window = get_window();
		xui_vector<s32> pt = args.point;
		xui_rect2d<s32> rt = window->get_renderrtabs();
		if (pt.x + m_contextmenu->get_renderw() > rt.bx)
			pt.x = rt.bx - m_contextmenu->get_renderw();
		if (pt.y + m_contextmenu->get_renderh() > rt.by)
			pt.y = rt.by - m_contextmenu->get_renderh();

		m_contextmenu->set_renderpt(pt);
		m_contextmenu->set_showsubmenu(NULL);
		m_contextmenu->req_focus();
		xui_desktop::get_ins()->set_floatctrl(window, m_contextmenu);
	}
}

void xui_container::on_mousewheel( xui_method_mouse& args )
{
	xui_control::on_mousewheel(args);
	xui_scroll* scroll = NULL;
	if		(m_vscroll) scroll = m_vscroll;
	else if (m_hscroll) scroll = m_hscroll;
	else
	{}

	if (scroll)
	{
		args.handle = true;

		xui_action_ctrl_impl<s32>* action = (xui_action_ctrl_impl<s32>*)scroll->get_valueaction();
		s32 start = scroll->get_value();
		s32 final = (action->has_data() ? action->get_data(1) : start) - args.wheel;
		action->clear();
		action->add_time(0.0f);
		action->add_time(0.5f);
		action->add_data(start);
		action->add_data(final);
		action->set_play(true);
	}
}

void xui_container::on_scroll( xui_component* sender, xui_method_args& args )
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

void xui_container::on_vertvalue( xui_method_args&  args )
{
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		m_ascrollitem[i]->set_scrolly(m_vscroll->get_value());
	}
}

void xui_container::on_horzvalue( xui_method_args&  args )
{
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		m_ascrollitem[i]->set_scrollx(m_hscroll->get_value());
	}
}

void xui_container::update_scroll( void )
{
	xui_rect2d<s32>  clientrt = get_clientrt();
	xui_rect2d<s32>  renderrt = get_renderrtins();

	if (m_hscroll && m_hscrollshow == false) renderrt.by += xui_scroll::k_default_size;
	if (m_vscroll && m_vscrollshow == false) renderrt.bx += xui_scroll::k_default_size;
	bool needvscroll = m_vscrollshow;
	bool needhscroll = m_hscrollshow;
	if (clientrt.get_h() > renderrt.get_h() && m_vscrollauto) needvscroll = true;
	if (clientrt.get_w() > renderrt.get_w() && m_hscrollauto) needhscroll = true;
	//if (needvscroll) renderrt.bx -= xui_scroll::default_size;
	//if (needhscroll) renderrt.by -= xui_scroll::default_size;

	if (needvscroll)	create_scroll(k_flowstyle_v);
	else				delete_scroll(k_flowstyle_v);

	if (needhscroll)	create_scroll(k_flowstyle_h);
	else				delete_scroll(k_flowstyle_h);

	renderrt = get_renderrtins();
	if (m_vscroll)		m_vscroll->set_enable(clientrt.get_h() > renderrt.get_h());
	if (m_hscroll)		m_hscroll->set_enable(clientrt.get_w() > renderrt.get_w());

	//更新滚动条位置大小和范围
	if (m_vscroll)		m_vscroll->set_range (clientrt.get_h() - renderrt.get_h());
	if (m_hscroll)		m_hscroll->set_range (clientrt.get_w() - renderrt.get_w());

    perform();
}

void xui_container::create_scroll( u08 style )
{
	if (style == k_flowstyle_h && m_hscroll)
		return;
	if (style == k_flowstyle_v && m_vscroll)
		return;

	xui_scroll* scroll = xui_scroll::create(style);
	scroll->xm_scroll += new xui_method_member<xui_method_args, xui_container>(this, &xui_container::on_scroll);
	scroll->set_parent(this);
	m_widgetvec.push_back(scroll);

	switch (style)
	{
	case k_flowstyle_h: m_hscroll = scroll; break;
	case k_flowstyle_v: m_vscroll = scroll; break;
	}

	invalid();
}

void xui_container::delete_scroll( u08 style )
{
	if (style == k_flowstyle_h && m_hscroll == NULL)
		return;
	if (style == k_flowstyle_v && m_vscroll == NULL)
		return;

	xui_scroll* scroll = NULL;
	switch (style)
	{
	case k_flowstyle_h: 
		scroll = m_hscroll; 
		m_hscroll->set_value(0);
		m_hscroll = NULL;	
		break;
	case k_flowstyle_v: 
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

void xui_container::on_ctrlgetfocus( xui_component* sender, xui_method_args& args )
{
	if (m_hscroll)
		m_hscroll->get_valueaction()->clear();
	if (m_vscroll)
		m_vscroll->get_valueaction()->clear();
}