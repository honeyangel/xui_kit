#ifndef __xui_bitmap_buffer_h__
#define __xui_bitmap_buffer_h__

#include "xui_vector.h"
#include "xui_bitmap_format.h"

class xui_bitmap_buffer
{
public:
	/*
	constructor
	*/
	xui_bitmap_buffer( const xui_vector<s32>& size, u08 format );

	/*
	destructor
	*/
	virtual ~xui_bitmap_buffer( void );

	/*
	method
	*/
	u08						get_format	( void ) const;
	u08						get_stride	( void ) const;
	const xui_vector<s32>&	get_size	( void ) const;
	const u08*				get_data	( void ) const;
		  u08*				get_data	( void );

	/*
	operator
	*/
	const u08*				operator()	( s32 x, s32 y ) const;
		  u08*				operator()	( s32 x, s32 y );

protected:
	/*
	protect default constructor
	*/
	xui_bitmap_buffer( void ){}; 

protected:
	/*
	member
	*/
	u08						m_format;
	u08						m_stride;
	xui_vector<s32>			m_size;
	u08*					m_data;
};

#endif//__xui_bitmap_buffer_h__