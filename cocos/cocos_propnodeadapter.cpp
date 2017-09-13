#include "external/AdapterNode.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_propdata.h"
#include "xui_textbox.h"
#include "cocos_propnodeadapter.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodeadapter)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_adapter(dynamic_cast<cocos2d::ui::AdapterNode*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, NULL);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
	add_custom		();
	add_adapter		();
}

/*
//method
*/
xui_method_explain(cocos_propnodeadapter, get_adapter,		cocos2d::ui::AdapterNode*	)( void )
{
	return m_adapter;
}
xui_method_explain(cocos_propnodeadapter, add_adapter,		void						)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "AdapterNode", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodeadapter>(this, &cocos_propnodeadapter::on_propchanged);
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"DesignHeight",
		xui_propctrl_number::create,
		get_designheight,
		set_designheight,
		this,
		NT_UNSIGNEDINT));
}

/*
//static
*/
xui_method_explain(cocos_propnodeadapter, get_designheight, f64							)( void* userptr )
{
	cocos_propnodeadapter* prop = (cocos_propnodeadapter*)userptr;
	return prop->get_adapter()->getDesignHeight();
}
xui_method_explain(cocos_propnodeadapter, set_designheight, void						)( void* userptr, f64 value )
{
	cocos_propnodeadapter* prop = (cocos_propnodeadapter*)userptr;
	prop->get_adapter()->setDesignHeight(value);
}
