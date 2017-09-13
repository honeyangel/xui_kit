#include "2d/WeCLabel.h"
#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_vector.h"
#include "cocos_propttf.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propdata_ttf.h"
#include "cocos_filedata.h"
#include "cocos_propctrl_fontsize.h"
#include "cocos_propnodeweclabel.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeweclabel)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_fontfile(NULL)
, m_weclabel(dynamic_cast<cocos2d::ui::WeCLabel*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, NULL);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
	add_custom		();
	add_weclabel	();
}

/*
//method
*/
xui_method_explain(cocos_propnodeweclabel, get_weclabel,		cocos2d::ui::WeCLabel*	)( void )
{
	return m_weclabel;
}
xui_method_explain(cocos_propnodeweclabel, add_weclabel,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "WeCLabel", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeweclabel>(this, &cocos_propnodeweclabel::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"Text",
		xui_propctrl_string::create,
		get_text,
		set_text,
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
	xui_propenum_map textmap;
	textmap[(s32)cocos2d::ui::WeCLabel::HorAlign::Left]		= L"Left";
	textmap[(s32)cocos2d::ui::WeCLabel::HorAlign::Center]	= L"Center";
	textmap[(s32)cocos2d::ui::WeCLabel::HorAlign::Right]	= L"Right";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"HorzAlign",
		xui_propctrl_enum::create,
		textmap,
		get_textalignhorz,
		set_textalignhorz,
		this));
	textmap.clear();
	textmap[(s32)cocos2d::ui::WeCLabel::VerAlign::Top]		= L"Top";
	textmap[(s32)cocos2d::ui::WeCLabel::VerAlign::Center]	= L"Center";
	textmap[(s32)cocos2d::ui::WeCLabel::VerAlign::Bottom]	= L"Bottom";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"VertAlign",
		xui_propctrl_enum::create,
		textmap,
		get_textalignvert,
		set_textalignvert,
		this));
	textmap.clear();
	textmap[(s32)cocos2d::ui::WeCLabel::Overflow::Clamp]		= L"Clamp";
	textmap[(s32)cocos2d::ui::WeCLabel::Overflow::ResizeFreely] = L"ResizeFreely";
	textmap[(s32)cocos2d::ui::WeCLabel::Overflow::ResizeHeight] = L"ResizeHeight";
	textmap[(s32)cocos2d::ui::WeCLabel::Overflow::Shrink]		= L"Shrink";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"Overflow",
		xui_propctrl_enum::create,
		textmap,
		get_overflow,
		set_overflow,
		this));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"CharSpace",
		xui_propctrl_number::create,
		get_chargrap,
		set_chargrap,
		this,
		NT_INT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"LineSpace",
		xui_propctrl_number::create,
		get_linegrap,
		set_linegrap,
		this,
		NT_INT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"MaxLine",
		xui_propctrl_number::create,
		get_maxline,
		set_maxline,
		this,
		NT_UNSIGNEDINT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Level",
		xui_propctrl_number::create,
		get_globalzorder,
		set_globalzorder,
		this,
		NT_UNSIGNEDINT));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Enable BBCode",
		xui_propctrl_bool::create,
		was_bbcode,
		set_bbcode,
		this));

	xui_propdata_vec					subprop;
	std::map<s32, std::vector<u32> >	showmap;
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"Effect Color",
		xui_propctrl_colour::create,
		get_effectcolor,
		set_effectcolor,
		this));
	subprop.push_back(new xui_propdata_number_func(
		kind,
		L"Outline Width",
		xui_propctrl_number::create,
		get_outlinewidth,
		set_outlinewidth,
		this,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_vector(
		kind,
		L"Shadow Offset",
		xui_propctrl_vector::create,
		get_shadowoffset,
		set_shadowoffset,
		this,
		NT_INT));
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	showmap[2].push_back(0);
	showmap[2].push_back(2);

	textmap.clear();
	textmap[(s32)cocos2d::ui::WeCLabel::Effect::None]		= L"None";
	textmap[(s32)cocos2d::ui::WeCLabel::Effect::Outline]	= L"Outline";
	textmap[(s32)cocos2d::ui::WeCLabel::Effect::Shadow]		= L"Shadow";
	kind->add_propdata(new xui_propdata_expand_enum_func(
		kind,
		L"Effect",
		xui_propctrl_expand_enum::create,
		textmap,
		get_effect,
		set_effect,
		this,
		subprop,
		true,
		showmap));

	subprop.clear();
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"Top",
		xui_propctrl_colour::create,
		get_gradienttop,
		set_gradienttop,
		this));
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"Bottom",
		xui_propctrl_colour::create,
		get_gradientbottom,
		set_gradientbottom,
		this));
	showmap.clear();
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	kind->add_propdata(new xui_propdata_expand_bool(
		kind,
		L"Enable Gradient",
		xui_propctrl_expand_bool::create,
		subprop,
		was_gradient,
		set_gradient,
		this,
		true,
		showmap));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodeweclabel, get_fontfile,		cocos_propfile*			)( void )
{
	if (m_fontfile == NULL && m_weclabel->getFontName().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_weclabel->getFontName());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_fontfile = project->get_pathfile(path, file);
	}

	return m_fontfile;
}
xui_method_explain(cocos_propnodeweclabel, set_fontfile,		void					)( cocos_propfile* file )
{
	m_fontfile = file;
	if (m_fontfile)
	{
		m_weclabel->setFontName(xui_global::unicode_to_ascii(m_fontfile->get_fullname()));
	}
	else
	{
		m_weclabel->setFontName("");
	}
}

