#ifndef __xui_bitmap_h__
#define __xui_bitmap_h__

#include "xui_vector.h"
#include "xui_rect2d.h"
#include "xui_family.h"

class xui_bitmap
{
public:
	/*
	constructor
	*/
	xui_bitmap( const xui_vector<s32>& size, u08 format, void* data );

	/*
	destructor
	*/
	virtual ~xui_bitmap( void );

	/*
	static
	*/
	static xui_bitmap*		create		( const std::string& filename );
	//static xui_bitmap*		create		( const xui_family&  family   );

	/*
	method
	*/
	void					set_bind	( void );
	void					non_bind	( void );
	const xui_vector<s32>&	get_size	( void ) const;
	GLuint					get_object	( void ) const;
	u08						get_format	( void ) const;
	void					get_buffer	( const xui_rect2d<s32>& rect, void* data );
	void					set_buffer	( const xui_rect2d<s32>& rect, void* data );

protected:
	/*
	method inner
	*/
	void					tbo_create	( const xui_vector<s32>& size, u08 format, void* data );
	void					tbo_delete	( void );

	GLenum					ogl_format	( void );
	GLenum					ogl_typeof	( void );

	/*
	member
	*/
	xui_vector<s32>			m_size;
	u08						m_format;
	GLuint					m_object;
};

#endif//__xui_bitmap_h__