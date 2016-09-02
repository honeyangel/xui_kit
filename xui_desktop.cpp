#include "xui_global.h"
#include "xui_syswnd.h"
#include "xui_convas.h"
#include "xui_window.h"
#include "xui_dialog.h"
#include "xui_desktop.h"

xui_implement_rtti(xui_desktop, xui_panel);
xui_implement_instance_member(xui_desktop);
xui_implement_instance_method(xui_desktop);


/*
//constructor
*/
xui_create_explain(xui_desktop)( void )
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

/*
//destructor
*/
xui_delete_explain(xui_desktop)( void )
{
	proc_recycle();
}

/*
//ctrl
*/
xui_method_explain(xui_desktop, get_catchctrl,	xui_component*			)( void )
{
	return m_catchctrl;
}
xui_method_explain(xui_desktop, set_catchctrl,	void					)( xui_component* component )
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
xui_method_explain(xui_desktop, get_focusctrl,	xui_component*			)( void )
{
	return m_focusctrl;
}
xui_method_explain(xui_desktop, set_focusctrl,	void					)( xui_component* component )
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
xui_method_explain(xui_desktop, get_hoverctrl,	xui_component*			)( void )
{
	return m_hoverctrl;
}
xui_method_explain(xui_desktop, set_hoverctrl,	void					)( xui_component* component )
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
xui_method_explain(xui_desktop, get_floatctrl,	xui_component*			)( void )
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
xui_method_explain(xui_desktop, set_floatctrl,	void					)( xui_window* owner, xui_component* component )
{
	for (u32 i = 0; i < m_childctrl.size(); ++i)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		window->set_float(window == owner ? component : NULL);
	}
}

/*
//mouse
*/
xui_method_explain(xui_desktop, get_mousedown,	const xui_vector<s32>&	)( void ) const
{
	return m_mousedown;
}
xui_method_explain(xui_desktop, get_mouselast,	const xui_vector<s32>&	)( void ) const
{
	return m_mouselast;
}
xui_method_explain(xui_desktop, get_mousecurr,	const xui_vector<s32>&	)( void ) const
{
	return m_mousecurr;
}
xui_method_explain(xui_desktop, get_mousemove,	xui_vector<s32>			)( void ) const
{
	return m_mousecurr-m_mouselast;
}
xui_method_explain(xui_desktop, set_mouselast,	void					)( const xui_vector<s32>& pt )
{
	m_mouselast = pt;
}
xui_method_explain(xui_desktop, set_mousecurr,	void					)( const xui_vector<s32>& pt )
{
	m_mousecurr = pt;
}

/*
//input
*/
//xui_method_explain(xui_desktop, get_pastetext, const std::wstring&		)( void ) const
//{
//	return m_pastetext;
//}
//xui_method_explain(xui_desktop, set_pastetext, void						)( const std::wstring& text )
//{
//	m_pastetext = text;
//}

/*
//modal
*/
xui_method_explain(xui_desktop, add_modalwnd,	void					)( xui_window* window )
{
	xui_global::mod_syswnd(window->get_owner());
	m_modalpool.push_back (window);
}
xui_method_explain(xui_desktop, del_modalwnd,	void					)( xui_window* window )
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

/*
//message
*/
xui_method_explain(xui_desktop, move_recycle,	void					)( xui_component* component )
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

	for (u32 i = 0; i < m_recyclebin.size(); ++i)
	{
		if (component == m_recyclebin[i])
			return;
	}

	m_recyclebin.push_back(component);
}
xui_method_explain(xui_desktop, send_message,	void					)( const xui_message& message )
{
	switch(message.msgidx)
	{
	case XM_GETCATCH:
		{
			set_catchctrl(message.sender);
		}
		break;
	case XM_GETFOCUS:
		{
			set_focusctrl(message.sender);
		}
		break;
	case XM_DEAD:
		{

		}
		break;
	}
}
xui_method_explain(xui_desktop, post_message,	void					)( const xui_message& message )
{
	m_messagevec.push_back(message);
}
xui_method_explain(xui_desktop, show_message,	xui_window*				)( const std::wstring& text, s32 num_button )
{
	xui_window* dialog = new xui_dialog(text, num_button);
	add_child(dialog);
	return dialog;
}

