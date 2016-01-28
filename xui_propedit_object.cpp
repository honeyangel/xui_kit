#include "xui_convas.h"
#include "xui_button.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_object.h"

/*
//constructor
*/
xui_create_explain(xui_propedit_object)( xui_propctrl* propctrl, xui_bitmap* icon )
: xui_propedit_base(propctrl)
{
	xui_drawer* textctrl = new xui_drawer(xui_vector<s32>(48, 18));
	xui_method_ptrcall(textctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(textctrl, set_drawcolor	)(true);
	xui_method_ptrcall(textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textctrl, set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(textctrl, set_icon		)(icon);
	xui_method_ptrcall(textctrl, set_iconsize	)(xui_vector<s32>(12));
	xui_method_ptrcall(textctrl, set_iconalign	)(IMAGE_FRONT_TEXT);
	xui_method_ptrcall(textctrl, set_textalign	)(TA_LC);
	xui_method_ptrcall(textctrl, set_iconoffset )(xui_vector<s32>(2, 0));
	xui_method_ptrcall(textctrl, set_textoffset )(xui_vector<s32>(2, 0));
	textctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_editctrlnonfocus);
	textctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_editctrlgetfocus);

	xui_drawer* pickctrl = new xui_drawer(xui_vector<s32>(16, 16));
	pickctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_object>(this, &xui_propedit_object::on_pickctrlclick);
	pickctrl->xm_renderself += new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_pickctrlrenderself);

	m_editctrl = textctrl;
	m_pickctrl = pickctrl;
}

/*
//method
*/
xui_method_explain(xui_propedit_object, get_pickctrl,			xui_drawer*	)( void ) const
{
	return m_pickctrl;
}

/*
//virtual
*/
xui_method_explain(xui_propedit_object, reset,					void		)( void )
{
	xui_propedit_base::reset();
	xui_drawer* editctrl = xui_dynamic_cast(xui_drawer, m_editctrl);
	editctrl->ini_drawer(L"");
}

/*
//event
*/
xui_method_explain(xui_propedit_object, on_pickctrlclick,		void		)( xui_component* sender, xui_method_mouse& args )
{
	m_propctrl->on_editvalue(this);
}
xui_method_explain(xui_propedit_object, on_pickctrlrenderself,	void		)( xui_component* sender, xui_method_args&  args )
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