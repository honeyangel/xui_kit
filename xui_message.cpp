#include "xui_message.h"

xui_message::xui_message( void )
: sender(NULL)
, msgidx(k_xm_none)
, wparam(0)
, lparam(0)
{}

xui_message::xui_message( xui_component* other_sender, u32 other_msgidx, u32 other_wparam, u32 other_lparam )
: sender(other_sender)
, msgidx(other_msgidx)
, wparam(other_wparam)
, lparam(other_lparam)
{}

xui_message::xui_message( const xui_message& other )
: sender(other.sender)
, msgidx(other.msgidx)
, wparam(other.wparam)
, lparam(other.lparam)
{}

xui_message& xui_message::operator =( const xui_message& other )
{
	sender = other.sender;
	msgidx = other.msgidx;
	wparam = other.wparam;
	lparam = other.lparam;
	return (*this);
}

bool xui_message::operator ==( const xui_message& other ) const
{
	return (sender == other.sender &&
			msgidx == other.msgidx &&
			wparam == other.wparam &&
			lparam == other.lparam);
}

bool xui_message::operator !=( const xui_message& other ) const
{
	return (sender != other.sender ||
			msgidx != other.msgidx ||
			wparam != other.wparam ||
			lparam != other.lparam);
}