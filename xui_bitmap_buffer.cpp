#include "xui_bitmap_buffer.h"
#include "png.h"

void xui_png_read( png_structp png, png_bytep buffer, png_size_t size )
{
    FILE* handle = static_cast<FILE*>(png_get_io_ptr(png));
    fread(buffer, 1, size, handle);
}

xui_bitmap_buffer* xui_bitmap_buffer::create(const std::string& filename)
{
    // file
    FILE*       pio = fopen(filename.c_str(), "rb");
    if (pio == NULL)
        return NULL;

    // create
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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
    png_set_read_fn(png, pio, xui_png_read);
    png_read_info(png, pngInfo);

    // get
    png_byte		depth = png_get_bit_depth(png, pngInfo);
    png_byte		colorType = png_get_color_type(png, pngInfo);
    png_uint_32		width = png_get_image_width(png, pngInfo);
    png_uint_32		height = png_get_image_height(png, pngInfo);

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
            format = xui_bitmap_format::k_pal8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            format = xui_bitmap_format::k_pala8;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            format = xui_bitmap_format::k_r8g8b8a8;
            break;
        }

        png_bytep trans_alpha = NULL;
        int num_trans = 0;
        png_color_16p trans_color = NULL;

        png_get_tRNS(png, pngInfo, &trans_alpha, &num_trans, &trans_color);

        if (colorType == PNG_COLOR_TYPE_RGB)
        {
            if (trans_color)
                format = xui_bitmap_format::k_r8g8b8a8;
            else
                format = xui_bitmap_format::k_r8g8b8;
        }

        if (colorType == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png);

            if (trans_color)
                format = xui_bitmap_format::k_r8g8b8a8;
            else
                format = xui_bitmap_format::k_r8g8b8;
        }
    }

    // not support format
    if (format == xui_bitmap_format::k_unknown)
    {
        png_destroy_read_struct(&png, &pngInfo, NULL);
        return NULL;
    }

    // convert transparency info into RGBA, but only if we have RGB/RGBA, not indexed colors
    png_set_tRNS_to_alpha(png);

    // create image data
    xui_bitmap_buffer* buffer = new xui_bitmap_buffer(xui_vector<s32>(width, height), format);
    u08* pixels = buffer->get_data();

    // make array with pointers to each individual row inside the image buffer
    u32	rowSize = buffer->get_stride() * width;

    png_bytepp	rowPointers = new png_bytep[height];
    for (u32 i = 0; i < height; ++i)
        rowPointers[i] = &(pixels[i*rowSize]);

    // read image
    png_read_image(png, rowPointers);

    // release
    png_read_end(png, NULL);
    png_destroy_read_struct(&png, &pngInfo, NULL);
    delete[] rowPointers;

    return buffer;
}

xui_bitmap_buffer::xui_bitmap_buffer( const xui_vector<s32>& size, u08 format )
{
	m_format = format;
	m_stride = xui_bitmap_format::get_stride(format);
	m_size	 = size;
	m_data   = new u08[size.w*size.h*m_stride];
}

xui_bitmap_buffer::~xui_bitmap_buffer( void )
{
	delete [] m_data;
}

u08 xui_bitmap_buffer::get_format( void ) const
{
	return m_format;
}

u08 xui_bitmap_buffer::get_stride( void ) const
{
	return m_stride;
}

const xui_vector<s32>& xui_bitmap_buffer::get_size( void ) const
{
	return m_size;
}

const u08* xui_bitmap_buffer::get_data( void ) const
{
	return m_data;
}

u08* xui_bitmap_buffer::get_data( void )
{
	return m_data;
}

const u08* xui_bitmap_buffer::operator()( s32 x, s32 y ) const
{
	return m_data + y * m_size.w + x;
}

u08* xui_bitmap_buffer::operator()( s32 x, s32 y )
{
	return m_data + y * m_size.w + x;
}