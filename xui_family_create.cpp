#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"
#include "freetype/ftstroke.h"
#include "xui_global.h"
#include "xui_family_create.h"

void xui_family_create::raster( s32 y, s32 count, const FT_Span* spans, void* user )
{
	spanvec* vec = (spanvec*)user;
	for (s32 i = 0; i < count; ++i) 
    {
        vec->push_back(span(spans[i].x, y, spans[i].len, spans[i].coverage));
    }
}

void xui_family_create::render_spans( FT_Library& library, FT_Outline* outline, spanvec* vec )
{
	FT_Raster_Params params;
	memset(&params, 0, sizeof(FT_Raster_Params));
	params.flags		= FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans	= xui_family_create::raster;
	params.user			= vec;

	FT_Outline_Render(library, outline, &params);
}

xui_family_create::xui_family_create( void )
{
	FT_Init_FreeType(&m_library);
}

xui_family_create::~xui_family_create( void )
{
    release();
	FT_Done_FreeType(m_library);
}

void xui_family_create::release( void )
{
    for (infomap::iterator itor = m_infomap.begin(); itor != m_infomap.end(); ++itor)
    {
        delete [] ((*itor).second->buffer_normal);
        delete [] ((*itor).second->buffer_stroke);
        delete    ((*itor).second);
    }
    for (facemap::iterator itor = m_facemap.begin(); itor != m_facemap.end(); ++itor)
    {
        FT_Done_Face((*itor).second);
    }

    m_infomap.clear();
    m_facemap.clear();
}

s32 xui_family_create::get_height( const xui_family& family )
{
	add_font(family);
	ft_info* info = m_infomap[family.to_key()];
	return info->height;
}

s32 xui_family_create::get_ascender( const xui_family& family )
{
	add_font(family);
	ft_info* info = m_infomap[family.to_key()];
	return info->ascender;
}

void xui_family_create::add_font( const xui_family& family )
{
	ft_info* info = NULL;
	FT_Face  face = NULL;

    infomap::iterator info_itor = m_infomap.find(family.to_key());
    facemap::iterator face_itor = m_facemap.find(family.face);

	// info
	if (info_itor == m_infomap.end())
	{
		info = new ft_info;
		info->buffer_normal = new u08[256*256];
		info->buffer_stroke = new u08[256*256];
		m_infomap[family.to_key()] = info;
	}
	else
	{
		info = (*info_itor).second;
	}

	if (face_itor == m_facemap.end())
	{
		FT_New_Face(m_library, xui_global::get_fontfile(family.face).c_str(), 0, &face);
		m_facemap[family.face] = face;
	}
	else
	{
		face = (*face_itor).second;
	}

	// face
	if (family.bold)
	{
		FT_Set_Char_Size(face, 0, (family.size << 6) - (family.bold * 2),	96, 96);
		info->height	= ((face->size->metrics.height   + family.bold * 128)	>> 6);
		info->ascender	= ((face->size->metrics.ascender + family.bold * 128)	>> 6);
	}
	else
	{
		FT_Set_Char_Size(face, 0, (family.size << 6),						96, 96);
		info->height	= ((face->size->metrics.height)							>> 6);
		info->ascender	= ((face->size->metrics.ascender)						>> 6);
	}
}

xui_family_create::ft_info* xui_family_create::get_info( const xui_family& family, u16 wc )
{
	if (family.bold)	return create_char_stroke(family, wc);
	else				return create_char_normal(family, wc);
}

xui_family_create::ft_info* xui_family_create::create_char_normal( const xui_family& family, u16 wc )
{
	add_font(family);

	ft_info* info = m_infomap[family.to_key()];
	FT_Face  face = m_facemap[family.face];

	FT_UInt charIndex = FT_Get_Char_Index(face, wc);
	FT_Load_Char(face, wc, FT_LOAD_RENDER);

	FT_Bitmap& bitmap = face->glyph->bitmap;
	info->size.w = bitmap.width;
	info->size.h = bitmap.rows;
	if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
	{
		s32 bitmapWidthInBytes = (info->size.w + 7) >> 3;

		for (s32 h = 0; h < info->size.h; ++h)
		{
			for (s32 w = 0; w < info->size.w; ++w)
			{
				s32 index  = h * info->size.w + w;
				s32 offset = w & 0x7;

				if (bitmap.buffer[(w >> 3) + bitmapWidthInBytes*h] & (128 >> offset))
					info->buffer_normal[index] = 255;
				else
					info->buffer_normal[index] = 0;
			}
		}
	}
	else 
	if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		memcpy(info->buffer_normal, bitmap.buffer, info->size.w*info->size.h * sizeof(u08));
	}
	else
	{
		//This is not supported yet
	}

	info->advance	= face->glyph->metrics.horiAdvance  / 64;
	info->bearing.x = face->glyph->metrics.horiBearingX / 64;
	info->bearing.y = face->glyph->metrics.horiBearingY / 64;

	return info;
}

