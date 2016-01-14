#include "xui_message.h"

/*
//constructor
*/
xui_create_explain(xui_message)( void )
: sender(NULL)
, msgidx(XM_NONE)
, wparam(0)
, lparam(0)
{}

xui_create_explain(xui_message)( xui_component* other_sender, u32 other_msgidx, u32 other_wparam, u32 other_lparam )
: sender(other_sender)
, msgidx(other_msgidx)
, wparam(other_wparam)
, lparam(other_lparam)
{}

xui_create_explain(xui_message)( const xui_message& other )
: sender(other.sender)
, msgidx(other.msgidx)
, wparam(other.wparam)
, lparam(other.lparam)
{}

/*
//operator
*/
xui_method_explain(xui_message, operator = , xui_message&)( const xui_message& other )
{
	sender = other.sender;
	msgidx = other.msgidx;
	wparam = other.wparam;
	lparam = other.lparam;
	return (*this);
}

xui_method_explain(xui_message, operator ==, bool		 )( const xui_message& other ) const
{
	return (sender == other.sender &&
			msgidx == other.msgidx &&
			wparam == other.wparam &&
			lparam == other.lparam);
}

xui_method_explain(xui_message, operator !=, bool		 )( const xui_message& other ) const
{
	return (sender != other.sender ||
			msgidx != other.msgidx ||
			wparam != other.wparam ||
			lparam != other.lparam);
}