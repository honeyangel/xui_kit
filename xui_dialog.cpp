#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_button.h"
#include "xui_dialog.h"

xui_implement_rtti(xui_dialog, xui_window)

xui_dialog::xui_dialog( const std::wstring& text, s32 num_button )
: xui_window(xui_vector<s32>(480, 140), true)
{
	m_sidestyle = k_sidestyle_s;
	m_alignhorz	= k_alignhorz_c;
	m_alignvert = k_alignvert_c;
	m_border	= xui_rect2d<s32>(20);
	m_corner	= 3;
	m_modal		= true;

	m_accept	= new xui_button(xui_vector<s32>(80, 24));
	m_accept->set_sidestyle(k_sidestyle_s);
	m_accept->set_drawcolor(true);
	m_accept->set_corner(3);
	m_accept->set_iconsize(xui_vector<s32>(0));
	m_accept->set_textalign(k_textalign_cc);
	m_accept->ini_component((num_button == 1) ? k_alignhorz_c : 0, k_alignvert_b, 0);
	m_accept->ini_component(true, num_button > 0);
	m_accept->ini_drawer(L"Accept");
	m_accept->xm_buttonclick += new xui_method_member<xui_method_args, xui_dialog>(this, &xui_dialog::on_accept);

	m_cancel	= new xui_button(xui_vector<s32>(80, 24));
	m_cancel->set_sidestyle(k_sidestyle_s);
	m_cancel->set_drawcolor(true);
	m_cancel->set_corner(3);
	m_cancel->set_iconsize(xui_vector<s32>(0));
	m_cancel->set_textalign(k_textalign_cc);
	m_cancel->ini_component(0, k_alignvert_b, 0);
	m_cancel->ini_component(true, num_button > 1);
	m_cancel->ini_drawer(L"Cancel");
	m_cancel->xm_buttonclick += new xui_method_member<xui_method_args, xui_dialog>(this, &xui_dialog::on_cancel);

	xui_family font;
	font.size	= 16;
	font.vert	=  2;
	xui_vector<s32> size = xui_canvas::get_ins()->calc_size(text, font, 400, false);
	m_detail	= new xui_drawer(xui_vector<s32>(0));
	m_detail->ini_component(k_alignhorz_c, k_alignvert_t, 0);
	m_detail->set_singleline(false);
	m_detail->set_textfont(font);
	m_detail->set_text(text);
	m_detail->on_perform_sz(size);

	size.w += m_border.ax;
	size.w += m_border.bx;
	size.h += m_border.ay;
	size.h += m_border.by;
	size.h += 40;
	size.w  = xui_max(size.w, m_render.get_w());
	size.h  = xui_max(size.h, m_render.get_h());
	m_render.set_sz(size);

	m_accept->on_perform_x((size.w-200)/2);
	m_cancel->on_perform_x((size.w-200)/2 + 120);

	add_child(m_accept);
	add_child(m_cancel);
	add_child(m_detail);

	refresh();
}

void xui_dialog::set_text( u08 index, const std::wstring& text )
{
	switch (index)
	{
	case k_accept_text:
		m_accept->set_text(text);
		break;
	case k_cancel_text:
		m_cancel->set_text(text);
		break;
	case k_detail_text:
		m_detail->set_text(text);
		break;
	}
}

void xui_dialog::on_accept( xui_component* sender, xui_method_args& args )
{
	xui_window::on_accept(sender, args);
	xui_desktop::get_ins()->del_child(this);
}

void xui_dialog::on_cancel( xui_component* sender, xui_method_args& args )
{
	xui_window::on_cancel(sender, args);
	xui_desktop::get_ins()->del_child(this);
}