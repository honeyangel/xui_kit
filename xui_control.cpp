#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_linebox.h"
#include "xui_gridbox.h"
#include "xui_control.h"

xui_implement_rtti(xui_control, xui_component)

const xui_colour xui_control::k_default_sidecolor = xui_colour(1.0f, 160.0f/255.0f);

xui_control::xui_control( const xui_vector<s32>& size )
: xui_component(size)
{
	m_client	= m_render.get_sz();
	m_border	= xui_rect2d<s32>(0);
	m_scroll	= xui_vector<s32>(0);
	m_corner	= 0;
	m_drawcolor = false;
	m_sidestyle = 0;
	m_sidecolor = k_default_sidecolor;
	m_backpivot = xui_vector<f32>(0.0f);
	m_backscale = xui_vector<f32>(1.0f);
}

xui_control::~xui_control( void )
{
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
		delete m_widgetvec[i];
}

s32 xui_control::get_clientw( void ) const
{
	return m_client.w;
}

s32 xui_control::get_clienth( void ) const
{
	return m_client.h;
}

xui_vector<s32> xui_control::get_clientsz( void ) const
{
	return m_client;
}

void xui_control::set_clientsz( const xui_vector<s32>& sz )
{
	if (m_client != sz)
	{
		m_client  = sz;

		xui_method_args      args; 
		on_setclientsz(      args); 
		xm_setclientsz(this, args);
	}
}

const xui_rect2d<s32>& xui_control::get_borderrt( void ) const
{
	return m_border;
}

void xui_control::set_borderrt( const xui_rect2d<s32>& rt )
{
	if (m_border != rt)
	{
		m_client -= xui_vector<s32>(rt.ax+rt.bx-m_border.ax-m_border.bx, rt.ay+rt.by-m_border.ay-m_border.by);
		m_border  = rt;

		xui_method_args      args; 
		on_setborderrt(      args);
		xm_setborderrt(this, args);
	}
}

s32 xui_control::get_scrollx( void ) const
{
	return m_scroll.x;
}

s32 xui_control::get_scrolly( void ) const
{
	return m_scroll.y;
}

void xui_control::set_scrollx( s32 x )
{
	m_scroll.x = x;
}

void xui_control::set_scrolly( s32 y )
{
	m_scroll.y = y;
}

const xui_vector<s32>& xui_control::get_scrollpt( void ) const
{
	return m_scroll;
}

void xui_control::set_scrollpt( const xui_vector<s32>& pt )
{
	m_scroll = pt;
}

s32 xui_control::get_corner( void ) const
{
	return m_corner;
}

void xui_control::set_corner( s32 corner )
{
	m_corner = corner;
}

bool xui_control::was_drawcolor( void ) const
{
	return m_drawcolor;
}

void xui_control::set_drawcolor( bool flag )
{
	m_drawcolor = flag;
}

u08 xui_control::get_sidestyle( void ) const
{
	return m_sidestyle;
}

void xui_control::set_sidestyle( u08 sidestyle )
{
	m_sidestyle = sidestyle;
}

const xui_colour& xui_control::get_sidecolor( void ) const
{
	return m_sidecolor;
}

void xui_control::set_sidecolor( const xui_colour& color )
{
	m_sidecolor = color;
}

xui_vector<s32> xui_control::get_screenpt( void ) const
{
	xui_vector<s32> pt = m_render.get_pt() - m_scroll;
	return (m_parent == NULL) 
		? pt 
		: pt + m_parent->get_screenpt();
}

xui_rect2d<s32> xui_control::get_cornerrt( void ) const
{
	if (m_parent)
	{
		if (xui_issub_kindof(xui_linebox, m_parent) ||
			xui_issub_kindof(xui_gridbox, m_parent))
			return xui_dynamic_cast(xui_control, m_parent)->get_cornerrt((xui_component*)this);
	}

	return xui_rect2d<s32>(m_corner);
}

xui_rect2d<s32> xui_control::get_cornerrt( xui_component* widget ) const
{
	return xui_rect2d<s32>(0);
}

xui_rect2d<s32> xui_control::get_clientrt( void ) const
{
	return xui_rect2d<s32>(xui_vector<s32>(0, 0), m_client);
}

