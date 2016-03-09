#ifndef __xui_family_h__
#define __xui_family_h__

#include "xui_header.h"

enum
{
	TEXTALIGN_LT,
	TEXTALIGN_LC,
	TEXTALIGN_LB,
	TEXTALIGN_CT,
	TEXTALIGN_CC,
	TEXTALIGN_CB,
	TEXTALIGN_RT,
	TEXTALIGN_RC,
	TEXTALIGN_RB,
};

class xui_family
{
public:
	static const xui_family default;

	/*
	//member
	*/
	s32			face;
	s32			size;
	s32			bold;
	s32			horz;
	s32			vert;

	/*
	//constructor
	*/
	xui_family( void )
	{
		face = 0;
		size = 10;
		bold = 0;
		horz = 0;
		vert = 0;
	}
	xui_family( s32 _size, s32 _bold = 0 )
	{
		face = 0;
		size = _size;
		bold = _bold;
		horz = 0;
		vert = 0;
	}
	xui_family( s32 _face, s32 _size, s32 _bold )
	{
		face = _face;
		size = _size;
		bold = _bold;
		horz = 0;
		vert = 0;
	}
	xui_family( const xui_family& other )
	{
		face = other.face;
		size = other.size;
		bold = other.bold;
		horz = other.horz;
		vert = other.vert;
	}

	/*
	//operator
	*/
	bool operator == ( const xui_family& other ) const
	{
		return (face == other.face &&
				size == other.size &&
				bold == other.bold &&
				horz == other.horz &&
				vert == other.vert);
	}
	bool operator != ( const xui_family& other ) const
	{
		return (face != other.face ||
				size != other.size ||
				bold != other.bold ||
				horz != other.horz ||
				vert != other.vert);
	}

	/*
	//method
	*/
	s32	to_key( void ) const
	{
		return  (face << 28) |
				(size << 20) |
				(bold << 16);
	}
};

#endif//__xui_family_h__