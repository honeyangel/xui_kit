#include "onity_resource.h"
#include "onity_likedata.h"

/*
//constructor
*/
xui_create_explain(onity_likedata)( xui_proproot* prop )
: onity_pathdata(onity_resource::icon_favorite, L"", prop)
{}

/*
//override
*/
xui_method_explain(onity_likedata, get_text,		std::wstring		)( u32 index )
{
	return L"Favorite";
}
//xui_method_explain(onity_likedata, get_textdraw,	xui_family_render	)( u32 index )
//{
//	xui_family_render draw = onity_pathdata::get_textdraw(index);
//	draw.normalcolor = xui_colour(1.0f, 210.0f/255.0f, 134.0f/255.0f, 50.0f/255.0f);
//	return draw;
//}