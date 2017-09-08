#include "ui/UITextField.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_numbbox.h"
#include "cocos_propttf.h"
#include "cocos_propdata_ttf.h"
#include "cocos_propctrl_fontsize.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_resource.h"
#include "cocos_propnodetextfield.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodetextfield)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_textfield(dynamic_cast<cocos2d::ui::TextField*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, def_size);
	add_aligndock	(true);
	add_common		();
	add_touch		();
	add_base		();
	add_color		();
	add_special		();
	add_textfield	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodetextfield, get_textfield,	cocos2d::ui::TextField*	)( void )
{
	return m_textfield;
}
xui_method_explain(cocos_propnodetextfield, get_fontfile,	cocos_propfile*			)( void )
{
	if (m_fontfile == NULL && m_textfield->getFontName().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_textfield->getFontName());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length()-1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_fontfile = project->get_pathfile(path, file);
	}

	return m_fontfile;
}
xui_method_explain(cocos_propnodetextfield, set_fontfile,	void					)( cocos_propfile* file )
{
	m_fontfile = file;
	if (m_fontfile)
	{
		m_textfield->setFontName(xui_global::unicode_to_ascii(m_fontfile->get_fullname()));
	}
	else
	{
		m_textfield->setFontName("");
	}
}
xui_method_explain(cocos_propnodetextfield, add_textfield,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "TextField", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodetextfield>(this, &cocos_propnodetextfield::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"PlaceHold",
		xui_propctrl_string::create,
		get_placehold,
		set_placehold,
		this));
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"Text",
		xui_propctrl_string::create,
		get_string,
		set_string,
		this));
	kind->add_propdata(new cocos_propdata_ttf(
		kind,
		L"Font",
		get_font,
		set_font,
		this));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"FontSize",
		cocos_propctrl_fontsize::create,
		get_fontsize,
		set_fontsize,
		this,
		NT_UNSIGNEDINT));
	kind->add_propdata(new xui_propdata_colour(
		kind,
		L"FontColor",
		xui_propctrl_colour::create,
		get_fontdraw,
		set_fontdraw,
		this));
	kind->add_propdata(new cocos_propdata_bs(
		kind,
		L"TextMaxLength",
		get_textlength,
		set_textlength,
		this,
		NT_UNSIGNEDINT,
		1.0));
	kind->add_propdata(new cocos_propdata_bs(
		kind,
		L"Password",
		get_password,
		set_password,
		this,
		NT_NONE,
		1.0));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodetextfield, def_size,		cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec value;
	value.w.pix = 100;
	value.h.pix = 20;
	return value;
}
xui_method_explain(cocos_propnodetextfield, get_placehold,	std::wstring			)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	return xui_global::ascii_to_unicode(prop->get_textfield()->getPlaceHolder());
}
xui_method_explain(cocos_propnodetextfield, set_placehold,	void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->get_textfield()->setPlaceHolder(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodetextfield, get_string,		std::wstring			)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	return xui_global::ascii_to_unicode(prop->get_textfield()->getString());
}
xui_method_explain(cocos_propnodetextfield, set_string,		void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->get_textfield()->setString(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodetextfield, get_font,		void*					)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	return prop->get_fontfile();
}
xui_method_explain(cocos_propnodetextfield, set_font,		void					)( void* userptr, void* value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->set_fontfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodetextfield, get_fontsize,	f64						)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	return prop->get_textfield()->getFontSize();
}
xui_method_explain(cocos_propnodetextfield, set_fontsize,	void					)( void* userptr, f64 value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->get_textfield()->setFontSize(value);
}
xui_method_explain(cocos_propnodetextfield, get_fontdraw,	xui_colour				)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	cocos2d::Color4B color = prop->get_textfield()->getTextColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodetextfield, set_fontdraw,	void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->get_textfield()->setTextColor(cocos2d::Color4B(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f));
}
xui_method_explain(cocos_propnodetextfield, get_textlength,	cocos_value_bs			)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	std::wstringstream text;
	text << prop->get_textfield()->getMaxLength();
	cocos_value_bs value;
	value.flag = prop->get_textfield()->isMaxLengthEnabled();
	value.text = text.str();
	return value;
}
xui_method_explain(cocos_propnodetextfield, set_textlength, void					)( void* userptr, const cocos_value_bs& value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	prop->get_textfield()->setMaxLengthEnabled(value.flag);
	std::wstringstream text(value.text.c_str());
	s32 length = 0;
	text >> length;
	prop->get_textfield()->setMaxLength(length);
}
xui_method_explain(cocos_propnodetextfield, get_password,	cocos_value_bs			)( void* userptr )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	cocos_value_bs value;
	value.flag = prop->get_textfield()->isPasswordEnabled();
	value.text = xui_global::ascii_to_unicode(prop->get_textfield()->getPasswordStyleText());
	return value;
}
xui_method_explain(cocos_propnodetextfield, set_password,	void					)( void* userptr, const cocos_value_bs& value )
{
	cocos_propnodetextfield* prop = (cocos_propnodetextfield*)userptr;
	std::string text = xui_global::unicode_to_ascii(value.text);
	if (text.length() > 1)
		text = text.substr(0, 1);

	prop->get_textfield()->setPasswordEnabled(value.flag);
	prop->get_textfield()->setPasswordStyleText(text.c_str());
}