#include "xui_convas.h"
#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propctrl.h"
#include "xui_propedit_colour.h"

/*
//constructor
*/
xui_create_explain(xui_propedit_colour)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_textbox* textctrl = new xui_textbox(xui_vector<s32>(48, 18));
	xui_method_ptrcall(textctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(textctrl, set_drawcolor	)(true);
	xui_method_ptrcall(textctrl, set_borderrt	)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl, set_textalign	)(TEXTALIGN_LC);
	textctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlnonfocus);
	textctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlgetfocus);
	textctrl->xm_textenter	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_textctrltextenter);

	m_editctrl = textctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_colour, get_value,				xui_colour	)( void ) const
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);

	std::vector<std::string> vec;
	std::string temp = xui_global::unicode_to_ascii(textctrl->get_text());
	while (true)
	{
		s32 pos = temp.find(';');
		if (pos == -1)
			break;

		vec.push_back(temp.substr(0, pos));
		temp.erase(0, pos+1);
	}

	if (!temp.empty())
		vec.push_back(temp);

	xui_colour result(1.0f);
	if (vec.size() == 4)
	{
		result.a = (f32)atoi(vec[0].c_str()) / 255.0f;
		result.r = (f32)atoi(vec[1].c_str()) / 255.0f;
		result.g = (f32)atoi(vec[2].c_str()) / 255.0f;
		result.b = (f32)atoi(vec[3].c_str()) / 255.0f;
	}

	return result;
}
xui_method_explain(xui_propedit_colour, set_value,				void		)( const xui_colour& value )
{
	std::wstringstream tmp;
	tmp << value.a;
	tmp << L";";
	tmp << value.r;
	tmp << L";";
	tmp << value.g;
	tmp << L";";
	tmp << value.b;
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(tmp.str());
}
xui_method_explain(xui_propedit_colour, reset,					void		)( void )
{
	xui_propedit_base::reset();
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_editctrl);
	textctrl->ini_textbox(L"");
}

/*
//event
*/
xui_method_explain(xui_propedit_colour, on_textctrltextenter,	void		)( xui_component* sender, xui_method_args& args )
{
	m_propctrl->on_editvalue(this);
}