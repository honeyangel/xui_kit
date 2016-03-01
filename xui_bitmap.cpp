#include "png.h"
#include "xui_bitmap_buffer.h"
#include "xui_bitmap.h"
#include "xui_family_bitmap.h"

void xui_png_readdata( png_structp png, png_bytep buffer, png_size_t size )
{
	FILE* handle = static_cast<FILE*>(png_get_io_ptr(png));
	fread(buffer, 1, size, handle);
}

/*
// static
*/
typedef std::map<std::string, xui_bitmap*> xui_bitmap_map;
static  xui_bitmap_map g_bitmaps;

/*
// static method
*/
xui_method_explain(xui_bitmap, create,		xui_bitmap*				)( const std::string& filename )
{
	// search
	xui_bitmap_map::iterator itor = g_bitmaps.find(filename);
	if (itor != g_bitmaps.end())
		return (*itor).second;

	// file
	FILE*       pio     = fopen(filename.c_str(), "rb");
	if (pio == NULL)
		return NULL;

	// create
	png_structp png		= png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL)
		return NULL;

	// info
	png_infop   pngInfo = png_create_info_struct(png);
	if (pngInfo == NULL)
	{
		png_destroy_read_struct(&png, NULL, NULL);
		return NULL;
	}

	// header
	png_set_read_fn(png, pio, xui_png_readdata);
	png_read_info  (png, pngInfo);

	// get
	png_byte		depth		= png_get_bit_depth	  (png, pngInfo);
	png_byte		colorType	= png_get_color_type  (png, pngInfo);
	png_uint_32		width		= png_get_image_width (png, pngInfo);
	png_uint_32		height		= png_get_image_height(png, pngInfo);

	// expand
	if (depth == 1 ||
		depth == 2 ||
		depth == 4)
		png_set_expand_gray_1_2_4_to_8(png);

	// format
	u08	format = 0;
	if (depth == 1 ||
		depth == 2 ||
		depth == 4 ||
		depth == 8)
	{
		switch (colorType)
		{
		case PNG_COLOR_TYPE_GRAY:
			format = xui_bitmap_format::PAL8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			format = xui_bitmap_format::PALA8;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			format = xui_bitmap_format::R8G8B8A8;
			break;
		}

        if (colorType == PNG_COLOR_TYPE_RGB)
        {
            if (pngInfo->num_trans)
                format = xui_bitmap_format::R8G8B8A8;
            else
                format = xui_bitmap_format::R8G8B8;
        }

		if (colorType == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png);

			if (pngInfo->num_trans)
				format = xui_bitmap_format::R8G8B8A8;
			else
				format = xui_bitmap_format::R8G8B8;
		}
	}

	// not support format
	if (format == xui_bitmap_format::UNKNOWN)
	{
		png_destroy_read_struct(&png, &pngInfo, NULL);
		return NULL;
	}

	// convert transparency info into RGBA, but only if we have RGB/RGBA, not indexed colors
	png_set_tRNS_to_alpha(png);

	// create image data
	xui_bitmap_buffer* buffer = new xui_bitmap_buffer(xui_vector<s32>(width, height), format);
	u08* pixels	= buffer->get_data();

	// make array with pointers to each individual row inside the image buffer
	u32	rowSize	= buffer->get_stride() * width;

	png_bytepp	rowPointers	= new png_bytep[height];
	for (u32 i = 0; i < height; ++i)
		rowPointers[i] = &(pixels[i*rowSize]);

	// read image
	png_read_image(png, rowPointers);

	// release
	png_read_end(png, NULL);
	png_destroy_read_struct(&png, &pngInfo, NULL);
	delete [] rowPointers;

	// create
	xui_bitmap* bitmap  = new xui_bitmap(
		buffer->get_size(), 
		buffer->get_format(), 
		buffer->get_data());

	g_bitmaps[filename] = bitmap;

	return bitmap;
}

xui_method_explain(xui_bitmap, create,		xui_bitmap*				)( const xui_family&  family   )
{
	// search
	xui_bitmap_map::iterator itor = g_bitmaps.find(family.to_string());
	if (itor != g_bitmaps.end())
		return (*itor).second;

	// create
	xui_bitmap* bitmap = new xui_family_bitmap(family);
	g_bitmaps[family.to_string()] = bitmap;

	return bitmap;
}

/*
// constructor
*/
xui_create_explain(xui_bitmap)( const xui_vector<s32>& size, u08 format, void* data )
{
	tbo_create(size, format, data);
}

/*
// destructor
*/
xui_delete_explain(xui_bitmap)( void )
{
	tbo_delete();
}

/*
// method
*/
xui_method_explain(xui_bitmap, set_bind,	void					)( void )
{
	glEnable	 (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_object);
}

xui_method_explain(xui_bitmap, non_bind,	void					)( void )
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable	 (GL_TEXTURE_2D);
}

xui_method_explain(xui_bitmap, get_size,	const xui_vector<s32>&	)( void ) const
{
	return m_size;
}

xui_method_explain(xui_bitmap, get_object,	GLuint					)( void ) const
{
	return m_object;
}

xui_method_explain(xui_bitmap, get_format,	u08						)( void ) const
{
	return m_format;
}

xui_method_explain(xui_bitmap, get_buffer,	void					)( const xui_rect2d<s32>& rect, void* data )
{
	//TODO
}

xui_method_explain(xui_bitmap, set_buffer,	void					)( const xui_rect2d<s32>& rect, void* data )
{
	GLint oldAlignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldAlignment);
	if (oldAlignment != 1)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	xui_vector<s32> pt = rect.get_pt();
	xui_vector<s32> sz = rect.get_sz();
	set_bind();
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		pt.x,
		pt.y,
		sz.w,
		sz.h,
		ogl_format(),
		ogl_typeof(),
		data);

	if (oldAlignment != 1)
		glPixelStorei(GL_UNPACK_ALIGNMENT, oldAlignment);
}

xui_method_explain(xui_bitmap, tbo_create,	void					)( const xui_vector<s32>& size, u08 format, void* data )
{
	m_size   = size;
	m_format = format;

	GLint oldAlignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldAlignment);
	if (oldAlignment != 1)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_object);
	set_bind();
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		ogl_format(),
		size.w,
		size.h,
		0,
		ogl_format(),
		ogl_typeof(),
		data);

	if (oldAlignment != 1)
		glPixelStorei(GL_UNPACK_ALIGNMENT, oldAlignment);

	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_REPEAT );

	//env
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	non_bind();
}

xui_method_explain(xui_bitmap, tbo_delete,	void					)( void )
{
	glDeleteTextures(1, &m_object);
}

xui_method_explain(xui_bitmap, ogl_format,	GLenum					)( void )
{
	switch (m_format)
	{
	case xui_bitmap_format::R8G8B8:
		return GL_RGB;
	case xui_bitmap_format::R8G8B8A8:
		return GL_RGBA;
	case xui_bitmap_format::PAL8:
		return GL_LUMINANCE;
	case xui_bitmap_format::PALA8:
		return GL_LUMINANCE_ALPHA;
	case xui_bitmap_format::A8:
		return GL_ALPHA;
	}

	return 0;
}

xui_method_explain(xui_bitmap, ogl_typeof,	GLenum					)( void )
{
	switch (m_format)
	{
	case xui_bitmap_format::R8G8B8:
	case xui_bitmap_format::R8G8B8A8:
	case xui_bitmap_format::PAL8:
	case xui_bitmap_format::PALA8:
	case xui_bitmap_format::A8:
		return GL_UNSIGNED_BYTE;
	}

	return 0;
}