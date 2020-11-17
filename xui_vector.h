#ifndef __xui_vector_h__
#define __xui_vector_h__

#include "xui_header.h"

template<typename T>
class xui_vector
{
public:
	union
	{
		T value[2];

		struct
		{
			T x;
			T y;
		};
		struct
		{
			T w;
			T h;
		};
	};

	xui_vector( void )
	{
		memset(value, 0, sizeof(T) * 2);
	}
	xui_vector( T _v )
	{
		for (u32 i = 0; i < 2; ++i)
			value[i] = _v;
	}
	xui_vector( T _x, T _y )
	{
		x = _x;
		y = _y;
	}
	xui_vector( const xui_vector<T>& other )
	{
		memcpy(value, other.value, sizeof(T) * 2 );
	}

	xui_vector<T>  operator - ( void ) const
	{
		return xui_vector<T>(-x, -y);
	}
	xui_vector<T>  operator + ( const xui_vector<T>& other ) const
	{
		return xui_vector<T>(x+other.x, y+other.y);
	}
	xui_vector<T>  operator - ( const xui_vector<T>& other ) const
	{
		return xui_vector<T>(x-other.x, y-other.y);
	}
	xui_vector<T>  operator * ( const xui_vector<T>& other ) const
	{
		return xui_vector<T>(x*other.x, y*other.y);
	}
	xui_vector<T>  operator / ( const xui_vector<T>& other ) const
	{
		return xui_vector<T>(x/other.x, y/other.y);
	}
	xui_vector<T>  operator * ( T s ) const
	{
		return xui_vector<T>(x*s, y*s);
	}
	xui_vector<T>  operator / ( T s ) const
	{
		return xui_vector<T>(x/s, y/s);
	}

	xui_vector<T>& operator = ( const xui_vector<T>& other )
	{
		memcpy(value, other.value, sizeof(T) * 2);
		return (*this);
	}
	xui_vector<T>& operator +=( const xui_vector<T>& other )
	{
		x += other.x;
		y += other.y;
		return (*this);
	}
	xui_vector<T>& operator -=( const xui_vector<T>& other )
	{
		x -= other.x;
		y -= other.y;
		return (*this);
	}
	xui_vector<T>& operator *=( const xui_vector<T>& other )
	{
		x *= other.x;
		y *= other.y;
		return (*this);
	}
	xui_vector<T>& operator /=( const xui_vector<T>& other )
	{
		x /= other.x;
		y /= other.y;
		return (*this);
	}
	xui_vector<T>& operator *=( T s )
	{
		x *= s;
		y *= s;
		return (*this);
	}
	xui_vector<T>& operator /=( T s )
	{
		x /= s;
		y /= s;
		return (*this);
	}

	bool operator ==( const xui_vector<T>& other ) const
	{
		return (x == other.x && y == other.y);
	}
	bool operator !=( const xui_vector<T>& other ) const
	{
		return (x != other.x || y != other.y);
	}

	template<typename C>
	xui_vector<C> to( void ) const
	{
		return xui_vector<C>((C)x, (C)y);
	}
};

#endif//__xui_vector_h__