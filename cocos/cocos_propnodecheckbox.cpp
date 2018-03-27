#include "ui/UICheckBox.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"

#include "xui_kindctrl.h"
#include "cocos_resource.h"
#include "cocos_propdata_image.h"
#include "cocos_propnodecheckbox.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodecheckbox)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_checkbox(dynamic_cast<cocos2d::ui::CheckBox*>(node))
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
	add_checkbox	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodecheckbox, get_checkbox,		cocos2d::ui::CheckBox*	)( void )
{
	return m_checkbox;
}
xui_method_explain(cocos_propnodecheckbox, add_checkbox,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "CheckBox", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodecheckbox>(this, &cocos_propnodecheckbox::on_propchanged);
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
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"CheckNormal",
		get_checknormal,
		set_checknormal,
		this));
	kind->add_propdata(new cocos_propdata_image(
		kind,
		L"CheckDisable",
		get_checkdisable,
		set_checkdisable,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Enable",
		xui_propctrl_bool::create,
		was_enable,
		set_enable,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Checked",
		xui_propctrl_bool::create,
		was_checked,
		set_checked,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodecheckbox, def_size,			cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec value;

	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	cocos2d::Sprite* sprite = prop->get_checkbox()->getRendererBackground();
	if (sprite)
	{
		cocos2d::Rect rect = sprite->getTextureRect();
		value.w.pix = rect.size.width;
		value.h.pix = rect.size.height;
	}

	return value;
}
xui_method_explain(cocos_propnodecheckbox, get_normal,			void*					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return get_image(prop->get_checkbox()->getRendererBackground());
}
xui_method_explain(cocos_propnodecheckbox, set_normal,			void					)( void* userptr, void* value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	set_image(prop->get_checkbox()->getRendererBackground(), value);
}
xui_method_explain(cocos_propnodecheckbox, get_press,			void*					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return get_image(prop->get_checkbox()->getRendererBackgroundSelected());
}
xui_method_explain(cocos_propnodecheckbox, set_press,			void					)( void* userptr, void* value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	set_image(prop->get_checkbox()->getRendererBackgroundSelected(), value);
}
xui_method_explain(cocos_propnodecheckbox, get_disable,			void*					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return get_image(prop->get_checkbox()->getRendererBackgroundDisabled());
}
xui_method_explain(cocos_propnodecheckbox, set_disable,			void					)( void* userptr, void* value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	set_image(prop->get_checkbox()->getRendererBackgroundDisabled(), value);
}
xui_method_explain(cocos_propnodecheckbox, get_checknormal,		void*					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return get_image(prop->get_checkbox()->getRendererFrontCross());
}
xui_method_explain(cocos_propnodecheckbox, set_checknormal,		void					)( void* userptr, void* value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	set_image(prop->get_checkbox()->getRendererFrontCross(), value);
}
xui_method_explain(cocos_propnodecheckbox, get_checkdisable,	void*					)( void* userptr )
{
	cocos_propnodecheckbox*	prop = (cocos_propnodecheckbox*)userptr;
	return get_image(prop->get_checkbox()->getRendererFrontCrossDisabled());
}
xui_method_explain(cocos_propnodecheckbox, set_checkdisable,	void					)( void* userptr, void* value )
{
	cocos_propnodecheckbox*	prop = (cocos_propnodecheckbox*)userptr;
	set_image(prop->get_checkbox()->getRendererFrontCrossDisabled(), value);
}
xui_method_explain(cocos_propnodecheckbox, was_enable,			bool					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return prop->get_checkbox()->isEnabled();
}
xui_method_explain(cocos_propnodecheckbox, set_enable,			void					)( void* userptr, bool  value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	prop->get_checkbox()->setEnabled(value);
}
xui_method_explain(cocos_propnodecheckbox, was_checked,			bool					)( void* userptr )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	return prop->get_checkbox()->isSelected();
}
xui_method_explain(cocos_propnodecheckbox, set_checked,			void					)( void* userptr, bool  value )
{
	cocos_propnodecheckbox* prop = (cocos_propnodecheckbox*)userptr;
	prop->get_checkbox()->setSelected(value);
}