/*
//static
*/
xui_method_explain(cocos_propnodeweclabel, get_text,			std::wstring			)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return xui_global::ascii_to_unicode(prop->get_weclabel()->getText());
}
xui_method_explain(cocos_propnodeweclabel, set_text,			void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setText(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodeweclabel, get_font,			void*					)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_fontfile();
}
xui_method_explain(cocos_propnodeweclabel, set_font,			void					)( void* userptr, void* value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->set_fontfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodeweclabel, get_fontsize,		f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getFontSize();
}
xui_method_explain(cocos_propnodeweclabel, set_fontsize,		void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setFontSize(value);
}
xui_method_explain(cocos_propnodeweclabel, get_fontdraw,		xui_colour				)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	cocos2d::Color4B color = prop->get_weclabel()->getFontColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodeweclabel, set_fontdraw,		void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setFontColor(cocos2d::Color4B(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f));
}
xui_method_explain(cocos_propnodeweclabel, get_textalignhorz,	s32						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return (s32)prop->get_weclabel()->getHorAlign();
}
xui_method_explain(cocos_propnodeweclabel, set_textalignhorz,	void					)( void* userptr, s32   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setHorAlign((cocos2d::ui::WeCLabel::HorAlign)value);
}
xui_method_explain(cocos_propnodeweclabel, get_textalignvert,	s32						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return (s32)prop->get_weclabel()->getVerAlign();
}
xui_method_explain(cocos_propnodeweclabel, set_textalignvert,	void					)( void* userptr, s32   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setVerAlign((cocos2d::ui::WeCLabel::VerAlign)value);
}
xui_method_explain(cocos_propnodeweclabel, get_overflow,		s32						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return (s32)prop->get_weclabel()->getOverflow();
}
xui_method_explain(cocos_propnodeweclabel, set_overflow,		void					)( void* userptr, s32   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setOverflow((cocos2d::ui::WeCLabel::Overflow)value);
}
xui_method_explain(cocos_propnodeweclabel, get_chargrap,		f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getSpacingX();
}
xui_method_explain(cocos_propnodeweclabel, set_chargrap,		void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setSpacingX(value);
}
xui_method_explain(cocos_propnodeweclabel, get_linegrap,		f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getSpacingY();
}
xui_method_explain(cocos_propnodeweclabel, set_linegrap,		void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setSpacingY(value);
}
xui_method_explain(cocos_propnodeweclabel, get_maxline,			f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getMaxLines();
}
xui_method_explain(cocos_propnodeweclabel, set_maxline,			void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setMaxLines(value);
}
xui_method_explain(cocos_propnodeweclabel, get_globalzorder,	f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getLevel();
}
xui_method_explain(cocos_propnodeweclabel, set_globalzorder,	void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setLevel(value);
}
xui_method_explain(cocos_propnodeweclabel, was_bbcode,			bool					)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getBbcode();
}
xui_method_explain(cocos_propnodeweclabel, set_bbcode,			void					)( void* userptr, bool  value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setBbcode(value);
}
xui_method_explain(cocos_propnodeweclabel, get_effect,			s32						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return (s32)prop->get_weclabel()->getEffect();
}
xui_method_explain(cocos_propnodeweclabel, set_effect,			void					)( void* userptr, s32   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setEffect((cocos2d::ui::WeCLabel::Effect)value);
}
xui_method_explain(cocos_propnodeweclabel, get_effectcolor,		xui_colour				)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	cocos2d::Color4B color = prop->get_weclabel()->getEffectColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodeweclabel, set_effectcolor,		void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setEffectColor(cocos2d::Color4B(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f));
}
xui_method_explain(cocos_propnodeweclabel, get_outlinewidth,	f64						)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getStrokerOutline();
}
xui_method_explain(cocos_propnodeweclabel, set_outlinewidth,	void					)( void* userptr, f64   value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setStrokerOutline(value);
}
xui_method_explain(cocos_propnodeweclabel, get_shadowoffset,	xui_vector<f64>			)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	f64 x = prop->get_weclabel()->getShadowOffsetX();
	f64 y = prop->get_weclabel()->getShadowOffsetY();
	return xui_vector<f64>(x, y);
}
xui_method_explain(cocos_propnodeweclabel, set_shadowoffset,	void					)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setShadowOffsetX(value.x);
	prop->get_weclabel()->setShadowOffsetY(value.y);
}
xui_method_explain(cocos_propnodeweclabel, was_gradient,		bool					)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	return prop->get_weclabel()->getGradientEnabled();
}
xui_method_explain(cocos_propnodeweclabel, set_gradient,		void					)( void* userptr, bool  value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setGradientEnabled(value);
}
xui_method_explain(cocos_propnodeweclabel, get_gradienttop,		xui_colour				)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	cocos2d::Color4B color = prop->get_weclabel()->getGradientTopColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodeweclabel, set_gradienttop,		void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setGradientTopColor(cocos2d::Color4B(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f));
}
xui_method_explain(cocos_propnodeweclabel, get_gradientbottom,	xui_colour				)( void* userptr )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	cocos2d::Color4B color = prop->get_weclabel()->getGradientBottomColor();
	return xui_colour(color.a/255.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodeweclabel, set_gradientbottom,	void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodeweclabel* prop = (cocos_propnodeweclabel*)userptr;
	prop->get_weclabel()->setGradientBottomColor(cocos2d::Color4B(value.r*255.0f, value.g*255.0f, value.b*255.0f, value.a*255.0f));
}