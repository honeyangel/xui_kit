#include "ui/UISlider.h"
#include "ui/UIScale9Sprite.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCSpriteFrameCache.h"
#include "renderer/CCTextureCache.h"
#include "cocostudio/CocosStudioExtension.h"

#include "xui_kindctrl.h"
#include "xui_numbbox.h"
#include "xui_propctrl_slider.h"
#include "cocos_propdata_image.h"
#include "cocos_resource.h"
#include "cocos_propnodeslider.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeslider)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_slider(dynamic_cast<cocos2d::ui::Slider*>(node))
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
	add_special		();
	add_slider		();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodeslider, get_slider,	cocos2d::ui::Slider*)( void )
{
	return m_slider;
}
xui_method_explain(cocos_propnodeslider, add_slider,	void)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "Slider", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeslider>(this, &cocos_propnodeslider::on_propchanged);
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"Background",
		get_backimage,
		set_backimage,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"ProgressBar",
		get_foreimage,
		set_foreimage,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"BallNormal",
		get_normal,
		set_normal,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"BallPress",
		get_press,
		set_press,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"BallDisable",
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
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Percent",
		xui_propctrl_slider::create,
		get_percent,
		set_percent,
		this,
		NT_UNSIGNEDINT,
		1.0,
		1.0,
		100.0));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodeslider, def_size,		cocos_value_unitvec	)( void* userptr )
{
	cocos_value_unitvec value;

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::Node* node = prop->get_slider()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
	cocos2d::Sprite* sprite = scale9->getSprite();
	if (sprite)
	{
		cocos2d::Rect rect = sprite->getTextureRect();
		value.w.pix = rect.size.width;
		value.h.pix = rect.size.height;
	}

	return value;
}
xui_method_explain(cocos_propnodeslider, get_backimage, void*				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::ResourceData data = prop->get_slider()->getBackFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodeslider, set_backimage, void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	if		(texture)	prop->get_slider()->loadBarTexture(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_slider()->loadBarTexture(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_slider()->loadBarTexture("",						cocos2d::ui::Widget::TextureResType::LOCAL);

	set_size(userptr, def_size(userptr));
}
xui_method_explain(cocos_propnodeslider, get_foreimage, void*				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::ResourceData data = prop->get_slider()->getProgressBarFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodeslider, set_foreimage, void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	if		(texture)	prop->get_slider()->loadProgressBarTexture(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_slider()->loadProgressBarTexture(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_slider()->loadProgressBarTexture("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodeslider, get_normal,	void*				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::ResourceData data = prop->get_slider()->getBallNormalFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodeslider, set_normal,	void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	if		(texture)	prop->get_slider()->loadSlidBallTextureNormal(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_slider()->loadSlidBallTextureNormal(frame->getName(),			cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_slider()->loadSlidBallTextureNormal("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodeslider, get_press,		void*				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::ResourceData data = prop->get_slider()->getBallPressedFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodeslider, set_press,		void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	if		(texture)	prop->get_slider()->loadSlidBallTexturePressed(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_slider()->loadSlidBallTexturePressed(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_slider()->loadSlidBallTexturePressed("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodeslider, get_disable,	void*				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	cocos2d::ResourceData data = prop->get_slider()->getBallDisabledFile();
	if (data.file.length() > 0)
	{
		if (data.type == (int)cocos2d::ui::Widget::TextureResType::PLIST)
			return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(data.file, false);
		else
			return cocos2d::TextureCache::getInstance()->addImage(data.file);
	}

	return NULL;
}
xui_method_explain(cocos_propnodeslider, set_disable,	void				)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	if		(texture)	prop->get_slider()->loadSlidBallTextureDisabled(texture->getPath(),		cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_slider()->loadSlidBallTextureDisabled(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_slider()->loadSlidBallTextureDisabled("",						cocos2d::ui::Widget::TextureResType::LOCAL);
}
xui_method_explain(cocos_propnodeslider, was_enable,	bool				)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	return prop->get_slider()->isEnabled();
}
xui_method_explain(cocos_propnodeslider, set_enable,	void				)( void* userptr, bool  value )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	prop->get_slider()->setEnabled(value);
}
xui_method_explain(cocos_propnodeslider, get_percent,	f64					)( void* userptr )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	return prop->get_slider()->getPercent();
}
xui_method_explain(cocos_propnodeslider, set_percent,	void				)( void* userptr, f64   value )
{
	cocos_propnodeslider* prop = (cocos_propnodeslider*)userptr;
	prop->get_slider()->setPercent(value);
}