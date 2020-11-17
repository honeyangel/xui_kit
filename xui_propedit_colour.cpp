#include "xui_canvas.h"
#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_numbbox.h"
#include "xui_plusctrl.h"
#include "xui_propctrl.h"
#include "xui_propedit_colour.h"

xui_propedit_colour::xui_propedit_colour( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_drawer* editctrl = new xui_drawer(xui_vector<s32>(48, 18));
	editctrl->set_sidestyle(k_sidestyle_s);
	editctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlnonfocus);
	editctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlgetfocus);
	editctrl->xm_renderself	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlrenderself);
	editctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlclick);

	xui_drawer* pickctrl = new xui_drawer(xui_vector<s32>(16, 16));
	pickctrl->ini_drawer(xui_global::s_icon_pickcolour);
	pickctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlnonfocus);
	pickctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlgetfocus);
	pickctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlclick);
	pickctrl->xm_keybddown	+= new xui_method_member<xui_method_keybd, xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlkeybddown);

	m_editctrl = editctrl;
	m_pickctrl = pickctrl;
}

xui_drawer* xui_propedit_colour::get_pickctrl( void ) const
{
	return m_pickctrl;
}

const xui_colour& xui_propedit_colour::get_value( void ) const
{
	return m_editctrl->get_backcolor();
}

void xui_propedit_colour::set_value( const xui_colour& value )
{
	m_editctrl->set_backcolor(value);
}

void xui_propedit_colour::reset( void )
{
	xui_propedit_base::reset();
	m_editctrl->set_backcolor(xui_colour::k_black);
}

void xui_propedit_colour::on_editctrlrenderself( xui_component* sender, xui_method_args& args )
{
	xui_colour      color = xui_global::was_scolorstart() ? xui_global::get_scolor() : m_editctrl->get_backcolor();
	xui_rect2d<s32> rt    = m_editctrl->get_renderrtabs();
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_black);
	rt.by -= 4;
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour(1.0f, color.r, color.g, color.b));
	rt.ay  = rt.by;
	rt.by += 4;
	rt.set_w((s32)((f32)rt.get_w() * color.a));
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_white);
}

void xui_propedit_colour::on_pickctrlnonfocus( xui_component* sender, xui_method_args& args )
{
	if (xui_global::was_scolorstart())
	{
		m_editctrl->set_backcolor(xui_global::get_scolor());
		m_propctrl->on_editvalue(this);
		xui_global::set_scolorclose();
	}
}

void xui_propedit_colour::on_pickctrlgetfocus( xui_component* sender, xui_method_args& args )
{
	xui_component* last = (xui_component*)args.wparam;
	if (last == NULL || last->get_ancestor(xui_propctrl::ptr_rtti()) != m_propctrl)
		m_propctrl->on_readyundo();
}

void xui_propedit_colour::on_editctrlclick( xui_component* sender, xui_method_mouse& args )
{
	xui_colour_pickwnd* wnd = xui_colour_pickwnd::get_ptr();
	if (wnd)
	{
		wnd->set_propctrl(m_propctrl);
		wnd->set_visible(true);
		wnd->set_value(get_value());
		wnd->req_focus();
	}
}

void xui_propedit_colour::on_pickctrlclick( xui_component* sender, xui_method_mouse& args )
{
	xui_window* window = sender->get_window();
	if (window)
	{
		xui_global::set_scolorstart(window->get_owner());
	}
}

void xui_propedit_colour::on_pickctrlkeybddown( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == k_key_esc)
	{
		xui_global::set_scolorclose();
	}
}

//////////////////////////////////////////////////////////////////////////
//xui_colour_pickwnd
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_colour_pickwnd, xui_window)

