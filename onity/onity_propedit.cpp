#include "onity_propedit.h"

/*
//constructor
*/
xui_create_explain(onity_propedit)( void )
: onity_proproot()
{}

/*
//virtual
*/
xui_method_explain(onity_propedit, get_position,	xui_vector<s32>	)( void )
{
	return xui_vector<s32>(0);
}
xui_method_explain(onity_propedit, set_position,	void			)( const xui_vector<s32>& pos )
{

}
xui_method_explain(onity_propedit, get_bounding,	xui_rect2d<s32>	)( void )
{
	return xui_rect2d<s32>(0);
}