#ifndef __xui_family_h__
#define __xui_family_h__

#include "xui_header.h"

class xui_family
{
public:
	/*
	//member
	*/
	std::string face;
	s32			size;
	bool		bold;

	/*
	//constructor
	*/
	xui_family( void )
	{
		face = "Arial";
		size = 12;
		bold = false;
	}
	xui_family( const std::string& _face, s32 _size, bool _bold )
	{
		face = _face;
		size = _size;
		bold = _bold;
	}
	xui_family( const xui_family& other )
	{
		face = other.face;
		size = other.size;
		bold = other.bold;
	}

	/*
	//operator
	*/
	bool operator == ( const xui_family& other ) const
	{
		return (face == other.face &&
				size == other.size &&
				bold == other.bold);
	}
	bool operator != ( const xui_family& other ) const
	{
		return (face != other.face ||
				size != other.size ||
				bold != other.bold);
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