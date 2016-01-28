#include "xui_convas.h"
#include "xui_textbox.h"
#include "xui_propctrl.h"
#include "xui_propedit_stdvec.h"

/*
//constructor
*/
xui_create_explain(xui_propedit_stdvec)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_textbox* textctrl = new xui_textbox(xui_vector<s32>(48, 18));
	xui_method_ptrcall(textctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(textctrl, set_drawcolor	)(true);
	xui_method_ptrcall(textctrl, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl, set_textalign	)(TA_LC);
	xui_method_ptrcall(textctrl, set_numbonly	)(true);
	textctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_editctrlnonfocus);
	textctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_editctrlgetfocus);
	textctrl->xm_textenter	+= new xui_method_member<xui_method_args,  xui_propedit_stdvec>(this, &xui_propedit_stdvec::on_textctrltextenter);

	m_editctrl = textctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_stdvec, get_value,				u32		)( void ) const
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstringstream tmp(textctrl->get_text());
	u32 value = 0;
	tmp >> value;
	return value;
}
xui_method_explain(xui_propedit_stdvec, set_value,				void	)( u32 value )
{
	std::wstringstream tmp;
	tmp << value;
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(tmp.str());
}
xui_method_explain(xui_propedit_stdvec, reset,					void	)( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(L"");
}

/*
//event
*/
xui_method_explain(xui_propedit_stdvec, on_textctrltextenter,	void	)( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstringstream tmp(textctrl->get_text());
	s32 value = 0;
	tmp >> value;
	if (value < 0)
		textctrl->ini_textbox(L"0");

	m_propctrl->on_editvalue(this);
}