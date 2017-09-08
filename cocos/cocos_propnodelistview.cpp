#include "ui/UIListView.h"

#include "xui_propctrl.h"
#include "xui_propdata.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_expand.h"
#include "xui_numbbox.h"
#include "cocos_propnodelistview.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodelistview)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodescroll(file, node)
, m_listview(dynamic_cast<cocos2d::ui::ListView*>(node))
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
	add_listview	();
	add_background	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodelistview, get_listview,		cocos2d::ui::ListView*	)( void )
{
	return m_listview;
}
xui_method_explain(cocos_propnodelistview, add_listview,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "ListView", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodelistview>(this, &cocos_propnodelistview::on_propchanged);

	xui_propenum_map textmap;
	xui_propdata_vec subprop;
	textmap[(s32)cocos2d::ui::ListView::Gravity::LEFT]				= L"Left";
	textmap[(s32)cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL] = L"Center";
	textmap[(s32)cocos2d::ui::ListView::Gravity::RIGHT]				= L"Right";
	subprop.push_back(new xui_propdata_enum_func(
		kind,
		L"ItemAlign",
		xui_propctrl_enum::create,
		textmap,
		get_subalign,
		set_subalign,
		this));
	textmap.clear();
	textmap[(s32)cocos2d::ui::ListView::Gravity::TOP]				= L"Top";
	textmap[(s32)cocos2d::ui::ListView::Gravity::CENTER_VERTICAL]	= L"Center";
	textmap[(s32)cocos2d::ui::ListView::Gravity::BOTTOM]			= L"Bottom";
	subprop.push_back(new xui_propdata_enum_func(
		kind,
		L"ItemAlign",
		xui_propctrl_enum::create,
		textmap,
		get_subalign,
		set_subalign,
		this));

	textmap.clear();
	textmap[(s32)cocos2d::ui::ScrollView::Direction::HORIZONTAL]	= L"Horizontal";
	textmap[(s32)cocos2d::ui::ScrollView::Direction::VERTICAL]		= L"Vertical";
	std::map<s32, std::vector<u32>> showmap;
	showmap[2].push_back(0);
	showmap[1].push_back(1);
	kind->add_propdata(new xui_propdata_expand_enum_func(
		kind,
		L"Direction",
		xui_propctrl_expand_enum::create,
		textmap,
		get_direction,
		set_direction,
		this,
		subprop,
		true,
		showmap));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"ItemMargin",
		xui_propctrl_number::create,
		get_submargin,
		set_submargin,
		this,
		NT_INT));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodelistview, get_subalign,		s32						)( void* userptr )
{
	cocos_propnodelistview* prop = (cocos_propnodelistview*)userptr;
	return (s32)prop->get_listview()->getGravity();
}
xui_method_explain(cocos_propnodelistview, set_subalign,		void					)( void* userptr, s32 value )
{
	cocos_propnodelistview* prop = (cocos_propnodelistview*)userptr;
	prop->get_listview()->setGravity((cocos2d::ui::ListView::Gravity)value);
}
xui_method_explain(cocos_propnodelistview, get_submargin,		f64						)( void* userptr )
{
	cocos_propnodelistview* prop = (cocos_propnodelistview*)userptr;
	return prop->get_listview()->getItemsMargin();
}
xui_method_explain(cocos_propnodelistview, set_submargin,		void					)( void* userptr, f64 value )
{
	cocos_propnodelistview* prop = (cocos_propnodelistview*)userptr;
	prop->get_listview()->setItemsMargin(value);
}