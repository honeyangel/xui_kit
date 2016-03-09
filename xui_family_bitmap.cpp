#include "xui_convas.h"
#include "xui_bitmap_format.h"
#include "xui_family_create.h"
#include "xui_family_member.h"
#include "xui_family_bitmap.h"

xui_family_create* g_family_create;

/*
//constructor
*/
xui_create_explain(xui_family_bitmap)( void )
: xui_bitmap(xui_vector<s32>(1024, 1024), xui_bitmap_format::A8, NULL)
{
	m_render_x	= 0;
	m_render_y	= 0;
	m_render_h	= 0;
}

/*
//destructor
*/
xui_delete_explain(xui_family_bitmap)( void )
{
	for (xui_member_map::iterator itor = m_member_map.begin(); itor != m_member_map.end(); ++itor)
		delete (*itor).second;
}

/*
//method
*/
xui_method_explain(xui_family_bitmap, get_member,		xui_family_member*	)( const xui_family& family, u16 wc )
{
	u32 key = (u32)(family.face << 28) |
			  (u32)(family.size << 20) |
			  (u32)(family.bold << 16) | 
			  (u32)wc;

	xui_member_map::iterator itor = m_member_map.find(key);
	if (itor != m_member_map.end())
		return (*itor).second;

	return add_member(family, wc);
}
xui_method_explain(xui_family_bitmap, add_member,		xui_family_member*	)( const xui_family& family, u16 wc )
{
	xui_family_create::FT_Info* info = xui_convas::get_ins()->get_family_create()->get_info(family, wc);
	if (add_member_check(info->size) == false)
		return NULL;

	//new char
	xui_family_member* member = new xui_family_member;
	member->wc = wc;
	member->advance = info->advance;
	member->bearing = info->bearing;
	member->bitmap	= this;

	// upload
	if (family.bold)
	{
		member->normal = upload(info->size, info->buffer_normal);
		member->stroke = upload(info->size, info->buffer_stroke);
	}
	else
	{
		member->normal = upload(info->size, info->buffer_normal);
		member->stroke = member->normal;
	}

	// add
	u32 key = (u32)(family.face << 28) |
			  (u32)(family.size << 20) |
			  (u32)(family.bold << 16) | 
			  (u32)wc;

	m_member_map[key] = member;

	return member;
}
xui_method_explain(xui_family_bitmap, add_member_check, bool				)( const xui_vector<s32>& size )
{
	if (size.w*2 < get_size().w - m_render_x &&
		size.h   < get_size().h - m_render_y)
		return true;
	if (size.h   < get_size().h - m_render_y - m_render_h)
		return true;

	return false;
}
xui_method_explain(xui_family_bitmap, upload,			xui_rect2d<s32>		)( const xui_vector<s32>& size, void* data )
{
	//compute available
	if (m_render_x  > get_size().w - size.w)
	{
		m_render_x  = 0;
		m_render_y += m_render_h;
		m_render_h  = 0;
	}

	xui_rect2d<s32> rt;
	rt.ax = m_render_x;
	rt.ay = m_render_y;
	rt.set_sz(size);
	set_buffer(rt, data);

	m_render_x += size.w;
	m_render_h  = xui_max(m_render_h, size.h);

	return rt;
}