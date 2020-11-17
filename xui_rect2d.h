#ifndef __xui_rect2d_h__
#define __xui_rect2d_h__

#include "xui_vector.h"

template<typename T>
class xui_rect2d
{
public:
	union
	{
		T value[4];

		struct
		{
			T ax;
			T ay;
			T bx;
			T by;
		};
	};

	xui_rect2d( void )
	{
		memset(value, 0, sizeof(T) * 4);
	}
	xui_rect2d( T _v )
	{
		for (u32 i = 0 ; i < 4; ++i)
			value[i] = _v;
	}
	xui_rect2d( T _ax, T _ay, T _bx, T _by )
	{
		ax = _ax;
		ay = _ay;
		bx = _bx;
		by = _by;
	}
	xui_rect2d( const xui_vector<T>& pt, const xui_vector<T>& sz )
	{
		set_pt(pt);
		set_sz(sz);
	}
	xui_rect2d( const xui_rect2d<T>& other )
	{
		memcpy(value, other.value, sizeof(T) * 4);
	}

	xui_rect2d<T>  operator + ( const xui_vector<T>& offset ) const
	{
		return xui_rect2d<T>(get_pt()+offset, get_sz());
	}
	xui_rect2d<T>  operator - ( const xui_vector<T>& offset ) const
	{
		return xui_rect2d<T>(get_pt()-offset, get_sz());
	}

	xui_rect2d<T>& operator = ( const xui_rect2d<T>& other  )
	{
		memcpy(value, other.value, sizeof(T) * 4);
		return (*this);
	}
	xui_rect2d<T>& operator +=( const xui_vector<T>& offset )
	{
		set_pt(get_pt()+offset);
		return (*this);
	}
	xui_rect2d<T>& operator -=( const xui_vector<T>& offset )
	{
		set_pt(get_pt()-offset);
		return (*this);
	}

	bool operator ==( const xui_rect2d<T>& other ) const
	{
		return (ax == other.ax && 
				ay == other.ay &&
				bx == other.bx &&
				by == other.by);
	}
	bool operator !=( const xui_rect2d<T>& other ) const
	{
		return (ax != other.ax || 
				ay != other.ay ||
				bx != other.bx ||
				by != other.by);
	}

	bool was_valid	( void ) const
	{
		return (bx > ax && by > ay);
	}
	bool was_border	( const xui_vector<T>& v ) const
	{
		return (v.x == ax  ||
				v.y == ay  ||
				v.x == bx-1||
				v.y == by-1);
	}
	bool was_inside	( const xui_vector<T>& v ) const
	{
		return (v.x >= ax  &&
				v.y >= ay  &&
				v.x <  bx  &&
				v.y <  by);
	}

	void set_include( const xui_vector<T>& v )
	{
		ax = xui_min(ax, v.x);
		ay = xui_min(ay, v.y);
		bx = xui_max(bx, v.x);
		by = xui_max(by, v.y);
	}

	xui_vector<T> get_pt( void ) const
	{
		return xui_vector<T>(ax, ay);
	}
	T			  get_x ( void ) const
	{
		return get_pt().x;
	}
	T			  get_y ( void ) const
	{
		return get_pt().y;
	}
	bool		  was_pt( const xui_vector<T>& pt ) const
	{
		return get_pt() == pt;
	}
	void		  set_pt( const xui_vector<T>& pt )
	{
		xui_vector<T> sz = get_sz();
		ax = pt.x;
		ay = pt.y;
		set_sz(sz);
	}
	void		  set_x ( T x )
	{
		set_pt(xui_vector<T>(x, get_y()));
	}
	void		  set_y ( T y )
	{
		set_pt(xui_vector<T>(get_x(), y));
	}
	void		  oft_x ( T x )
	{
		set_pt(xui_vector<T>(ax+x, ay));
	}
	void		  oft_y ( T y )
	{
		set_pt(xui_vector<T>(ax, ay+y));
	}

	void		  set_w ( T w )
	{
		bx = ax + w;
	}
	void		  set_h ( T h )
	{
		by = ay + h;
	}
	xui_vector<T> get_sz( void ) const
	{
		return xui_vector<T>(bx-ax, by-ay);
	}
	T			  get_w ( void ) const
	{
		return get_sz().w;
	}
	T			  get_h ( void ) const
	{
		return get_sz().h;
	}
	bool		  was_sz( const xui_vector<T>& sz ) const
	{
		return get_sz() == sz;
	}
	void		  set_sz( const xui_vector<T>& sz )
	{
		set_w(sz.w);
		set_h(sz.h);
	}

	xui_rect2d<T> get_inter( const xui_rect2d<T>& other ) const
	{
		xui_rect2d<T> temp;
		temp.ax = (ax > other.ax) ? ax : other.ax;
		temp.ay = (ay > other.ay) ? ay : other.ay;
		temp.bx = (bx < other.bx) ? bx : other.bx;
		temp.by = (by < other.by) ? by : other.by;

		return temp;
	}
	xui_rect2d<T> get_union( const xui_rect2d<T>& other ) const
	{
		if (!was_valid())
			return other;

		xui_rect2d<T> temp;
		temp.ax = (ax < other.ax) ? ax : other.ax;
		temp.ay = (ay < other.ay) ? ay : other.ay;
		temp.bx = (bx > other.bx) ? bx : other.bx;
		temp.by = (by > other.by) ? by : other.by;

		return temp;
	}

	template<typename C>
	xui_rect2d<C> to( void ) const
	{
		return xui_rect2d<C>((C)ax, (C)ay, (C)bx, (C)by);
	}
};

#endif//__xui_rect2d_h__
