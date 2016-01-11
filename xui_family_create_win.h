#ifndef __xui_family_create_win_h__
#define __xui_family_create_win_h__

#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include "xui_family_create.h"

class xui_family_create_win : public xui_family_create
{
public:
	/*
	//
	*/
	struct gdiinfo 
	{
	public:
		Font*			gdif;
		Bitmap*			gdib;
		Graphics*		gdig;

		xui_vector<s32>	size;
		u08*			bits;
	};

	/*
	//constructor
	*/
	xui_family_create_win( void );

	/*
	//destructor
	*/
	virtual ~xui_family_create_win( void );

	/* 
	//implement
	*/
	virtual void					add_font		( const xui_family& family );
	virtual xui_vector<s32>			get_size		( const xui_family& family );
	virtual void*					get_bits		( const xui_family& family, u16 wc );

protected:
	/*
	member
	*/
	GdiplusStartupInput				m_startup_input;
	ULONG_PTR						m_startup_token;
	std::map<std::string, gdiinfo*>	m_gdiinfo_map;
};

#endif//__xui_family_create_win_h__