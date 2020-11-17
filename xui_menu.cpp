#include "xui_desktop.h"
#include "xui_separate.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"

xui_implement_rtti(xui_menu, xui_control)

xui_menu* xui_menu::create( s32 width )
{
	xui_menu* menu = new xui_menu(xui_vector<s32>(width, 0));
	menu->set_backcolor(xui_colour::k_darkgray);
	menu->set_drawcolor(true);
	menu->set_sidestyle(k_sidestyle_s);
	menu->set_borderrt(xui_rect2d<s32>(4));
	menu->set_corner(3);

	return menu;
}

xui_menu::xui_menu( const xui_vector<s32>& size )
: xui_control(size)
{
	m_ownertoggle = NULL;
	m_showsubmenu = NULL;

	xui_action_ctrl_impl<f32>* action = new xui_action_ctrl_impl<f32>(this);
	action->add_time(0.0f);
	action->add_time(0.1f);
	action->add_data(0.2f);
	action->add_data(1.0f);
	action->set_soft(true);
	action->xm_tick += new xui_method_member<xui_method_args, xui_menu>(this, &xui_menu::on_popactiontick);
	m_popaction	= action;
}

bool xui_menu::was_series( xui_component* comp )
{
	if (comp == this)
		return true;

	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		if (m_widgetvec[i] == comp)
			return true;
	}

	if (m_showsubmenu && m_showsubmenu->was_series(comp))
		return true;

	return false;
}

xui_toggle* xui_menu::get_ownertoggle( void )
{
	return m_ownertoggle;
}

void xui_menu::set_ownertoggle( xui_toggle* toggle )
{
	m_ownertoggle = toggle;
}

xui_menu* xui_menu::get_showsubmenu( void )
{
	return m_showsubmenu;
}

void xui_menu::set_showsubmenu( xui_menu* submenu )
{
	if (m_showsubmenu != submenu)
	{
		m_showsubmenu  = submenu;
		if (m_showsubmenu)
		{
			m_showsubmenu->refresh();
			m_showsubmenu->set_showsubmenu(NULL);
		}
	}
}

std::vector<xui_menuitem*> xui_menu::get_itemall( void )
{
	std::vector<xui_menuitem*> result;
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
	{
		xui_menuitem* item = xui_dynamic_cast(xui_menuitem, m_widgetvec[i]);
		if (item)
			result.push_back(item);
	}

	return result;
}

xui_component* xui_menu::add_separate( void )
{
	xui_separate* separate = new xui_separate(xui_vector<s32>(8), k_flowstyle_v);
	separate->set_parent(this);
	m_widgetvec.push_back(separate);
	invalid();

	return separate;
}

xui_menuitem* xui_menu::add_item( xui_bitmap* icon, const std::wstring& text, const std::wstring& hint, const xui_family_render& draw )
{
	xui_menuitem* item = new xui_menuitem();
	item->ini_drawer(icon);
	item->ini_drawer(text);
	item->set_hint(hint);
	item->set_draw(draw);
	add_item(item);

	return item;
}

void xui_menu::add_item( xui_menuitem* item )
{
	if (item->get_parent())
		return;

	item->set_drawcolor(true);
	item->set_borderrt(xui_rect2d<s32>(4, 2, 4, 2));
	item->set_iconsize(xui_vector<s32>(16));
	item->set_iconalign(k_image_front_text);
	item->set_textalign(k_textalign_lc);
	item->set_textoffset(xui_vector<s32>(8, 0));
	item->set_renderh(20);
	item->set_parent(this);
	m_widgetvec.push_back(item);
	invalid();
}

void xui_menu::del_item( xui_menuitem* item )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		item);

	if (itor == m_widgetvec.end())
		return;

	item->set_parent(NULL);
	xui_desktop::get_ins()->move_recycle(item);
	m_widgetvec.erase(itor);
	invalid();
}

xui_component* xui_menu::choose( const xui_vector<s32>& pt )
{
	if (m_showsubmenu)
	{
		xui_component* component = NULL;
		if (component = m_showsubmenu->choose(pt))
			return component;
	}

	return xui_control::choose(pt);
}

void xui_menu::update( f32 delta )
{
	xui_control::update(delta);
	if (m_showsubmenu)
		m_showsubmenu->update(delta);
}

void xui_menu::render( void )
{
	xui_control::render();
	if (m_showsubmenu)
		m_showsubmenu->render();
}

void xui_menu::on_nonfocus( xui_method_args& args )
{
	xui_control::on_nonfocus(args);
	xui_menu* menu = xui_dynamic_cast(xui_menu, xui_desktop::get_ins()->get_floatctrl());
	xui_component* focusctrl = (xui_component*)args.wparam;
	if (focusctrl == NULL || menu == NULL || menu->was_series(focusctrl) == false)
	{
		xui_desktop::get_ins()->set_floatctrl(NULL, NULL);
		xui_toggle* toggle = menu->get_ownertoggle();
		if (toggle && focusctrl != toggle)
			toggle->ini_toggle(false);
	}
}

void xui_menu::on_invalid( xui_method_args& args )
{
	s32 maxw = 0;
	s32 curh = 0;
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
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

void xui_menu::on_perform( xui_method_args& args )
{
	xui_vector<s32> pt(m_border.ax, m_border.ay);
	xui_rect2d<s32> rt = get_renderrtins();
	for (u32 i = 0; i < m_widgetvec.size(); ++i)
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

void xui_menu::on_popactiontick( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_popaction;
	m_backscale = xui_vector<f32>(action->sample());
}