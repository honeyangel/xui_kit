#include "ui/UIWidget.h"
#include "ui/UIScale9Sprite.h"
#include "2d/CCSprite.h"

#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "cocos_propctrl_callback.h"
#include "cocos_propnodewidget.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodewidget)( xui_bitmap* icon, cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodebase(icon, file, node)
, m_widget(dynamic_cast<cocos2d::ui::Widget*>(node))
{}

/*
//method
*/
xui_method_explain(cocos_propnodewidget, get_widget,		cocos2d::ui::Widget*)( void )
{
	return m_widget;
}
xui_method_explain(cocos_propnodewidget, add_touch,			void				)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeTouch", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodewidget>(this, &cocos_propnodewidget::on_propchanged);
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"TouchEnable",
		xui_propctrl_bool::create,
		was_touchenable,
		set_touchenable,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodewidget, add_scale9,		void				)( cocos_propdata_scale9::get_func userget, cocos_propdata_scale9::set_func userset )
{
	xui_propkind* kind = new xui_propkind(this, L"Scale9", "NodeScale9", xui_kindctrl::create, m_icon, true);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodewidget>(this, &cocos_propnodewidget::on_propchanged);
	kind->add_propdata(new cocos_propdata_scale9(
		kind,
		L"Enable",
		userget,
		userset,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodewidget, add_callback,		void				)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeTouch", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodewidget>(this, &cocos_propnodewidget::on_propchanged);
	kind->add_propdata(new cocos_propdata_callback(kind, this));
	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodewidget, was_touchenable,	bool				)( void* userptr )
{
	cocos_propnodewidget* prop = (cocos_propnodewidget*)userptr;
	return prop->get_widget()->isTouchEnabled();
}
xui_method_explain(cocos_propnodewidget, set_touchenable,	void				)( void* userptr, bool value )
{
	cocos_propnodewidget* prop = (cocos_propnodewidget*)userptr;
	prop->get_widget()->setTouchEnabled(value);
}
