#ifndef __xui_colour_h__
#define __xui_colour_h__

#include "xui_header.h"

class xui_colour
{
public:
	/*
	//static
	*/
	static const xui_colour transparent;
	static const xui_colour white;
	static const xui_colour black;
	static const xui_colour red;
	static const xui_colour green;
	static const xui_colour blue;
	static const xui_colour lightgray;

	/*
	// member
	*/
	union
	{
		f32 value[4];

		struct
		{
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
	};

	/*
	// constructor
	*/
	xui_colour( void )
	{
		memset(value, 0, sizeof(f32) * 4);
	}
	xui_colour( f32 _v )
	{
		for (u32 i = 0; i < 4; ++i)
			value[i] = _v;
	}
	xui_colour( f32 _a, f32 _v )
	{
		a = _a;
		r = _v;
		g = _v;
		b = _v;
	}
	xui_colour( f32 _a, f32 _r, f32 _g, f32 _b )
	{
		a = _a;
		r = _r;
		g = _g;
		b = _b;
	}
	xui_colour( u32 argb )
	{
		set_argb(argb);
	}
	xui_colour( const xui_colour& other )
	{
		memcpy(value, other.value, sizeof(f32) * 4);
	}

	/*
	// operator
	*/
	xui_colour  operator + ( const xui_colour& other ) const
	{
		return xui_colour(a+other.a, r+other.r, g+other.g, b+other.b);
	}
	xui_colour  operator - ( const xui_colour& other ) const
	{
		return xui_colour(a-other.a, r-other.r, g-other.g, b-other.b);
	}
	xui_colour	operator * ( const xui_colour& other ) const
	{
		return xui_colour(a*other.a, r*other.r, g*other.g, b*other.b);
	}
	xui_colour  operator * ( f32 s ) const
	{
		return xui_colour(a*s, r*s, g*s, b*s);
	}
	xui_colour  operator / ( f32 s ) const
	{
		return xui_colour(a/s, r/s, g/s, b/s);
	}

	xui_colour& operator = ( const xui_colour& other )
	{
		memcpy(value, other.value, sizeof(f32) * 4);
		return (*this);
	}
	xui_colour& operator +=( const xui_colour& other )
	{
		a += other.a;
		r += other.r;
		g += other.g;
		b += other.b;
		return (*this);
	}
	xui_colour& operator -=( const xui_colour& other )
	{
		a -= other.a;
		r -= other.r;
		g -= other.g;
		b -= other.b;
		return (*this);
	}
	xui_colour& operator *=( const xui_colour& other )
	{
		a *= other.a;
		r *= other.r;
		g *= other.g;
		b *= other.b;
		return (*this);
	}
	xui_colour& operator *=( f32 s )
	{
		a *= s;
		r *= s;
		g *= s;
		b *= s;
		return (*this);
	}
	xui_colour& operator /=( f32 s )
	{
		a /= s;
		r /= s;
		g /= s;
		b /= s;
		return (*this);
	}

	/*
	// operator
	*/
	bool operator ==( const xui_colour& other ) const
	{
		return (a == other.a && r == other.r && g == other.g && b == other.b);
	}
	bool operator !=( const xui_colour& other ) const
	{
		return (a != other.a || r != other.r || g != other.g || b != other.b);
	}

	/*
	// method
	*/
	u32  get_abgr( void ) const
	{
		return (((u32)(a * 255.0f)) << 24) |
			   (((u32)(b * 255.0f)) << 16) |
			   (((u32)(g * 255.0f)) <<  8) |
			   (((u32)(r * 255.0f))      );
	}
	u32  get_argb( void ) const
	{
		return (((u32)(a * 255.0f)) << 24) |
			   (((u32)(r * 255.0f)) << 16) |
			   (((u32)(g * 255.0f)) <<  8) |
			   (((u32)(b * 255.0f))      );
	}
	void set_argb( u32 argb )
	{
		static const f32 inv = 1.0f / 255.0f;
		a = inv * ((argb & 0xFF000000) >> 24);
		r = inv * ((argb & 0x00FF0000) >> 16);
		g = inv * ((argb & 0x0000FF00) >>  8);
		b = inv * ((argb & 0x000000FF)      );
	}
};

#endif//__xui_colour_h__