/*
//virtual
*/
xui_method_explain(xui_desktop, choose_else,	xui_component*			)( const xui_vector<s32>& pt )
{
	xui_component* component = xui_container::choose_else(pt);
	if (component == NULL)
	{
		xui_rect2d<s32> rt = get_renderrtins() + m_render.get_pt();
		if (rt.was_inside(pt))
		{
			xui_vector<s32> relative = pt - m_render.get_pt();
			xui_vecptr_delloop(m_childctrl)
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
xui_method_explain(xui_desktop, update,			void					)( f32 delta )
{
	xui_panel::update(delta);
	proc_message();
	proc_recycle();
	proc_settext();
}
xui_method_explain(xui_desktop, render,			void					)( void )
{
	xui_convas::get_ins()->set_cliprect(m_render);
	xui_vecptr_addloop(m_childctrl)
	{
		xui_window* window = xui_dynamic_cast(xui_window, m_childctrl[i]);
		if (window->get_owner())
			continue;

		if (m_childctrl[i]->was_visible())
			m_childctrl[i]->render();
	}
	xui_convas::get_ins()->set_cliprect(m_render);
}

/*
//os
*/
xui_method_explain(xui_desktop, os_mousewheel,	void					)( xui_method_mouse& args )
{
	if (m_hoverctrl)
	{
		xui_component* root = m_hoverctrl;
		while (root && args.handle == false)
		{
			root->on_mousewheel(	  args);
			root->xm_mousewheel(root, args);
			if (xui_issub_kindof(xui_desktop, root))
				break;

			root = root->get_parent();
		}
	}
}
xui_method_explain(xui_desktop, os_mousedown,	void					)( xui_method_mouse& args )
{
	xui_component* component = NULL;

	xui_syswnd* syswnd = xui_global::get_syswnd(args.wparam);
	if (syswnd)
		component = syswnd->get_popupctrl()->choose(args.point);
	else
		component = choose(args.point);

	if (args.mouse == MB_L)
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
}
xui_method_explain(xui_desktop, os_mouserise,	void					)( xui_method_mouse& args )
{
	xui_component* component = NULL;

	if (args.mouse == MB_L && m_catchctrl)
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
		xui_syswnd* syswnd = xui_global::get_syswnd(args.wparam);
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

	if (args.mouse == MB_L)
	{
		set_catchctrl(NULL);
		m_allowdrag = false;
		m_catchdata = NULL;
	}

	xui_global::set_cursor(m_hoverctrl == NULL ? CURSOR_DEFAULT : m_hoverctrl->get_cursor());
}
xui_method_explain(xui_desktop, os_mousemove,	void					)( xui_method_mouse& args )
{
	m_mouselast = m_mousecurr;
	m_mousecurr = args.point;

	xui_component* component = NULL;
	xui_syswnd* syswnd = xui_global::get_syswnd(args.wparam);
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
			if (component = wnd->get_popupctrl()->choose(pt))
				break;
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
		xui_global::set_cursor((m_catchctrl == m_hoverctrl || m_allowdrag) ? CURSOR_DRAG : CURSOR_FORBID);
	}
	else
	{
		xui_global::set_cursor((m_hoverctrl == NULL) ? CURSOR_DEFAULT : m_hoverctrl->get_cursor());
	}
}
xui_method_explain(xui_desktop, os_keybddown,	void					)( xui_method_keybd& args )
{
	xui_component* component = m_focusctrl;
	if (component)
	{
		component->on_keybddown(           args);
		component->xm_keybddown(component, args);
	}
}
xui_method_explain(xui_desktop, os_keybdrise,	void					)( xui_method_keybd& args )
{
	xui_component* component = m_focusctrl;
	if (component)
	{
		component->on_keybdrise(           args);
		component->xm_keybdrise(component, args);
	}
}
xui_method_explain(xui_desktop, os_keybdchar,	void					)( u16 c )
{
	m_inputtext.append(1, c);
}

/*
//callback
*/
xui_method_explain(xui_desktop, on_addchild,	void					)( xui_method_args& args )
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
xui_method_explain(xui_desktop, on_delchild,	void					)( xui_method_args& args )
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
xui_method_explain(xui_desktop, on_invalid,		void					)( xui_method_args& args )
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

/*
//proc
*/
xui_method_explain(xui_desktop, proc_message,	void					)( void )
{
	for (u32 i = 0; i < m_messagevec.size(); ++i)
		send_message(m_messagevec[i]);

	m_messagevec.clear();
}
xui_method_explain(xui_desktop, proc_recycle,	void					)( void )
{
	for (u32 i = 0; i < m_recyclebin.size(); ++i)
		delete m_recyclebin[i];

	m_recyclebin.clear();
}
xui_method_explain(xui_desktop, proc_settext,	void					)( void )
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