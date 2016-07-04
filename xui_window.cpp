#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_window.h"

xui_implement_rtti(xui_window, xui_panel);

/*
//constructor
*/
xui_create_explain(xui_window)( const xui_vector<s32>& size, bool modal )
: xui_panel(size)
, m_modal(modal)
, m_owner(NULL)
{}

/*
//modal
*/
xui_method_explain(xui_window, was_modal,		bool			)( void ) const
{
	return m_modal;
}
xui_method_explain(xui_window, get_owner,		xui_syswnd*		)( void )
{
	return m_owner;
}
xui_method_explain(xui_window, set_owner,		void			)( xui_syswnd* owner )
{
	m_owner = owner;
}

/*
//virtual
*/
xui_method_explain(xui_window, get_screenpt,	xui_vector<s32>	)( void ) const
{
	if (m_owner)
		return xui_vector<s32>(0);

	return xui_panel::get_screenpt();
}
xui_method_explain(xui_window, choose,			xui_component*	)( const xui_vector<s32>& pt )
{
	xui_vector<s32> fixed = pt;
	if (m_owner)
		fixed += get_renderpt();

	return xui_panel::choose(fixed);
}

/*
//override
*/
xui_method_explain(xui_window, on_show,			void			)( xui_method_args&	 args )
{
	xui_panel::on_show(args);

	if (m_modal)
	{
		xui_desktop::get_ins()->add_modalwnd(this);
	}
}
xui_method_explain(xui_window, on_hide,			void			)( xui_method_args&	 args )
{
	xui_panel::on_hide(args);
	if (m_owner)
	{
		xui_global::del_syswnd(m_owner);
	}
}
xui_method_explain(xui_window, on_keybddown,	void			)( xui_method_keybd& args )
{
	xui_panel::on_keybddown(args);
	if (args.kcode == KEY_ENTER)
	{
		xui_method_args args;
		on_accept(this, args);
	}
	if (args.kcode == KEY_ESC)
	{
		xui_method_args args;
		on_cancel(this, args);
	}
}
xui_method_explain(xui_window, on_setrenderpt,	void			)( xui_method_args&  args )
{
	xui_panel::on_setrenderpt(args);
	if (m_owner)
	{
		invalid();
	}
}
xui_method_explain(xui_window, on_invalid,		void			)( xui_method_args&  args )
{
	xui_panel::on_invalid(args);
	if (m_owner)
	{
		xui_rect2d<s32> rt = get_renderrt() + get_renderpt();
		xui_global::set_syswndrect(m_owner, rt);
	}
}
xui_method_explain(xui_window, on_accept,		void			)( xui_component* sender, xui_method_args& args )
{
	xm_accept(sender, args);
}
xui_method_explain(xui_window, on_cancel,		void			)( xui_component* sender, xui_method_args& args )
{
	xm_cancel(sender, args);
}
