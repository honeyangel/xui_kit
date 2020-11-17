#ifndef __xui_bitmap_h__
#define __xui_bitmap_h__

#include "xui_vector.h"
#include "xui_rect2d.h"
#include "xui_family.h"

typedef std::map<std::string, xui_bitmap*> xui_bitmap_map;
class xui_bitmap
{
public:
	xui_bitmap( const xui_vector<s32>& size, u08 format, void* data );
	virtual ~xui_bitmap( void );

	static xui_bitmap*		create		( const std::string& filename );

	void					set_bind	( void );
	void					non_bind	( void );
	const xui_vector<s32>&	get_size	( void ) const;
	GLuint					get_object	( void ) const;
	u08						get_format	( void ) const;
	void					get_buffer	( const xui_rect2d<s32>& rect, void* data );
	void					set_buffer	( const xui_rect2d<s32>& rect, void* data );

protected:
	void					tbo_create	( const xui_vector<s32>& size, u08 format, void* data );
	void					tbo_delete	( void );

	GLenum					ogl_format	( void );
	GLenum					ogl_typeof	( void );

	xui_vector<s32>			m_size;
	u08						m_format;
	GLuint					m_object;

    static  xui_bitmap_map  s_bitmap_map;
};

#endif//__xui_bitmap_h__