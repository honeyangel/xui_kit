#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_button.h"
#include "xui_numbbox.h"
#include "xui_plusctrl.h"
#include "xui_propctrl.h"
#include "xui_propedit_colour.h"

/*
//constructor
*/
xui_create_explain(xui_propedit_colour)( xui_propctrl* propctrl )
: xui_propedit_base(propctrl)
{
	xui_drawer* editctrl = new xui_drawer(xui_vector<s32>(48, 18));
	xui_method_ptrcall(editctrl, set_sidestyle	)(SIDESTYLE_S);
	editctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlnonfocus);
	editctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlgetfocus);
	editctrl->xm_renderself	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlrenderself);
	editctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_colour>(this, &xui_propedit_colour::on_editctrlclick);

	xui_drawer* pickctrl = new xui_drawer(xui_vector<s32>(16, 16));
	pickctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlnonfocus);
	pickctrl->xm_renderself += new xui_method_member<xui_method_args,  xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlrenderself);
	pickctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_colour>(this, &xui_propedit_colour::on_pickctrlclick);

	m_editctrl = editctrl;
	m_pickctrl = pickctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_colour, get_pickctrl,			xui_drawer*			)( void ) const
{
	return m_pickctrl;
}
xui_method_explain(xui_propedit_colour, get_value,				const xui_colour&	)( void ) const
{
	return m_editctrl->get_backcolor();
}
xui_method_explain(xui_propedit_colour, set_value,				void				)( const xui_colour& value )
{
	m_editctrl->set_backcolor(value);
}
xui_method_explain(xui_propedit_colour, reset,					void				)( void )
{
	xui_propedit_base::reset();
	m_editctrl->set_backcolor(xui_colour::black);
}

