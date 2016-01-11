#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_desktop.h"
#include "xui_toggle.h"

/*
//constructor
*/
xui_create_explain(xui_toggle)( const std::string& name, const xui_rect2d<s32>& rect, u08 style )
: xui_button(name, rect)
{
	m_type += "toggle";
	m_check = false;
	set_style(style);
}

/*
//init
*/
xui_method_explain(xui_toggle, ini_toggle,			void		)( bool flag )
{
	m_check = flag;
}

/*
//check
*/
xui_method_explain(xui_toggle, was_check,			bool		)( void ) const
{
	return m_check;
}

xui_method_explain(xui_toggle, set_check,			void		)( bool flag )
{
	if (m_check != flag)
	{
		m_check  = flag;

		xui_method_args args;
		xm_click(this,  args);
	}
}

/*
//style
*/
xui_method_explain(xui_toggle, get_style,			u08			)( void ) const
{
	return m_style;
}

xui_method_explain(xui_toggle, set_style,			void		)( u08 style )
{
	m_style = style;
	set_drawcolor(m_style != TOGGLE_CIRCLE);
}

/*
//callback
*/
xui_method_explain(xui_toggle, on_mousedown,		void		)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	set_check(!m_check);
}

xui_method_explain(xui_toggle, on_renderself,		void		)( xui_method_args&  args )
{
	switch (m_style)
	{
	case TOGGLE_CIRCLE:
		{
			xui_colour      color = get_vertexcolor();
			xui_rect2d<s32> rt    = get_renderrtins()+get_screenpt();

			s32 d = rt.get_sz().w < rt.get_sz().h ? rt.get_sz().w : rt.get_sz().h;
			rt.oft_x((rt.get_sz().w-d) / 2);
			rt.oft_y((rt.get_sz().h-d) / 2);
			rt.set_w(d);
			rt.set_h(d);
			g_convas->fill_round(rt, color*get_rendercolor(), d/2);
			g_convas->draw_round(rt, color*m_sidecolor,		  d/2);
		}
		break;
	case TOGGLE_NORMAL:
	case TOGGLE_BUTTON:
		{
			xui_button::on_renderself(args);
		}
		break;
	}
}

/*
//virtual
*/
xui_method_explain(xui_toggle, get_rendercolor,		xui_colour		)( void ) const
{
	if (m_check)
		return m_downcolor;

	return xui_button::get_rendercolor();
}

xui_method_explain(xui_toggle, get_rendericon,		xui_bitmap*		)( void ) const
{
	if (m_style == TOGGLE_NORMAL && m_check == false)
		return NULL;

	return xui_button::get_rendericon();
}

xui_method_explain(xui_toggle, get_rendericonpt,	xui_vector<s32>	)( void ) const
{
	xui_bitmap* icon = get_rendericon();
	if (m_style == TOGGLE_NORMAL && icon)
	{
		xui_vector<s32> pt;
		xui_rect2d<s32> rt = get_renderrtins();
		pt.x = rt.ax + (rt.get_sz().w - icon->get_size().w) / 2;
		pt.y = rt.ay + (rt.get_sz().h - icon->get_size().h) / 2;
		return pt + m_iconoffset;
	}
	else
	{
		return xui_button::get_rendericonpt();
	}
}

xui_method_explain(xui_toggle, get_rendertextrt,	xui_rect2d<s32>	)( void ) const
{
	std::wstring text = get_rendertext();
	if (m_style == TOGGLE_NORMAL && text.length() > 0)
	{
		xui_rect2d<s32> rt = get_renderrtins() + get_screenpt();
		rt.oft_x(rt.get_sz().w);
		rt = g_convas->calc_rect(
			text, 
			m_font, 
			rt,
			TA_LC, 
			false);

		return rt + m_textoffset;
	}
	else
	{
		return xui_button::get_rendertextrt();
	}
}