xui_colour_pickwnd::xui_colour_pickwnd( void )
: xui_window(xui_vector<s32>(226, 300), false)
, m_propctrl(NULL)
{
	m_pickctrl = new xui_drawer(xui_vector<s32>(32, 20));
	m_pickctrl->xm_mouseclick	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_pickctrlclick);
	m_pickctrl->xm_keybddown	 += new xui_method_member<xui_method_keybd,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_pickctrlkeybddown);
	m_pickctrl->set_iconalign(k_image_c);
	m_pickctrl->ini_component(0, 0, k_dockstyle_l);
	m_pickctrl->ini_drawer(xui_global::s_icon_pickcolour);
	m_mainview = new xui_drawer(xui_vector<s32>(80, 20));
	m_mainview->xm_renderself	 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainviewrenderself);
	m_mainview->set_sidestyle(k_sidestyle_s);
	m_mainview->ini_component(0, 0, k_dockstyle_f);
	m_mainpane = new xui_panel(xui_vector<s32>(32));
	m_mainpane->ini_component(0, 0, k_dockstyle_t);
	m_mainpane->set_drawcolor(false);
	m_mainpane->set_borderrt(xui_rect2d<s32>(8, 0, 8, 12));
	m_mainpane->set_hscrollauto(false);
	m_mainpane->add_child(m_pickctrl);
	m_mainpane->add_child(m_mainview);

	m_viewplus = new xui_plusctrl(k_plusrender_normal, true);
	m_viewplus->ini_component(0, 0, k_dockstyle_l);
	m_viewplus->ini_component(true, true);
	m_viewplus->xm_expand		 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_plusctrlexpand);
	m_viewname = new xui_drawer(xui_vector<s32>(80, 16));
	m_viewname->ini_component(0, 0, k_dockstyle_l);
	m_viewname->ini_drawer(L"Colors");
	m_compctrl = new xui_drawer(xui_vector<s32>(16));
	m_compctrl->xm_renderself	 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compctrlrenderself);
	m_compctrl->xm_mouseclick	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compctrlclick);
	m_compctrl->ini_component(0, 0, k_dockstyle_r);
	m_viewhead = new xui_panel(xui_vector<s32>(20));
	m_viewhead->ini_component(0, 0, k_dockstyle_t);
	m_viewhead->set_drawcolor(false);
	m_viewhead->set_borderrt(xui_rect2d<s32>(8, 2, 8, 2));
	m_viewhead->set_hscrollauto(false);
	m_viewhead->add_child(m_viewplus);
	m_viewhead->add_child(m_viewname);
	m_viewhead->add_child(m_compctrl);

	m_viewctrl = new xui_drawer(xui_vector<s32>(180));
	m_viewctrl->xm_renderself	 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewctrlrenderself);
	m_viewctrl->xm_mousedown	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewctrldowndrag);
	m_viewctrl->xm_mousemove	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewctrldowndrag);
	m_viewctrl->ini_component(0, 0, k_dockstyle_l);
	m_viewctrl->set_sidestyle(k_sidestyle_s);
	m_mainroll = new xui_drawer(xui_vector<s32>(18));
	m_mainroll->xm_renderself	 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrollrenderself);
	m_mainroll->xm_mousedown	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrolldowndrag);
	m_mainroll->xm_mousemove	 += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrolldowndrag);
	m_mainroll->ini_component(0, 0, k_dockstyle_r);
	m_mainroll->set_sidestyle(k_sidestyle_s);
	m_viewpane = new xui_panel(xui_vector<s32>(210, 168));
	m_viewpane->xm_renderself	 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewpanerenderself);
	m_viewpane->ini_component(0, 0, k_dockstyle_t);
	m_viewpane->set_drawcolor(false);
	m_viewpane->set_borderrt(xui_rect2d<s32>(8, 4, 8, 4));
	m_viewpane->set_hscrollauto(false);
	m_viewpane->add_child(m_viewctrl);
	m_viewpane->add_child(m_mainroll);

	m_compplus = new xui_plusctrl(k_plusrender_normal, true);
	m_compplus->xm_expand		 += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_plusctrlexpand);
	m_compplus->ini_component(0, 0, k_dockstyle_l);
	m_compplus->ini_component(true, true);
	m_compname = new xui_drawer(xui_vector<s32>(80, 16));
	m_compname->ini_component(0, 0, k_dockstyle_l);
	m_compname->ini_drawer(L"Slider");
	m_comphead = new xui_panel(xui_vector<s32>(20));
	m_comphead->ini_component(0, 0, k_dockstyle_t);
	m_comphead->set_drawcolor(false);
	m_comphead->set_borderrt(xui_rect2d<s32>(8, 2, 8, 2));
	m_comphead->set_hscrollauto(false);
	m_comphead->add_child(m_compplus);
	m_comphead->add_child(m_compname);

	m_comppane = new xui_panel(xui_vector<s32>(96));
	m_comppane->xm_renderself	 += new xui_method_member<xui_method_args,  xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comppanerenderself);
	m_comppane->ini_component(0, 0, k_dockstyle_t);
	m_comppane->set_borderrt(xui_rect2d<s32>(8, 0, 8, 0));
	m_comppane->set_drawcolor(false);
	m_comppane->set_hscrollauto(false);
	for (u32 i = 0; i < 4; ++i)
	{
		xui_drawer*  text = new xui_drawer(xui_vector<s32>( 16, 16));
		text->ini_component(0, 0, k_dockstyle_l);
		xui_drawer*  roll = new xui_drawer(xui_vector<s32>(130, 16));
		roll->xm_renderself	    += new xui_method_member<xui_method_args,  xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprollrenderself);
		roll->xm_mousedown	    += new xui_method_member<xui_method_mouse, xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprolldowndrag);
		roll->xm_mousemove	    += new xui_method_member<xui_method_mouse, xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprolldowndrag);
		roll->ini_component(0, 0, k_dockstyle_l);
		roll->set_sidestyle(k_sidestyle_s);
		xui_numbbox* edit = new xui_numbbox(xui_vector<s32>(60, 16), k_nt_unsignedint, 1, false);
		edit->xm_textchanged	 += new xui_method_member<xui_method_args,  xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compedittextchanged);
		edit->ini_component(0, 0, k_dockstyle_r);
		edit->set_sidestyle(k_sidestyle_s);
		edit->set_borderrt(xui_rect2d<s32>(2));
		edit->set_textalign(k_textalign_rc);

		xui_panel*   pane = new xui_panel(xui_vector<s32>(24));
		pane->ini_component(0, 0, k_dockstyle_t);
		pane->set_drawcolor(false);
		pane->set_borderrt(xui_rect2d<s32>(0, 4, 0, 4));
		pane->set_hscrollauto(false);
		pane->add_child(text);
		pane->add_child(roll);
		pane->add_child(edit);

		switch (i)
		{
		case 0: text->ini_drawer(L"R"); break;
		case 1: text->ini_drawer(L"G"); break;
		case 2: text->ini_drawer(L"B"); break;
		case 3: text->ini_drawer(L"A"); break;
		}

		m_comptext[i] = text;
		m_comproll[i] = roll;
		m_compedit[i] = edit;
		m_comppane->add_child(pane);
	}

	add_child(m_mainpane);
	add_child(m_viewhead);
	add_child(m_viewpane);
	add_child(m_comphead);
	add_child(m_comppane);
}

