#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_window.h"
#include "xui_component.h"

xui_implement_root(xui_component)

xui_component::xui_component( const xui_vector<s32>& size )
{
	m_data		= NULL;
	m_init		= false;
	m_cursor	= k_cursor_default;
	m_parent	= NULL;
	m_render	= xui_rect2d<s32>(xui_vector<s32>(0), size);
	m_enable	= true;
	m_visible	= true;
	m_invalid	= false;

	m_alignhorz = 0;
	m_alignvert = 0;
	m_dockstyle = 0;
	m_backalpha = 1.0f;
	m_backcolor = xui_colour::k_zero;
	m_maskcolor	= xui_colour::k_white;
	m_clicktime = -1.0f;
	m_clickdown = xui_vector<s32>(0);
	m_popaction = NULL;
}

xui_component::~xui_component( void )
{
	delete m_popaction;
}

void xui_component::ini_component( bool enable, bool visible )
{
	m_enable	= enable;
	m_visible	= visible;
}

void xui_component::ini_component( u08 alignhorz, u08 alignvert, u08 dockstyle )
{
	m_alignhorz = alignhorz;
	m_alignvert = alignvert;
	m_dockstyle = dockstyle;
}

const std::string& xui_component::get_name( void ) const
{
	return m_name;
}

void xui_component::set_name( const std::string& name )
{
	m_name = name;
}

void* xui_component::get_data( void )
{
	return m_data;
}

void xui_component::set_data( void* data )
{
	m_data = data;
}

u32 xui_component::get_cursor( void ) const
{
	return m_cursor;
}

void xui_component::set_cursor( u32 cursor )
{
	m_cursor = cursor;
}

xui_component* xui_component::get_parent( void )
{
	return m_parent;
}

void xui_component::set_parent( xui_component* component )
{
	m_parent = component;
}

bool xui_component::was_parent( xui_component* component ) const
{
	return (m_parent && m_parent == component);
}

bool xui_component::was_ancestor( xui_component* component ) const
{
	if (m_parent == NULL)
		return false;

	return (m_parent == component) || m_parent->was_ancestor(component);
}

xui_component* xui_component::get_ancestor( const xui_rtti* rtti )
{
	xui_component* parent = m_parent;
	while (parent)
	{
		if (parent->issub(rtti))
			return parent;

		parent = parent->get_parent();
	}

	return NULL;
}

xui_window* xui_component::get_window( void )
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

bool xui_component::was_enable( void ) const
{
	return m_enable;
}

void xui_component::set_enable( bool flag )
{
	if (m_enable != flag)
	{
		m_enable  = flag;

		xui_method_args args;
		if (m_enable)	{ on_free(args); xm_free(this, args); } 
		else			{ on_lock(args); xm_lock(this, args); } 
	}
}

bool xui_component::was_visible( void ) const
{
	return m_visible;
}

void xui_component::set_visible( bool flag )
{
	if (m_visible != flag)
	{
		m_visible  = flag;

		xui_method_args args;
		if (m_visible)	{ on_show(args); xm_show(this, args); }
		else			{ on_hide(args); xm_hide(this, args); } 

		//Ë¢ÐÂ¸¸¿Ø¼þ
		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->refresh();
	}
}

bool xui_component::has_focus( void ) const
{
	return xui_desktop::get_ins()->get_focusctrl() == this;
}

void xui_component::req_focus( void )
{
	xui_desktop::get_ins()->post_message(xui_message(this, k_xm_getfocus, 0, 0));
}

bool xui_component::has_catch( void ) const
{
	return xui_desktop::get_ins()->get_catchctrl() == this;
}

void xui_component::req_catch( void )
{
	xui_desktop::get_ins()->post_message(xui_message(this, k_xm_getcatch, 0, 0));
}

bool xui_component::was_hover( void ) const
{
	return xui_desktop::get_ins()->get_hoverctrl() == this;
}

f32 xui_component::get_backalpha( void ) const
{
	return m_backalpha;
}

void xui_component::set_backalpha( f32 alpha )
{
	m_backalpha = alpha;
}

const xui_colour& xui_component::get_backcolor( void ) const
{
	return m_backcolor;
}

