#include "cocos_resource.h"
#include "cocos_propc3b.h"

/*
//constructor
*/
xui_create_explain(cocos_propc3b)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_physics, full)
{}

/*
//override
*/
xui_method_explain(cocos_propc3b, get_dragtype, std::string	)( void )
{
	return "C3B";
}
xui_method_explain(cocos_propc3b, get_dragdata, void*		)( void )
{
	return this;
}