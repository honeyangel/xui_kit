#ifndef __xui_message_h__
#define __xui_message_h__

#include "xui_header.h"

enum
{
	k_xm_none				= -1,
	k_xm_born				=  0,
	k_xm_dead				=  1,

	k_xm_addnode			=  2,
	k_xm_delnode			=  3,
	k_xm_setrenderpt		=  4,
	k_xm_setrendersz		=  5,
	k_xm_setclientsz		=  6,
	k_xm_setexternpd		=  7,

	k_xm_getfocus			=  8,
	k_xm_nonfocus			=  9,
	k_xm_getcatch			= 10,
	k_xm_noncatch			= 11,

	k_xm_free				= 12,
	k_xm_gray				= 13,
	k_xm_show				= 14,
	k_xm_hide				= 15,

	k_xm_keybddown		    = 16,
	k_xm_keybdrise		    = 17,
	k_xm_keybdchar		    = 18,
	k_xm_mousedown		    = 19,
	k_xm_mouserise		    = 20,
	k_xm_mouseenter		    = 21,
	k_xm_mouseleave		    = 22,
	k_xm_mouseclick		    = 23,
	k_xm_mousewheel		    = 24,

	k_xm_mousedragitem	    = 25,
	k_xm_mousedragover	    = 26,
	k_xm_mousedragdrop	    = 27,
	k_xm_mousedoubleclick	= 28,

	k_xm_actionplay		    = 29,
	k_xm_actionstop		    = 30,
	k_xm_actionloop		    = 31,

	k_xm_update			    = 32,
	k_xm_render			    = 33,
	k_xm_invalid			= 34,
	k_xm_perform			= 35,

	k_xm_user				= 36,
};

class xui_message
{
public:
	xui_component*	sender;
	u32				msgidx;
	u32				wparam;
	u32				lparam;

	xui_message( void );
	xui_message( xui_component* other_sender, u32 other_msgidx, u32 other_wparam, u32 other_lparam );
	xui_message( const xui_message& other );

	xui_message&	operator = ( const xui_message& other );
	bool			operator ==( const xui_message& other ) const;
	bool			operator !=( const xui_message& other ) const;
};

#endif//__xui_message_h__