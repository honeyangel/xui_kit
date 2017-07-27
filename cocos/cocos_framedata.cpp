#include "xui_global.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_treenode.h"
#include "cocos_resource.h"
#include "cocos_propframe.h"
#include "cocos_framedata.h"

/*
//constructor
*/
xui_create_explain(cocos_framedata)( xui_bitmap* icon, xui_proproot* prop )
: cocos_treedata(icon, prop)
{}

/*
//override
*/
xui_method_explain(cocos_framedata, get_text, std::wstring)( u32 index )
{
	cocos_propframe* prop = dynamic_cast<cocos_propframe*>(m_prop);
	return xui_global::ascii_to_unicode(prop->get_framename());
}
