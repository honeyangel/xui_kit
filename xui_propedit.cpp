#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_toggle.h"
#include "xui_numbbox.h"
#include "xui_dropbox.h"
#include "xui_propctrl.h"
#include "xui_propedit.h"

//////////////////////////////////////////////////////////////////////////
//prop_edit
//////////////////////////////////////////////////////////////////////////
xui_propedit::xui_propedit( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_base
//////////////////////////////////////////////////////////////////////////
xui_propedit_base::xui_propedit_base( xui_propctrl* propctrl )
: xui_propedit(propctrl)
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	m_namectrl->set_textalign(k_textalign_lc);
	m_namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_editctrlnonfocus);
	m_namectrl->xm_getfocus		+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_editctrlgetfocus);
	m_namectrl->xm_textchanged	+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_namectrltextchanged);
}

xui_drawer* xui_propedit_base::get_namectrl( void ) const
{
	return m_namectrl;
}

xui_control* xui_propedit_base::get_editctrl( void ) const
{
	return m_editctrl;
}

void xui_propedit_base::reset( void )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.8f));
	m_editctrl->set_sidecolor(xui_control::k_default_sidecolor);
}

void xui_propedit_base::on_editctrlnonfocus( xui_component* sender, xui_method_args& args )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.8f));
	m_editctrl->set_sidecolor(xui_control::k_default_sidecolor);
}

void xui_propedit_base::on_editctrlgetfocus( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_linkpropdata();
	m_namectrl->set_textcolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	m_editctrl->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));

	xui_component* last = (xui_component*)args.wparam;
	if (last == NULL || last->get_ancestor(xui_propctrl::ptr_rtti()) != m_propctrl)
		m_propctrl->on_readyundo();
}

void xui_propedit_base::on_namectrltextchanged( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_namectrl->get_borderrt();
	xui_vector<s32> sz = xui_canvas::get_ins()->calc_size(m_namectrl->get_text(), m_namectrl->get_textfont(), 0, true);
	m_namectrl->on_perform_w(sz.w+rt.ax+rt.bx);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_bool
//////////////////////////////////////////////////////////////////////////
xui_propedit_bool::xui_propedit_bool( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_toggle* boolctrl = new xui_toggle(xui_vector<s32>(16), k_toggle_normal);
	boolctrl->set_corner(3);
	boolctrl->set_drawcolor(true);
	boolctrl->set_backcolor(xui_colour::k_darkgray);
	boolctrl->xm_nonfocus	 += new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_editctrlnonfocus);
	boolctrl->xm_getfocus	 += new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_editctrlgetfocus);
	boolctrl->xm_toggleclick += new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_boolctrlclick);

	m_editctrl = boolctrl;
}

bool xui_propedit_bool::get_value( void ) const
{
	xui_toggle* boolctrl = xui_dynamic_cast(xui_toggle, m_editctrl);
	return boolctrl->was_push();
}

void xui_propedit_bool::set_value( bool value )
{
	xui_toggle* boolctrl = xui_dynamic_cast(xui_toggle, m_editctrl);
	boolctrl->ini_toggle(value);
}

void xui_propedit_bool::reset( void )
{
	xui_propedit_base::reset();
	xui_toggle* boolctrl = xui_dynamic_cast(xui_toggle, m_editctrl);
	boolctrl->set_sidestyle(k_sidestyle_n);
	boolctrl->ini_toggle(false);
}

void xui_propedit_bool::on_editctrlnonfocus( xui_component* sender, xui_method_args& args )
{
	xui_propedit_base::on_editctrlnonfocus(sender, args);
	m_editctrl->set_sidestyle(k_sidestyle_n);
}

void xui_propedit_bool::on_editctrlgetfocus( xui_component* sender, xui_method_args& args )
{
	xui_propedit_base::on_editctrlgetfocus(sender, args);
	m_editctrl->set_sidestyle(k_sidestyle_s);
}

void xui_propedit_bool::on_boolctrlclick( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_enum
//////////////////////////////////////////////////////////////////////////
xui_propedit_enum::xui_propedit_enum( xui_propctrl* propctrl, const xui_propenum_map& textmap )
: xui_propedit_base(propctrl)
{
	xui_dropbox* enumctrl = new xui_dropbox(xui_vector<s32>(128, 18), NULL);
	enumctrl->set_backcolor(xui_colour::k_darkgray);
	enumctrl->set_drawcolor(true);
	enumctrl->set_borderrt(xui_rect2d<s32>(4, 2, 0, 2));
	enumctrl->set_sidestyle(k_sidestyle_s);
	enumctrl->set_corner(3);
	enumctrl->set_readonly(true);
	enumctrl->xm_nonfocus	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_editctrlnonfocus);
	enumctrl->xm_getfocus	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_editctrlgetfocus);
	enumctrl->xm_selection	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_enumctrlselection);
	for (xui_propenum_map::const_iterator itor = textmap.begin(); itor != textmap.end(); ++itor)
	{
		enumctrl->add_item((*itor).second);
	}

	m_editctrl = enumctrl;
}

