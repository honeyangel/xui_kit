#include "ui/UIImageView.h"
#include "2d/CCSprite.h"
#include "ui/UIScale9Sprite.h"

#include "xui_kindctrl.h"
#include "cocos_resource.h"
#include "cocos_propdata_image.h"
#include "cocos_propnodeimageview.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeimageview)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_imageview(dynamic_cast<cocos2d::ui::ImageView*>(node))
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
	add_background	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodeimageview, get_imageview,	cocos2d::ui::ImageView*	)( void )
{
	return m_imageview;
}
xui_method_explain(cocos_propnodeimageview, add_background, void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "ImageView", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeimageview>(this, &cocos_propnodeimageview::on_propchanged);
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
xui_method_explain(cocos_propnodeimageview, def_size,		cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec value;

	cocos_propnodeimageview* prop = (cocos_propnodeimageview*)userptr;
	cocos2d::Node* node = prop->get_imageview()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
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
xui_method_explain(cocos_propnodeimageview, get_background, void*					)( void* userptr )
{
	cocos_propnodeimageview* prop = (cocos_propnodeimageview*)userptr;
	cocos2d::Node* node = prop->get_imageview()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
	return get_image(scale9->getSprite());
}
xui_method_explain(cocos_propnodeimageview, set_background, void					)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeimageview* prop = (cocos_propnodeimageview*)userptr;
	if		(texture)	prop->get_imageview()->loadTexture(texture->getFileName(),	cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_imageview()->loadTexture(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_imageview()->loadTexture("",						cocos2d::ui::Widget::TextureResType::LOCAL);

	set_size(userptr, def_size(userptr));
}
xui_method_explain(cocos_propnodeimageview, get_scale9,		cocos_value_scale9		)( void* userptr )
{
	cocos_propnodeimageview* prop = (cocos_propnodeimageview*)userptr;
	cocos2d::Node* node = prop->get_imageview()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
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
xui_method_explain(cocos_propnodeimageview, set_scale9,		void					)( void* userptr, const cocos_value_scale9& value )
{
	cocos_propnodeimageview* prop = (cocos_propnodeimageview*)userptr;
	prop->get_imageview()->setScale9Enabled(value.flag);
	cocos2d::Rect rect;
	rect.origin.x	 = value.rect.ax;
	rect.origin.y	 = value.rect.ay;
	rect.size.width  = value.size.w - value.rect.ax - value.rect.bx;
	rect.size.height = value.size.h - value.rect.ay - value.rect.by;
	prop->get_imageview()->setCapInsets(rect);
}