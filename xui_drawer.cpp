#include "xui_canvas.h"
#include "xui_bitmap.h"
#include "xui_drawer.h"

xui_implement_rtti(xui_drawer, xui_control)

xui_drawer::xui_drawer( const xui_vector<s32>& size )
: xui_control(size)
{
	m_icon			= NULL;
	m_iconsize		= xui_vector<s32>(0);
	m_textalign		= k_textalign_lc;
	m_iconalign		= k_image_front_text;
	m_textoffset	= xui_vector<s32>(0);
	m_iconoffset	= xui_vector<s32>(0);
	m_singleline	= true;
}

xui_drawer* xui_drawer::create( xui_bitmap* icon )
{
    xui_drawer* drawer = new xui_drawer(xui_vector<s32>(24));
    drawer->set_borderrt(xui_rect2d<s32>(4));
    drawer->set_iconalign(k_image_c);
    drawer->ini_drawer(icon);
    return drawer;
}

xui_drawer* xui_drawer::create( const std::wstring& text )
{
    xui_drawer* drawer = new xui_drawer(xui_vector<s32>(80, 24));
    drawer->set_borderrt(xui_rect2d<s32>(4));
    drawer->set_textalign(k_textalign_lc);
    drawer->ini_drawer(text);
    return drawer;
}

void xui_drawer::ini_drawer( xui_bitmap* icon )
{
	ini_drawer(icon, (icon == NULL) ? xui_vector<s32>(0) : icon->get_size());
}

void xui_drawer::ini_drawer( xui_bitmap* icon, const xui_vector<s32>& iconsize )
{
	m_icon		= icon;
	m_iconsize	= iconsize;
}

void xui_drawer::ini_drawer( const std::wstring& text )
{
	ini_drawer(text, m_textfont, m_textdraw);
}

void xui_drawer::ini_drawer( const std::wstring& text, const xui_family& textfont, const xui_family_render& textdraw )
{
	m_text		= text;
	m_textfont	= textfont;
	m_textdraw	= textdraw;
}

xui_bitmap* xui_drawer::get_icon( void )
{
	return m_icon;
}

void xui_drawer::set_icon( xui_bitmap* icon )
{
	if (m_icon != icon)
	{
		m_icon  = icon;

		xui_method_args args;
		on_iconchanged( args);
	}
}

const std::wstring& xui_drawer::get_text( void ) const
{
	return m_text;
}

void xui_drawer::set_text( const std::wstring& text )
{
	if (m_text != text)
	{
		m_text  = text;

		xui_method_args args;
		on_textchanged( args);
	}
}

const xui_vector<s32>& xui_drawer::get_iconsize( void ) const
{
	return m_iconsize;
}

void xui_drawer::set_iconsize( const xui_vector<s32>& iconsize )
{
	m_iconsize = iconsize;
}

const xui_family& xui_drawer::get_textfont( void ) const
{
	return m_textfont;
}

void xui_drawer::set_textfont( const xui_family& textfont )
{
	if (m_textfont != textfont)
	{
		m_textfont  = textfont;

		xui_method_args args;
		on_fontchanged( args);
	}
}

const xui_family_render& xui_drawer::get_textdraw( void ) const
{
	return m_textdraw;
}

void xui_drawer::set_textdraw( const xui_family_render& textdraw )
{
	m_textdraw = textdraw;
}

const xui_colour& xui_drawer::get_textcolor( void ) const
{
	return m_textdraw.normalcolor;
}

void xui_drawer::set_textcolor( const xui_colour& color )
{
	set_textdraw(xui_family_render(color));
}

u08 xui_drawer::get_textalign( void ) const
{
	return m_textalign;
}

void xui_drawer::set_textalign( u08 align )
{
	m_textalign = align;
}

const xui_vector<s32>& xui_drawer::get_textoffset( void ) const
{
	return m_textoffset;
}

void xui_drawer::set_textoffset( const xui_vector<s32>& offset )
{
	m_textoffset = offset;
}

u08 xui_drawer::get_iconalign( void ) const
{
	return m_iconalign;
}

void xui_drawer::set_iconalign( u08 align )
{
	m_iconalign = align;
}

const xui_vector<s32>& xui_drawer::get_iconoffset( void ) const
{
	return m_iconoffset;
}

void xui_drawer::set_iconoffset( const xui_vector<s32>& offset )
{
	m_iconoffset = offset;
}

bool xui_drawer::was_singleline( void ) const
{
	return m_singleline;
}

void xui_drawer::set_singleline( bool flag )
{
	m_singleline = flag;
}

void xui_drawer::on_iconchanged( xui_method_args& args )
{
	m_iconsize = (m_icon == NULL) ? xui_vector<s32>(0) : m_icon->get_size();
	xm_iconchanged(this, args);
}

void xui_drawer::on_textchanged( xui_method_args& args )
{
	xm_textchanged(this, args);
}

void xui_drawer::on_fontchanged( xui_method_args& args )
{
	xm_fontchanged(this, args);
}

void xui_drawer::on_renderself( xui_method_args& args )
{
	xui_control::on_renderself(args);
	xui_bitmap*  icon = get_rendericon();
	std::wstring text = get_rendertext();
	if (icon == NULL && text.empty())
		return;

	xui_colour color  = get_vertexcolor();
	// draw text
	xui_family_render textdraw = m_textdraw;
	textdraw.normalcolor *= color;
	textdraw.strokecolor *= color;
	if (text.length() > 0)
	{
		xui_canvas::get_ins()->draw_text(
			text, 
			m_textfont, 
			get_rendertextrt()+get_screenpt(), 
			textdraw,
			m_singleline);
	}

	// draw icon
	if (icon)
	{
		xui_canvas::get_ins()->draw_image(
			icon, 
			xui_rect2d<s32>(get_rendericonpt()+get_screenpt(), m_iconsize), 
			color);
	}
}

xui_bitmap* xui_drawer::get_rendericon( void ) const
{
	return m_icon;
}

std::wstring xui_drawer::get_rendertext( void ) const
{
	return m_text;
}

xui_vector<s32> xui_drawer::get_rendericonpt( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	switch (m_iconalign)
	{
	case k_image_l:
		pt.x = rt.ax;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	case k_image_t:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay;
		break;
	case k_image_r:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w);
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	case k_image_b:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h);
		break;
	case k_image_c:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	}

	return pt + m_iconoffset;
}

xui_rect2d<s32> xui_drawer::get_rendertextrt( void ) const
{
	std::wstring    text = get_rendertext  ();
	xui_rect2d<s32> rt   = get_renderrtins ();
	xui_vector<s32> pt   = get_rendericonpt();
	switch (m_iconalign)
	{
	case k_image_front_text:
		rt.ax = pt.x + m_iconsize.w;
		break;
	case k_image_after_text:
		rt.bx = pt.x;
		break;
	case k_image_above_text:
		rt.ay = pt.y + m_iconsize.h;
		break;
	case k_image_below_text: 
		rt.by = pt.y;
		break;
	}

	rt.ax += m_textoffset.x;
	rt.ay += m_textoffset.y;
	if (text.length() > 0)
	{
		rt = xui_canvas::get_ins()->calc_draw(
			text, 
			m_textfont, 
			rt, 
			m_textalign, 
			m_singleline);
	}

	return rt;
}