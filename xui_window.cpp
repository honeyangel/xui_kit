#include "xui_desktop.h"
#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_window.h"

xui_implement_rtti(xui_window, xui_panel)

xui_window::xui_window( const xui_vector<s32>& size, bool modal, bool popup )
: xui_panel(size)
, m_modal(modal)
, m_popup(popup)
, m_owner(NULL)
, m_float(NULL)
{}

bool xui_window::was_modal( void ) const
{
	return m_modal;
}

bool xui_window::was_popup( void ) const
{
	return m_popup;
}

xui_syswnd* xui_window::get_owner( void )
{
	return m_owner;
}

void xui_window::set_owner( xui_syswnd* owner )
{
	m_owner = owner;
}

xui_component* xui_window::get_float( void )
{
	return m_float;
}

void xui_window::set_float( xui_component* component )
{
	m_float = component;
	if (m_float)
	{
		m_float->set_visible(false);
		m_float->set_visible(true );
	}
}

xui_vector<s32> xui_window::get_screenpt( void ) const
{
	if (m_owner)
		return xui_vector<s32>(0);

	return xui_panel::get_screenpt();
}

xui_component* xui_window::choose( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		if (m_float)
			component = m_float->choose(pt);
		if (component == NULL)
		{
			xui_vector<s32> fixed = pt;
			if (m_owner)
				fixed += get_renderpt();

			component = xui_panel::choose(fixed);
		}
	}

	return component;
}

void xui_window::update( f32 delta )
{
	xui_panel::update(delta);
	if (m_float)
	{
		m_float->update(delta);
	}
}

void xui_window::render( void )
{
	xui_panel::render();
	xui_component* catchctrl = xui_desktop::get_ins()->get_catchctrl();
	if (catchctrl)
	{
		xui_method_args args;
		args.wparam = this;
		catchctrl->on_topdraw(           args);
		catchctrl->xm_topdraw(catchctrl, args);
	}

	xui_canvas::get_ins()->set_cliprect(xui_canvas::get_ins()->get_viewport());
	if (m_float)
	{
		m_float->render();
	}
}

void xui_window::on_show( xui_method_args& args )
{
	xui_panel::on_show(args);

	if (m_modal || m_popup)
	{
		xui_global::add_syswnd(this, m_modal == false);

		if (m_modal)
			xui_desktop::get_ins()->add_modalwnd(this);
	}
}

void xui_window::on_hide( xui_method_args& args )
{
	xui_panel::on_hide(args);
	if (m_owner)
	{
		xui_global::del_syswnd(m_owner);
	}
}

void xui_window::on_keybddown( xui_method_keybd& args )
{
	xui_panel::on_keybddown(args);
	if (args.kcode == k_key_enter)
	{
		xui_method_args args;
		on_accept(this, args);
	}
	if (args.kcode == k_key_esc)
	{
		xui_method_args args;
		on_cancel(this, args);
	}
}

void xui_window::on_setrenderpt( xui_method_args& args )
{
	xui_panel::on_setrenderpt(args);
	if (m_owner)
	{
		invalid();
	}
}

void xui_window::on_invalid( xui_method_args& args )
{
	xui_panel::on_invalid(args);
	if (m_owner)
	{
		xui_rect2d<s32> rt = get_renderrt() + get_renderpt();
		xui_global::set_syswndrect(m_owner, rt);
	}
}

void xui_window::on_accept( xui_component* sender, xui_method_args& args )
{
	xm_accept(sender, args);
}

void xui_window::on_cancel( xui_component* sender, xui_method_args& args )
{
	xm_cancel(sender, args);
}