void xui_component::set_backcolor( const xui_colour& color )
{
	m_backcolor = color;
}

const xui_colour& xui_component::get_maskcolor( void ) const
{
	return m_maskcolor;
}

void xui_component::set_maskcolor( const xui_colour& color )
{
	m_maskcolor = color;
}

u08 xui_component::get_alignhorz( void ) const
{
	return m_alignhorz;
}

void xui_component::set_alignhorz( u08 alignhorz )
{
	if (m_alignhorz != alignhorz)
	{
		m_alignhorz  = alignhorz;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}

u08 xui_component::get_alignvert( void ) const
{
	return m_alignvert;
}

void xui_component::set_alignvert( u08 alignvert )
{
	if (m_alignvert != alignvert)
	{
		m_alignvert  = alignvert;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}

u08 xui_component::get_dockstyle( void ) const
{
	return m_dockstyle;
}

void xui_component::set_dockstyle( u08 dockstyle )
{
	if (m_dockstyle != dockstyle)
	{
		m_dockstyle  = dockstyle;

		if (m_parent && xui_issub_kindof(xui_panel, m_parent))
			m_parent->perform();
	}
}

s32 xui_component::get_renderx( void ) const
{
	return m_render.ax;
}

s32 xui_component::get_rendery( void ) const
{
	return m_render.ay;
}

xui_vector<s32> xui_component::get_renderpt( void ) const
{
	return m_render.get_pt();
}

void xui_component::set_renderx( s32 x )
{
	set_renderpt(xui_vector<s32>(x, m_render.ay));
}

void xui_component::set_rendery( s32 y )
{
	set_renderpt(xui_vector<s32>(m_render.ax, y));
}

void xui_component::set_renderpt( const xui_vector<s32>& pt )
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

s32 xui_component::get_renderw( void ) const
{
	return m_render.get_sz().w;
}

s32 xui_component::get_renderh( void ) const
{
	return m_render.get_sz().h;
}

xui_vector<s32> xui_component::get_rendersz( void ) const
{
	return m_render.get_sz();
}

void xui_component::set_renderw( s32 w )
{
	set_rendersz(xui_vector<s32>(w, m_render.get_sz().h));
}

void xui_component::set_renderh( s32 h )
{
	set_rendersz(xui_vector<s32>(m_render.get_sz().w, h));
}

void xui_component::set_rendersz( const xui_vector<s32>& sz )
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

xui_vector<s32> xui_component::get_renderpt( const xui_vector<s32>& pt ) const
{
	return pt - get_screenpt();
}

xui_vector<s32> xui_component::get_screenpt( void ) const
{
	xui_vector<s32> pt = m_render.get_pt();
	return (m_parent == NULL) 
		? pt 
		: pt + m_parent->get_screenpt();
}

xui_colour xui_component::get_vertexcolor( void ) const
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

xui_colour xui_component::get_rendercolor( void ) const
{
	return m_backcolor;
}

xui_rect2d<s32> xui_component::get_renderrt( void ) const
{
	return xui_rect2d<s32>(xui_vector<s32>(0, 0), get_rendersz());
}

xui_rect2d<s32> xui_component::get_renderrtabs( void ) const
{
	return get_renderrt() + get_screenpt();
}

xui_component* xui_component::choose( const xui_vector<s32>& pt )
{
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		if (m_enable && m_visible)
		{
			if (m_render.was_inside(pt))
				return this;
		}
	}

	return NULL;
}

void xui_component::invalid( void )
{
	m_invalid = true;
}

void xui_component::refresh( void )
{
	m_invalid = false;

	xui_method_args  args; 
	on_invalid(      args); 
	xm_invalid(this, args);
}

void xui_component::perform( void )
{
	xui_method_args  args;
	on_perform(      args);
	xm_perform(this, args);
}

void xui_component::update( f32 delta )
{
	if (m_invalid)
	{
		refresh();
	}

	if (m_init == false)
	{
		xui_method_args args;
		on_load(		args);
		xm_load(this,	args);

		m_init = true;
	}

	if (m_clicktime >= 0.0f)
	{
		m_clicktime += delta;
		if (m_clicktime > 0.3f)
			m_clicktime =-1.0f;
	}

	if (m_popaction)
	{
		m_popaction->update(delta);
	}

	xui_method_update   args ;
	args.delta = delta;
	on_updateself(      args);
	xm_updateself(this, args);
}

void xui_component::render( void )
{
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		xui_method_args     args; 
		on_renderback(      args);
		xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
		xui_canvas::get_ins()->set_cliprect(cliprect.get_inter(get_renderrtabs()));
		on_renderself(      args);
		xm_renderself(this, args);
		xui_canvas::get_ins()->set_cliprect(cliprect);
	}
}

