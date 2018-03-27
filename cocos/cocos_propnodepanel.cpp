#include "ui/UILayout.h"
#include "cocostudio/CocosStudioExtension.h"
#include "2d/CCSpriteFrameCache.h"
#include "renderer/CCTextureCache.h"
#include "ui/UIScale9Sprite.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"

#include "xui_kindctrl.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_slider.h"
#include "xui_numbbox.h"
#include "cocos_propdata_image.h"
#include "cocos_snaptool.h"
#include "cocos_resource.h"
#include "cocos_propnodepanel.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodepanel)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_panel(dynamic_cast<cocos2d::ui::Layout*>(node))
{}

/*
//method
*/
xui_method_explain(cocos_propnodepanel, get_panel,		cocos2d::ui::Layout*)( void )
{
	return m_panel;
}
xui_method_explain(cocos_propnodepanel, add_clip,		void				)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "PanelClip", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodepanel>(this, &cocos_propnodepanel::on_propchanged);
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"ClipEnable",
		xui_propctrl_bool::create,
		was_clip,
		set_clip,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodepanel, add_background, void				)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "PanelBackground", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodepanel>(this, &cocos_propnodepanel::on_propchanged);
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_func(
		kind,
		L"ColourOpacity",
		xui_propctrl_slider::create,
		get_backalpha,
		set_backalpha,
		this,
		NT_UNSIGNEDINT,
		1.0,
		0.0,
		100.0));
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"BackColour",
		xui_propctrl_colour::create,
		get_backcolor,
		set_backcolor,
		this));
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"StartColour",
		xui_propctrl_colour::create,
		get_startcolor,
		set_startcolor,
		this));
	subprop.push_back(new xui_propdata_colour(
		kind,
		L"EndColour",
		xui_propctrl_colour::create,
		get_endcolor,
		set_endcolor,
		this));
	subprop.push_back(new xui_propdata_number_func(
		kind,
		L"ColourDir",
		xui_propctrl_slider::create,
		get_colordir,
		set_colordir,
		this,
		NT_UNSIGNEDINT,
		1.0,
		0.0,
		360.0));

	xui_propenum_map				textmap;
	std::map<s32, std::vector<u32>> showmap;
	textmap[(s32)cocos2d::ui::Layout::BackGroundColorType::NONE]		= L"None";
	textmap[(s32)cocos2d::ui::Layout::BackGroundColorType::SOLID]		= L"Solid";
	textmap[(s32)cocos2d::ui::Layout::BackGroundColorType::GRADIENT]	= L"Gradient";
	showmap[1].push_back(0);
	showmap[1].push_back(1);
	showmap[2].push_back(0);
	showmap[2].push_back(2);
	showmap[2].push_back(3);
	showmap[2].push_back(4);
	kind->add_propdata(new xui_propdata_expand_enum_func(
		kind,
		L"ColorType",
		xui_propctrl_expand_enum::create,
		textmap,
		get_colortype,
		set_colortype,
		this,
		subprop,
		true,
		showmap));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"Background",
		get_background,
		set_background,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodepanel, def_size,		cocos_value_unitvec	)( void* userptr )
{
	cocos_value_unitvec value;

	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Node* node = prop->get_panel()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
	if (scale9)
	{
		cocos2d::Sprite* sprite = scale9->getSprite();
		if (sprite)
		{
			cocos2d::Rect rect = sprite->getTextureRect();
			value.w.pix = rect.size.width;
			value.h.pix = rect.size.height;
		}
	}

	return value;
}
xui_method_explain(cocos_propnodepanel, get_scale9,		cocos_value_scale9	)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Node* node = prop->get_panel()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
	cocos_value_scale9 value;
	if (scale9)
	{
		value.flag    = scale9->isScale9Enabled();
		value.rect.ax = scale9->getInsetLeft();
		value.rect.bx = scale9->getInsetRight();
		value.rect.ay = scale9->getInsetTop();
		value.rect.by = scale9->getInsetBottom();

		cocos2d::Sprite* sprite = scale9->getSprite();
		if (sprite)
		{
			cocos2d::Rect rect = sprite->getTextureRect();
			value.size.w = rect.size.width;
			value.size.h = rect.size.height;
		}
	}

	return value;
}
xui_method_explain(cocos_propnodepanel, set_scale9,		void				)( void* userptr, const cocos_value_scale9& value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	prop->get_panel()->setBackGroundImageScale9Enabled(value.flag);
	cocos2d::Rect rect;
	rect.origin.x	 = value.rect.ax;
	rect.origin.y	 = value.rect.ay;
	rect.size.width  = value.size.w - value.rect.ax - value.rect.bx;
	rect.size.height = value.size.h - value.rect.ay - value.rect.by;
	prop->get_panel()->setBackGroundImageCapInsets(rect);
}
xui_method_explain(cocos_propnodepanel, was_clip,		bool				)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	return prop->get_panel()->isClippingEnabled();
}
xui_method_explain(cocos_propnodepanel, set_clip,		void				)( void* userptr, bool  value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	prop->get_panel()->setClippingEnabled(value);
}
xui_method_explain(cocos_propnodepanel, get_colortype,	s32					)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	return (s32)prop->get_panel()->getBackGroundColorType();
}
xui_method_explain(cocos_propnodepanel, set_colortype,	void				)( void* userptr, s32   value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	prop->get_panel()->setBackGroundColorType((cocos2d::ui::Layout::BackGroundColorType)value);
}
xui_method_explain(cocos_propnodepanel, get_backcolor,	xui_colour			)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Color3B color = prop->get_panel()->getBackGroundColor();
	return xui_colour(1.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodepanel, set_backcolor,	void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	prop->get_panel()->setBackGroundColor(cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f));
}
xui_method_explain(cocos_propnodepanel, get_backalpha,	f64					)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	return prop->get_panel()->getBackGroundColorOpacity()/255.0f*100.0;
}
xui_method_explain(cocos_propnodepanel, set_backalpha,	void				)( void* userptr, f64   value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	prop->get_panel()->setBackGroundColorOpacity(value/100.0*255.0f);
}
xui_method_explain(cocos_propnodepanel, get_startcolor, xui_colour			)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Color3B color = prop->get_panel()->getBackGroundStartColor();
	return xui_colour(1.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodepanel, set_startcolor, void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Color3B color = prop->get_panel()->getBackGroundEndColor();
	prop->get_panel()->setBackGroundColor(cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f), color);
}
xui_method_explain(cocos_propnodepanel, get_endcolor,	xui_colour			)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Color3B color = prop->get_panel()->getBackGroundEndColor();
	return xui_colour(1.0f, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
}
xui_method_explain(cocos_propnodepanel, set_endcolor,	void				)( void* userptr, const xui_colour& value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Color3B color = prop->get_panel()->getBackGroundStartColor();
	prop->get_panel()->setBackGroundColor(color, cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f));
}
xui_method_explain(cocos_propnodepanel, get_colordir,	f64					)( void* userptr )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Vec2 vec = prop->get_panel()->getBackGroundColorVector();
	return xui_round(CC_RADIANS_TO_DEGREES(acos(vec.x)));
}
xui_method_explain(cocos_propnodepanel, set_colordir,	void				)( void* userptr, f64   value )
{
	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	cocos2d::Vec2 vec;
	vec.x = cos(CC_DEGREES_TO_RADIANS(value));
	vec.y = sin(CC_DEGREES_TO_RADIANS(value));
	prop->get_panel()->setBackGroundColorVector(vec);
}
xui_method_explain(cocos_propnodepanel, get_background, void*				)( void* userptr )
{
	cocos_propnodepanel*  prop = (cocos_propnodepanel*)userptr;
	cocos2d::ResourceData data = prop->get_panel()->getRenderFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodepanel, set_background, void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodepanel* prop = (cocos_propnodepanel*)userptr;
	if		(texture)	prop->get_panel()->setBackGroundImage(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_panel()->setBackGroundImage(frame->getName(),			cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_panel()->removeBackGroundImage();
}

//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propnodelayout)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodepanel(file, node)
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
	add_scale9		(get_scale9, set_scale9);
	add_special		();
	add_clip		();
	add_background	();
	add_advance		();
	add_callback	();
	add_userdata	();
}