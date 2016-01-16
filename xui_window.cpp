#include "xui_desktop.h"
#include "xui_window.h"

xui_implement_rtti(xui_window, xui_panel);

/*
//constructor
*/
xui_create_explain(xui_window)( const xui_vector<s32>& size, xui_component* parent )
: xui_panel(size, parent)
{
	m_modal = false;
}

/*
//modal
*/
xui_method_explain(xui_window, was_modal,		bool)( void ) const
{
	return m_modal;
}
xui_method_explain(xui_window, set_modal,		void)( bool flag )
{
	if (m_modal != flag)
	{
		m_modal  = flag;

		if (m_visible)
		{
			if (m_modal)	g_desktop->add_modalwnd(this);
			else			g_desktop->del_modalwnd(this);
		}
	}
}

/*
//override
*/
xui_method_explain(xui_window, on_show,			void)( xui_method_args&	 args )
{
	xui_panel::on_show(args);

	if (m_modal)
		g_desktop->add_modalwnd(this);
}
xui_method_explain(xui_window, on_hide,			void)( xui_method_args&	 args )
{
	xui_panel::on_hide(args);

	if (m_modal)
		g_desktop->del_modalwnd(this);
}
xui_method_explain(xui_window, on_keybddown,	void)( xui_method_keybd& args )
{
	xui_panel::on_keybddown(args);
}
xui_method_explain(xui_window, on_accept,		void)( xui_method_args&	 args )
{

}
xui_method_explain(xui_window, on_cancel,		void)( xui_method_args&	 args )
{

}
