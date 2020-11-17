#ifndef __xui_family_create_h__
#define __xui_family_create_h__

#include "xui_family.h"
#include "xui_vector.h"
#include "xui_rect2d.h"
#include "ft2build.h"
#include "freetype/freetype.h"

class xui_family_create
{
public:
	struct ft_info
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

	xui_family_create( void );
	virtual ~xui_family_create( void );

	s32						get_height			( const xui_family& family );
	s32						get_ascender		( const xui_family& family );
	void					add_font			( const xui_family& family );
	ft_info*			    get_info			( const xui_family& family, u16 wc );
    
    void                    release             ( void );

protected:
	ft_info*			    create_char_normal	( const xui_family& family, u16 wc );
	ft_info*			    create_char_stroke	( const xui_family& family, u16 wc );

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

	typedef std::vector<span> 
		spanvec;
	typedef std::map<s32, ft_info*>
		infomap;
	typedef std::map<s32, FT_Face >
		facemap;

	static void			raster				    ( s32 y, s32 count, const FT_Span* spans, void* user );
	static void			render_spans		    ( FT_Library& library, FT_Outline* outline, spanvec* vec );

	FT_Library			m_library;
	infomap			    m_infomap;
	facemap			    m_facemap;
};

#endif//__xui_family_create_h__
