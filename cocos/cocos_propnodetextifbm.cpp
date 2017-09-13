#include "external/TextIFBM.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "cocos_propnodetextifbm.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodetextifbm)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_textifbm(dynamic_cast<cocos2d::ui::TextIFBM*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_aligndock	(false);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
	add_custom		();
	add_textifbm	();
}

/*
//method
*/
xui_method_explain(cocos_propnodetextifbm, get_textifbm, cocos2d::ui::TextIFBM*	)( void )
{
	return m_textifbm;
}
xui_method_explain(cocos_propnodetextifbm, add_textifbm, void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "TextIFBM", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodetextifbm>(this, &cocos_propnodetextifbm::on_propchanged);
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
		L"Kerning",
		xui_propctrl_number::create,
		get_chargrap,
		set_chargrap,
		this,
		NT_INT));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodetextifbm, get_linegrap, f64					)( void* userptr )
{
	cocos_propnodetextifbm* prop = (cocos_propnodetextifbm*)userptr;
	return prop->get_textifbm()->getLineSpacing();
}
xui_method_explain(cocos_propnodetextifbm, set_linegrap, void					)( void* userptr, f64 value )
{
	cocos_propnodetextifbm* prop = (cocos_propnodetextifbm*)userptr;
	prop->get_textifbm()->setLineSpacing(value);
}
xui_method_explain(cocos_propnodetextifbm, get_chargrap, f64					)( void* userptr )
{
	cocos_propnodetextifbm* prop = (cocos_propnodetextifbm*)userptr;
	return prop->get_textifbm()->getAdditionalKerning();
}
xui_method_explain(cocos_propnodetextifbm, set_chargrap, void					)( void* userptr, f64 value )
{
	cocos_propnodetextifbm* prop = (cocos_propnodetextifbm*)userptr;
	prop->get_textifbm()->setAdditionalKerning(value);
}