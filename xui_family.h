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
	std::string face;
	s32			size;
	bool		bold;
	s32			horz;
	s32			vert;

	/*
	//constructor
	*/
	xui_family( void )
	{
		face = "Euphemia";
		size = 14;
		bold = false;
		horz = 0;
		vert = 1;
	}
	xui_family( s32 _size, bool _bold = false )
	{
		face = "Euphemia";
		size = _size;
		bold = _bold;
		horz = 0;
		vert = 1;
	}
	xui_family( const std::string& _face, s32 _size, bool _bold )
	{
		face = _face;
		size = _size;
		bold = _bold;
		horz = 0;
		vert = 1;
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
	std::string to_string( void ) const
	{
		std::stringstream ss;
		ss << face; ss << "_";
		ss << size; ss << "_";
		if (bold)	ss << "b";
		else		ss << "r";

		return ss.str();
	}
};

#endif//__xui_family_h__