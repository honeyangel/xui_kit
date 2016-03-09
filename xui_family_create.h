#ifndef __xui_family_create_h__
#define __xui_family_create_h__

#include "xui_family.h"
#include "xui_vector.h"
#include "xui_rect2d.h"
#include "ft2build.h"
#include "freetype/freetype.h"

class xui_family_create
{
	//xui_declare_instance(xui_family_create)

public:
	struct FT_Info
	{
	public:
		s32				height;
		s32				ascender;
		u08*			buffer_normal;
		u08*			buffer_stroke;

		xui_vector<s32> size;
		xui_vector<s32> bearing;
		s32				advance;
	};

	/*
	//constructor
	*/
	xui_family_create( void );

	/*
	//destructor
	*/
	virtual ~xui_family_create( void );

	/*
	//method
	*/
	s32						get_height			( const xui_family& family );
	s32						get_ascender		( const xui_family& family );

	/*
	//override
	*/
	void					add_font			( const xui_family& family );
	FT_Info*				get_info			( const xui_family& family, u16 wc );

protected:
	/*
	//method
	*/
	FT_Info*				create_char_normal	( const xui_family& family, u16 wc );
	FT_Info*				create_char_stroke	( const xui_family& family, u16 wc );

	/*
	//struct
	*/
	struct span
	{
	public:
		span( void ){}
		span( s32 _x, s32 _y, s32 _w, u08 _c )
			: x(_x)
			, y(_y)
			, w(_w)
			, c(_c) 
		{}

		s32 x;
		s32 y;
		s32 w;
		u08 c;
	};

	/*
	//typedef
	*/
	typedef std::vector<span> 
		spanvec;
	typedef std::map<s32, FT_Info*>
		FT_InfoMap;
	typedef std::map<s32, FT_Face >
		FT_FaceMap;

	/*
	//static method
	*/
	static void			raster				( int y, int count, const FT_Span* spans, void* user );
	static void			render_spans		( FT_Library& library, FT_Outline* outline, spanvec* vec );

	/*
	//member
	*/
	FT_Library			m_FTLibrary;
	FT_InfoMap			m_FTInfoMap;
	FT_FaceMap			m_FTFaceMap;
};

#endif//__xui_family_create_h__