xui_colour_pickwnd* xui_colour_pickwnd::get_ptr( void )
{
	static xui_colour_pickwnd* ptr = NULL;
	if (ptr == NULL)
	{
		ptr = new xui_colour_pickwnd;
		ptr->xm_nonfocus  += new xui_method_member<xui_method_args, xui_colour_pickwnd>(ptr, &xui_colour_pickwnd::on_windownonfocus);
		ptr->set_sidestyle(k_sidestyle_s);
		ptr->set_borderrt(xui_rect2d<s32>(0, 8, 0, 8));
		ptr->set_renderpt(xui_vector<s32>(100));
		ptr->ini_component(k_alignhorz_c, 0, 0);
		ptr->ini_component(true, false);
		xui_desktop::get_ins()->add_child(ptr);
	}

	return ptr;
}

const xui_colour& xui_colour_pickwnd::get_value( void ) const
{
	return m_mainview->get_backcolor();
}

void xui_colour_pickwnd::set_value( const xui_colour& value )
{
	m_mainview->set_backcolor(value);
	set_colortext();
}

xui_propctrl* xui_colour_pickwnd::get_propctrl( void )
{
	return m_propctrl;
}

void xui_colour_pickwnd::set_propctrl( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}

void xui_colour_pickwnd::on_invalid( xui_method_args& args )
{
	xui_window::on_invalid(args);

	s32 h = m_border.ay + m_border.by;
	h += m_mainpane->get_renderh();
	h += m_viewhead->get_renderh();
	h += m_comphead->get_renderh();
	h += m_viewpane->was_visible() ? m_viewpane->get_renderh() : 0;
	h += m_comppane->was_visible() ? m_comppane->get_renderh() : 0;

	set_renderh(h);
}

