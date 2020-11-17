#include "xui_global.h"
#include "xui_syswnd.h"
#include "xui_canvas.h"
#include "xui_window.h"
#include "xui_dialog.h"
#include "xui_desktop.h"

xui_implement_rtti(xui_desktop, xui_panel)
xui_implement_instance_member(xui_desktop)
xui_implement_instance_method(xui_desktop)

xui_desktop::xui_desktop( void )
: xui_panel(xui_vector<s32>(800, 600))
{
	m_catchctrl = NULL;
	m_focusctrl = NULL;
	m_hoverctrl = NULL;
	m_allowdrag = 0;
	m_catchdata = NULL;
	m_mousedown = xui_vector<s32>(0, 0);
	m_mouselast = xui_vector<s32>(0, 0);
	m_mousecurr = xui_vector<s32>(0, 0);
}

xui_desktop::~xui_desktop( void )
{
	proc_recycle();
}

xui_component* xui_desktop::get_catchctrl( void )
{
	return m_catchctrl;
}

void xui_desktop::set_catchctrl( xui_component* component )
{
	if (m_catchctrl != component)
	{
		xui_component* last = m_catchctrl;
		xui_component* curr = component;

		m_catchctrl  = curr;

		if (last)
		{
			xui_method_args args;
			args.wparam = curr;
			last->on_noncatch(      args);
			last->xm_noncatch(last, args);
		}
		if (curr)
		{
			xui_method_args args;
			args.wparam = last;
			curr->on_getcatch(		args);
			curr->xm_getcatch(curr, args);
		}

		if (m_catchctrl)
			set_focusctrl(m_catchctrl);
	}
}

xui_component* xui_desktop::get_focusctrl( void )
{
	return m_focusctrl;
}

void xui_desktop::set_focusctrl( xui_component* component )
{
	if (m_focusctrl != component)
	{
		xui_component* last = m_focusctrl;
		xui_component* curr = component;

		m_focusctrl  = curr;

		if (m_focusctrl != m_catchctrl)
			set_catchctrl(NULL);
		
		if (last)
		{
			xui_method_args args;
			args.wparam = curr;
			last->on_nonfocus(		args);
			last->xm_nonfocus(last, args);
		}
		if (curr)
		{
			xui_method_args args;
			args.wparam = last;
			curr->on_getfocus(		args);
			curr->xm_getfocus(curr, args);
		}
	}
}

xui_component* xui_desktop::get_hoverctrl( void )
{
	return m_hoverctrl;
}

void xui_desktop::set_hoverctrl( xui_component* component )
{
	if (m_hoverctrl != component)
	{
		xui_component* last = m_hoverctrl;
		xui_component* curr = component;

		m_hoverctrl  = curr;

		if (m_catchctrl)
		{
			if (m_catchctrl != last && last && m_catchdata)
			{
				xui_method_dragdrop other_args;
				other_args.drag = m_catchctrl;
				other_args.type = m_catchtype;
				other_args.data = m_catchdata;
				last->on_mousedragleave(      other_args);
				last->xm_mousedragleave(last, other_args);
			}
			if (m_catchctrl != curr && curr && m_catchdata)
			{
				xui_method_dragdrop other_args;
				other_args.drag = m_catchctrl;
				other_args.type = m_catchtype;
				other_args.data = m_catchdata;
				curr->on_mousedragenter(      other_args);
				curr->xm_mousedragenter(curr, other_args);
			}
		}
		else
		{
			if (last)
			{
				xui_method_mouse args;
				last->on_mouseleave(      args);
				last->xm_mouseleave(last, args);
			}
			if (curr)
			{
				xui_method_mouse args;
				curr->on_mouseenter(      args);
				curr->xm_mouseenter(curr, args);
				xui_global::set_cursor(curr->get_cursor());
			}
		}
	}
}

xui_component* xui_desktop::get_floatctrl( void )
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		if (window->get_float() == NULL)
			continue;

		return window->get_float();
	}

	return NULL;
}

