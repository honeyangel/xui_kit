#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_window.h"
#include "xui_component.h"

xui_implement_root(xui_component);

/*
//constructor
*/
xui_create_explain(xui_component)( const xui_vector<s32>& size, xui_component* parent )
{
	m_data		= NULL;
	m_cursor	= CURSOR_DEFAULT;
	m_parent	= parent;
	m_render	= xui_rect2d<s32>(xui_vector<s32>(0), size);
	m_enable	= true;
	m_visible	= true;
	m_invalid	= false;

	m_alignhorz = 0;
	m_alignvert = 0;
	m_dockstyle = 0;
	m_backalpha = 1.0f;
	m_backcolor = xui_colour::transparent;
	m_maskcolor = xui_colour::white;
}

/*
//destructor
*/
xui_delete_explain(xui_component)( void )
{}

/*
//ini
*/
xui_method_explain(xui_component, ini_component,		void					)( bool enable, bool visible )
{
	m_enable	= enable;
	m_visible	= visible;
}
xui_method_explain(xui_component, ini_component,		void					)( u08 alignhorz, u08 alignvert, u08 dockstyle )
{
	m_alignhorz = alignhorz;
	m_alignvert = alignvert;
	m_dockstyle = dockstyle;
}

/*
//name
*/
xui_method_explain(xui_component, get_name,				const std::string&		)( void ) const
{
	return m_name;
}
xui_method_explain(xui_component, set_name,				void					)( const std::string& name )
{
	m_name = name;
}

/*
//data
*/
xui_method_explain(xui_component, get_data,				void*					)( void )
{
	return m_data;
}
xui_method_explain(xui_component, set_data,				void					)( void* data )
{
	m_data = data;
}

/*
//cursor
*/
xui_method_explain(xui_component, get_cursor,			u32						)( void ) const
{
	return m_cursor;
}
xui_method_explain(xui_component, set_cursor,			void					)( u32 cursor )
{
	m_cursor = cursor;
}

/*
//parent
*/
xui_method_explain(xui_component, get_parent,			xui_component*			)( void )
{
	return m_parent;
}
xui_method_explain(xui_component, set_parent,			void					)( xui_component* component )
{
	m_parent = component;
}
xui_method_explain(xui_component, was_parent,			bool					)( xui_component* component ) const
{
	return (m_parent && m_parent == component);
}
xui_method_explain(xui_component, was_ancestor,			bool					)( xui_component* component ) const
{
	if (m_parent == NULL)
		return false;

	return (m_parent == component) || m_parent->was_ancestor(component);
}

/*
//window
*/
xui_method_explain(xui_component, get_window,			xui_window*				)( void )
{
	xui_component* parent = this;
	while (parent)
	{
		xui_window* window = xui_dynamic_cast(xui_window, parent);
		if (window)
			return window;

		parent = parent->get_parent();
	}

	return NULL;
}

/*
//enable
*/
xui_method_explain(xui_component, was_enable,			bool					)( void ) const
{
	return m_enable;
}
xui_method_explain(xui_component, set_enable,			void					)( bool flag )
{
	if (m_enable != flag)
	{
		m_enable  = flag;

		xui_method_args args;
		if (m_enable)	{ on_free(args); xm_free(this, args); } 
		else			{ on_lock(args); xm_lock(this, args); } 
	}
}

/*
//visible
*/
xui_method_explain(xui_component, was_visible,			bool					)( void ) const
{
	return m_visible;
}
xui_method_explain(xui_component, set_visible,			void					)( bool flag )
{
	if (m_visible != flag)
	{
		m_visible  = flag;

		xui_method_args args;
		if (m_visible)	{ on_show(args); xm_show(this, args); }
		else			{ on_hide(args); xm_hide(this, args); } 

		//刷新父控件
		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->refresh();
	}
}

/*
//focus
*/
xui_method_explain(xui_component, has_focus,			bool					)( void ) const
{
	return g_desktop->get_focusctrl() == this;
}
xui_method_explain(xui_component, req_focus,			void					)( void )
{
	g_desktop->post_message(xui_message(this, XM_GETFOCUS, 0, 0));
}
xui_method_explain(xui_component, has_catch,			bool					)( void ) const
{
	return g_desktop->get_catchctrl() == this;
}
xui_method_explain(xui_component, req_catch,			void					)( void )
{
	g_desktop->post_message(xui_message(this, XM_GETCATCH, 0, 0));
}
xui_method_explain(xui_component, was_hover,			bool					)( void ) const
{
	return g_desktop->get_hoverctrl() == this;
}