void xui_colour_pickwnd::on_windownonfocus( xui_component* sender, xui_method_args& args )
{
	if (xui_global::was_scolorstart())
	{
		m_mainview->set_backcolor(xui_global::get_scolor());
		m_compctrl->set_visible(true);
		m_viewctrl->set_visible(true);
		m_mainroll->set_visible(true);
		set_colordata();
		set_colortext();
		xui_global::set_scolorclose();

		req_focus();
	}
	else
	{
		xui_component* focusctrl = (xui_component*)args.wparam;
		if (focusctrl != this && focusctrl->was_ancestor(this) == false)
			set_visible(false);
	}
}

void xui_colour_pickwnd::on_plusctrlexpand( xui_component* sender, xui_method_args& args )
{
	m_viewpane->set_visible(m_viewplus->was_expanded());
	m_comppane->set_visible(m_compplus->was_expanded());
	refresh();
}

void xui_colour_pickwnd::on_pickctrlclick( xui_component* sender, xui_method_mouse& args )
{
	xui_global::set_scolorstart(get_owner());
	m_compctrl->set_visible(false);
	m_viewctrl->set_visible(false);
	m_mainroll->set_visible(false);
}

void xui_colour_pickwnd::on_pickctrlkeybddown( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == k_key_esc)
	{
		m_compctrl->set_visible(true);
		m_viewctrl->set_visible(true);
		m_mainroll->set_visible(true);
		xui_global::set_scolorclose();
	}
}

void xui_colour_pickwnd::on_mainviewrenderself( xui_component* sender, xui_method_args& args )
{
	xui_colour      color = xui_global::was_scolorstart() ? xui_global::get_scolor() : m_mainview->get_backcolor();
	xui_rect2d<s32> rt    = m_mainview->get_renderrtabs();
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_black);
	rt.by -= 4;
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour(1.0f, color.r, color.g, color.b));
	rt.ay  = rt.by;
	rt.by += 4;
	rt.set_w((s32)((f32)rt.get_w() * color.a));
	xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_white);
}

void xui_colour_pickwnd::on_compctrlrenderself( xui_component* sender, xui_method_args& args )
{
	std::vector<u32> vec;
	for (u32 i = 0; i < 3; ++i)
	{
		vec.push_back(((u32)(long long)m_compctrl->get_data()+i) % 3);
	}
	xui_rect2d<s32> rt(sender->get_screenpt(), xui_vector<s32>(8));
	for (s32 i = (s32)vec.size()-1; i >= 0; --i)
	{
		u32 index = vec[i];
		xui_colour color(1.0f, 0.0f);
		color.value[index] = 1.0f;
		xui_canvas::get_ins()->fill_rectangle(rt, color);
		rt.oft_x(4);
		rt.oft_y(4);
	}
}

void xui_colour_pickwnd::on_compctrlclick( xui_component* sender, xui_method_mouse& args )
{
	u32 index = (u32)(long long)m_compctrl->get_data() + 1;
	if (index > 2)
		index = 0;

	m_compctrl->set_data((void*)index);
}

void xui_colour_pickwnd::on_viewpanerenderself( xui_component* sender, xui_method_args& args )
{
	if (m_mainroll->was_visible())
	{
		xui_colour color = m_mainview->get_backcolor();
		u32 index = (u32)(long long)m_compctrl->get_data();
		xui_rect2d<s32> rt = m_mainroll->get_renderrtabs();
		s32 y = rt.by - (s32)(rt.get_h()*color.value[index]);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(rt.ax-1, y  );
		poly[1] = xui_vector<s32>(rt.ax-3, y-2);
		poly[2] = xui_vector<s32>(rt.ax-3, y+2);
		xui_canvas::get_ins()->draw_path(poly, 3, xui_colour::k_white);
		poly[0] = xui_vector<s32>(rt.bx,   y  );
		poly[1] = xui_vector<s32>(rt.bx+2, y-2);
		poly[2] = xui_vector<s32>(rt.bx+2, y+2);
		xui_canvas::get_ins()->draw_path(poly, 3, xui_colour::k_white);
	}
	else
	{
		xui_rect2d<s32> rt = m_viewpane->get_renderrtins() + m_viewpane->get_screenpt();
		s32 r = rt.get_h()/10;
		s32 c = rt.get_w()/10;
		std::vector<xui_colour> vec = xui_global::get_scolor(xui_vector<s32>(c, r));
		for (s32 ir = 0; ir < r; ++ir)
		{
			for (s32 ic = 0; ic < c; ++ic)
			{
				s32 index = ir*c+ic;
				xui_rect2d<s32> colorrt = rt;
				colorrt.oft_x(ic*10);
				colorrt.oft_y(ir*10);
				colorrt.set_w(10);
				colorrt.set_h(10);
				xui_canvas::get_ins()->fill_rectangle(colorrt, vec[index]);
			}
		}

		for (s32 ir = 0; ir < r+1; ++ir)
		{
			xui_vector<s32> p1 = rt.get_pt() + xui_vector<s32>(0,			ir*10);
			xui_vector<s32> p2 = rt.get_pt() + xui_vector<s32>(rt.get_w(),	ir*10);
			xui_canvas::get_ins()->draw_line(p1, p2, xui_colour::k_darkgray);
		}
		for (s32 ic = 0; ic < c+1; ++ic)
		{
			xui_vector<s32> p1 = rt.get_pt() + xui_vector<s32>(ic*10,	 		0);
			xui_vector<s32> p2 = rt.get_pt() + xui_vector<s32>(ic*10,  rt.get_h());
			xui_canvas::get_ins()->draw_line(p1, p2, xui_colour::k_darkgray);
		}

		rt.oft_x(c/2*10);
		rt.oft_y(r/2*10);
		rt.set_w(10);
		rt.set_h(10);
		xui_canvas::get_ins()->draw_rectangle(rt, xui_colour::k_white);
	}
}

