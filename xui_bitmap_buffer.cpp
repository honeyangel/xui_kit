#include "xui_bitmap_buffer.h"

/*
constructor
*/
xui_create_explain(xui_bitmap_buffer)( const xui_vector<s32>& size, u08 format )
{
	m_format = format;
	m_stride = xui_bitmap_format::get_stride(format);
	m_size	 = size;
	m_data   = new u08[size.w*size.h*m_stride];
}

/*
destructor
*/
xui_delete_explain(xui_bitmap_buffer)( void )
{
	delete [] m_data;
}

/*
method
*/
xui_method_explain(xui_bitmap_buffer, get_format,	u08						)( void ) const
{
	return m_format;
}

xui_method_explain(xui_bitmap_buffer, get_stride,	u08						)( void ) const
{
	return m_stride;
}

xui_method_explain(xui_bitmap_buffer, get_size,		const xui_vector<s32>&	)( void ) const
{
	return m_size;
}

xui_method_explain(xui_bitmap_buffer, get_data,		const u08*				)( void ) const
{
	return m_data;
}

xui_method_explain(xui_bitmap_buffer, get_data,		u08*					)( void )
{
	return m_data;
}

/*
operator
*/
xui_method_explain(xui_bitmap_buffer, operator(),	const u08*				)( s32 x, s32 y ) const
{
	return m_data + y * m_size.w + x;
}

xui_method_explain(xui_bitmap_buffer, operator(),	u08*					)( s32 x, s32 y )
{
	return m_data + y * m_size.w + x;
}