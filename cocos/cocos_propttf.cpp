#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propttf.h"

/*
//constructor
*/
xui_create_explain(cocos_propttf)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_state, full)
{
	xui_global::add_fontfile(xui_global::unicode_to_ascii(full));
}

/*
//method
*/
xui_method_explain(cocos_propttf, get_dragtype,	std::string	)(void)
{
	return "TTF";
}
xui_method_explain(cocos_propttf, get_dragdata,	void*		)(void)
{
	return this;
}