/*
//event
*/
xui_method_explain(xui_propedit_colour, on_editctrlrenderself,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_colour      color = m_pickctrl->has_focus() ? xui_global::get_scolor() : m_editctrl->get_backcolor();
	xui_rect2d<s32> rt    = m_editctrl->get_renderrtabs();
	xui_convas::get_ins()->fill_rectangle(rt, xui_colour::black);
	rt.by -= 4;
	xui_convas::get_ins()->fill_rectangle(rt, xui_colour(1.0f, color.r, color.g, color.b));
	rt.ay  = rt.by;
	rt.by += 4;
	rt.set_w((s32)((f32)rt.get_w() * color.a));
	xui_convas::get_ins()->fill_rectangle(rt, xui_colour::white);
}
xui_method_explain(xui_propedit_colour, on_pickctrlrenderself,	void				)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt     = m_pickctrl->get_renderrtabs();
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->draw_circle(center, 5, m_pickctrl->get_sidecolor(), 0, 360);
	rt.ax = center.x-1;
	rt.ay = center.y-1;
	rt.bx = center.x+2;
	rt.by = center.y+2;
	xui_convas::get_ins()->fill_rectangle(rt, m_pickctrl->get_sidecolor());
}
xui_method_explain(xui_propedit_colour, on_pickctrlnonfocus,	void				)( xui_component* sender, xui_method_args&  args )
{
	m_editctrl->set_backcolor(xui_global::get_scolor());
	m_propctrl->on_editvalue(this);
	xui_global::set_scolorclose();
}
xui_method_explain(xui_propedit_colour, on_editctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{
	xui_colour_pickwnd* wnd = xui_colour_pickwnd::get_ptr();
	if (wnd)
	{
		xui_method_ptrcall(wnd, set_propctrl)(m_propctrl);
		xui_method_ptrcall(wnd, set_modal	)(true);
		xui_method_ptrcall(wnd, set_visible	)(true);
		xui_method_ptrcall(wnd, set_value	)(get_value());
	}
}
xui_method_explain(xui_propedit_colour, on_pickctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{
	xui_global::set_scolorstart();
}

//////////////////////////////////////////////////////////////////////////
//xui_colour_pickwnd
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_colour_pickwnd, xui_window);

/*
//constructor
*/
xui_create_explain(xui_colour_pickwnd)( void )
: xui_window(xui_vector<s32>(300))
, m_propctrl(NULL)
{
	m_pickctrl = new xui_drawer(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_pickctrl,	xm_nonfocus		) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_pickctrlnonfocus);
	xui_method_ptrcall(m_pickctrl,	xm_mouseclick	) += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_pickctrlclick);
	xui_method_ptrcall(m_pickctrl,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_pickctrl,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_pickctrl,	ini_drawer		)(NULL);
	m_mainview = new xui_drawer(xui_vector<s32>(80, 16));
	xui_method_ptrcall(m_mainview,	xm_renderself	) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainviewrenderself);
	xui_method_ptrcall(m_mainview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_mainview,	ini_component	)(0, 0, DOCKSTYLE_L);
	m_mainpane = new xui_panel(xui_vector<s32>(24));
	xui_method_ptrcall(m_mainpane,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_mainpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_mainpane,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_mainpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_mainpane,	add_child		)(m_pickctrl);
	xui_method_ptrcall(m_mainpane,	add_child		)(m_mainview);

	m_viewplus = new xui_plusctrl(PLUSRENDER_NORMAL, true);
	xui_method_ptrcall(m_viewplus,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_viewplus,	xm_expand		) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_plusctrlexpand);
	m_viewname = new xui_drawer(xui_vector<s32>(80, 16));
	xui_method_ptrcall(m_viewname,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_viewname,	ini_drawer		)(L"Colors");
	m_compctrl = new xui_drawer(xui_vector<s32>(16));
	xui_method_ptrcall(m_compctrl,	xm_renderself	) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compctrlrenderself);
	xui_method_ptrcall(m_compctrl,	xm_mouseclick	) += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compctrlclick);
	xui_method_ptrcall(m_compctrl,	ini_component	)(0, 0, DOCKSTYLE_R);
	m_viewhead = new xui_panel(xui_vector<s32>(16));
	xui_method_ptrcall(m_viewhead,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_viewhead,	set_drawcolor	)(false);
	xui_method_ptrcall(m_viewhead,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_viewhead,	add_child		)(m_viewplus);
	xui_method_ptrcall(m_viewhead,	add_child		)(m_viewname);
	xui_method_ptrcall(m_viewhead,	add_child		)(m_compctrl);

	m_viewctrl = new xui_drawer(xui_vector<s32>(200));
	xui_method_ptrcall(m_viewctrl,	xm_renderself	) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewctrlrenderself);
	xui_method_ptrcall(m_viewctrl,	xm_mouseclick	) += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_viewctrlclick);
	xui_method_ptrcall(m_viewctrl,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_viewctrl,	set_sidestyle	)(SIDESTYLE_S);
	m_mainroll = new xui_drawer(xui_vector<s32>(20, 200));
	xui_method_ptrcall(m_mainroll,	xm_renderself	) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrollrenderself);
	xui_method_ptrcall(m_mainroll,	xm_mousedown	) += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrolldowndrag);
	xui_method_ptrcall(m_mainroll,	xm_mousemove	) += new xui_method_member<xui_method_mouse,	xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_mainrolldowndrag);
	xui_method_ptrcall(m_mainroll,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_mainroll,	set_sidestyle	)(SIDESTYLE_S);
	m_viewpane = new xui_panel(xui_vector<s32>(200));
	xui_method_ptrcall(m_viewpane,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_viewpane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_viewpane,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_viewpane,	add_child		)(m_viewctrl);
	xui_method_ptrcall(m_viewpane,	add_child		)(m_mainroll);

	m_compplus = new xui_plusctrl(PLUSRENDER_NORMAL, true);
	xui_method_ptrcall(m_compplus,	xm_expand		) += new xui_method_member<xui_method_args,		xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_plusctrlexpand);
	xui_method_ptrcall(m_compplus,	ini_component	)(0, 0, DOCKSTYLE_L);
	m_compname = new xui_drawer(xui_vector<s32>(80, 16));
	xui_method_ptrcall(m_compname,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_compname,	ini_drawer		)(L"Slider");
	m_comphead = new xui_panel(xui_vector<s32>(16));
	xui_method_ptrcall(m_comphead,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_comphead,	set_drawcolor	)(false);
	xui_method_ptrcall(m_comphead,	set_hscrollauto	)(false);
	xui_method_ptrcall(m_comphead,	add_child		)(m_compplus);
	xui_method_ptrcall(m_comphead,	add_child		)(m_compname);

	m_comppane = new xui_panel(xui_vector<s32>(100));
	xui_method_ptrcall(m_comppane,	ini_component	)(0, 0, DOCKSTYLE_L);
	xui_method_ptrcall(m_comppane,	set_drawcolor	)(false);
	xui_method_ptrcall(m_comppane,	set_hscrollauto	)(false);
	for (u32 i = 0; i < 4; ++i)
	{
		xui_drawer*  text = new xui_drawer(xui_vector<s32>(32, 16));
		xui_method_ptrcall(text,	ini_component	)(0, 0, DOCKSTYLE_L);
		xui_drawer*  roll = new xui_drawer(xui_vector<s32>(80, 16));
		xui_method_ptrcall(roll,	xm_renderself	) += new xui_method_member<xui_method_args,  xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprollrenderself);
		xui_method_ptrcall(roll,	xm_mousedown	) += new xui_method_member<xui_method_mouse, xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprolldowndrag);
		xui_method_ptrcall(roll,	xm_mousemove	) += new xui_method_member<xui_method_mouse, xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_comprolldowndrag);
		xui_method_ptrcall(roll,	ini_component	)(0, 0, DOCKSTYLE_L);
		xui_method_ptrcall(roll,	set_sidestyle	)(SIDESTYLE_S);
		xui_numbbox* edit = new xui_numbbox(xui_vector<s32>(60, 16), NT_UNSIGNEDINT, 1);
		xui_method_ptrcall(edit,	xm_textchanged	) += new xui_method_member<xui_method_args,  xui_colour_pickwnd>(this, &xui_colour_pickwnd::on_compedittextchanged);
		xui_method_ptrcall(edit,	ini_component	)(0, 0, DOCKSTYLE_R);
		xui_method_ptrcall(edit,	set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(edit,	set_borderrt	)(xui_rect2d<s32>(2));
		xui_method_ptrcall(edit,	set_textalign	)(TEXTALIGN_RC);

		xui_panel*   pane = new xui_panel(xui_vector<s32>(20));
		xui_method_ptrcall(pane,	ini_component	)(0, 0, DOCKSTYLE_L);
		xui_method_ptrcall(pane,	set_drawcolor	)(false);
		xui_method_ptrcall(pane,	set_hscrollauto	)(false);
		xui_method_ptrcall(pane,	add_child		)(text);
		xui_method_ptrcall(pane,	add_child		)(roll);
		xui_method_ptrcall(pane,	add_child		)(edit);

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

/*
//static
*/
xui_method_explain(xui_colour_pickwnd,	get_ptr,				xui_colour_pickwnd*	)( void )
{
	static xui_colour_pickwnd* ptr = NULL;
	if (ptr == NULL)
	{
		ptr = new xui_colour_pickwnd;
		xui_method_ptrcall(ptr, set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(ptr, set_borderrt	)(xui_rect2d<s32>(8));
		xui_method_ptrcall(ptr, ini_component	)(ALIGNHORZ_C, ALIGNVERT_C, 0);
		xui_method_ptrcall(ptr, ini_component	)(true, false);
		xui_desktop::get_ins()->add_child(ptr);
	}

	return ptr;
}

/*
//method
*/
xui_method_explain(xui_colour_pickwnd,	get_value,				const xui_colour&	)( void ) const
{
	return m_mainview->get_backcolor();
}
xui_method_explain(xui_colour_pickwnd,	set_value,				void				)( const xui_colour& value )
{
	m_mainview->set_backcolor(value);
}
xui_method_explain(xui_colour_pickwnd,	get_propctrl,			xui_propctrl*		)( void )
{
	return m_propctrl;
}
xui_method_explain(xui_colour_pickwnd,	set_propctrl,			void				)( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}

/*
//callback
*/
xui_method_explain(xui_colour_pickwnd,	on_invalid,				void				)( xui_method_args& args )
{
	s32 h = m_border.ay + m_border.by;
	h += m_mainpane->get_renderh();
	h += m_viewhead->get_renderh();
	h += m_comphead->get_renderh();
	h += m_viewpane->was_visible() ? m_viewpane->get_renderh() : 0;
	h += m_comppane->was_visible() ? m_comppane->get_renderh() : 0;

	set_renderh(h);
}

/*
//event
*/
xui_method_explain(xui_colour_pickwnd,	on_plusctrlexpand,		void				)( xui_component* sender, xui_method_args& args )
{
	m_viewpane->set_visible(m_viewplus->was_expanded());
	m_comppane->set_visible(m_compplus->was_expanded());
	refresh();
}
xui_method_explain(xui_colour_pickwnd,	on_pickctrlnonfocus,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_pickctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_mainviewrenderself,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_compctrlrenderself,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_compctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_viewctrlrenderself,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_viewctrlclick,		void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_mainrollrenderself,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_mainrolldowndrag,	void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_comprollrenderself,	void				)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_comprolldowndrag,	void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(xui_colour_pickwnd,	on_compedittextchanged,	void				)( xui_component* sender, xui_method_args& args )
{

}