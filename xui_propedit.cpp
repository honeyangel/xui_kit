#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_dropbox.h"
#include "xui_propctrl.h"
#include "xui_propedit.h"

//////////////////////////////////////////////////////////////////////////
//prop_edit
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit)( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_base
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit_base)( xui_propctrl* propctrl )
: xui_propedit(propctrl)
{
	m_namectrl = new xui_drawer("", xui_rect2d<s32>(0, 0, 32, 20));
	xui_method_ptrcall(m_namectrl, set_backcolor)(xui_colour(0.0f));
	xui_method_ptrcall(m_namectrl, set_font		)(xui_family("Consolas", 16, false));
	xui_method_ptrcall(m_namectrl, set_textalign)(TA_LC);
	xui_method_ptrcall(m_namectrl, set_textcolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_editctrlnonfocus);
	m_namectrl->xm_getfocus		+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_editctrlgetfocus);
	m_namectrl->xm_textchanged	+= new xui_method_member<xui_method_args, xui_propedit_base>(this, &xui_propedit_base::on_namectrltextchanged);
}

/*
//method
*/
xui_method_explain(xui_propedit_base,	get_namectrl,			xui_drawer*			)( void ) const
{
	return m_namectrl;
}
xui_method_explain(xui_propedit_base,	get_editctrl,			xui_control*		)( void ) const
{
	return m_editctrl;
}

/*
//override
*/
xui_method_explain(xui_propedit_base,	reset,					void				)( void )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_editctrl->set_sidecolor(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
}

/*
//event
*/
xui_method_explain(xui_propedit_base,	on_editctrlnonfocus,	void				)( xui_component* sender, xui_method_args&  args )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	m_editctrl->set_sidecolor(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
}
xui_method_explain(xui_propedit_base,	on_editctrlgetfocus,	void				)( xui_component* sender, xui_method_args&  args )
{
	m_propctrl->on_linkpropdata();
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.2f, 0.3f, 0.9f));
	m_editctrl->set_sidecolor(xui_colour(1.0f, 0.2f, 0.3f, 0.9f));
}
xui_method_explain(xui_propedit_base,	on_namectrltextchanged,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_vector<s32> size = g_convas->calc_size(m_namectrl->get_text(), m_namectrl->get_font(), 0, true);
	size.w += m_namectrl->get_borderrt().ax + m_namectrl->get_borderrt().bx;
	size.h  = m_namectrl->get_renderh();
	m_namectrl->set_rendersz(size, false);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_bool
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit_bool)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_toggle* boolctrl = new xui_toggle("", xui_rect2d<s32>(0, 0, 16, 16));
	xui_method_ptrcall(boolctrl, set_backcolor)(xui_colour(0.0f));
	xui_method_ptrcall(boolctrl, set_sidecolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(boolctrl, set_sidestyle)(SIDESTYLE_S);
	boolctrl->xm_nonfocus	+= new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_editctrlnonfocus);
	boolctrl->xm_getfocus	+= new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_editctrlgetfocus);
	boolctrl->xm_click		+= new xui_method_member<xui_method_args, xui_propedit_bool>(this, &xui_propedit_bool::on_boolctrlclick);

	m_editctrl = boolctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_bool, get_value,				bool				)( void ) const
{
	xui_toggle* boolctrl = (xui_toggle*)m_editctrl;
	return boolctrl->was_check();
}
xui_method_explain(xui_propedit_bool, set_value,				void				)( bool value )
{
	xui_toggle* boolctrl = (xui_toggle*)m_editctrl;
	boolctrl->ini_toggle(value);
}
xui_method_explain(xui_propedit_bool, reset,					void				)( void )
{
	xui_propedit_base::reset();
	xui_toggle* boolctrl = (xui_toggle*)m_editctrl;
	boolctrl->ini_toggle(false);
}

