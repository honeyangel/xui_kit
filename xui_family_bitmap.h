#ifndef __xui_family_bitmap_h__
#define __xui_family_bitmap_h__

#include "xui_family.h"
#include "xui_bitmap.h"

class xui_family_bitmap : public xui_bitmap
{
public:
	xui_family_bitmap( void );
	virtual ~xui_family_bitmap( void );

	xui_family_member*	get_member			( const xui_family& family, u16 wc );
	xui_family_member*	add_member			( const xui_family& family, u16 wc );
	bool				add_member_check	( const xui_vector<s32>& size );

protected:
	xui_rect2d<s32>		upload				( const xui_vector<s32>& size, void* data );

	typedef std::map<u32, xui_family_member*>
		xui_member_map;

	s32					m_render_x;
	s32					m_render_y;
	s32					m_render_h;
	xui_member_map		m_member_map;
};

#endif//__xui_family_bitmap_h__