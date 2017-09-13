#include "xui_kindctrl.h"
#include "cocos_resource.h"
#include "cocos_propnodecustom.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodecustom)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
{}

/*
//method
*/
xui_method_explain(cocos_propnodecustom, add_custom, void)( void )
{
	add_propkind(new xui_propkind(
		this,
		L"Custom",
		"NodeCustom",
		xui_kindctrl::create,
		m_icon,
		true,
		true,
		false,
		true));
}