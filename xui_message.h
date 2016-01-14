#ifndef __xui_message_h__
#define __xui_message_h__

#include "xui_header.h"

enum
{
	XM_NONE				= -1,
	XM_BORN				=  0,
	XM_DEAD				=  1,

	XM_ADDNODE			=  2,
	XM_DELNODE			=  3,
	XM_SETRENDERPT		=  4,
	XM_SETRENDERSZ		=  5,
	XM_SETCLIENTSZ		=  6,
	XM_SETEXTERNPD		=  7,

	XM_GETFOCUS			=  8,
	XM_NONFOCUS			=  9,
	XM_GETCATCH			= 10,
	XM_NONCATCH			= 11,

	XM_FREE				= 12,
	XM_GRAY				= 13,
	XM_SHOW				= 14,
	XM_HIDE				= 15,

	XM_KEYBDDOWN		= 16,
	XM_KEYBDRISE		= 17,
	XM_KEYBDCHAR		= 18,
	XM_MOUSEDOWN		= 19,
	XM_MOUSERISE		= 20,
	XM_MOUSEENTER		= 21,
	XM_MOUSELEAVE		= 22,
	XM_MOUSECLICK		= 23,
	XM_MOUSEWHEEL		= 24,

	XM_MOUSEDRAGITEM	= 25,
	XM_MOUSEDRAGOVER	= 26,
	XM_MOUSEDRAGDROP	= 27,
	XM_MOUSEDOUBLECLICK	= 28,

	XM_ACTIONPLAY		= 29,
	XM_ACTIONSTOP		= 30,
	XM_ACTIONLOOP		= 31,

	XM_UPDATE			= 32,
	XM_RENDER			= 33,
	XM_INVALID			= 34,
	XM_PERFORM			= 35,

	XM_USER				= 36,
};

class xui_message
{
public:
	xui_component*	sender;
	u32				msgidx;
	u32				wparam;
	u32				lparam;

	/*
	//constructor
	*/
	xui_message( void );
	xui_message( xui_component* other_sender, u32 other_msgidx, u32 other_wparam, u32 other_lparam );
	xui_message( const xui_message& other );

	/*
	//operator
	*/
	xui_message&	operator = ( const xui_message& other );
	bool			operator ==( const xui_message& other ) const;
	bool			operator !=( const xui_message& other ) const;
};

#endif//__xui_message_h__