void xui_desktop::set_floatctrl( xui_window* owner, xui_component* component )
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		window->set_float(window == owner ? component : NULL);
	}
}

const xui_vector<s32>& xui_desktop::get_mousedown( void ) const
{
	return m_mousedown;
}

const xui_vector<s32>& xui_desktop::get_mouselast( void ) const
{
	return m_mouselast;
}

const xui_vector<s32>& xui_desktop::get_mousecurr( void ) const
{
	return m_mousecurr;
}

xui_vector<s32> xui_desktop::get_mousemove( void ) const
{
	return m_mousecurr-m_mouselast;
}

void xui_desktop::set_mouselast( const xui_vector<s32>& pt )
{
	m_mouselast = pt;
}

void xui_desktop::set_mousecurr( const xui_vector<s32>& pt )
{
	m_mousecurr = pt;
}

void xui_desktop::add_modalwnd( xui_window* window )
{
	xui_global::mod_syswnd(window->get_owner());
	m_modalpool.push_back (window);
}

void xui_desktop::del_modalwnd( xui_window* window )
{
	xui_global::res_syswnd();
	for (u32 i = 0; i < m_modalpool.size(); ++i)
	{
		if (m_modalpool[i] == window)
		{
			m_modalpool.erase(m_modalpool.begin()+i);
			break;
		}
	}

	if (m_modalpool.empty())
		return;

	xui_window* top = m_modalpool.back();
	xui_global::mod_syswnd(top->get_owner());
}

void xui_desktop::move_recycle( xui_component* component )
{
	if (m_catchctrl)
	{
		if (m_catchctrl == component || m_catchctrl->was_ancestor(component))
			set_catchctrl(NULL);
	}
	if (m_focusctrl)
	{
		if (m_focusctrl == component || m_focusctrl->was_ancestor(component))
			set_focusctrl(NULL);
	}
	if (m_hoverctrl)
	{
		if (m_hoverctrl == component || m_hoverctrl->was_ancestor(component))
			m_hoverctrl = NULL;
	}

	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		if (window->get_float() == component)
			window->set_float(NULL);
	}

    m_recyclebin.insert(component);
}

void xui_desktop::send_message( const xui_message& message )
{
	switch(message.msgidx)
	{
	case k_xm_getcatch:
		{
			set_catchctrl(message.sender);
		}
		break;
	case k_xm_getfocus:
		{
			set_focusctrl(message.sender);
		}
		break;
	case k_xm_dead:
		{

		}
		break;
	}
}

void xui_desktop::post_message( const xui_message& message )
{
	m_messagevec.push_back(message);
}

xui_dialog* xui_desktop::show_message( const std::wstring& text, s32 num_button )
{
    xui_dialog* dialog = new xui_dialog(text, num_button);
    add_child(dialog);
    return dialog;
}

xui_component* xui_desktop::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = xui_container::choose_else(pt);
	if (component == NULL)
	{
		xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
		if (rt.was_inside(pt))
		{
			xui_vector<s32> relative = pt - m_render.get_pt();
			for (s32 i = (s32)m_childctrl.size()-1; i >= 0; --i)
			{
				xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
				if (window->get_owner())
					continue;

				if (component = window->choose(relative))
					return component;
			}
		}
	}

	return component;
}

void xui_desktop::update( f32 delta )
{
	xui_panel::update(delta);
	proc_message();
	proc_recycle();
	proc_settext();
}

void xui_desktop::render( void )
{
	xui_canvas::get_ins()->set_cliprect(m_render);
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		if (window->get_owner())
			continue;

		if (m_childctrl[i]->was_visible())
			m_childctrl[i]->render();
	}
	xui_canvas::get_ins()->set_cliprect(m_render);
}

void xui_desktop::os_mousewheel( xui_method_mouse& args )
{
	if (m_hoverctrl)
	{
		xui_component* root = m_hoverctrl;

		while (root && args.handle == false)
		{
			root->on_mousewheel(args);
			root->xm_mousewheel(root, args);
			if (xui_issub_kindof(xui_desktop, root))
				break;

			root = root->get_parent();
		}
	}
}

