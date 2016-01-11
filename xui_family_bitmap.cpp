#include "xui_bitmap_format.h"
#include "xui_family_create.h"
#include "xui_family_member.h"
#include "xui_family_bitmap.h"

xui_family_create* g_family_create;

/*
//constructor
*/
xui_create_explain(xui_family_bitmap)( const xui_family& family )
: xui_bitmap(xui_vector<s32>(1024, 1024), xui_bitmap_format::A8, NULL)
{
	m_family	= family;
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
xui_method_explain(xui_family_bitmap, get_family,		const xui_family&	)( void ) const
{
	return m_family;
}

xui_method_explain(xui_family_bitmap, get_member,		xui_family_member*	)( u16 wc )
{
	xui_member_map::iterator itor = m_member_map.find(wc);
	if (itor != m_member_map.end())
		return (*itor).second;

	return add_member(wc);
}

xui_method_explain(xui_family_bitmap, add_member,		xui_family_member*	)( u16 wc )
{
	void*			 bits = g_family_create->get_bits(m_family, wc);
	xui_vector<s32>  size = g_family_create->get_size(m_family);
	add_member_check(size);

	//compute available
	if (m_render_x  > 1024 - size.w)
	{
		m_render_x  = 0;
		m_render_y += m_render_h;
		m_render_h  = 0;
	}

	xui_rect2d<s32> rt;
	rt.set_pt(xui_vector<s32>(m_render_x, m_render_y));
	rt.set_sz(size);

	//upload
	set_buffer(rt, bits);

	//create
	xui_family_member* member = new xui_family_member;
	member->wc	   = wc;
	member->rt	   = rt;
	member->bitmap = this;

	//next
	m_render_x += size.w;
	m_render_h  = size.h > m_render_h ? size.h : m_render_h;

	//add
	m_member_map[wc] = member;

	return member;
}

xui_method_explain(xui_family_bitmap, add_member_check, void				)( const xui_vector<s32>& size )
{
	if (size.w*2 < 1024 - m_render_x)
		return;
	if (size.h*2 < 1024 - m_render_y - m_render_h)
		return;

	//reset
	for (xui_member_map::iterator itor = m_member_map.begin(); itor != m_member_map.end(); ++itor)
		delete (*itor).second;

	m_member_map.clear();
	m_render_x = 0;
	m_render_y = 0;
	m_render_h = 0;
}
