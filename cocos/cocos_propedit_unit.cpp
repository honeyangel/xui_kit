#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_numbbox.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_propctrl.h"
#include "xui_desktop.h"
#include "cocos_propedit_unit.h"

/*
//constructor
*/
xui_create_explain(cocos_propedit_unit)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	m_namectrl->set_cursor(CURSOR_WE);
	xui_method_ptrcall(m_namectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse, cocos_propedit_unit>(this, &cocos_propedit_unit::on_namectrlmousemove);

	xui_textbox* textctrl = new xui_numbbox(xui_vector<s32>(48, 18), NT_FLOAT, 1, false);
	xui_method_ptrcall(textctrl,	set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(textctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(textctrl,	set_borderrt	)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(textctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(textctrl,	xm_nonfocus		) += new xui_method_member<xui_method_args,  cocos_propedit_unit>(this, &cocos_propedit_unit::on_editctrlnonfocus);
	xui_method_ptrcall(textctrl,	xm_getfocus		) += new xui_method_member<xui_method_args,  cocos_propedit_unit>(this, &cocos_propedit_unit::on_editctrlgetfocus);
	xui_method_ptrcall(textctrl,	xm_textchanged	) += new xui_method_member<xui_method_args,  cocos_propedit_unit>(this, &cocos_propedit_unit::on_textctrltextchanged);

	m_editctrl = textctrl;

	xui_menu* menu = xui_menu::create(80);
	m_pixcheck	= menu->add_item(NULL, L"Pixel");
	m_percheck	= menu->add_item(NULL, L"Percent By Parent");
	xui_method_ptrcall(m_pixcheck,	xm_click		) += new xui_method_member<xui_method_args,  cocos_propedit_unit>(this, &cocos_propedit_unit::on_menuitemclick);
	xui_method_ptrcall(m_percheck,	xm_click		) += new xui_method_member<xui_method_args,  cocos_propedit_unit>(this, &cocos_propedit_unit::on_menuitemclick);

	m_menuctrl = new xui_toggle(xui_vector<s32>(40, 18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_menuctrl,	set_borderrt	)(xui_rect2d<s32>(0, 2, 4, 2));
	xui_method_ptrcall(m_menuctrl,	set_menu		)(menu);
	xui_method_ptrcall(m_menuctrl,	xm_getfocus		) += new xui_method_member<xui_method_args,	 cocos_propedit_unit>(this, &cocos_propedit_unit::on_menuctrlgetfocus);
}

/*
//method
*/
xui_method_explain(cocos_propedit_unit, get_value,				cocos_value_unit)( void )
{
	cocos_value_unit value;
	value.useper	= m_percheck->get_flag();
	value.per		= get_textnumb(!value.useper);
	value.pix		= get_textnumb( value.useper);

	return value;
}
xui_method_explain(cocos_propedit_unit, set_value,				void			)( const cocos_value_unit& value )
{
	set_textnumb(value.per, !value.useper);
	set_textnumb(value.pix,  value.useper);

	xui_method_ptrcall(m_menuctrl,	ini_drawer	)( value.useper ? L"%" : L"Pix");
	xui_method_ptrcall(m_percheck,	set_flag	)( value.useper);
	xui_method_ptrcall(m_pixcheck,	set_flag	)(!value.useper);
}
xui_method_explain(cocos_propedit_unit, get_menuctrl,			xui_toggle*		)( void )
{
	return m_menuctrl;
}
xui_method_explain(cocos_propedit_unit,	get_textnumb,			f64				)( bool byname )
{
	f64 value = 0.0;
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	std::wstring text = byname ? xui_global::ascii_to_unicode(textctrl->get_name()) : textctrl->get_text();
	std::wstringstream tmp(text.c_str());
	tmp >> value;
	return value;
}
xui_method_explain(cocos_propedit_unit, set_textnumb,			void			)( f64 value, bool toname )
{
	char temp[32];
	sprintf(temp, "%.2f", (f32)value);

	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	if (toname) xui_method_ptrcall(textctrl, set_name	)(std::string(temp));
	else		xui_method_ptrcall(textctrl, ini_textbox)(xui_global::ascii_to_unicode(temp));
}

/*
//override
*/
xui_method_explain(cocos_propedit_unit, reset,					void			)( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	xui_method_ptrcall(textctrl,	ini_textbox	)(L"");
	xui_method_ptrcall(m_menuctrl,	ini_drawer	)(L"Pix");
	xui_method_ptrcall(m_pixcheck,	set_flag	)(true);
	xui_method_ptrcall(m_percheck,	set_flag	)(true);
}

/*
//event
*/
xui_method_explain(cocos_propedit_unit, on_textctrltextchanged, void			)( xui_component* sender, xui_method_args&  args )
{
	f64 value = get_textnumb(false);
	if (value < 0.0 && m_percheck->get_flag())
		set_textnumb(0.0, false);

	m_propctrl->on_editvalue(this);
}
//xui_method_explain(cocos_propedit_unit, on_textctrlmousewheel,	void			)( xui_component* sender, xui_method_mouse& args )
//{
//	f64 value = get_textnumb(false) + (args.wheel > 0 ? 1 : -1);
//	if (value < 0.0 && m_percheck->get_flag())
//		value = 0.0;
//
//	set_textnumb(value, false);
//	m_propctrl->on_editvalue(this);
//
//	args.handle = true;
//}
xui_method_explain(cocos_propedit_unit, on_namectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		s32 delta = args.point.x - xui_desktop::get_ins()->get_mouselast().x;
		f64 value = get_textnumb(false) + (f64)delta;
		if (value < 0.0 && m_percheck->get_flag())
			value = 0.0;

		set_textnumb(value, false);
		m_propctrl->on_editvalue(this);
	}
}
xui_method_explain(cocos_propedit_unit, on_menuitemclick,		void			)( xui_component* sender, xui_method_args&  args )
{
	m_propctrl->on_linkpropdata();

	cocos_value_unit value = get_value();
	value.useper = (sender == m_percheck);
	set_value(value);
	m_propctrl->on_editvalue(this);
}
xui_method_explain(cocos_propedit_unit, on_menuctrlgetfocus,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_component* last = (xui_component*)args.wparam;
	if (last == NULL || last->get_ancestor(xui_propctrl::RTTIPTR()) != m_propctrl)
		m_propctrl->on_readyundo();
}