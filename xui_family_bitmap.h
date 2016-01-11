#ifndef __xui_family_bitmap_h__
#define __xui_family_bitmap_h__

#include "xui_family.h"
#include "xui_bitmap.h"

class xui_family_bitmap : public xui_bitmap
{
public:
	/*
	//constructor
	*/
	xui_family_bitmap( const xui_family& family );

	/*
	//destructor
	*/
	virtual ~xui_family_bitmap( void );

	/*
	//method
	*/
	const xui_family&	get_family			( void ) const;

	/*
	//member
	*/
	xui_family_member*	get_member			( u16 wc );
	xui_family_member*	add_member			( u16 wc );
	void				add_member_check	( const xui_vector<s32>& size );

protected:
	/*
	//typedef
	*/
	typedef std::map<u16, xui_family_member*>
		xui_member_map;

	/*
	//member
	*/
	xui_family			m_family;
	s32					m_render_x;
	s32					m_render_y;
	s32					m_render_h;
	xui_member_map		m_member_map;
};

#endif//__xui_family_bitmap_h__