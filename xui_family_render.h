#ifndef __xui_family_render_h__
#define __xui_family_render_h__

#include "xui_header.h"
#include "xui_colour.h"

enum 
{
	TEXTDRAW_NORMAL,
	TEXTDRAW_STROKE,
	TEXTDRAW_SHADOW,
};

class xui_family_render
{
public:
	/*
	//member
	*/
	u08			renderstyle;
	xui_colour	normalcolor;
	u08			strokewidth;
	xui_colour	strokecolor;

	/*
	//constructor
	*/
	xui_family_render( void )
	{
		renderstyle = 0;
		normalcolor = xui_colour(1.0f, 0.7f, 0.7f, 0.7f);
		strokewidth = 1;
		strokecolor = xui_colour(1.0f, 0.0f, 0.0f, 0.0f);
	}
	xui_family_render( const xui_colour& _normalcolor )
	{
		renderstyle = TEXTDRAW_NORMAL;
		normalcolor = _normalcolor;
		strokewidth = 1;
		strokecolor = xui_colour(1.0f, 0.0f, 0.0f, 0.0f);
	}
	xui_family_render( u08 _renderstyle, const xui_colour& _normalcolor, u08 _strokewidth, const xui_colour& _strokecolor )
	{
		renderstyle = _renderstyle;
		normalcolor = _normalcolor;
		strokewidth = _strokewidth;
		strokecolor = _strokecolor;
	}
	xui_family_render( const xui_family_render& other )
	{
		renderstyle = other.renderstyle;
		normalcolor = other.normalcolor;
		strokewidth = other.strokewidth;
		strokecolor = other.strokecolor;
	}

	/*
	//operator
	*/
	bool operator == ( const xui_family_render& other ) const
	{
		return (renderstyle == other.renderstyle &&
				normalcolor == other.normalcolor &&
				strokewidth == other.strokewidth &&
				strokecolor == other.strokecolor);
	}
	bool operator != ( const xui_family_render& other ) const
	{
		return (renderstyle != other.renderstyle ||
				normalcolor != other.normalcolor ||
				strokewidth != other.strokewidth ||
				strokecolor != other.strokecolor);
	}
};

#endif//__xui_family_render_h__