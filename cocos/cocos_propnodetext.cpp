#include "ui/UIText.h"

#include "xui_numbbox.h"
#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_slider.h"
#include "cocos_propctrl_fontsize.h"
#include "cocos_resource.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propttf.h"
#include "cocos_propdata_ttf.h"
#include "cocos_propnodetext.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodetext)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_text(dynamic_cast<cocos2d::ui::Text*>(node))
{
	add_location	();
	add_position	();
	add_size		(false, NULL);
	add_aligndock	(false);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_special		();
	add_text		();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodetext, get_text,			cocos2d::ui::Text*	)( void )
{
	return m_text;
}
xui_method_explain(cocos_propnodetext, get_fontfile,		cocos_propfile*		)( void )
{
	if (m_fontfile == NULL && m_text->getFontName().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_text->getFontName());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_fontfile = project->get_pathfile(path, file);
	}

	return m_fontfile;
}
xui_method_explain(cocos_propnodetext, set_fontfile,		void				)( cocos_propfile* file )
{
	m_fontfile = file;
	if (m_fontfile)
	{
		m_text->setFontName(xui_global::unicode_to_ascii(m_fontfile->get_fullname()));
	}
	else
	{
		m_text->setFontName("");
	}
}
xui_method_explain(cocos_propnodetext, add_text,			void				)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "Text", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodetext>(this, &cocos_propnodetext::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"Text",
		xui_propctrl_string::create,
		get_string,
		set_string,
		this));

	xui_propenum_map textmap;
	textmap[(s32)cocos2d::TextHAlignment::LEFT]		= L"Left";
	textmap[(s32)cocos2d::TextHAlignment::CENTER]	= L"Center";
	textmap[(s32)cocos2d::TextHAlignment::RIGHT]	= L"Right";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"HorzAlign",
		xui_propctrl_enum::create,
		textmap,
		get_horzalign,
		set_horzalign,
		this));
	textmap.clear();
	textmap[(s32)cocos2d::TextVAlignment::TOP]		= L"Top";
	textmap[(s32)cocos2d::TextVAlignment::CENTER]	= L"Center";
	textmap[(s32)cocos2d::TextVAlignment::BOTTOM]	= L"Bottom";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"VertAlign",
		xui_propctrl_enum::create,
		textmap,
		get_vertalign,
		set_vertalign,
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

	xui_propdata_vec				subprop;
	std::map<s32, std::vector<u32>> showmap;
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"ShadowColor",
		xui_propctrl_colour::create,
		get_shadowcolor,
		set_shadowcolor,
		this));
	subprop.push_back(new xui_propdata_vector(
		kind,
		L"ShadowOffset",
		xui_propctrl_vector::create,
		get_shadowoffset,
		set_shadowoffset,
		this,
		NT_INT));
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	kind->add_propdata(new xui_propdata_expand_bool(
		kind,
		L"EnableShadow",
		xui_propctrl_expand_bool::create,
		subprop,
		was_enableshadow,
		set_enableshadow,
		this,
		true,
		showmap));

	subprop.clear();
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"OutlineColor",
		xui_propctrl_colour::create,
		get_outlinecolor,
		set_outlinecolor,
		this));
	subprop.push_back(new xui_propdata_number_func(
		kind,
		L"OutlineWidth",
		xui_propctrl_slider::create,
		get_outlinewidth,
		set_outlinewidth,
		this,
		NT_UNSIGNEDINT,
		1.0,
		1.0,
		5.0));
	kind->add_propdata(new xui_propdata_expand_bool(
		kind,
		L"EnableOutline",
		xui_propctrl_expand_bool::create,
		subprop,
		was_enableoutline,
		set_enableoutline,
		this,
		true,
		showmap));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodetext, get_string,			std::wstring		)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return xui_global::ascii_to_unicode(prop->get_text()->getString());
}
xui_method_explain(cocos_propnodetext, set_string,			void				)( void* userptr, const std::wstring& value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->get_text()->setString(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodetext, get_horzalign,		s32					)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return (s32)prop->get_text()->getTextHorizontalAlignment();
}
xui_method_explain(cocos_propnodetext, set_horzalign,		void				)( void* userptr, s32   value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->get_text()->setTextHorizontalAlignment((cocos2d::TextHAlignment)value);
}
xui_method_explain(cocos_propnodetext, get_vertalign,		s32					)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return (s32)prop->get_text()->getTextVerticalAlignment();
}
xui_method_explain(cocos_propnodetext, set_vertalign,		void				)( void* userptr, s32   value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->get_text()->setTextVerticalAlignment((cocos2d::TextVAlignment)value);
}
xui_method_explain(cocos_propnodetext, get_font,			void*				)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return prop->get_fontfile();
}
xui_method_explain(cocos_propnodetext, set_font,			void				)( void* userptr, void* value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->set_fontfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodetext, get_fontsize,		f64					)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return (f64)prop->get_text()->getFontSize();
}
xui_method_explain(cocos_propnodetext, set_fontsize,		void				)( void* userptr, f64   value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->get_text()->setFontSize(value);
}
xui_method_explain(cocos_propnodetext, get_fontdraw,		xui_colour			)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Color3B color = prop->get_text()->getColor();
	return xui_colour(1.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodetext, set_fontdraw,		void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	prop->get_text()->setColor(cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f));
}
xui_method_explain(cocos_propnodetext, was_enableshadow,	bool				)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return prop->get_text()->isShadowEnabled();
}
xui_method_explain(cocos_propnodetext, set_enableshadow,	void				)( void* userptr, bool  value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	if (value)
	{
		prop->get_text()->enableShadow();
	}
	else
	{
		prop->get_text()->disableEffect(cocos2d::LabelEffect::SHADOW);
	}
}
xui_method_explain(cocos_propnodetext, get_shadowoffset,	xui_vector<f64>		)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Size offset = prop->get_text()->getShadowOffset();
	return xui_vector<f64>(offset.width, offset.height);
}
xui_method_explain(cocos_propnodetext, set_shadowoffset,	void				)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Color4B color = prop->get_text()->getShadowColor();
	prop->get_text()->enableShadow(color, cocos2d::Size(value.x, value.y));
}
xui_method_explain(cocos_propnodetext, get_shadowcolor,		xui_colour			)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Color4B color = prop->get_text()->getShadowColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodetext, set_shadowcolor,		void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Size offset = prop->get_text()->getShadowOffset();
	cocos2d::Color4B color(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f);
	prop->get_text()->enableShadow(color, offset);
}
xui_method_explain(cocos_propnodetext, was_enableoutline,	bool				)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return prop->get_text()->getOutlineSize() > 0;
}
xui_method_explain(cocos_propnodetext, set_enableoutline,	void				)( void* userptr, bool  value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	if (value)
	{
		prop->get_text()->enableOutline(cocos2d::Color4B::BLACK);
	}
	else
	{
		prop->get_text()->disableEffect(cocos2d::LabelEffect::OUTLINE);
	}
}
xui_method_explain(cocos_propnodetext, get_outlinewidth,	f64					)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	return prop->get_text()->getOutlineSize();
}
xui_method_explain(cocos_propnodetext, set_outlinewidth,	void				)( void* userptr, f64   value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Color4B color = prop->get_text()->getEffectColor();
	prop->get_text()->enableOutline(color, value);
}
xui_method_explain(cocos_propnodetext, get_outlinecolor,	xui_colour			)( void* userptr )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	cocos2d::Color4B color = prop->get_text()->getEffectColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodetext, set_outlinecolor,	void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodetext* prop = (cocos_propnodetext*)userptr;
	int width = prop->get_text()->getOutlineSize();
	cocos2d::Color4B color(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f);
	prop->get_text()->enableOutline(color, width);
}