void xui_colour_pickwnd::on_viewctrlrenderself( xui_component* sender, xui_method_args& args )
{
	std::vector<u32> vec;
	for (u32 i = 1; i < 3; ++i)
	{
		vec.push_back(((u32)(long long)m_compctrl->get_data()+i) % 3);
	}

	xui_colour  color = m_mainview->get_backcolor();
	xui_colour  colors[4];
	colors[0] = color;
	colors[1] = color;
	colors[2] = color;
	colors[3] = color;

	colors[1].value[ vec[0] ] = 0.0f;
	colors[1].value[ vec[1] ] = 0.0f;
	colors[0].value[ vec[0] ] = 1.0f;
	colors[0].value[ vec[1] ] = 0.0f;
	colors[2].value[ vec[0] ] = 0.0f;
	colors[2].value[ vec[1] ] = 1.0f;
	colors[3].value[ vec[0] ] = 1.0f;
	colors[3].value[ vec[1] ] = 1.0f;
	xui_rect2d<s32> rt = sender->get_renderrtabs();
	xui_canvas::get_ins()->fill_rectangle(rt, colors);

	s32 x = rt.ax + (s32)(rt.get_w() * color.value[ vec[1] ]);
	s32 y = rt.by - (s32)(rt.get_h() * color.value[ vec[0] ]);
	xui_canvas::get_ins()->draw_circle(xui_vector<s32>(x, y), 3, xui_colour::k_white, 0, 360);
}

void xui_colour_pickwnd::on_viewctrldowndrag( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		std::vector<u32> vec;
		for (u32 i = 1; i < 3; ++i)
		{
			vec.push_back(((u32)(long long)m_compctrl->get_data()+i) % 3);
		}

		xui_vector<s32> pt = sender->get_renderpt(args.point);
		f32 xvalue =		(f32)pt.x / (f32)sender->get_renderw();
		f32 yvalue = 1.0f - (f32)pt.y / (f32)sender->get_renderh();
		xvalue = xui_min(xvalue, 1.0f);
		xvalue = xui_max(xvalue, 0.0f);
		yvalue = xui_min(yvalue, 1.0f);
		yvalue = xui_max(yvalue, 0.0f);

		xui_colour color = m_mainview->get_backcolor();
		color.value[ vec[0] ] = yvalue;
		color.value[ vec[1] ] = xvalue;
		m_mainview->set_backcolor(color);
		set_colordata();
		set_colortext();
	}
}

void xui_colour_pickwnd::on_mainrollrenderself( xui_component* sender, xui_method_args&  args )
{
	u32 index = (u32)(long long)m_compctrl->get_data();
	xui_colour mincolor = m_mainview->get_backcolor();
	xui_colour maxcolor = m_mainview->get_backcolor();
	mincolor.value[index] = 0.0f;
	maxcolor.value[index] = 1.0f;

	xui_colour colors[4];
	colors[0] = maxcolor;
	colors[1] = mincolor;
	colors[2] = mincolor;
	colors[3] = maxcolor;
	xui_canvas::get_ins()->fill_rectangle(sender->get_renderrtabs(), colors);
}

