#include "cocos_resource.h"
#include "cocos_propspineatlas.h"

/*
//constructor
*/
xui_create_explain(cocos_propspineatlas)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_module, full)
{}

/*
//override
*/
xui_method_explain(cocos_propspineatlas, get_dragtype,	std::string			)( void )
{
	return "SpineAtlas";
}
xui_method_explain(cocos_propspineatlas, get_dragdata,	void*				)( void )
{
	return this;
}