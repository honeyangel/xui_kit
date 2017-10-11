#include "2d/CCNode.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_textbox.h"
#include "cocos_resource.h"
#include "cocos_propnodelayer.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodelayer)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodebase(cocos_resource::icon_layer, file, node)
{
	add_content	();
	add_advance	();
	add_userdata();
}

/*
//method
*/
xui_method_explain(cocos_propnodelayer, add_content,		void			)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"Paint", "NodeContent", xui_kindctrl::create, cocos_resource::icon_transform, true);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodelayer>(this, &cocos_propnodelayer::on_propchanged);
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"ContentSize",
		xui_propctrl_vector::create,
		get_contentsize,
		set_contentsize,
		this,
		NT_UNSIGNEDINT));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodelayer, get_contentsize,	xui_vector<f64>	)( void* userptr )
{
	cocos_propnodelayer* prop = (cocos_propnodelayer*)userptr;
	cocos2d::Size size = prop->get_node()->getContentSize();
	return xui_vector<f64>(size.width, size.height);
}
xui_method_explain(cocos_propnodelayer, set_contentsize,	void			)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodelayer* prop = (cocos_propnodelayer*)userptr;
	prop->get_node()->setContentSize(cocos2d::Size(value.w, value.h));
}
