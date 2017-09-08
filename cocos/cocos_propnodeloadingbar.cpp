#include "ui/UILoadingBar.h"
#include "ui/UIScale9Sprite.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"

#include "xui_kindctrl.h"
#include "xui_numbbox.h"
#include "xui_propctrl_slider.h"
#include "cocos_resource.h"
#include "cocos_propdata_image.h"
#include "cocos_propnodeloadingbar.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeloadingbar)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_loadingbar(dynamic_cast<cocos2d::ui::LoadingBar*>(node))
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
	add_loadingbar	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodeloadingbar, get_loadingbar,	cocos2d::ui::LoadingBar*)( void )
{
	return m_loadingbar;
}
xui_method_explain(cocos_propnodeloadingbar, add_loadingbar,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "LoadingBar", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeloadingbar>(this, &cocos_propnodeloadingbar::on_propchanged);
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"Background",
		get_background,
		set_background,
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

	xui_propenum_map textmap;
	textmap[(s32)cocos2d::ui::LoadingBar::Direction::LEFT]  = L"LeftToRight";
	textmap[(s32)cocos2d::ui::LoadingBar::Direction::RIGHT] = L"RightToLeft";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"Direction",
		xui_propctrl_enum::create,
		textmap,
		get_direction,
		set_direction,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodeloadingbar, def_size,			cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec value;

	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	cocos2d::Node* node = prop->get_loadingbar()->getVirtualRenderer();
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
xui_method_explain(cocos_propnodeloadingbar, get_background,	void*					)( void* userptr )
{
	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	cocos2d::Node* node = prop->get_loadingbar()->getVirtualRenderer();
	cocos2d::ui::Scale9Sprite* scale9 = dynamic_cast<cocos2d::ui::Scale9Sprite*>(node);
	return get_image(scale9->getSprite());
}
xui_method_explain(cocos_propnodeloadingbar, set_background,	void					)( void* userptr, void* value )
{
	cocos2d::Ref*			base	= (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame	= dynamic_cast<cocos2d::SpriteFrame*>(base);

	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	if		(texture)	prop->get_loadingbar()->loadTexture(texture->getFileName(), cocos2d::ui::Widget::TextureResType::LOCAL);
	else if (frame)		prop->get_loadingbar()->loadTexture(frame->getName(),		cocos2d::ui::Widget::TextureResType::PLIST);
	else				prop->get_loadingbar()->loadTexture("",						cocos2d::ui::Widget::TextureResType::LOCAL);

	set_size(userptr, def_size(userptr));
}
xui_method_explain(cocos_propnodeloadingbar, get_percent,		f64						)( void* userptr )
{
	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	return prop->get_loadingbar()->getPercent();
}
xui_method_explain(cocos_propnodeloadingbar, set_percent,		void					)( void* userptr, f64 value )
{
	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	prop->get_loadingbar()->setPercent(value);
}
xui_method_explain(cocos_propnodeloadingbar, get_direction,		s32						)( void* userptr )
{
	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	return (s32)prop->get_loadingbar()->getDirection();
}
xui_method_explain(cocos_propnodeloadingbar, set_direction,		void					)( void* userptr, s32 value )
{
	cocos_propnodeloadingbar* prop = (cocos_propnodeloadingbar*)userptr;
	prop->get_loadingbar()->setDirection((cocos2d::ui::LoadingBar::Direction)value);
}