void xui_colour_pickwnd::on_mainrolldowndrag( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		xui_vector<s32> pt = sender->get_renderpt(args.point);
		f32 value = 1.0f - (f32)pt.y / (f32)sender->get_renderh();
		value = xui_min(value, 1.0f);
		value = xui_max(value, 0.0f);

		u32 index = (u32)(long long)m_compctrl->get_data();
		xui_colour color = m_mainview->get_backcolor();
		color.value[index] = value;
		m_mainview->set_backcolor(color);
		set_colortext();
		set_colordata();
	}
}

void xui_colour_pickwnd::on_comppanerenderself( xui_component* sender, xui_method_args& args )
{
	xui_colour color = m_mainview->get_backcolor();
	for (u32 i = 0; i < 4; ++i)
	{
		xui_rect2d<s32> rt = m_comproll[i]->get_renderrtabs();
		s32 x = rt.ax+(s32)(rt.get_w()*color.value[i]);

		xui_vector<s32> poly[3];
		poly[0] = xui_vector<s32>(x,   rt.ay-1);
		poly[1] = xui_vector<s32>(x-2, rt.ay-3);
		poly[2] = xui_vector<s32>(x+2, rt.ay-3);
		xui_canvas::get_ins()->draw_path(poly, 3, xui_colour::k_white);
		poly[0] = xui_vector<s32>(x,   rt.by  );
		poly[1] = xui_vector<s32>(x-2, rt.by+2);
		poly[2] = xui_vector<s32>(x+2, rt.by+2);
		xui_canvas::get_ins()->draw_path(poly, 3, xui_colour::k_white);
	}
}

void xui_colour_pickwnd::on_comprollrenderself( xui_component* sender, xui_method_args& args )
{
	xui_colour mincolor = m_mainview->get_backcolor();
	xui_colour maxcolor = m_mainview->get_backcolor();

	for (u32 i = 0; i < 4; ++i)
	{
		if (m_comproll[i] == sender)
		{
			mincolor.value[i] = 0.0f;
			maxcolor.value[i] = 1.0f;
			break;
		}
	}

	xui_colour colors[4];
	colors[0] = mincolor;
	colors[1] = mincolor;
	colors[2] = maxcolor;
	colors[3] = maxcolor;
	xui_canvas::get_ins()->fill_rectangle(sender->get_renderrtabs(), colors);
}

void xui_colour_pickwnd::on_comprolldowndrag( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		xui_vector<s32> pt = sender->get_renderpt(args.point);
		f32 value = (f32)pt.x / (f32)sender->get_renderw();
		value = xui_min(value, 1.0f);
		value = xui_max(value, 0.0f);

		for (u32 i = 0; i < 4; ++i)
		{
			if (m_comproll[i] == sender)
			{
				xui_colour color = m_mainview->get_backcolor();
				color.value[i] = value;
				m_mainview->set_backcolor(color);
				set_colortext();
				set_colordata();
				break;
			}
		}
	}
}

void xui_colour_pickwnd::on_compedittextchanged( xui_component* sender, xui_method_args& args )
{
	xui_numbbox* numbbox = xui_dynamic_cast(xui_numbbox, sender);
	f32 number = 0;  
	std::wstringstream temp(numbbox->get_text().c_str());
	temp >> number;

	for (u32 i = 0; i < 4; ++i)
	{
		if (m_compedit[i] == numbbox)
		{
			xui_colour color = m_mainview->get_backcolor();
			color.value[i] = number/255.0f;
			m_mainview->set_backcolor(color);
			set_colordata();
			break;
		}
	}
}

void xui_colour_pickwnd::set_colordata( void )
{
	xui_colour value = m_mainview->get_backcolor();
	xui_propdata_vec vec = m_propctrl->get_propdata();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_propdata_colour* data = dynamic_cast<xui_propdata_colour*>(vec[i]);
		data->set_value(value);
	}

	m_propctrl->on_linkpropdata();
}

void xui_colour_pickwnd::set_colortext( void )
{
	xui_colour color = m_mainview->get_backcolor();
	for (u32 i = 0; i < 4; ++i)
	{
		std::wstringstream temp;
		temp << (s32)(color.value[i]*255.0f);
		m_compedit[i]->ini_drawer(temp.str());
	}
}
