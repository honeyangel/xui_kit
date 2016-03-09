#ifndef __xui_family_render_h__
#define __xui_family_render_h__

#include "xui_header.h"
#include "xui_colour.h"

enum 
{
	TEXTDRAW_NORMAL,
	TEXTDRAW_SHADOW,
};

class xui_family_render
{
public:
	/*
	//static
	*/
	static const xui_family_render default;

	/*
	//member
	*/
	u08			renderstyle;
	xui_colour	normalcolor;
	xui_colour	strokecolor;
	xui_colour	shadowcolor;

	/*
	//constructor
	*/
	xui_family_render( void )
	{
		renderstyle = 0;
		normalcolor = xui_colour(1.0f, 0.8f);
		strokecolor = xui_colour(1.0f, 0.0f);
		shadowcolor = xui_colour(1.0f, 0.0f);
	}
	xui_family_render( const xui_colour& _normalcolor )
	{
		renderstyle = TEXTDRAW_NORMAL;
		normalcolor = _normalcolor;
		strokecolor = xui_colour(1.0f, 0.0f);
		shadowcolor = xui_colour(1.0f, 0.0f);
	}
	xui_family_render( u08 _renderstyle, const xui_colour& _normalcolor, const xui_colour& _strokecolor, const xui_colour& _shadowcolor )
	{
		renderstyle = _renderstyle;
		normalcolor = _normalcolor;
		strokecolor = _strokecolor;
		shadowcolor = _shadowcolor;
	}
	xui_family_render( const xui_family_render& other )
	{
		renderstyle = other.renderstyle;
		normalcolor = other.normalcolor;
		strokecolor = other.strokecolor;
		shadowcolor = other.shadowcolor;
	}

	/*
	//operator
	*/
	bool operator == ( const xui_family_render& other ) const
	{
		return (renderstyle == other.renderstyle &&
				normalcolor == other.normalcolor &&
				strokecolor == other.strokecolor &&
				shadowcolor == other.shadowcolor);
	}
	bool operator != ( const xui_family_render& other ) const
	{
		return (renderstyle != other.renderstyle ||
				normalcolor != other.normalcolor ||
				strokecolor != other.strokecolor ||
				shadowcolor != other.shadowcolor);
	}
};

#endif//__xui_family_render_h__