void xui_desktop::os_mousedown( xui_method_mouse& args )
{
	xui_component* component = NULL;

	xui_syswnd* syswnd = xui_global::get_syswnd((xui_native_window*)args.wparam);
	if (syswnd)
		component = syswnd->get_popupctrl()->choose(args.point);
	else
		component = choose(args.point);

	if (args.mouse == k_mb_left)
	{
		set_catchctrl(component);
		m_mousedown = args.point;
		m_mouselast = args.point;
		m_mousecurr = args.point;
	}

	if (component)
	{
		component->on_mousedown(           args);
		component->xm_mousedown(component, args);
	}

	xui_global::set_cursor(m_catchctrl == NULL ? k_cursor_default : m_catchctrl->get_cursor());
}

void xui_desktop::os_mouserise( xui_method_mouse& args )
{
	xui_component* component = NULL;

	if (args.mouse == k_mb_left && m_catchctrl)
	{
		if (m_catchctrl != m_hoverctrl && m_hoverctrl && m_allowdrag && m_catchdata)
		{
			xui_method_dragdrop other_args;
			other_args.allow = m_allowdrag;
			other_args.drag  = m_catchctrl;
			other_args.data  = m_catchdata;
			other_args.type  = m_catchtype;
			xui_component* hoverctrl = m_hoverctrl;
			hoverctrl->on_mousedragdrop(           other_args);
			hoverctrl->xm_mousedragdrop(hoverctrl, other_args);
		}
	}

	if (m_catchctrl)
	{
		component = m_catchctrl;
	}
	else
	{
		xui_syswnd* syswnd = xui_global::get_syswnd((xui_native_window*)args.wparam);
		if (syswnd)
			component = syswnd->get_popupctrl()->choose(args.point);
		else
			component = choose(args.point);
	}

	if (component)
	{
		component->on_mouserise(           args);
		component->xm_mouserise(component, args);
	}

	if (args.mouse == k_mb_left)
	{
		set_catchctrl(NULL);
		m_allowdrag = false;
		m_catchdata = NULL;
	}

	xui_global::set_cursor(m_hoverctrl == NULL ? k_cursor_default : m_hoverctrl->get_cursor());
}

void xui_desktop::os_mousemove( xui_method_mouse& args )
{
	m_mouselast = m_mousecurr;
	m_mousecurr = args.point;

	xui_component* component = NULL;
	xui_syswnd* syswnd = xui_global::get_syswnd((xui_native_window*)args.wparam);
	if (syswnd)
		component = syswnd->get_popupctrl()->choose(args.point);
	if (component == NULL && m_catchctrl && m_modalpool.empty())
	{
		std::vector<xui_syswnd*> vec = xui_global::get_syswndall();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_syswnd* wnd = vec[i];
			if (wnd == syswnd)
				continue;

			xui_vector<s32> pt = xui_global::get_syswndmouse(wnd);
			component = wnd->get_popupctrl()->choose(pt);
			if (component)
			{
				m_mousecurr = pt;
				args.point  = pt;
				break;
			}
		}
	}
	if (component == NULL)
		component = choose(xui_global::get_syswndmouse(NULL));

	set_hoverctrl(component);
	if (m_catchctrl)
	{
		xui_vector<s32> delta = args.point - m_mousedown;
		bool hasdrag = (xui_abs(delta.x) > 4) || (xui_abs(delta.y) > 4);
		if (m_catchdata == NULL && hasdrag)
		{
			xui_method_dragdrop other_args;
			other_args.drag = m_catchctrl;
			xui_component* catchctrl = m_catchctrl;
			catchctrl->on_mousedragitem(           other_args);
			catchctrl->xm_mousedragitem(catchctrl, other_args);
			m_catchdata = other_args.data;
			m_catchtype = other_args.type;
			m_allowdrag = other_args.data != NULL;
		}
		if (m_catchctrl != m_hoverctrl && m_hoverctrl && m_catchdata)
		{
			xui_method_dragdrop other_args;
			other_args.drag = m_catchctrl;
			other_args.data = m_catchdata;
			other_args.type = m_catchtype;
			xui_component* hoverctrl = m_hoverctrl;
			hoverctrl->on_mousedragover(           other_args);
			hoverctrl->xm_mousedragover(hoverctrl, other_args);
			m_allowdrag = other_args.allow;
		}

		component = m_catchctrl;
		component->on_mousemove(           args);
		component->xm_mousemove(component, args);
	}
	else
	if (m_hoverctrl)
	{
		component = m_hoverctrl;
		component->on_mousemove(           args);
		component->xm_mousemove(component, args);
	}

	if (m_catchctrl && m_catchdata)
	{
		xui_global::set_cursor((m_catchctrl == m_hoverctrl || m_allowdrag) ? k_cursor_drag : k_cursor_forbid);
	}
	else
	if (m_catchctrl)
	{
		xui_global::set_cursor(m_catchctrl->get_cursor());
	}
	else
	{
		xui_global::set_cursor((m_hoverctrl == NULL) ? k_cursor_default : m_hoverctrl->get_cursor());
	}
}

