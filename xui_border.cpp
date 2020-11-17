#include "xui_control.h"
#include "xui_desktop.h"
#include "xui_canvas.h"
#include "xui_border.h"

xui_implement_rtti(xui_border, xui_component)

xui_border::xui_border( const xui_vector<s32>& size, u08 resize )
: xui_component(size)
{
	m_resize	= resize;
	m_downpt	= xui_vector<s32>(0);
	m_backcolor = xui_colour(0.5f, 0.0f, 0.0f, 0.0f);

	//dock
	switch(m_resize)
	{
	case k_resizeflag_l: m_dockstyle = k_dockstyle_l; break;
	case k_resizeflag_t: m_dockstyle = k_dockstyle_t; break;
	case k_resizeflag_r: m_dockstyle = k_dockstyle_r; break;
	case k_resizeflag_b: m_dockstyle = k_dockstyle_b; break;
	}

	//cursor
	switch (m_resize)
	{
	case k_resizeflag_l:
	case k_resizeflag_r:
		m_cursor = k_cursor_we;
		break;
	case k_resizeflag_t:
	case k_resizeflag_b:
		m_cursor = k_cursor_ns;
		break;
	}
}

u08 xui_border::get_resize( void ) const
{
	return m_resize;
}

void xui_border::on_mousedown( xui_method_mouse& args )
{
	xui_component::on_mousedown(args);
	if (args.mouse == k_mb_left)
	{
		m_downpt = args.point;
	}
}

void xui_border::on_mouserise( xui_method_mouse& args )
{
	xui_component::on_mouserise(args);
	if (args.mouse == k_mb_left && m_parent)
	{
		xui_vector<s32> pt = m_parent->get_renderpt();
		xui_vector<s32> sz = m_parent->get_rendersz();
		xui_vector<s32> deltapt(0);
		xui_vector<s32> deltasz(0);

		switch (m_resize)
		{
		case k_resizeflag_l: deltasz.w = m_downpt.x - args.point.x; break;
		case k_resizeflag_t: deltasz.h = m_downpt.y - args.point.y; break;
		case k_resizeflag_r: deltasz.w = args.point.x - m_downpt.x; break;
		case k_resizeflag_b: deltasz.h = args.point.y - m_downpt.y; break;
		}

		switch (m_resize)
		{
		case k_resizeflag_l: deltapt.x = args.point.x - m_downpt.x; break;
		case k_resizeflag_t: deltapt.y = args.point.y - m_downpt.y; break;
		}

		m_parent->set_renderpt(pt+deltapt);
		m_parent->set_rendersz(sz+deltasz);
	}
}

void xui_border::on_topdraw( xui_method_args& args )
{
	xui_component::on_topdraw(args);
	if (m_parent)
	{
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_rect2d<s32> rt = get_renderrtabs();
		switch (m_resize)
		{
		case k_resizeflag_l:
		case k_resizeflag_r:
			rt.oft_x(pt.x - m_downpt.x);
			break;
		case k_resizeflag_t:
		case k_resizeflag_b:
			rt.oft_y(pt.y - m_downpt.y);
			break;
		}

		xui_canvas::get_ins()->fill_rectangle(rt, m_backcolor);
	}
}