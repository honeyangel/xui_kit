#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_button.h"
#include "xui_dialog.h"

xui_implement_rtti(xui_dialog, xui_window);

/*
//constructor
*/
xui_create_explain(xui_dialog)( const std::wstring& text, s32 num_button )
: xui_window(xui_vector<s32>(320, 140))
{
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz	= ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;
	m_border	= xui_rect2d<s32>(20);
	m_corner	= 3;
	m_modal		= true;

	m_accept = xui_button::create(NULL, L"OK",     80);
	xui_method_ptrcall(m_accept, ini_component	)((num_button == 1) ? ALIGNHORZ_C : 0, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_accept, ini_component	)(true, num_button > 0);
	xui_method_ptrcall(m_accept, xm_buttonclick	) += new xui_method_member<xui_method_args, xui_dialog>(this, &xui_dialog::on_accept);

	m_cancel = xui_button::create(NULL, L"Cancel", 80);
	xui_method_ptrcall(m_cancel, ini_component	)(0, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_cancel, ini_component	)(true, num_button > 1);
	xui_method_ptrcall(m_cancel, xm_buttonclick	) += new xui_method_member<xui_method_args, xui_dialog>(this, &xui_dialog::on_cancel);

	m_detail = xui_drawer::create(text);
	xui_vector<s32> size = xui_convas::get_ins()->calc_size(text, m_detail->get_textfont(), 240, false);
	xui_method_ptrcall(m_detail, set_borderrt	)(xui_rect2d<s32>(0));
	xui_method_ptrcall(m_detail, ini_component	)(ALIGNHORZ_C, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_detail, set_singleline	)(false);
	xui_method_ptrcall(m_detail, on_perform_sz	)(size);

	size.w += m_border.ax;
	size.w += m_border.bx;
	size.h += m_border.ay;
	size.h += m_border.by;
	size.h += 40;
	size.w  = xui_max(size.w, m_render.get_w());
	size.h  = xui_max(size.h, m_render.get_h());
	m_render.set_sz(size);

	xui_method_ptrcall(m_accept, on_perform_x	)((size.w-200)/2);
	xui_method_ptrcall(m_cancel, on_perform_x	)((size.w-200)/2 + 120);

	add_child(m_accept);
	add_child(m_cancel);
	add_child(m_detail);

	refresh();
}

/*
//method
*/
xui_method_explain(xui_dialog, set_text,	void		)( u32 index, const std::wstring& text )
{
	switch (index)
	{
	case 0:
		m_accept->set_text(text);
		break;
	case 1:
		m_cancel->set_text(text);
		break;
	}
}

/*
//callback
*/
xui_method_explain(xui_dialog, on_accept,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_window::on_accept(sender, args);
	xui_desktop::get_ins()->del_child(this);
}
xui_method_explain(xui_dialog, on_cancel,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_window::on_cancel(sender, args);
	xui_desktop::get_ins()->del_child(this);
}