xui_rect2d<s32> xui_control::get_renderrtins( void ) const
{
	xui_rect2d<s32> rect = get_renderrt();
	rect.ax += m_border.ax;
	rect.ay += m_border.ay;
	rect.bx -= m_border.bx;
	rect.by -= m_border.by;
	return rect;
}

xui_component* xui_control::choose( const xui_vector<s32>& pt )
{
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		if (m_enable && m_visible)
		{
			xui_component* component = choose_else(pt);
			if (component == NULL)
				component  = xui_component::choose(pt);

			return component;
		}
	}

	return NULL;
}

xui_component* xui_control::choose_else( const xui_vector<s32>& pt )
{
	xui_component* component = NULL;
	if (m_render.was_inside(pt))
	{
		xui_vector<s32> relative = pt - m_render.get_pt();
		for (s32 i = (s32)m_widgetvec.size()-1; i >= 0; --i)
		{
			if (component = m_widgetvec[i]->choose(relative))
				return component;
		}
	}

	return NULL;
}

void xui_control::update( f32 delta )
{
	xui_component::update(delta);
	update_else(delta);
}

void xui_control::update_else( f32 delta )
{
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i]->was_visible())
			m_widgetvec[i]->update(delta);
	}
}

void xui_control::render( void )
{
	xui_rect2d<s32> cornerrt = get_cornerrt   ();
	xui_rect2d<s32> renderrt = get_renderrtabs();
	xui_colour      color    = get_vertexcolor();

	f32 ox = -m_backpivot.x * renderrt.get_w() * m_backscale.x + m_backpivot.x * renderrt.get_w();
	f32 oy = -m_backpivot.y * renderrt.get_h() * m_backscale.y + m_backpivot.y * renderrt.get_h();
	f32 fw =  m_backscale.x * renderrt.get_w();
	f32 fh =  m_backscale.y * renderrt.get_h();
	renderrt.oft_x((s32)ox);
	renderrt.oft_y((s32)oy);
	renderrt.set_w((s32)fw);
	renderrt.set_h((s32)fh);
	if (renderrt.get_w() < cornerrt.ax ||
		renderrt.get_h() < cornerrt.ay)
		return;

	if (m_drawcolor)
	{
		xui_colour fill_color = get_rendercolor() * color;
		xui_canvas::get_ins()->fill_round(renderrt, fill_color, cornerrt);
	}

	xui_component::render();
	if (m_popaction == NULL || m_popaction->was_play() == false)
	{
		render_else();
	}

	if (m_sidestyle)
	{
		renderrt.bx -= 1;
		renderrt.by -= 1;
		xui_colour side_color = m_sidecolor * color;
		xui_canvas::get_ins()->draw_round(renderrt, side_color, cornerrt);

		if (m_sidestyle == k_sidestyle_d)
		{
			xui_rect2d<s32> temp(
				renderrt.ax+1, 
				renderrt.ay+1, 
				renderrt.bx-1, 
				renderrt.by-1);

			side_color.r = 1.0f - side_color.r;
			side_color.g = 1.0f - side_color.g;
			side_color.b = 1.0f - side_color.b;
			xui_canvas::get_ins()->draw_round(temp, side_color, cornerrt);
		}
	}
}

void xui_control::render_else( void )
{
	xui_rect2d<s32> cliprect = xui_canvas::get_ins()->get_cliprect();
	xui_rect2d<s32> currrect = cliprect.get_inter(get_renderrtabs());
	xui_canvas::get_ins()->set_cliprect(currrect);
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i]->was_visible() && currrect.get_inter(m_widgetvec[i]->get_renderrtabs()).was_valid())
			m_widgetvec[i]->render();
	}
	xui_canvas::get_ins()->set_cliprect(cliprect);

	xui_method_args		args;
	xm_renderelse(this, args);
}

void xui_control::on_setclientsz( xui_method_args& args )
{
	perform();
}

void xui_control::on_setborderrt( xui_method_args& args )
{
	perform();
}

void xui_control::on_renderback( xui_method_args& args )
{}

void xui_control::on_renderself( xui_method_args& args )
{}

void xui_control::on_perform( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		xui_rect2d<s32> rt = get_renderrtins();
		perform_alignhorz(rt, m_widgetvec);
		perform_alignvert(rt, m_widgetvec);
		perform_dockstyle(rt, m_widgetvec);
	}
}