/*
//background
*/
xui_method_explain(xui_component, get_backalpha,		f32						)( void ) const
{
	return m_backalpha;
}
xui_method_explain(xui_component, set_backalpha,		void					)( f32 alpha )
{
	m_backalpha = alpha;
}
xui_method_explain(xui_component, get_backcolor,		const xui_colour&		)( void ) const
{
	return m_backcolor;
}
xui_method_explain(xui_component, set_backcolor,		void					)( const xui_colour& color )
{
	m_backcolor = color;
}
xui_method_explain(xui_component, get_maskcolor,		const xui_colour&		)( void ) const
{
	return m_maskcolor;
}
xui_method_explain(xui_component, set_maskcolor,		void					)( const xui_colour& color )
{
	m_maskcolor = color;
}

/*
//layout
*/
xui_method_explain(xui_component, get_alignhorz,		u08						)( void ) const
{
	return m_alignhorz;
}
xui_method_explain(xui_component, set_alignhorz,		void					)( u08 alignhorz )
{
	if (m_alignhorz != alignhorz)
	{
		m_alignhorz  = alignhorz;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}
xui_method_explain(xui_component, get_alignvert,		u08						)( void ) const
{
	return m_alignvert;
}
xui_method_explain(xui_component, set_alignvert,		void					)( u08 alignvert )
{
	if (m_alignvert != alignvert)
	{
		m_alignvert  = alignvert;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}
xui_method_explain(xui_component, get_dockstyle,		u08						)( void ) const
{
	return m_dockstyle;
}
xui_method_explain(xui_component, set_dockstyle,		void					)( u08 dockstyle )
{
	if (m_dockstyle != dockstyle)
	{
		m_dockstyle  = dockstyle;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}

/*
//location
*/
xui_method_explain(xui_component, get_renderx,			s32						)( void ) const
{
	return m_render.ax;
}
xui_method_explain(xui_component, get_rendery,			s32						)( void ) const
{
	return m_render.ay;
}
xui_method_explain(xui_component, get_renderpt,			xui_vector<s32>			)( void ) const
{
	return m_render.get_pt();
}
xui_method_explain(xui_component, set_renderx,			void					)( s32 x )
{
	set_renderpt(xui_vector<s32>(x, m_render.ay));
}
xui_method_explain(xui_component, set_rendery,			void					)( s32 y )
{
	set_renderpt(xui_vector<s32>(m_render.ax, y));
}
xui_method_explain(xui_component, set_renderpt,			void					)( const xui_vector<s32>& pt )
{
	if (m_render.was_pt(pt) == false)
	{
		m_render.set_pt(pt);

		xui_method_args      args; 
		on_setrenderpt(      args); 
		xm_setrenderpt(this, args);

		if (m_parent)
			m_parent->invalid();
	}
}
xui_method_explain(xui_component, get_renderw,			s32						)( void ) const
{
	return m_render.get_sz().w;
}
xui_method_explain(xui_component, get_renderh,			s32						)( void ) const
{
	return m_render.get_sz().h;
}
xui_method_explain(xui_component, get_rendersz,			xui_vector<s32>			)( void ) const
{
	return m_render.get_sz();
}
xui_method_explain(xui_component, set_renderw,			void					)( s32 w )
{
	set_rendersz(xui_vector<s32>(w, m_render.get_sz().h));
}
xui_method_explain(xui_component, set_renderh,			void					)( s32 h )
{
	set_rendersz(xui_vector<s32>(m_render.get_sz().w, h));
}
xui_method_explain(xui_component, set_rendersz,			void					)( const xui_vector<s32>& sz )
{
	if (m_render.was_sz(sz) == false)
	{
		m_render.set_sz(sz);

		xui_method_args	     args; 
		on_setrendersz(      args); 
		xm_setrendersz(this, args);

		if (m_parent)
			m_parent->invalid();
	}
}
xui_method_explain(xui_component, get_renderpt,			xui_vector<s32>			)( const xui_vector<s32>& pt ) const
{
	return pt - get_screenpt();
}
xui_method_explain(xui_component, get_screenpt,			xui_vector<s32>			)( void ) const
{
	xui_vector<s32> pt = m_render.get_pt();
	return (m_parent == NULL) 
		? pt 
		: pt + m_parent->get_screenpt();
}

/*
//color
*/
xui_method_explain(xui_component, get_vertexcolor,		xui_colour				)( void ) const
{
	xui_colour color(
		m_backalpha,
		m_maskcolor.r,
		m_maskcolor.g,
		m_maskcolor.b);

	return (m_parent == NULL)
		? color
		: color * m_parent->get_vertexcolor();
}
xui_method_explain(xui_component, get_rendercolor,		xui_colour				)( void ) const
{
	return m_backcolor;
}

/*
//rectangle
*/
xui_method_explain(xui_component, get_renderrt,			xui_rect2d<s32>			)( void ) const
{
	return xui_rect2d<s32>(xui_vector<s32>(0, 0), get_rendersz());
}
xui_method_explain(xui_component, get_renderrtabs,		xui_rect2d<s32>			)( void ) const
{
	return get_renderrt() + get_screenpt();
}

/*
//hit
*/
xui_method_explain(xui_component, choose,				xui_component*			)( const xui_vector<s32>& pt )
{
	//判定区域
	if (m_enable && m_visible)
	{
		if (m_render.was_inside(pt))
			return this;
	}

	return NULL;
}

/*
//perform
*/
xui_method_explain(xui_component, invalid,				void					)( void )
{
	m_invalid = true;
}
xui_method_explain(xui_component, refresh,				void					)( void )
{
	m_invalid = false;

	xui_method_args  args; 
	on_invalid(      args); 
	xm_invalid(this, args);
}
xui_method_explain(xui_component, perform,				void					)( void )
{
	xui_method_args  args;
	on_perform(      args);
	xm_perform(this, args);
}

/*
//update&render
*/
xui_method_explain(xui_component, update,				void					)( f32 delta )
{
	if (m_invalid) 
		refresh();

	xui_method_args     args; 
	on_updateself(      args);
	xm_updateself(this, args);
}
xui_method_explain(xui_component, render,				void					)( void )
{
	xui_method_args     args; 
	on_renderback(      args);
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtabs()));
	on_renderself(      args);
	xm_renderself(this, args);
	xui_convas::get_ins()->set_cliprect(cliprect);
}

/*
//perform set render method
*/
xui_method_explain(xui_component, on_perform_x,			void					)( s32 x )
{
	on_perform_pt(x, m_render.ay);
}
xui_method_explain(xui_component, on_perform_y,			void					)( s32 y )
{
	on_perform_pt(m_render.ax, y);
}
xui_method_explain(xui_component, on_perform_pt,		void					)( s32 x, s32 y )
{
	on_perform_pt(xui_vector<s32>(x, y));
}
xui_method_explain(xui_component, on_perform_pt,		void					)( const xui_vector<s32>& pt )
{
	if (m_render.get_pt() != pt)
	{
		m_render.set_pt(pt);

		xui_method_args      args; 
		on_setrenderpt(      args); 
		xm_setrenderpt(this, args);
	}
}
xui_method_explain(xui_component, on_perform_w,			void					)( s32 w )
{
	on_perform_sz(w, m_render.get_h());
}
xui_method_explain(xui_component, on_perform_h,			void					)( s32 h )
{
	on_perform_sz(m_render.get_w(), h);
}
xui_method_explain(xui_component, on_perform_sz,		void					)( s32 w, s32 h )
{
	on_perform_sz(xui_vector<s32>(w, h));
}
xui_method_explain(xui_component, on_perform_sz,		void					)( const xui_vector<s32>& sz )
{
	if (m_render.get_sz() != sz)
	{
		m_render.set_sz(sz);

		xui_method_args      args; 
		on_setrendersz(      args); 
		xm_setrendersz(this, args);
	}
}

/*
//callback
*/
xui_method_explain(xui_component, on_setrenderpt,		void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_setrendersz,		void					)( xui_method_args&			args )
{
	refresh();
}
xui_method_explain(xui_component, on_getfocus,			void					)( xui_method_args&			args )
{
	xui_component* root = m_parent;
	while (root && xui_issub_kindof(xui_panel, root) == false)
	{
		root->xm_getfocus(this, args);
		root = root->get_parent();
	}
}
xui_method_explain(xui_component, on_nonfocus,			void					)( xui_method_args&			args )
{
	xui_component* root = m_parent;
	while (root && xui_issub_kindof(xui_panel, root) == false)
	{
		root->xm_nonfocus(this, args);
		root = root->get_parent();
	}
}
xui_method_explain(xui_component, on_getcatch,			void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_noncatch,			void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_free,				void					)( xui_method_args&			args )
{
	m_maskcolor = xui_colour(1.0f, 1.0f, 1.0f, 1.0f);
}
xui_method_explain(xui_component, on_lock,				void					)( xui_method_args&			args )
{
	m_maskcolor = xui_colour(1.0f, 0.5f, 0.5f, 0.5f);
}
xui_method_explain(xui_component, on_show,				void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_hide,				void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_keybddown,			void					)( xui_method_keybd&		args )
{

}
xui_method_explain(xui_component, on_keybdrise,			void					)( xui_method_keybd&		args )
{

}
xui_method_explain(xui_component, on_keybdchar,			void					)( xui_method_keybd&		args )
{

}
xui_method_explain(xui_component, on_mousedown,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouserise,			void					)( xui_method_mouse&		args )
{
	if (g_desktop->get_catchctrl() == this &&
		g_desktop->get_hoverctrl() == this &&
		args.mouse == MB_L)
	{
		on_mouseclick(      args);
		xm_mouseclick(this, args);
	}
}
xui_method_explain(xui_component, on_mousemove,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseenter,		void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseleave,		void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseclick,		void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mousewheel,		void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mousedoubleclick,	void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mousedragitem,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragover,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragenter,	void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragleave,	void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragdrop,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_renderback,		void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_renderself,		void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_updateself,		void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_topdraw,			void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_invalid,			void					)( xui_method_args&			args )
{
	perform();
}
xui_method_explain(xui_component, on_perform,			void					)( xui_method_args&			args )
{

}

/*
//perform implement
*/
xui_method_explain(xui_component, perform_alignhorz,	void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->get_alignhorz() == 0)
			continue;

		//计算横坐标
		s32 x = rect.ax;
		switch (comp->get_alignhorz())
		{
		case ALIGNHORZ_R:	x = rect.ax + (rect.get_w() - comp->get_renderw());			break;
		case ALIGNHORZ_C:	x = rect.ax + (rect.get_w() - comp->get_renderw()) / 2;		break;
		}

		comp->on_perform_x(x);
	}
}
xui_method_explain(xui_component, perform_alignvert,	void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->get_alignvert() == 0)
			continue;

		//计算纵坐标
		s32 y = rect.ay;
		switch (comp->get_alignvert())
		{
		case ALIGNVERT_B:	y = rect.ay + (rect.get_h() - comp->get_renderh());			break;
		case ALIGNVERT_C:	y = rect.ay + (rect.get_h() - comp->get_renderh()) / 2;		break;
		}

		comp->on_perform_y(y);
	}
}
xui_method_explain(xui_component, perform_dockstyle,	void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	xui_rect2d<s32> rest = rect;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->was_visible()   == false ||
			comp->get_dockstyle() == DOCKSTYLE_N)
			continue;

		xui_vector<s32> pt = comp->get_renderpt();
		xui_vector<s32> sz = comp->get_rendersz();
		switch (comp->get_dockstyle())
		{
		case DOCKSTYLE_L:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.h	= rest.get_h();
			rest.ax = rest.ax + comp->get_renderw();
			break;
		case DOCKSTYLE_T:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.w	= rest.get_w();
			rest.ay = rest.ay + comp->get_renderh();
			break;
		case DOCKSTYLE_R:
			pt.x	= rest.bx - comp->get_renderw();
			pt.y	= rest.ay;
			sz.h	= rest.get_h();
			rest.bx = pt.x;
			break;
		case DOCKSTYLE_B:
			pt.x	= rest.ax;
			pt.y	= rest.by - comp->get_renderh();
			sz.w	= rest.get_w();
			rest.by = pt.y;
			break;
		case DOCKSTYLE_F:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.w	= rest.get_w();
			sz.h	= rest.get_h();
			break;
		}

		comp->on_perform_pt(pt);
		comp->on_perform_sz(sz);

		if (comp->get_dockstyle() == DOCKSTYLE_F)
			break;
	}
}
