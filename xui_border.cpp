#include "xui_control.h"
#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_border.h"

/*
//constructor
*/
xui_create_explain(xui_border)( const std::string& name, const xui_rect2d<s32>& rect, u08 flag, xui_control* parent )
: xui_component(name, rect)
{
	m_type	+= "border";
	m_flag	 = flag;
	m_down	 = xui_vector<s32>(0);
	m_parent = parent;

	//dock
	switch(m_flag)
	{
	case RESIZEFLAG_L: m_dockstyle = DOCKSTYLE_L; break;
	case RESIZEFLAG_T: m_dockstyle = DOCKSTYLE_T; break;
	case RESIZEFLAG_R: m_dockstyle = DOCKSTYLE_R; break;
	case RESIZEFLAG_B: m_dockstyle = DOCKSTYLE_B; break;
	}

	//cursor
	switch (m_flag)
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
xui_method_explain(xui_border, get_flag,		u08	)( void ) const
{
	return m_flag;
}

/*
//callback
*/
xui_method_explain(xui_border, on_mousedown,	void)( xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		m_down = args.point;
	}
}

xui_method_explain(xui_border, on_mouserise,	void)( xui_method_mouse& args )
{
	if (m_parent)
	{
		if (args.mouse == MB_L)
		{
			xui_vector<s32> pt = m_parent->get_renderpt();
			xui_vector<s32> sz = m_parent->get_rendersz();
			xui_vector<s32> deltapt(0);
			xui_vector<s32> deltasz(0);

			switch (m_flag)
			{
			case RESIZEFLAG_L: deltasz.w = m_down.x - args.point.x; break;
			case RESIZEFLAG_T: deltasz.h = m_down.y - args.point.y; break;
			case RESIZEFLAG_R: deltasz.w = args.point.x - m_down.x; break;
			case RESIZEFLAG_B: deltasz.h = args.point.y - m_down.y; break;
			}

			switch (m_flag)
			{
			case RESIZEFLAG_L: deltapt.x = args.point.x - m_down.x; break;
			case RESIZEFLAG_T: deltapt.y = args.point.y - m_down.y; break;
			}

			m_parent->set_renderpt(pt+deltapt);
			m_parent->set_rendersz(sz+deltasz);
		}
	}
}

xui_method_explain(xui_border, on_topdraw,		void)( xui_method_args&  args )
{
	if (m_parent)
	{
		xui_vector<s32> curr = g_desktop->get_mousecurr();
		xui_rect2d<s32> rt   = get_renderrtabs();
		switch (m_flag)
		{
		case RESIZEFLAG_L:
		case RESIZEFLAG_R:
			rt.oft_x(curr.x - m_down.x);
			break;
		case RESIZEFLAG_T:
		case RESIZEFLAG_B:
			rt.oft_y(curr.y - m_down.y);
			break;
		}

		g_convas->fill_rectangle(rt, xui_colour(0.5f, 0.0f, 0.0f, 0.0f));
	}
}