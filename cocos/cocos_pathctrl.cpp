#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "cocos_pathdata.h"
#include "cocos_proppath.h"
#include "cocos_pathctrl.h"

xui_implement_rtti(cocos_pathctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(cocos_pathctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{}

/*
//static
*/
xui_method_explain(cocos_pathctrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new cocos_pathctrl(propkind);
}