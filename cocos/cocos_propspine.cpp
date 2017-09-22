#include "xui_global.h"
#include "cocos_resource.h"
#include "cocos_propspine.h"

/*
//constructor
*/
xui_create_explain(cocos_propspine)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_legend, full)
{}

/*
//method
*/
xui_method_explain(cocos_propspine, get_dragtype, std::string	)( void )
{
	return "Spine";
}
xui_method_explain(cocos_propspine, get_dragdata, void*			)( void )
{
	return this;
}