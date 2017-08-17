#include "2d/CCNode.h"
#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propnodebase.h"
#include "cocos_nodedata.h"

/*
//constructor
*/
xui_create_explain(cocos_nodedata)( xui_proproot* prop )
: cocos_treedata(NULL, prop)
{
	m_icon = dynamic_cast<cocos_propnodebase*>(prop)->get_icon();
}

/*
//virtual
*/
xui_method_explain(cocos_nodedata, get_text, std::wstring	)( u32 index )
{
	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(m_prop);
	return xui_global::ascii_to_unicode(prop->get_node()->getName());
}
xui_method_explain(cocos_nodedata, set_text, void			)( u32 index, const std::wstring& text )
{
	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(m_prop);
	prop->get_node()->setName(xui_global::unicode_to_ascii(text));
}
xui_method_explain(cocos_nodedata, get_flag, bool			)( u32 index )
{
	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(m_prop);
	if (index == NODEFLAG_SHOW)
		return prop->get_node()->isVisible();
	else 
		return m_flag;
}
xui_method_explain(cocos_nodedata, set_flag, void			)( u32 index, bool flag )
{
	cocos_propnodebase* prop = dynamic_cast<cocos_propnodebase*>(m_prop);
	if (index == NODEFLAG_SHOW)
		prop->get_node()->setVisible(flag);
	else
		m_flag = flag;
}