xui_family_create::ft_info* xui_family_create::create_char_stroke( const xui_family& family, u16 wc )
{
	add_font(family);

	ft_info* info = m_infomap[family.to_key()];
	FT_Face  face = m_facemap[family.face];

	// Load the glyph we are looking for.
	FT_UInt gindex = FT_Get_Char_Index(face, wc);
	if (FT_Load_Glyph(face, gindex, FT_LOAD_NO_BITMAP) == 0)
	{
		if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
		{
			spanvec spanvec_normal;
			render_spans(m_library, &face->glyph->outline, &spanvec_normal);

			// next we need the spans for the outline.
			spanvec spanvec_stroke;

			// Set up a stroker.
			FT_Stroker stroker;
			FT_Stroker_New(m_library, &stroker);
			FT_Stroker_Set(stroker, family.bold * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

			FT_Glyph glyph;
			if (FT_Get_Glyph(face->glyph, &glyph) == 0)
			{
				FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
				if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
				{
					FT_Outline* o = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
					render_spans(m_library, o, &spanvec_stroke);
				}

				FT_Stroker_Done(stroker);
				FT_Done_Glyph(glyph);

				info->advance	= (face->glyph->advance.x				+ (family.bold * 64)) / 64;
				info->bearing.x = (face->glyph->metrics.horiBearingX)  						  / 64;
				info->bearing.y = (face->glyph->metrics.horiBearingY	+ (family.bold * 64)) / 64;

				if (spanvec_normal.size() > 0 && 
					spanvec_stroke.size() > 0)
				{
					xui_rect2d<s32> rt(
						spanvec_normal.front().x, 
						spanvec_normal.front().y, 
						spanvec_normal.front().x, 
						spanvec_normal.front().y);

					for (spanvec::iterator itor = spanvec_normal.begin(); itor != spanvec_normal.end(); ++itor)
					{
						rt.set_include(xui_vector<s32>((*itor).x,				(*itor).y));
						rt.set_include(xui_vector<s32>((*itor).x+(*itor).w-1,	(*itor).y));
					}
					for (spanvec::iterator itor = spanvec_stroke.begin(); itor != spanvec_stroke.end(); ++itor)
					{
						rt.set_include(xui_vector<s32>((*itor).x,				(*itor).y));
						rt.set_include(xui_vector<s32>((*itor).x+(*itor).w-1,	(*itor).y));
					}

					// Get some metrics of our image.
					info->size.w = rt.get_w()+1;
					info->size.h = rt.get_h()+1;

					memset(info->buffer_normal, 0, info->size.w * info->size.h * sizeof(u08));
					for (spanvec::iterator itor = spanvec_normal.begin(); itor != spanvec_normal.end(); ++itor)
					{
						s32 index = (info->size.h - 1 - ((*itor).y - rt.ay)) * info->size.w + (*itor).x - rt.ax;
						for (s32 w = 0; w < (*itor).w; ++w)
							info->buffer_normal[index+w] = (*itor).c;
					}

					memset(info->buffer_stroke, 0, info->size.w * info->size.h * sizeof(u08));
					for (spanvec::iterator itor = spanvec_stroke.begin(); itor != spanvec_stroke.end(); ++itor)
					{
						s32 index = (info->size.h - 1 - ((*itor).y - rt.ay)) * info->size.w + (*itor).x - rt.ax;
						for (s32 w = 0; w < (*itor).w; ++w)
							info->buffer_stroke[index+w] = (*itor).c;
					}
				}
				else
				{
					info->size.w	= 0;
					info->size.h	= 0;
					info->bearing.x	= 0;
					info->bearing.y	= 0;
					info->advance	= info->height;
				}
			}
		}
	}

	return info;
}
