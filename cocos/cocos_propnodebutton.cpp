#include "ui/UIButton.h"
#include "ui/UIScale9Sprite.h"
#include "2d/CCSprite.h"

#include "xui_kindctrl.h"
#include "xui_global.h"
#include "xui_textbox.h"
#include "cocos_resource.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propttf.h"
#include "cocos_propdata_image.h"
#include "cocos_propcsd.h"
#include "cocos_propdata_ttf.h"
#include "cocos_propctrl_fontsize.h"
#include "cocos_propnodebutton.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodebutton)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_button(dynamic_cast<cocos2d::ui::Button*>(node))
, m_fontfile(NULL)
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, def_size);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_flip		();
	add_scale9		(get_scale9, set_scale9);
	add_special		();
	add_button		();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodebutton, get_button,	cocos2d::ui::Button*	)( void )
{
	return m_button;
}
xui_method_explain(cocos_propnodebutton, get_fontfile,	cocos_propfile*			)( void )
{
	if (m_fontfile == NULL && m_button->getTitleFontName().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_button->getTitleFontName());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length()-1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_fontfile = project->get_pathfile(path, file);
	}

	return m_fontfile;
}
xui_method_explain(cocos_propnodebutton, set_fontfile,	void					)( cocos_propfile* file )
{
	m_fontfile = file;
	if (m_fontfile)
	{
		m_button->setTitleFontName(xui_global::unicode_to_ascii(m_fontfile->get_fullname()));
	}
	else
	{
		m_button->setTitleFontName("");
	}
}
xui_method_explain(cocos_propnodebutton, add_button,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "Button", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebutton>(this, &cocos_propnodebutton::on_propchanged);
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"ImageNormal",
		get_normal,
		set_normal,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"ImagePress",
		get_press,
		set_press,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"ImageDisable",
		get_disable,
		set_disable,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Enable",
		xui_propctrl_bool::create,
		was_enable,
		set_enable,
		this));
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

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodebutton, def_size,		cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec value;
	
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::ui::Scale9Sprite* scale9 = prop->get_button()->getRendererNormal();
	cocos2d::Sprite* sprite = scale9->getSprite();
	if (sprite)
	{
		cocos2d::SpriteFrame* frame = sprite->getSpriteFrame(false);
		if (frame)
		{
			cocos2d::Size size = frame->getOriginalSize();
			value.w.pix = size.width;
			value.h.pix = size.height;
		}
	}
	
	return value;
}
xui_method_explain(cocos_propnodebutton, get_scale9,	cocos_value_scale9		)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::ui::Scale9Sprite* scale9 = prop->get_button()->getRendererNormal();
	cocos_value_scale9 value;
	value.flag		= scale9->isScale9Enabled();
	value.rect.ax	= scale9->getInsetLeft();
	value.rect.bx	= scale9->getInsetRight();
	value.rect.ay	= scale9->getInsetTop();
	value.rect.by	= scale9->getInsetBottom();

	cocos2d::Sprite* sprite = scale9->getSprite();
	if (sprite)
	{
		cocos2d::SpriteFrame* frame = sprite->getSpriteFrame(false);
		if (frame)
		{
			cocos2d::Size  size = frame->getOriginalSize();
			value.size.w = size.width;
			value.size.h = size.height;
		}
	}

	return value;
}
xui_method_explain(cocos_propnodebutton, set_scale9,	void					)( void* userptr, const cocos_value_scale9& value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	std::vector<cocos2d::ui::Scale9Sprite*> vec;
	vec.push_back(prop->get_button()->getRendererNormal());
	vec.push_back(prop->get_button()->getRendererClicked());
	vec.push_back(prop->get_button()->getRendererDisabled());
	for (u32 i = 0; i < vec.size(); ++i)
	{
		cocos2d::ui::Scale9Sprite* scale9 = vec[i];
		scale9->setScale9Enabled(value.flag);
		scale9->setInsetLeft(value.rect.ax);
		scale9->setInsetRight(value.rect.bx);
		scale9->setInsetTop(value.rect.ay);
		scale9->setInsetBottom(value.rect.by);
	}
}
xui_method_explain(cocos_propnodebutton, get_normal,	void*					)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::ui::Scale9Sprite* scale9 = prop->get_button()->getRendererNormal();
	return get_image(scale9->getSprite());
}
xui_method_explain(cocos_propnodebutton, set_normal,	void					)( void* userptr, void* value )
{
	cocos2d::Ref*			base = (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame = dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	if		(texture)	prop->get_button()->loadTextureNormal(texture->getFileName(),	cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_button()->loadTextureNormal(frame->getName(),			cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_button()->loadTextureNormal("",						cocos2d::ui::Widget::TextureResType::LOCAL);

	set_size(userptr, def_size(userptr));
}
xui_method_explain(cocos_propnodebutton, get_press,		void*					)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::ui::Scale9Sprite* scale9 = prop->get_button()->getRendererClicked();
	return get_image(scale9->getSprite());
}
xui_method_explain(cocos_propnodebutton, set_press,		void					)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	if		(texture)	prop->get_button()->loadTexturePressed(texture->getFileName(),	cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_button()->loadTexturePressed(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_button()->loadTexturePressed("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodebutton, get_disable,	void*					)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::ui::Scale9Sprite* scale9 = prop->get_button()->getRendererDisabled();
	return get_image(scale9->getSprite());
}
xui_method_explain(cocos_propnodebutton, set_disable,	void					)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	if		(texture)	prop->get_button()->loadTextureDisabled(texture->getFileName(), cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_button()->loadTextureDisabled(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_button()->loadTextureDisabled("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodebutton, was_enable,	bool					)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	return prop->get_button()->isEnabled();
}
xui_method_explain(cocos_propnodebutton, set_enable,	void					)( void* userptr, bool  value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	prop->get_button()->setEnabled(value);
}
xui_method_explain(cocos_propnodebutton, get_text,		std::wstring			)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	return xui_global::ascii_to_unicode(prop->get_button()->getTitleText());
}
xui_method_explain(cocos_propnodebutton, set_text,		void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	prop->get_button()->setTitleText(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodebutton, get_font,		void*					)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	return prop->get_fontfile();
}
xui_method_explain(cocos_propnodebutton, set_font,		void					)( void* userptr, void* value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	prop->set_fontfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodebutton, get_fontsize,	f64						)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	return prop->get_button()->getTitleFontSize();
}
xui_method_explain(cocos_propnodebutton, set_fontsize,	void					)( void* userptr, f64   value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	prop->get_button()->setTitleFontSize(value);
}
xui_method_explain(cocos_propnodebutton, get_fontdraw,	xui_colour				)( void* userptr )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	cocos2d::Color3B color = prop->get_button()->getTitleColor();
	return xui_colour(1.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodebutton, set_fontdraw,	void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodebutton* prop = (cocos_propnodebutton*)userptr;
	prop->get_button()->setTitleColor(cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f));
}