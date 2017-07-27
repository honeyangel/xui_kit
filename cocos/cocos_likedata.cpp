#include "cocos_resource.h"
#include "cocos_likedata.h"

/*
//constructor
*/
xui_create_explain(cocos_likedata)( xui_proproot* prop )
: cocos_pathdata(cocos_resource::icon_favorite, L"", prop)
{}

/*
//override
*/
xui_method_explain(cocos_likedata, get_text,		std::wstring		)( u32 index )
{
	return L"Favorite";
}