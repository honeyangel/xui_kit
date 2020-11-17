#include "xui_canvas.h"
#include "xui_textbox.h"
#include "xui_propctrl.h"
#include "xui_propedit_stdvec.h"

xui_propedit_stdvec::xui_propedit_stdvec( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_textbox* textctrl = new xui_textbox(xui_vector<s32>(48, 18));
	textctrl->set_backcolor(xui_colour::k_darkgray);
	textctrl->set_drawcolor(true);
	textctrl->set_borderrt(xui_rect2d<s32>(4, 2, 2, 2));
	textctrl->set_sidestyle(k_sidestyle_s);
	textctrl->set_textalign(k_textalign_lc);
	textctrl->set_numbtype(k_nt_unsignedint);
	textctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_editctrlnonfocus);
	textctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_editctrlgetfocus);
	textctrl->xm_textenter	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_textctrltextenter);

	m_editctrl = textctrl;
}

u32 xui_propedit_stdvec::get_value( void ) const
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstringstream tmp(textctrl->get_text());
	u32 value = 0;
	tmp >> value;
	return value;
}

void xui_propedit_stdvec::set_value( u32 value )
{
	std::wstringstream tmp;
	tmp << value;
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(tmp.str());
}

void xui_propedit_stdvec::reset( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(L"");
}

void xui_propedit_stdvec::on_textctrltextenter( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstringstream tmp(textctrl->get_text());
	s32 value = 0;
	tmp >> value;
	if (value < 0)
		textctrl->ini_textbox(L"0");

	m_propctrl->on_editvalue(this);
}