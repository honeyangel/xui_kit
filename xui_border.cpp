#include "xui_control.h"
#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_border.h"

xui_implement_rtti(xui_border, xui_component);

/*
//constructor
*/
xui_create_explain(xui_border)( const xui_vector<s32>& size, xui_component* parent, u08 resize )
: xui_component(size, parent)
{
	m_resize	= resize;
	m_downpt	= xui_vector<s32>(0);
	m_backcolor = xui_colour(0.5f, 0.0f, 0.0f, 0.0f);

	//dock
	switch(m_resize)
	{
	case RESIZEFLAG_L: m_dockstyle = DOCKSTYLE_L; break;
	case RESIZEFLAG_T: m_dockstyle = DOCKSTYLE_T; break;
	case RESIZEFLAG_R: m_dockstyle = DOCKSTYLE_R; break;
	case RESIZEFLAG_B: m_dockstyle = DOCKSTYLE_B; break;
	}

	//cursor
	switch (m_resize)
	{
	case RESIZEFLAG_L:
	case RESIZEFLAG_R:
		m_cursor = CURSOR_WE;
		break;
	case RESIZEFLAG_T:
	case RESIZEFLAG_B:
		m_cursor = CURSOR_NS;
		break;
	}
}

/*
//flag
*/
xui_method_explain(xui_border, get_resize,		u08	)( void ) const
{
	return m_resize;
}

/*
//callback
*/
xui_method_explain(xui_border, on_mousedown,	void)( xui_method_mouse& args )
{
	xui_component::on_mousedown(args);
	if (args.mouse == MB_L)
	{
		m_downpt = args.point;
	}
}
xui_method_explain(xui_border, on_mouserise,	void)( xui_method_mouse& args )
{
	xui_component::on_mouserise(args);
	if (args.mouse == MB_L && m_parent)
	{
		xui_vector<s32> pt = m_parent->get_renderpt();
		xui_vector<s32> sz = m_parent->get_rendersz();
		xui_vector<s32> deltapt(0);
		xui_vector<s32> deltasz(0);

		switch (m_resize)
		{
		case RESIZEFLAG_L: deltasz.w = m_downpt.x - args.point.x; break;
		case RESIZEFLAG_T: deltasz.h = m_downpt.y - args.point.y; break;
		case RESIZEFLAG_R: deltasz.w = args.point.x - m_downpt.x; break;
		case RESIZEFLAG_B: deltasz.h = args.point.y - m_downpt.y; break;
		}

		switch (m_resize)
		{
		case RESIZEFLAG_L: deltapt.x = args.point.x - m_downpt.x; break;
		case RESIZEFLAG_T: deltapt.y = args.point.y - m_downpt.y; break;
		}

		m_parent->set_renderpt(pt+deltapt);
		m_parent->set_rendersz(sz+deltasz);
	}
}
xui_method_explain(xui_border, on_topdraw,		void)( xui_method_args&  args )
{
	xui_component::on_topdraw(args);
	if (m_parent)
	{
		xui_vector<s32> pt = g_desktop->get_mousecurr();
		xui_rect2d<s32> rt = get_renderrtabs();
		switch (m_resize)
		{
		case RESIZEFLAG_L:
		case RESIZEFLAG_R:
			rt.oft_x(pt.x - m_downpt.x);
			break;
		case RESIZEFLAG_T:
		case RESIZEFLAG_B:
			rt.oft_y(pt.y - m_downpt.y);
			break;
		}

		xui_convas::get_ins()->fill_rectangle(rt, m_backcolor);
	}
}