u32 xui_propedit_enum::get_value( void ) const
{
	xui_dropbox* enumctrl = xui_dynamic_cast(xui_dropbox, m_editctrl);
	return enumctrl->get_selectedindex();
}

void xui_propedit_enum::set_value( u32 index )
{
	xui_dropbox* enumctrl = xui_dynamic_cast(xui_dropbox, m_editctrl);
	enumctrl->ini_dropbox(index);
}

void xui_propedit_enum::reset( void )
{
	xui_propedit_base::reset();
	xui_dropbox* enumctrl = xui_dynamic_cast(xui_dropbox, m_editctrl);
	enumctrl->ini_dropbox(-1);
}

void xui_propedit_enum::on_enumctrlselection( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_number
//////////////////////////////////////////////////////////////////////////
xui_propedit_number::xui_propedit_number( xui_propctrl* propctrl, u08 numbtype, f64 interval, bool showarrow )
: xui_propedit_base(propctrl)
{
	m_namectrl->set_cursor(k_cursor_we);
	m_namectrl->xm_mousemove	+= new xui_method_member<xui_method_mouse, xui_propedit_number>(this, &xui_propedit_number::on_namectrlmousemove);

	xui_textbox* textctrl = new xui_numbbox(xui_vector<s32>(48, 18), numbtype, interval, showarrow);
	textctrl->set_backcolor(xui_colour::k_darkgray);
	textctrl->set_drawcolor(true);
	textctrl->set_borderrt(xui_rect2d<s32>(4, 2, 4, 2));
	textctrl->set_sidestyle(k_sidestyle_s);
	textctrl->set_textalign(k_textalign_lc);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_editctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_editctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_textctrltextchanged);
	//textctrl->xm_mousewheel		+= new xui_method_member<xui_method_mouse, xui_propedit_number>(this, &xui_propedit_number::on_textctrlmousewheel);


	m_interval = interval;
	m_editctrl = textctrl;
}

f64 xui_propedit_number::get_interval( void ) const
{
	return m_interval;
}

f64 xui_propedit_number::get_value( void ) const
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstringstream tmp(textctrl->get_text());
	f64 value = 0.0;
	tmp >> value;
	return value;
}

void xui_propedit_number::set_value( f64 value )
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	char temp[32];
	if (textctrl->get_numbtype() == k_nt_float)	sprintf(temp, "%.2f", (f32)value);
	else										sprintf(temp, "%d",	  (s32)value);

	textctrl->ini_textbox(xui_global::utf8_to_unicode(temp));
}

void xui_propedit_number::reset( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(L"");
}

void xui_propedit_number::on_textctrltextchanged( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}
//xui_method_explain(xui_propedit_number, on_textctrlmousewheel,	void				)( xui_component* sender, xui_method_mouse& args )
//{
//	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
//	f64 value = get_value() + (args.wheel > 0 ? m_interval : -m_interval);
//	if (value < 0.0 && textctrl->get_numbtype() == NT_UNSIGNEDINT)
//		value = 0.0;
//
//	std::wstringstream tmp;
//	tmp << value;
//	textctrl->set_text(tmp.str());
//
//	args.handle = true;
//}
void xui_propedit_number::on_namectrlmousemove( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
        s32 delta = args.point.x - xui_desktop::get_ins()->get_mouselast().x;
        f64 value = get_value() + (f64)delta * m_interval;
        if (value < 0.0 && textctrl->get_numbtype() == k_nt_unsignedint)
            value = 0.0;

        set_value(value);
        m_propctrl->on_editvalue(this);
	}
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_string
//////////////////////////////////////////////////////////////////////////
xui_propedit_string::xui_propedit_string( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_textbox* textctrl = new xui_textbox(xui_vector<s32>(64, 18));
	textctrl->set_backcolor(xui_colour::k_darkgray);
	textctrl->set_drawcolor(true);
	textctrl->set_borderrt(xui_rect2d<s32>(4, 2, 2, 2));
	textctrl->set_sidestyle(k_sidestyle_s);
	textctrl->set_textalign(k_textalign_lc);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_editctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_editctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_textctrltextchanged);

	m_editctrl = textctrl;
}

const std::wstring& xui_propedit_string::get_value( void ) const
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	return textctrl->get_text();
}

void xui_propedit_string::set_value( const std::wstring& value )
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(value);
}

void xui_propedit_string::reset( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(L"");
}

void xui_propedit_string::on_textctrltextchanged( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}