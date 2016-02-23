#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"

xui_implement_rtti(xui_menu, xui_control);

/*
//static
*/
xui_method_explain(xui_menu, create,			xui_menu*		)( s32 width )
{
	xui_menu* menu = new xui_menu(xui_vector<s32>(width, 0));
	xui_method_ptrcall(menu, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(menu, set_drawcolor	)(true);
	xui_method_ptrcall(menu, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(menu, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(menu, set_corner		)(3);

	return menu;
}
/*
//constructor
*/
xui_create_explain(xui_menu)( const xui_vector<s32>& size )
: xui_control(size)
{
	m_ownertoggle = NULL;
	m_showsubmenu = NULL;
}

/*
//method
*/
xui_method_explain(xui_menu, was_series,		bool			)( xui_component* comp )
{
	if (comp == this)
		return true;

	xui_vecptr_addloop(m_widgetvec)
	{
		if (m_widgetvec[i] == comp)
			return true;
	}

	if (m_showsubmenu && m_showsubmenu->was_series(comp))
		return true;

	return false;
}
xui_method_explain(xui_menu, get_ownertoggle,	xui_toggle*		)( void )
{
	return m_ownertoggle;
}
xui_method_explain(xui_menu, set_ownertoggle,	void			)( xui_toggle* toggle )
{
	m_ownertoggle = toggle;
}
xui_method_explain(xui_menu, get_showsubmenu,	xui_menu*		)( void )
{
	return m_showsubmenu;
}
xui_method_explain(xui_menu, set_showsubmenu,	void			)( xui_menu* submenu )
{
	if (m_showsubmenu != submenu)
	{
		m_showsubmenu  = submenu;
		if (m_showsubmenu)
			m_showsubmenu->set_showsubmenu(NULL);
	}
}

/*
//item
*/
xui_method_explain(xui_menu, add_separate,		xui_component*	)( void )
{
	xui_separate* separate = new xui_separate(xui_vector<s32>(8), FLOWSTYLE_V);
	separate->set_parent(this);
	m_widgetvec.push_back(separate);
	invalid();

	return separate;
}
xui_method_explain(xui_menu, add_item,			xui_menuitem*	)( xui_bitmap* icon, const std::wstring& text, const std::wstring& hint, const xui_family_render& draw )
{
	xui_menuitem* item = new xui_menuitem();
	xui_method_ptrcall(item, ini_drawer	)(icon);
	xui_method_ptrcall(item, ini_drawer	)(text);
	xui_method_ptrcall(item, set_hint	)(hint);
	xui_method_ptrcall(item, set_draw	)(draw);
	add_item(item);

	return item;
}
xui_method_explain(xui_menu, add_item,			void			)( xui_menuitem* item )
{
	if (item->get_parent())
		return;

	xui_method_ptrcall(item, set_drawcolor	)(true);
	xui_method_ptrcall(item, set_borderrt	)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(item, set_iconsize	)(xui_vector<s32>(16));
	xui_method_ptrcall(item, set_iconalign	)(IMAGE_FRONT_TEXT);
	xui_method_ptrcall(item, set_textalign	)(TA_LC);
	xui_method_ptrcall(item, set_textoffset	)(xui_vector<s32>(8, 0));
	xui_method_ptrcall(item, set_renderh	)(20);
	xui_method_ptrcall(item, set_parent		)(this);
	m_widgetvec.push_back(item);
	invalid();
}
xui_method_explain(xui_menu, del_item,			void			)( xui_menuitem* item )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		item);

	if (itor == m_widgetvec.end())
		return;

	item->set_parent(NULL);
	delete item;
	m_widgetvec.erase(itor);
	invalid();
}

/*
//override
*/
xui_method_explain(xui_menu, choose,			xui_component*	)( const xui_vector<s32>& pt )
{
	if (m_showsubmenu)
	{
		xui_component* component = NULL;
		if (component = m_showsubmenu->choose(pt))
			return component;
	}

	return xui_control::choose(pt);
}
xui_method_explain(xui_menu, update,			void			)( f32 delta )
{
	xui_control::update(delta);
	if (m_showsubmenu)
		m_showsubmenu->update(delta);
}
xui_method_explain(xui_menu, render,			void			)( void )
{
	xui_control::render();
	if (m_showsubmenu)
		m_showsubmenu->render();
}

/*
//callback
*/
xui_method_explain(xui_menu, on_nonfocus,		void			)( xui_method_args& args )
{
	xui_control::on_nonfocus(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, xui_desktop::get_ins()->get_floatctrl());
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || menu == NULL || menu->was_series(focusctrl) == false)
	{
		xui_desktop::get_ins()->set_floatctrl(NULL);
		xui_toggle* toggle = menu->get_ownertoggle();
		if (toggle && focusctrl != toggle)
			toggle->ini_toggle(false);
	}
}
xui_method_explain(xui_menu, on_invalid,		void			)( xui_method_args& args )
{
	s32 maxw = 0;
	s32 curh = 0;
	xui_vecptr_addloop(m_widgetvec)
	{
		curh += m_widgetvec[i]->get_renderh();
		xui_menuitem* item = xui_dynamic_cast(xui_menuitem, m_widgetvec[i]);
		if (item)
			maxw = xui_max(maxw, item->get_maxwidth());
	}

	xui_vector<s32> sz(maxw+m_border.ax+m_border.bx, curh+m_border.ay+m_border.by);
	sz.w = xui_max(sz.w, m_render.get_w());
	sz.h = xui_max(sz.h, m_render.get_h());

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}

}
xui_method_explain(xui_menu, on_perform,		void			)( xui_method_args& args )
{
	xui_vector<s32> pt(m_border.ax, m_border.ay);
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vecptr_addloop(m_widgetvec)
	{
		xui_component* comp = m_widgetvec[i];
		xui_menuitem*  item = xui_dynamic_cast(xui_menuitem, comp);
		if (item)
		{
			item->on_perform_pt(pt);
			item->on_perform_w (rt.get_w());
		}
		else
		{
			comp->on_perform_pt(pt+xui_vector<s32>(24, 0));
			comp->on_perform_w (rt.get_w()-24);
		}

		pt.y += comp->get_renderh();
	}
}