void xui_component::on_perform_x( s32 x )
{
	on_perform_pt(x, m_render.ay);
}

void xui_component::on_perform_y( s32 y )
{
	on_perform_pt(m_render.ax, y);
}

void xui_component::on_perform_pt( s32 x, s32 y )
{
	on_perform_pt(xui_vector<s32>(x, y));
}

void xui_component::on_perform_pt( const xui_vector<s32>& pt )
{
	if (m_render.get_pt() != pt)
	{
		m_render.set_pt(pt);

		xui_method_args      args; 
		on_setrenderpt(      args); 
		xm_setrenderpt(this, args);
	}
}

void xui_component::on_perform_w( s32 w )
{
	on_perform_sz(w, m_render.get_h());
}

void xui_component::on_perform_h( s32 h )
{
	on_perform_sz(m_render.get_w(), h);
}

void xui_component::on_perform_sz( s32 w, s32 h )
{
	on_perform_sz(xui_vector<s32>(w, h));
}

void xui_component::on_perform_sz( const xui_vector<s32>& sz )
{
	if (m_render.get_sz() != sz)
	{
		m_render.set_sz(sz);

		xui_method_args      args; 
		on_setrendersz(      args); 
		xm_setrendersz(this, args);
	}
}

void xui_component::on_setrenderpt( xui_method_args& args )
{}

void xui_component::on_setrendersz( xui_method_args& args )
{
	refresh();
}

void xui_component::on_getfocus( xui_method_args& args )
{
	xui_component* root  = m_parent;
	xui_component* other = (xui_component*)args.wparam;
	if (other == NULL)
	{
		while (root)
		{
			root->xm_getfocus(this, args);
			root = root->get_parent();
		}
	}
	else
	{
		while (root && other != root && other->was_ancestor(root) == false)
		{
			root->xm_getfocus(this, args);
			root = root->get_parent();
		}
	}
}

void xui_component::on_nonfocus( xui_method_args& args )
{
	xui_component* root  = m_parent;
	xui_component* other = (xui_component*)args.wparam;
	if (other == NULL)
	{
		while (root)
		{
			root->xm_nonfocus(this, args);
			root = root->get_parent();
		}
	}
	else
	{
		while (root && other != root && other->was_ancestor(root) == false)
		{
			root->xm_nonfocus(this, args);
			root = root->get_parent();
		}
	}
}

void xui_component::on_getcatch( xui_method_args& args )
{}

void xui_component::on_noncatch( xui_method_args& args )
{}

void xui_component::on_load( xui_method_args& args )
{}

void xui_component::on_free( xui_method_args& args )
{
	m_maskcolor = xui_colour(1.0f, 1.0f, 1.0f, 1.0f);
}

void xui_component::on_lock( xui_method_args& args )
{
	m_maskcolor = xui_colour(1.0f, 0.5f, 0.5f, 0.5f);
}

void xui_component::on_show( xui_method_args& args )
{
	if (m_popaction)
	{
		m_popaction->reset();
		m_popaction->set_play(true);
		m_popaction->update(0.0f);
	}
}

void xui_component::on_hide( xui_method_args& args )
{}

void xui_component::on_keybddown( xui_method_keybd&	args )
{}

void xui_component::on_keybdrise( xui_method_keybd&	args )
{}

void xui_component::on_keybdchar( xui_method_keybd&	args )
{}

void xui_component::on_mousedown( xui_method_mouse& args )
{}