void xui_desktop::os_keybddown( xui_method_keybd& args )
{
	if (m_focusctrl)
	{
		xui_component* root = m_focusctrl;
		root->on_keybddown(args);

		while (root && args.handle == false)
		{
			root->xm_keybddown(root, args);
			if (xui_issub_kindof(xui_desktop, root))
				break;

			root = root->get_parent();
		}
	}
}

void xui_desktop::os_keybdrise( xui_method_keybd& args )
{
	if (m_focusctrl)
	{
		xui_component* root = m_focusctrl;
		root->on_keybdrise(args);

		while (root && args.handle == false)
		{
			root->xm_keybdrise(root, args);
			if (xui_issub_kindof(xui_desktop, root))
				break;

			root = root->get_parent();
		}
	}
}

void xui_desktop::os_keybdchar( u16 c )
{
	m_inputtext.append(1, c);
}

void xui_desktop::on_addchild( xui_method_args& args )
{
	xui_panel::on_addchild(args);

	xui_component* component = (xui_component*)(args.wparam);
	if (component)
	{
		xui_window* window = xui_dynamic_cast(xui_window, component);
		if (window->was_visible())
		{
			if (window->was_modal() || window->was_popup())
				xui_global::add_syswnd(window, window->was_modal() == false);

			if (window->was_modal())
				add_modalwnd(window);
		}
	}
}

void xui_desktop::on_delchild( xui_method_args& args )
{
	xui_panel::on_delchild(args);

	xui_component* component = (xui_component*)(args.wparam);
	if (component)
	{
		xui_window* window = xui_dynamic_cast(xui_window, component);
		if (window->get_owner())
		{
			xui_global::del_syswnd(window->get_owner());
		}
	}
}

void xui_desktop::on_invalid( xui_method_args& args )
{
	xui_vector<s32> sz(0);
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

void xui_desktop::proc_message( void )
{
	for (u32 i = 0; i < m_messagevec.size(); ++i)
		send_message(m_messagevec[i]);

	m_messagevec.clear();
}

void xui_desktop::proc_recycle( void )
{
    for (std::set<xui_component*>::iterator itor = m_recyclebin.begin(); itor != m_recyclebin.end(); ++itor)
    {
        delete (*itor);
    }
    m_recyclebin.clear();
}

void xui_desktop::proc_settext( void )
{
	if (m_focusctrl)
	{
		xui_component* component = m_focusctrl;
		xui_method_keybd args;
		for (u32 i = 0; i < m_inputtext.length(); ++i)
		{
			args.wchar = m_inputtext[i];
			component->on_keybdchar(			args);
			component->xm_keybdchar(component,	args);
		}
	}

	m_inputtext.clear();
}