#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_component.h"

xui_implement_root(xui_component);

/*
//constructor
*/
xui_create_explain(xui_component)( const std::string& name, const xui_rect2d<s32>& rect )
{
	m_type		= "component";
	m_name		= name;
	m_data		= NULL;
	m_cursor	= CURSOR_DEFAULT;
	m_parent	= NULL;
	m_render	= rect;
	m_cliped	= true;
	m_enable	= true;
	m_visible	= true;
	m_inherit	= false;
	m_invalid	= false;
	m_raycast	= true;

	m_alignhorz = 0;
	m_alignvert = 0;
	m_dockstyle = 0;
	m_backalpha = 1.0f;
	m_backcolor = xui_colour(1.0f, 1.0f, 1.0f, 1.0f);
	m_maskcolor = xui_colour(1.0f, 1.0f, 1.0f, 1.0f);
	m_drawcolor = true;
}

/*
//destructor
*/
xui_delete_explain(xui_component)( void )
{}

/*
//type
*/
xui_method_explain(xui_component, get_type,				const std::string&		)( void ) const
{
	return m_type;
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

xui_method_explain(xui_component, set_parent,			void					)( xui_component* componet )
{
	m_parent = componet;
}

xui_method_explain(xui_component, was_parent,			bool					)( xui_component* componet ) const
{
	return (m_parent && m_parent == componet);
}

xui_method_explain(xui_component, was_ancestor,			bool					)( xui_component* componet ) const
{
	if (m_parent == NULL)
		return false;

	return (m_parent == componet) || m_parent->was_ancestor(componet);
}

/*
//window
*/
xui_method_explain(xui_component, get_window,			xui_window*				)( void )
{
	xui_component* parent = this;
	while (parent)
	{
		if (parent->get_type().find("window") != -1)
			return (xui_window*)parent;

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
//cliped
*/
xui_method_explain(xui_component, was_cliped,			bool					)( void ) const
{
	return m_cliped;
}
xui_method_explain(xui_component, set_cliped,			void					)( bool flag )
{
	m_cliped = flag;
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
		//if (m_parent)
		//	m_parent->refresh();
	}
}

/*
//inherit
*/
xui_method_explain(xui_component, was_inherit,			bool					)( void ) const
{
	return m_inherit;
}

xui_method_explain(xui_component, set_inherit,			void					)( bool flag )
{
	m_inherit = flag;
}

/*
//raycast
*/
xui_method_explain(xui_component, was_raycast,			bool					)( void ) const
{
	return m_raycast;
}
xui_method_explain(xui_component, set_raycast,			void					)( bool flag )
{
	m_raycast = flag;
}

/*
//focus
*/
xui_method_explain(xui_component, has_focus,				bool					)( void ) const
{
	return g_desktop->get_focusctrl() == this;
}

xui_method_explain(xui_component, req_focus,				void					)( void )
{
	g_desktop->post_message(xui_message(this, XM_GETFOCUS, 0, 0));
}

xui_method_explain(xui_component, has_catch,				bool					)( void ) const
{
	return g_desktop->get_catchctrl() == this;
}

xui_method_explain(xui_component, req_catch,				void					)( void )
{
	g_desktop->post_message(xui_message(this, XM_GETCATCH, 0, 0));
}

/*
//background
*/
xui_method_explain(xui_component, get_backalpha,			f32						)( void ) const
{
	return m_backalpha;
}

xui_method_explain(xui_component, set_backalpha,			void					)( f32 alpha )
{
	m_backalpha = alpha;
}

xui_method_explain(xui_component, get_backcolor,			const xui_colour&		)( void ) const
{
	return m_backcolor;
}

xui_method_explain(xui_component, set_backcolor,			void					)( const xui_colour& color )
{
	m_backcolor = color;
}

xui_method_explain(xui_component, get_maskcolor,			const xui_colour&		)( void ) const
{
	return m_maskcolor;
}

xui_method_explain(xui_component, set_maskcolor,			void					)( const xui_colour& color )
{
	m_maskcolor = color;
}

xui_method_explain(xui_component, was_drawcolor,			bool					)( void ) const
{
	return m_drawcolor;
}

xui_method_explain(xui_component, set_drawcolor,			void					)( bool flag )
{
	m_drawcolor = flag;
}

/*
//layout
*/
xui_method_explain(xui_component, get_alignhorz,			u08						)( void ) const
{
	return m_alignhorz;
}

xui_method_explain(xui_component, set_alignhorz,			void					)( u08 alignhorz )
{
	if (m_alignhorz != alignhorz)
	{
		m_alignhorz  = alignhorz;

		if (m_parent)
			m_parent->perform();
	}
}

xui_method_explain(xui_component, get_alignvert,			u08						)( void ) const
{
	return m_alignvert;
}

xui_method_explain(xui_component, set_alignvert,			void					)( u08 alignvert )
{
	if (m_alignvert != alignvert)
	{
		m_alignvert  = alignvert;

		if (m_parent)
			m_parent->perform();
	}
}

xui_method_explain(xui_component, get_dockstyle,			u08						)( void ) const
{
	return m_dockstyle;
}

xui_method_explain(xui_component, set_dockstyle,			void					)( u08 dockstyle )
{
	if (m_dockstyle != dockstyle)
	{
		m_dockstyle  = dockstyle;

		if (m_parent)
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

xui_method_explain(xui_component, set_renderpt,			void					)( const xui_vector<s32>& pt, bool notify_parent )
{
	if (m_render.was_pt(pt) == false)
	{
		m_render.set_pt(pt);

		xui_method_args      args; 
		on_setrenderpt(      args); 
		xm_setrenderpt(this, args);

		if (m_parent && notify_parent)
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

xui_method_explain(xui_component, set_rendersz,			void					)( const xui_vector<s32>& sz, bool notify_parent )
{
	if (m_render.was_sz(sz) == false)
	{
		m_render.set_sz(sz);

		xui_method_args	     args; 
		on_setrendersz(      args); 
		xm_setrendersz(this, args);

		if (m_parent && notify_parent)
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

xui_method_explain(xui_component, get_clipedrt,			xui_rect2d<s32>			)( void ) const
{
	if		(m_parent == NULL || m_cliped == false)	return get_renderrtabs();
	else if (m_inherit)								return m_parent->get_clipedrt();
	else											return m_parent->get_clipedrt().get_inter(m_parent->get_renderrtabs());
}

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
	if (m_enable && m_visible && m_raycast)
	{
		xui_rect2d<s32> rt = get_clipedrt().get_inter(get_renderrtabs());
		if (rt.was_inside(pt))
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
	//paint->set_clip_rect(get_clipedrt());

	xui_method_args     args; 
	on_renderback(      args);
	on_renderself(      args);
	xm_renderself(this, args);
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
	while (root && root->get_type().find("panel") == -1)
	{
		root->xm_getfocus(this, args);
		root = root->get_parent();
	}
}
xui_method_explain(xui_component, on_nonfocus,			void					)( xui_method_args&			args )
{
	xui_component* root = m_parent;
	while (root && root->get_type().find("panel") == -1)
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
	if (g_desktop->get_hoverctrl() == this)
	{
		on_mouseclick(      args);
		xm_mouseclick(this, args);
	}
}
xui_method_explain(xui_component, on_mousemove,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseenter,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseleave,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mouseclick,			void					)( xui_method_mouse&		args )
{

}
xui_method_explain(xui_component, on_mousewheel,			void					)( xui_method_mouse&		args )
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
xui_method_explain(xui_component, on_mousedragenter,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragleave,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_mousedragdrop,		void					)( xui_method_dragdrop&		args )
{

}
xui_method_explain(xui_component, on_renderback,			void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_renderself,			void					)( xui_method_args&			args )
{

}
xui_method_explain(xui_component, on_updateself,			void					)( xui_method_args&			args )
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
xui_method_explain(xui_component, perform_alignhorz,		void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& compVec )
{
	for (u32 i = 0; i < compVec.size(); ++i)
	{
		xui_component* comp = compVec[i];
		if (comp->get_alignhorz() == 0)
			continue;

		//计算横坐标
		xui_vector<s32> pt = comp->get_renderpt();
		switch (comp->get_alignhorz())
		{
		case ALIGNHORZ_L:
			pt.x = rect.ax;			
			break;
		case ALIGNHORZ_R:	
			pt.x = rect.ax + (rect.get_sz().w - comp->get_renderw());			
			break;
		case ALIGNHORZ_C:	
			pt.x = rect.ax + (rect.get_sz().w - comp->get_renderw()) / 2;	
			break;
		}

		comp->set_renderpt(pt, false);
	}
}

xui_method_explain(xui_component, perform_alignvert,		void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& compVec )
{
	for (u32 i = 0; i < compVec.size(); ++i)
	{
		xui_component* comp = compVec[i];
		if (comp->get_alignvert() == 0)
			continue;

		//计算纵坐标
		xui_vector<s32> pt = comp->get_renderpt();
		switch (comp->get_alignvert())
		{
		case ALIGNVERT_T:	
			pt.y = rect.ay;											
			break;							
		case ALIGNVERT_B:	
			pt.y = rect.ay + (rect.get_sz().h - comp->get_renderh());			
			break;
		case ALIGNVERT_C:	
			pt.y = rect.ay + (rect.get_sz().h - comp->get_renderh()) / 2;	
			break;
		}

		comp->set_renderpt(pt, false);
	}
}

xui_method_explain(xui_component, perform_dockstyle,		void					)( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& compVec )
{
	xui_rect2d<s32> temp = rect;
	for (u32 i = 0; i < compVec.size(); ++i)
	{
		xui_component* comp = compVec[i];
		if (comp->was_visible()   == false ||
			comp->get_dockstyle() == DOCKSTYLE_N)
			continue;

		xui_vector<s32> pt = comp->get_renderpt();
		xui_vector<s32> sz = comp->get_rendersz();
		switch (comp->get_dockstyle())
		{
		case DOCKSTYLE_L:
			pt.x	= temp.ax;
			pt.y	= temp.ay;
			sz.h	= temp.get_sz().h;
			temp.ax = temp.ax + comp->get_renderw();
			break;
		case DOCKSTYLE_T:
			pt.x	= temp.ax;
			pt.y	= temp.ay;
			sz.w	= temp.get_sz().w;
			temp.ay = temp.ay + comp->get_renderh();
			break;
		case DOCKSTYLE_R:
			pt.x	= temp.bx - comp->get_renderw();
			pt.y	= temp.ay;
			sz.h	= temp.get_sz().h;
			temp.bx = pt.x;
			break;
		case DOCKSTYLE_B:
			pt.x	= temp.ax;
			pt.y	= temp.by - comp->get_renderh();
			sz.w	= temp.get_sz().w;
			temp.by = pt.y;
			break;
		case DOCKSTYLE_F:
			pt.x	= temp.ax;
			pt.y	= temp.ay;
			sz.w	= temp.get_sz().w;
			sz.h	= temp.get_sz().h;
			break;
		}

		comp->set_renderpt(pt, false);
		comp->set_rendersz(sz, false);

		if (comp->get_dockstyle() == DOCKSTYLE_F)
			break;
	}
}
