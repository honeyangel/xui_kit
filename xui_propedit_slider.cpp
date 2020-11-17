#include "xui_textbox.h"
#include "xui_slider.h"
#include "xui_scrollthumb.h"
#include "xui_propctrl.h"
#include "xui_propedit_slider.h"

xui_propedit_slider::xui_propedit_slider( xui_propctrl* propctrl, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue )
: xui_propedit(propctrl)
{
	m_minvalue = minvalue;
	m_maxvalue = maxvalue;
	m_editnumb = new xui_propedit_number(propctrl, numbtype, interval, numbtype != k_nt_float);
	xui_drawer*  namectrl = m_editnumb->get_namectrl();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editnumb->get_editctrl());
	namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	namectrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	namectrl->xm_mouserise		+= new xui_method_member<xui_method_mouse, xui_propedit_slider>(this, &xui_propedit_slider::on_namectrlmouserise);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_textctrltextchanged);
	textctrl->xm_textenter		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_textctrltextenter);

	m_spinctrl = xui_slider::create(k_flowstyle_h);
	m_spinctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_base>(m_editnumb, &xui_propedit_base::on_editctrlnonfocus);
	m_spinctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_base>(m_editnumb, &xui_propedit_base::on_editctrlgetfocus);
	m_spinctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	m_spinctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	m_spinctrl->xm_scroll		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlscroll);
}

xui_propedit_slider::~xui_propedit_slider( void )
{
	delete m_editnumb;
}

xui_propedit_number* xui_propedit_slider::get_editnumb( void ) const
{
	return m_editnumb;
}

xui_slider* xui_propedit_slider::get_spinctrl( void ) const
{
	return m_spinctrl;
}

f64 xui_propedit_slider::get_value( void ) const
{
	return m_editnumb->get_value();
}

void xui_propedit_slider::set_value( f64 value )
{
	m_editnumb->set_value(value);

	f64 interval = m_editnumb->get_interval();
	s32 scroll_range = (s32)((m_maxvalue - m_minvalue) / interval);
	s32 scroll_value = (s32)((value      - m_minvalue) / interval);
	m_spinctrl->ini_scroll(scroll_range, scroll_value);
}

void xui_propedit_slider::reset( void )
{
	m_editnumb->reset();

	m_spinctrl->ini_scroll(m_spinctrl->get_range(), 0);
	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(k_sidestyle_n);
}

void xui_propedit_slider::on_spinctrlgetfocus( xui_component* sender, xui_method_args& args )
{
	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(k_sidestyle_s);
	thumb->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}

void xui_propedit_slider::on_spinctrlnonfocus( xui_component* sender, xui_method_args& args )
{
	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(k_sidestyle_n);
	if (sender == m_editnumb->get_editctrl())
	{
		check_value();
	}
}

void xui_propedit_slider::on_namectrlmouserise( xui_component* sender, xui_method_mouse& args )
{
	check_value();
}

void xui_propedit_slider::on_textctrltextchanged( xui_component* sender, xui_method_args& args )
{
	f64 interval = m_editnumb->get_interval();
	s32 scroll_range = (s32)((m_maxvalue - m_minvalue) / interval);
	s32 scroll_value = (s32)((get_value()- m_minvalue) / interval);
	m_spinctrl->ini_scroll(scroll_range, scroll_value);
}

void xui_propedit_slider::on_textctrltextenter( xui_component* sender, xui_method_args& args )
{
	check_value();
}

void xui_propedit_slider::on_spinctrlscroll( xui_component* sender, xui_method_args& args )
{
	f64 interval = m_editnumb->get_interval();
	f64 value    = m_minvalue + (f64)m_spinctrl->get_value() * interval;
	m_editnumb->set_value(value);
	m_propctrl->on_editvalue(this);
}

void xui_propedit_slider::check_value( void )
{
	f64 value = get_value();
	if (value < m_minvalue || value > m_maxvalue)
	{
		value = xui_max(value, m_minvalue);
		value = xui_min(value, m_maxvalue);

		std::wstringstream tmp;
		tmp << value;
		xui_textbox* textctrl = (xui_textbox*)m_editnumb->get_editctrl();
		textctrl->set_text(tmp.str());
	}
}