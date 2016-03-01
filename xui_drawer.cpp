#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_drawer.h"

xui_implement_rtti(xui_drawer, xui_control);

/*
//static
*/
xui_method_explain(xui_drawer, create,				xui_drawer*				)( xui_bitmap* icon )
{
	xui_drawer* drawer = new xui_drawer(xui_vector<s32>(24));
	xui_method_ptrcall(drawer, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(drawer, set_iconalign)(IMAGE_C);
	xui_method_ptrcall(drawer, ini_drawer	)(icon);
	return drawer;
}
xui_method_explain(xui_drawer, create,				xui_drawer*				)( const std::wstring& text )
{
	xui_drawer* drawer = new xui_drawer(xui_vector<s32>(80, 24));
	xui_method_ptrcall(drawer, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(drawer, set_textalign)(TEXTALIGN_LC);
	xui_method_ptrcall(drawer, ini_drawer	)(text);
	return drawer;
}

/*
//constructor
*/
xui_create_explain(xui_drawer)( const xui_vector<s32>& size )
: xui_control(size)
{
	m_icon			= NULL;
	m_iconsize		= xui_vector<s32>(0);
	m_textalign		= TEXTALIGN_LC;
	m_iconalign		= IMAGE_FRONT_TEXT;
	m_textoffset	= xui_vector<s32>(0);
	m_iconoffset	= xui_vector<s32>(0);
	m_singleline	= true;
}

/*
//ini
*/
xui_method_explain(xui_drawer, ini_drawer,			void					)( xui_bitmap* icon )
{
	ini_drawer(icon, (icon == NULL) ? xui_vector<s32>(0) : icon->get_size());
}
xui_method_explain(xui_drawer, ini_drawer,			void					)( xui_bitmap* icon, const xui_vector<s32>& iconsize )
{
	m_icon		= icon;
	m_iconsize	= iconsize;
}
xui_method_explain(xui_drawer, ini_drawer,			void					)( const std::wstring& text )
{
	ini_drawer(text, m_textfont, m_textdraw);
}
xui_method_explain(xui_drawer, ini_drawer,			void					)( const std::wstring& text, const xui_family& textfont, const xui_family_render& textdraw )
{
	m_text		= text;
	m_textfont	= textfont;
	m_textdraw	= textdraw;
}

/*
//method
*/
xui_method_explain(xui_drawer, get_icon,			xui_bitmap*				)( void )
{
	return m_icon;
}
xui_method_explain(xui_drawer, set_icon,			void					)( xui_bitmap* icon )
{
	if (m_icon != icon)
	{
		m_icon  = icon;

		xui_method_args args;
		on_iconchanged( args);
	}
}
xui_method_explain(xui_drawer, get_text,			const std::wstring&		)( void ) const
{
	return m_text;
}
xui_method_explain(xui_drawer, set_text,			void					)( const std::wstring& text )
{
	if (m_text != text)
	{
		m_text  = text;

		xui_method_args args;
		on_textchanged( args);
	}
}
xui_method_explain(xui_drawer, get_iconsize,		const xui_vector<s32>&	)( void ) const
{
	return m_iconsize;
}
xui_method_explain(xui_drawer, set_iconsize,		void					)( const xui_vector<s32>& iconsize )
{
	m_iconsize = iconsize;
}
xui_method_explain(xui_drawer, get_textfont,		const xui_family&		)( void ) const
{
	return m_textfont;
}
xui_method_explain(xui_drawer, set_textfont,		void					)( const xui_family& textfont )
{
	if (m_textfont != textfont)
	{
		m_textfont  = textfont;

		xui_method_args args;
		on_fontchanged( args);
	}
}
xui_method_explain(xui_drawer, get_textdraw,		const xui_family_render&)( void ) const
{
	return m_textdraw;
}
xui_method_explain(xui_drawer, set_textdraw,		void					)( const xui_family_render& textdraw )
{
	m_textdraw = textdraw;
}
xui_method_explain(xui_drawer, get_textcolor,		const xui_colour&		)( void ) const
{
	return m_textdraw.normalcolor;
}
xui_method_explain(xui_drawer, set_textcolor,		void					)( const xui_colour& color )
{
	set_textdraw(xui_family_render(color));
}
xui_method_explain(xui_drawer, get_textalign,		u08						)( void ) const
{
	return m_textalign;
}
xui_method_explain(xui_drawer, set_textalign,		void					)( u08 align )
{
	m_textalign = align;
}
xui_method_explain(xui_drawer, get_textoffset,		const xui_vector<s32>&	)( void ) const
{
	return m_textoffset;
}
xui_method_explain(xui_drawer, set_textoffset,		void					)( const xui_vector<s32>& offset )
{
	m_textoffset = offset;
}
xui_method_explain(xui_drawer, get_iconalign,		u08						)( void ) const
{
	return m_iconalign;
}
xui_method_explain(xui_drawer, set_iconalign,		void					)( u08 align )
{
	m_iconalign = align;
}
xui_method_explain(xui_drawer, get_iconoffset,		const xui_vector<s32>&	)( void ) const
{
	return m_iconoffset;
}
xui_method_explain(xui_drawer, set_iconoffset,		void					)( const xui_vector<s32>& offset )
{
	m_iconoffset = offset;
}
xui_method_explain(xui_drawer, was_singleline,		bool					)( void ) const
{
	return m_singleline;
}
xui_method_explain(xui_drawer, set_singleline,		void					)( bool flag )
{
	m_singleline = flag;
}

/*
//callback
*/
xui_method_explain(xui_drawer, on_iconchanged,		void					)( xui_method_args& args )
{
	m_iconsize = (m_icon == NULL) ? xui_vector<s32>(0) : m_icon->get_size();
	xm_iconchanged(this, args);
}
xui_method_explain(xui_drawer, on_textchanged,		void					)( xui_method_args& args )
{
	xm_textchanged(this, args);
}
xui_method_explain(xui_drawer, on_fontchanged,		void					)( xui_method_args& args )
{
	xm_fontchanged(this, args);
}
xui_method_explain(xui_drawer, on_renderself,		void					)( xui_method_args& args )
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
		xui_convas::get_ins()->draw_text(
			text, 
			m_textfont, 
			get_rendertextrt()+get_screenpt(), 
			textdraw);
	}

	// draw icon
	if (icon)
	{
		xui_convas::get_ins()->draw_image(
			icon, 
			xui_rect2d<s32>(get_rendericonpt()+get_screenpt(), m_iconsize), 
			color);
	}
}
/*
//virtual
*/
xui_method_explain(xui_drawer, get_rendericon,		xui_bitmap*				)( void ) const
{
	return m_icon;
}
xui_method_explain(xui_drawer, get_rendertext,		std::wstring			)( void ) const
{
	return m_text;
}
xui_method_explain(xui_drawer, get_rendericonpt,	xui_vector<s32>			)( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	switch (m_iconalign)
	{
	case IMAGE_L:
		pt.x = rt.ax;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	case IMAGE_T:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay;
		break;
	case IMAGE_R:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w);
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	case IMAGE_B:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h);
		break;
	case IMAGE_C:
		pt.x = rt.ax + (rt.get_w() - m_iconsize.w) / 2;
		pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;
		break;
	}

	return pt + m_iconoffset;
}
xui_method_explain(xui_drawer, get_rendertextrt,	xui_rect2d<s32>			)( void ) const
{
	std::wstring    text = get_rendertext  ();
	xui_rect2d<s32> rt   = get_renderrtins ();
	xui_vector<s32> pt   = get_rendericonpt();
	switch (m_iconalign)
	{
	case IMAGE_FRONT_TEXT:
		rt.ax = pt.x + m_iconsize.w;
		break;
	case IMAGE_AFTER_TEXT:
		rt.bx = pt.x;
		break;
	case IMAGE_ABOVE_TEXT:
		rt.ay = pt.y + m_iconsize.h;
		break;
	case IMAGE_BELOW_TEXT: 
		rt.by = pt.y;
		break;
	}

	rt.ax += m_textoffset.x;
	rt.ay += m_textoffset.y;
	if (text.length() > 0)
	{
		rt = xui_convas::get_ins()->calc_draw(
			text, 
			m_textfont, 
			rt, 
			m_textalign, 
			m_singleline);
	}

	return rt;
}