void xui_component::on_mouserise( xui_method_mouse&		args )
{
	if (args.mouse == k_mb_left && has_catch() && was_hover())
	{
		on_mouseclick(      args);
		xm_mouseclick(this, args);
		if (m_clicktime < 0.0f)
		{
			m_clicktime = 0.0f;
			m_clickdown = args.point;
		}
		else
		{
			if (xui_abs(m_clickdown.x-args.point.x) <= 4 &&
				xui_abs(m_clickdown.y-args.point.y) <= 4)
			{
				on_mousedoubleclick(	  args);
				xm_mousedoubleclick(this, args);
			}

			m_clicktime =-1.0f;
			m_clickdown = xui_vector<s32>(0);
		}
	}
}

void xui_component::on_mousemove( xui_method_mouse&	args )
{}

void xui_component::on_mouseenter( xui_method_mouse& args )
{}

void xui_component::on_mouseleave( xui_method_mouse& args )
{}

void xui_component::on_mouseclick( xui_method_mouse& args )
{}

void xui_component::on_mousewheel( xui_method_mouse& args )
{}

void xui_component::on_mousedoubleclick( xui_method_mouse& args )
{}

void xui_component::on_mousedragitem( xui_method_dragdrop& args )
{}

void xui_component::on_mousedragover( xui_method_dragdrop& args )
{}

void xui_component::on_mousedragenter( xui_method_dragdrop&	args )
{}

void xui_component::on_mousedragleave( xui_method_dragdrop& args )
{}

void xui_component::on_mousedragdrop( xui_method_dragdrop& args )
{}

void xui_component::on_renderback( xui_method_args& args )
{}

void xui_component::on_renderself( xui_method_args&	args )
{}

void xui_component::on_updateself( xui_method_update& args )
{}

void xui_component::on_topdraw( xui_method_args& args )
{}

void xui_component::on_invalid( xui_method_args& args )
{
	perform();
}

void xui_component::on_perform( xui_method_args& args )
{}

void xui_component::perform_alignhorz( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->get_alignhorz() == 0)
			continue;

		s32 x = rect.ax;
		switch (comp->get_alignhorz())
		{
		case k_alignhorz_r:	x = rect.ax + (rect.get_w() - comp->get_renderw());			break;
		case k_alignhorz_c:	x = rect.ax + (rect.get_w() - comp->get_renderw()) / 2;		break;
		}

		comp->on_perform_x(x);
	}
}

void xui_component::perform_alignvert( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->get_alignvert() == 0)
			continue;

		s32 y = rect.ay;
		switch (comp->get_alignvert())
		{
		case k_alignvert_b:	y = rect.ay + (rect.get_h() - comp->get_renderh());			break;
		case k_alignvert_c:	y = rect.ay + (rect.get_h() - comp->get_renderh()) / 2;		break;
		}

		comp->on_perform_y(y);
	}
}

void xui_component::perform_dockstyle( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec )
{
	xui_rect2d<s32> rest = rect;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_component* comp = vec[i];
		if (comp->was_visible()   == false ||
			comp->get_dockstyle() == k_dockstyle_n)
			continue;

		xui_vector<s32> pt = comp->get_renderpt();
		xui_vector<s32> sz = comp->get_rendersz();
		switch (comp->get_dockstyle())
		{
		case k_dockstyle_l:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.h	= rest.get_h();
			rest.ax = rest.ax + comp->get_renderw();
			break;
		case k_dockstyle_t:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.w	= rest.get_w();
			rest.ay = rest.ay + comp->get_renderh();
			break;
		case k_dockstyle_r:
			pt.x	= rest.bx - comp->get_renderw();
			pt.y	= rest.ay;
			sz.h	= rest.get_h();
			rest.bx = pt.x;
			break;
		case k_dockstyle_b:
			pt.x	= rest.ax;
			pt.y	= rest.by - comp->get_renderh();
			sz.w	= rest.get_w();
			rest.by = pt.y;
			break;
		case k_dockstyle_f:
			pt.x	= rest.ax;
			pt.y	= rest.ay;
			sz.w	= rest.get_w();
			sz.h	= rest.get_h();
			break;
		}

		comp->on_perform_pt(pt);
		comp->on_perform_sz(sz);

		if (comp->get_dockstyle() == k_dockstyle_f)
			break;
	}
}
