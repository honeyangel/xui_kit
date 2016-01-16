#include "xui_convas.h"
#include "xui_window.h"
#include "xui_desktop.h"

/*
//global
*/
xui_desktop* g_desktop = NULL;

/*
//constructor
*/
xui_create_explain(xui_desktop)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_panel(name, rect)
{
	m_catchctrl = NULL;
	m_focusctrl = NULL;
	m_hoverctrl = NULL;
	m_floatctrl = NULL;
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
				os_setcursor(curr->get_cursor());
			}
		}
	}
}
xui_method_explain(xui_desktop, get_floatctrl,	xui_component*			)( void )
{
	return m_floatctrl;
}
xui_method_explain(xui_desktop, set_floatctrl,	void					)( xui_component* component )
{
	m_floatctrl = component;
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

/*
//input
*/
xui_method_explain(xui_desktop, get_pastetext, const std::wstring&		)( void ) const
{
	return m_pastetext;
}

xui_method_explain(xui_desktop, set_pastetext, void						)( const std::wstring& text )
{
	m_pastetext = text;
}

/*
//modal
*/
xui_method_explain(xui_desktop, get_modaltop,	xui_window*				)( void )
{
	return m_modalpool.empty() ? NULL : m_modalpool.back();
}

xui_method_explain(xui_desktop, add_modalwnd,	void					)( xui_window* window )
{
	del_modalwnd(window);
	m_modalpool.push_back(window);
}

xui_method_explain(xui_desktop, del_modalwnd,	void					)( xui_window* window )
{
	for (u32 i = 0; i < m_modalpool.size(); ++i)
	{
		if (m_modalpool[i] == window)
		{
			m_modalpool.erase(m_modalpool.begin()+i);
			break;
		}
	}
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
	if (m_floatctrl)
	{
		if (m_floatctrl == component || m_floatctrl->was_ancestor(component))
			m_floatctrl = NULL;
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

/*
//virtual
*/
xui_method_explain(xui_desktop, update,			void					)( f32 delta )
{
	xui_panel::update(delta);
	proc_message();
	proc_recycle();
	proc_settext();
}

xui_method_explain(xui_desktop, render,			void					)( void )
{
	// render 
	xui_vecptr_addloop(m_childctrl)
	{
		xui_window* window = (xui_window*)m_childctrl[i];
		if (window->was_modal())
			continue;

		m_childctrl[i]->render();
	}

	// render modal
	for (u32 i = 0; i < m_modalpool.size(); ++i)
	{
		m_modalpool[i]->render();
	}

	// render float
	if (m_floatctrl)
	{
		m_floatctrl->render();
	}

	if (m_catchctrl)
	{
		xui_method_args args;
		m_catchctrl->on_topdraw(             args);
		m_catchctrl->xm_topdraw(m_catchctrl, args);
	}

	if (m_catchctrl && m_catchctrl != m_hoverctrl && m_catchdata)
	{
		if (m_allowdrag)
		{
			xui_rect2d<s32> rt;
			rt.ax = m_mousecurr.x;
			rt.ay = m_mousecurr.y+16;
			rt.set_w(16);
			rt.set_h( 8);
			g_convas->draw_rectangle(rt, xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
			rt.oft_x( 1);
			rt.oft_y( 1);
			rt.set_w(14);
			rt.set_h( 6);
			g_convas->draw_rectangle(rt, xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
		}
		else
		{
			xui_rect2d<s32> rt;
			rt.ax = m_mousecurr.x;
			rt.ay = m_mousecurr.y+16;
			rt.set_w(16);
			rt.set_h(16);
			g_convas->draw_rectangle(rt, xui_colour(1.0f, 1.0f, 0.0f, 0.0f));

			g_convas->draw_line(xui_vector<s32>(rt.ax,   rt.ay),   xui_vector<s32>(rt.bx,   rt.by),   xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
			g_convas->draw_line(xui_vector<s32>(rt.ax,   rt.ay+1), xui_vector<s32>(rt.bx-1, rt.by),   xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
			g_convas->draw_line(xui_vector<s32>(rt.ax+1, rt.ay),   xui_vector<s32>(rt.bx,   rt.by-1), xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
			g_convas->draw_line(xui_vector<s32>(rt.bx,   rt.ay),   xui_vector<s32>(rt.ax,   rt.by),   xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
			g_convas->draw_line(xui_vector<s32>(rt.bx,   rt.ay+1), xui_vector<s32>(rt.ax+1, rt.by),   xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
			g_convas->draw_line(xui_vector<s32>(rt.bx-1, rt.ay),   xui_vector<s32>(rt.ax,   rt.by-1), xui_colour(1.0f, 1.0f, 0.0f, 0.0f));

			rt.oft_x( 1);
			rt.oft_y( 1);
			rt.set_w(14);
			rt.set_h(14);
			g_convas->draw_rectangle(rt, xui_colour(1.0f, 1.0f, 0.0f, 0.0f));
		}
	}
}

/*
//os
*/
xui_method_explain(xui_desktop, os_mousedown,	void					)( xui_method_mouse& args )
{
	m_mousedown = args.point;

	xui_component* component = NULL;

	if (m_floatctrl)
		component = m_floatctrl->choose(args.point);

	if (component == NULL)
	{
		if (get_modaltop())
			component = get_modaltop()->choose(args.point);
		else
			component = choose(args.point);
	}

	set_catchctrl(component);

	if (m_catchctrl)
	{
		m_catchctrl->on_mousedown(             args);
		m_catchctrl->xm_mousedown(m_catchctrl, args);
	}
}

xui_method_explain(xui_desktop, os_mouserise,	void					)( xui_method_mouse& args )
{
	if (m_catchctrl)
	{
		if (m_catchctrl != m_hoverctrl && m_hoverctrl && m_allowdrag && m_catchdata)
		{
			xui_method_dragdrop other_args;
			other_args.allow = m_allowdrag;
			other_args.drag  = m_catchctrl;
			other_args.data  = m_catchdata;
			other_args.type  = m_catchtype;
			m_hoverctrl->on_mousedragdrop(             other_args);
			m_hoverctrl->xm_mousedragdrop(m_hoverctrl, other_args);
		}

		m_catchctrl->on_mouserise(             args);
		m_catchctrl->xm_mouserise(m_catchctrl, args);

		set_catchctrl(NULL);
	}

	m_allowdrag = false;
	m_catchdata = NULL;
}

xui_method_explain(xui_desktop, os_mousemove,	void					)( xui_method_mouse& args )
{
	m_mouselast = m_mousecurr;
	m_mousecurr = args.point;

	xui_component* component = NULL;

	if (m_floatctrl)
		component = m_floatctrl->choose(args.point);

	if (component == NULL)
	{
		if (get_modaltop())
			component = get_modaltop()->choose(args.point);
		else
			component = choose(args.point);
	}

	set_hoverctrl(component);

	if (m_catchctrl)
	{
		xui_vector<s32> delta = args.point - m_mousedown;
		bool hasdrag = (xui_abs(delta.x) > 4) || (xui_abs(delta.y) > 4);
		if (m_catchdata == NULL && hasdrag)
		{
			xui_method_dragdrop other_args;
			other_args.drag = m_catchctrl;
			m_catchctrl->on_mousedragitem(             other_args);
			m_catchctrl->xm_mousedragitem(m_catchctrl, other_args);
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
			m_hoverctrl->on_mousedragover(             other_args);
			m_hoverctrl->xm_mousedragover(m_hoverctrl, other_args);
			m_allowdrag = other_args.allow;
		}

		m_catchctrl->on_mousemove(             args);
		m_catchctrl->xm_mousemove(m_catchctrl, args);
	}
	else
	if (m_hoverctrl)
	{
		m_hoverctrl->on_mousemove(             args);
		m_hoverctrl->xm_mousemove(m_hoverctrl, args);
	}
}

xui_method_explain(xui_desktop, os_keybddown,	void					)( xui_method_keybd& args )
{
	if (m_focusctrl)
	{
		m_focusctrl->on_keybddown(             args);
		m_focusctrl->xm_keybddown(m_focusctrl, args);
	}
}

xui_method_explain(xui_desktop, os_keybdrise,	void					)( xui_method_keybd& args )
{
	if (m_focusctrl)
	{
		m_focusctrl->on_keybdrise(             args);
		m_focusctrl->xm_keybdrise(m_focusctrl, args);
	}
}

xui_method_explain(xui_desktop, os_keybdchar,	void					)( u08 c )
{
	m_inputtext.append(1, c);
}

xui_method_explain(xui_desktop, os_setcursor,	void					)( u32 cursor )
{
	switch (cursor)
	{
	case CURSOR_DEFAULT:	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW); break;
	case CURSOR_NS:			glutSetCursor(GLUT_CURSOR_UP_DOWN);		break;
	case CURSOR_WE:			glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);	break;
	case CURSOR_TEXT:		glutSetCursor(GLUT_CURSOR_TEXT);		break;
	case CURSOR_STOP:		glutSetCursor(GLUT_CURSOR_WAIT);		break;
	case CURSOR_MOVE:		glutSetCursor(GLUT_CURSOR_CROSSHAIR);	break;
	}
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
		xui_window* window = (xui_window*)component;
		if (window->was_visible() && window->was_modal())
			add_modalwnd(window);
	}
}

xui_method_explain(xui_desktop, on_delchild,	void					)( xui_method_args& args )
{
	xui_panel::on_delchild(args);

	xui_component* component = (xui_component*)(args.wparam);
	if (component)
	{
		xui_window* window = (xui_window*)component;
		if (window->was_visible() && window->was_modal())
			del_modalwnd(window);
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
		wchar_t buffer[128];
		MultiByteToWideChar(CP_ACP, 0, m_inputtext.c_str(), -1, buffer, 128);

		xui_method_keybd args;
		for (u32 i = 0; i < wcslen(buffer); ++i)
		{
			args.wchar = buffer[i];
			m_focusctrl->on_keybdchar(args);
		}
	}

	m_inputtext.clear();
}