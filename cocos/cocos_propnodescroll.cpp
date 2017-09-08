#include "ui/UIScrollView.h"

#include "xui_propctrl_vector.h"
#include "xui_propdata.h"
#include "xui_kindctrl.h"
#include "xui_numbbox.h"
#include "cocos_resource.h"
#include "cocos_propnodescroll.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodescroll)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodepanel(file, node)
, m_scroll(dynamic_cast<cocos2d::ui::ScrollView*>(node))
{}

/*
//method
*/
xui_method_explain(cocos_propnodescroll, get_scroll,	cocos2d::ui::ScrollView*)( void )
{
	return m_scroll;
}
xui_method_explain(cocos_propnodescroll, add_scroll,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "ScrollView", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodescroll>(this, &cocos_propnodescroll::on_propchanged);
	xui_propenum_map textmap;
	textmap[(s32)cocos2d::ui::ScrollView::Direction::HORIZONTAL]	= L"Horizontal";
	textmap[(s32)cocos2d::ui::ScrollView::Direction::VERTICAL]		= L"Vertical";
	textmap[(s32)cocos2d::ui::ScrollView::Direction::BOTH]			= L"Both";
	kind->add_propdata(new xui_propdata_enum_func(
		kind,
		L"Direction",
		xui_propctrl_enum::create,
		textmap,
		get_direction,
		set_direction,
		this));
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"InnerSize",
		xui_propctrl_vector::create,
		get_innersize,
		set_innersize,
		this,
		NT_UNSIGNEDINT));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodescroll, add_bounce,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "ScrollBounce", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodescroll>(this, &cocos_propnodescroll::on_propchanged);
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"BounceEnable",
		xui_propctrl_bool::create,
		was_bounce,
		set_bounce,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodescroll, was_bounce,	bool					)( void* userptr )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	return prop->get_scroll()->isBounceEnabled();
}
xui_method_explain(cocos_propnodescroll, set_bounce,	void					)( void* userptr, bool value )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	prop->get_scroll()->setBounceEnabled(value);
}
xui_method_explain(cocos_propnodescroll, get_direction, s32						)( void* userptr )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	return (s32)prop->get_scroll()->getDirection();
}
xui_method_explain(cocos_propnodescroll, set_direction, void					)( void* userptr, s32  value )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	prop->get_scroll()->setDirection((cocos2d::ui::ScrollView::Direction)value);
}
xui_method_explain(cocos_propnodescroll, get_innersize, xui_vector<f64>			)( void* userptr )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	cocos2d::Size size = prop->get_scroll()->getInnerContainerSize();
	return xui_vector<f64>(size.width, size.height);
}
xui_method_explain(cocos_propnodescroll, set_innersize, void					)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodescroll* prop = (cocos_propnodescroll*)userptr;
	prop->get_scroll()->setInnerContainerSize(cocos2d::Size(value.w, value.h));
}

//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propnodescrollview)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodescroll(file, node)
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
	add_bounce		();
	add_scroll		();
	add_background	();
	add_advance		();
	add_callback	();
	add_userdata	();
}