#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_toggle.h"

xui_implement_rtti(xui_toggle, xui_button);

/*
//static
*/
xui_method_explain(xui_toggle, circle,				xui_toggle*		)( void )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16), TOGGLE_CIRCLE);
	xui_method_ptrcall(toggle, set_drawcolor	)(false);
	return toggle;
}
xui_method_explain(xui_toggle, create,				xui_toggle*		)( void )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(16), TOGGLE_NORMAL);
	xui_method_ptrcall(toggle, set_corner		)(3);
	xui_method_ptrcall(toggle, set_drawcolor	)(true);
	xui_method_ptrcall(toggle, set_backcolor	)(xui_colour::darkgray);

	return toggle;
}
xui_method_explain(xui_toggle, create,				xui_toggle*		)( xui_bitmap* icon )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(24), TOGGLE_BUTTON);
	xui_method_ptrcall(toggle, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(toggle, set_corner		)(3);
	xui_method_ptrcall(toggle, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(toggle, set_drawcolor	)(true);
	xui_method_ptrcall(toggle, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(toggle, ini_drawer		)(icon, xui_vector<s32>(16));

	return toggle;
}
xui_method_explain(xui_toggle, create,				xui_toggle*		)( xui_bitmap* icon, const std::wstring& text, s32 width )
{
	xui_toggle* toggle = new xui_toggle(xui_vector<s32>(width, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(toggle, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(toggle, set_corner		)(3);
	xui_method_ptrcall(toggle, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(toggle, set_drawcolor	)(true);
	xui_method_ptrcall(toggle, set_textoffset	)(xui_vector<s32>(6, 0));
	xui_method_ptrcall(toggle, ini_drawer		)(icon, xui_vector<s32>(16));
	xui_method_ptrcall(toggle, ini_drawer		)(text);

	return toggle;
}

/*
//constructor
*/
xui_create_explain(xui_toggle)( const xui_vector<s32>& size, u08 drawstyle )
: xui_button(size)
{
	m_menu		= NULL;
	m_push		= false;
	m_drawstyle = drawstyle;
}

/*
//destructor
*/
xui_delete_explain(xui_toggle)( void )
{
	delete m_menu;
}

/*
//init
*/
xui_method_explain(xui_toggle, ini_toggle,			void			)( bool push )
{
	m_push = push;
}

/*
//check
*/
xui_method_explain(xui_toggle, was_push,			bool			)( void ) const
{
	return m_push;
}
xui_method_explain(xui_toggle, set_push,			void			)( bool push )
{
	if (m_push != push)
	{
		m_push  = push;

		xui_method_args args;
		xm_click(this,  args);

		if (m_menu)
		{
			if (m_push)
			{
				xui_vector<s32> pt = get_screenpt() + xui_vector<s32>(0, get_renderh());
				xui_method_ptrcall(m_menu, set_renderpt		)(pt);
				xui_method_ptrcall(m_menu, set_showsubmenu	)(NULL);
				xui_method_ptrcall(m_menu, req_focus		)();
				xui_desktop::get_ins()->set_floatctrl(m_menu);
			}
			else
			{
				xui_desktop::get_ins()->set_floatctrl(NULL);
			}
		}
	}
}

/*
//menu
*/
xui_method_explain(xui_toggle, get_menu,			xui_menu*		)( void )
{
	return m_menu;
}
xui_method_explain(xui_toggle, set_menu,			void			)( xui_menu* menu )
{
	m_menu = menu;

	if (m_menu)
		m_menu->set_ownertoggle(this);
}

/*
//callback
*/
xui_method_explain(xui_toggle, on_mousedown,		void			)( xui_method_mouse& args )
{
	xui_button::on_mousedown(args);
	if (args.mouse == MB_L)
		set_push(!m_push);
}
xui_method_explain(xui_toggle, on_renderself,		void			)( xui_method_args&  args )
{
	if (m_drawstyle == TOGGLE_NORMAL && m_push == false)
		return;

	xui_colour      color  = get_vertexcolor();
	xui_rect2d<s32> rt     = get_renderrtabs();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	switch (m_drawstyle)
	{
	case TOGGLE_CIRCLE:
		{
			if (was_hover() || m_push)
			xui_convas::get_ins()->fill_circle(center, 5,	 color*get_rendercolor(),	0, 360);
			xui_convas::get_ins()->draw_circle(center, 5,	 color*m_sidecolor,			0, 360);
		}
		break;
	case TOGGLE_NORMAL:
		{
			s32	half = rt.get_w()/2;
			xui_convas::get_ins()->draw_tick  (center, half, color*xui_colour::white);
		}
		break;
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
	if (m_drawstyle == TOGGLE_NORMAL)
	{
		return m_backcolor;
	}
	else
	if (m_drawstyle == TOGGLE_CIRCLE)
	{
		if (m_push)
		{
			if (was_hover())return m_movecolor;
			else			return m_backcolor;
		}
		else
		{
			return xui_colour::transparent;
		}
	}
	else
	{
		if (m_push)
			return m_downcolor;
		else
			return xui_button::get_rendercolor();
	}
}
xui_method_explain(xui_toggle, update_else,			void			)( f32 delta )
{
	xui_button::update_else(delta);
	if (m_menu)
		m_menu->update(delta);
}