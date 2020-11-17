#include "xui_bitmap_buffer.h"
#include "xui_bitmap.h"
#include "xui_family_bitmap.h"

xui_bitmap_map xui_bitmap::s_bitmap_map;

xui_bitmap* xui_bitmap::create( const std::string& filename )
{
	xui_bitmap_map::iterator itor = s_bitmap_map.find(filename);
	if (itor != s_bitmap_map.end())
		return (*itor).second;

    xui_bitmap_buffer* buffer = xui_bitmap_buffer::create(filename);
    if (buffer == NULL)
        return NULL;

	xui_bitmap* bitmap  = new xui_bitmap(
		buffer->get_size(), 
		buffer->get_format(), 
		buffer->get_data());

	s_bitmap_map[filename] = bitmap;

	return bitmap;
}

xui_bitmap::xui_bitmap( const xui_vector<s32>& size, u08 format, void* data )
{
	tbo_create(size, format, data);
}

xui_bitmap::~xui_bitmap( void )
{
	tbo_delete();
}

void xui_bitmap::set_bind( void )
{
	glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_object);
}

void xui_bitmap::non_bind( void )
{
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const xui_vector<s32>& xui_bitmap::get_size( void ) const
{
	return m_size;
}

GLuint xui_bitmap::get_object( void ) const
{
	return m_object;
}

u08 xui_bitmap::get_format( void ) const
{
	return m_format;
}

void xui_bitmap::get_buffer( const xui_rect2d<s32>& rect, void* data )
{
	//TODO
}

void xui_bitmap::set_buffer( const xui_rect2d<s32>& rect, void* data )
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

void xui_bitmap::tbo_create( const xui_vector<s32>& size, u08 format, void* data )
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

void xui_bitmap::tbo_delete( void )
{
	glDeleteTextures(1, &m_object);
}

GLenum xui_bitmap::ogl_format( void )
{
	switch (m_format)
	{
	case xui_bitmap_format::k_r8g8b8:
		return GL_RGB;
	case xui_bitmap_format::k_r8g8b8a8:
		return GL_RGBA;
	case xui_bitmap_format::k_pal8:
		return GL_LUMINANCE;
	case xui_bitmap_format::k_pala8:
		return GL_LUMINANCE_ALPHA;
	case xui_bitmap_format::k_a8:
		return GL_ALPHA;
	}

	return 0;
}

GLenum xui_bitmap::ogl_typeof( void )
{
	switch (m_format)
	{
	case xui_bitmap_format::k_r8g8b8:
	case xui_bitmap_format::k_r8g8b8a8:
	case xui_bitmap_format::k_pal8:
	case xui_bitmap_format::k_pala8:
	case xui_bitmap_format::k_a8:
		return GL_UNSIGNED_BYTE;
	}

	return 0;
}