/*
//event
*/
xui_method_explain(xui_propedit_bool, on_boolctrlclick,			void				)( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_enum
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit_enum)( xui_propctrl* propctrl, const xui_propenum_map& textmap )
: xui_propedit_base(propctrl)
{
	xui_dropbox* enumctrl = new xui_dropbox("", xui_rect2d<s32>(0, 0, 32, 18));
	xui_method_ptrcall(enumctrl, set_backcolor)(xui_colour(0.0f));
	xui_method_ptrcall(enumctrl, set_borderrt )(xui_rect2d<s32>(1));
	xui_method_ptrcall(enumctrl, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(enumctrl, set_sidecolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(enumctrl, set_font	  )(xui_family("Arial", 16, false));
	xui_method_ptrcall(enumctrl, set_textcolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(enumctrl, set_textalign)(TA_LC);
	xui_method_ptrcall(enumctrl, set_readonly )(true);
	enumctrl->xm_nonfocus	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_editctrlnonfocus);
	enumctrl->xm_getfocus	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_editctrlgetfocus);
	enumctrl->xm_selection	+= new xui_method_member<xui_method_args, xui_propedit_enum>(this, &xui_propedit_enum::on_enumctrlselection);
	for (xui_propenum_map::const_iterator itor = textmap.begin(); itor != textmap.end(); ++itor)
	{
		enumctrl->add_item((*itor).second);
	}

	m_editctrl = enumctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_enum, get_value,				u32					)( void ) const
{
	xui_dropbox* enumctrl = (xui_dropbox*)m_editctrl;
	return enumctrl->get_selectedindex();
}
xui_method_explain(xui_propedit_enum, set_value,				void				)( u32 index )
{
	xui_dropbox* enumctrl = (xui_dropbox*)m_editctrl;
	enumctrl->ini_dropbox(index);
}

/*
//override
*/
xui_method_explain(xui_propedit_enum, reset,					void				)( void )
{
	xui_propedit_base::reset();
	xui_dropbox* enumctrl = (xui_dropbox*)m_editctrl;
	enumctrl->ini_dropbox(-1);
}

/*
//event
*/
xui_method_explain(xui_propedit_enum, on_enumctrlselection,		void				)( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_number
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit_number)( xui_propctrl* propctrl, f64 interval )
: xui_propedit_base(propctrl)
{
	m_namectrl->set_cursor(CURSOR_WE);
	m_namectrl->xm_mousemove	+= new xui_method_member<xui_method_mouse, xui_propedit_number>(this, &xui_propedit_number::on_namectrlmousemove);

	xui_textbox* textctrl = new xui_textbox("", xui_rect2d<s32>(0, 0, 32, 18));
	xui_method_ptrcall(textctrl, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(textctrl, set_borderrt	)(xui_rect2d<s32>(1));
	xui_method_ptrcall(textctrl, set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl, set_font		)(xui_family("Arial", 16, false));
	xui_method_ptrcall(textctrl, set_textcolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(textctrl, set_textalign	)(TA_LC);
	xui_method_ptrcall(textctrl, set_numbonly	)(true);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_editctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_editctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_number>(this, &xui_propedit_number::on_textctrltextchanged);

	m_interval = interval;
	m_editctrl = textctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_number, get_interval,			f64					)( void ) const
{
	return m_interval;
}
xui_method_explain(xui_propedit_number, get_value,				f64					)( void ) const
{
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	std::wstringstream tmp(textctrl->get_text());
	f64 value = 0.0;
	tmp >> value;
	return value;
}
xui_method_explain(xui_propedit_number, set_value,				void				)( f64 value )
{
	std::wstringstream tmp;
	tmp << value;
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	textctrl->ini_textbox(tmp.str());
}

/*
//override
*/
xui_method_explain(xui_propedit_number, reset,					void				)( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	textctrl->ini_textbox(L"");
}

/*
//event
*/
xui_method_explain(xui_propedit_number, on_textctrltextchanged, void				)( xui_component* sender, xui_method_args&  args )
{
	m_propctrl->on_editvalue(this);
}
xui_method_explain(xui_propedit_number, on_namectrlmousemove,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (g_desktop->get_catchctrl() == sender)
	{
		s32 delta = args.point.x - g_desktop->get_mouselast().x;
		f64 value = get_value() + (f64)delta * m_interval;

		std::wstringstream tmp;
		tmp << value;
		xui_textbox* textctrl = (xui_textbox*)m_editctrl;
		textctrl->set_text(tmp.str());
	}
}

//////////////////////////////////////////////////////////////////////////
//prop_edit_string
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propedit_string)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_textbox* textctrl = new xui_textbox("", xui_rect2d<s32>(0, 0, 32, 18));
	xui_method_ptrcall(textctrl, set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(textctrl, set_borderrt	)(xui_rect2d<s32>(1));
	xui_method_ptrcall(textctrl, set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl, set_font		)(xui_family("Arial", 16, false));
	xui_method_ptrcall(textctrl, set_textcolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(textctrl, set_textalign	)(TA_LC);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_editctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_editctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_string>(this, &xui_propedit_string::on_textctrltextchanged);

	m_editctrl = textctrl;
}

/*
//value
*/
xui_method_explain(xui_propedit_string, get_value,				const std::wstring&	)( void ) const
{
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	return textctrl->get_text();
}
xui_method_explain(xui_propedit_string, set_value,				void				)( const std::wstring& value )
{
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	textctrl->ini_textbox(value);
}

/*
//override
*/
xui_method_explain(xui_propedit_string, reset,					void				)( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = (xui_textbox*)m_editctrl;
	textctrl->ini_textbox(L"");
}

/*
//event
*/
xui_method_explain(xui_propedit_string, on_textctrltextchanged, void				)( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}