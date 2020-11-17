#include "xui_canvas.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_object.h"

xui_propedit_object::xui_propedit_object( xui_propctrl* propctrl, xui_bitmap* icon )
: xui_propedit_base(propctrl)
{
	xui_drawer* textctrl = new xui_drawer(xui_vector<s32>(48, 18));
	textctrl->set_backcolor(xui_colour::k_darkgray);
	textctrl->set_drawcolor(true);
	textctrl->set_sidestyle(k_sidestyle_s);
	textctrl->set_borderrt(xui_rect2d<s32>(4, 2, 2, 2));
	textctrl->set_icon(icon);
	textctrl->set_iconsize(xui_vector<s32>(16));
	textctrl->set_iconalign(k_image_front_text);
	textctrl->set_textalign(k_textalign_lc);
	textctrl->set_iconoffset(xui_vector<s32>(2, 0));
	textctrl->set_textoffset(xui_vector<s32>(2, 0));
	textctrl->xm_nonfocus	+= new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_editctrlnonfocus);
	textctrl->xm_getfocus	+= new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_editctrlgetfocus);

	xui_drawer* pickctrl = new xui_drawer(xui_vector<s32>(16, 16));
	pickctrl->ini_drawer(xui_global::s_icon_pickselect);
	pickctrl->xm_mouseclick += new xui_method_member<xui_method_mouse, xui_propedit_object>(this, &xui_propedit_object::on_pickctrlclick);
	//pickctrl->xm_renderself += new xui_method_member<xui_method_args,  xui_propedit_object>(this, &xui_propedit_object::on_pickctrlrenderself);

	m_editctrl = textctrl;
	m_pickctrl = pickctrl;
}

xui_drawer* xui_propedit_object::get_pickctrl( void ) const
{
	return m_pickctrl;
}

void xui_propedit_object::reset( void )
{
	xui_propedit_base::reset();
	xui_drawer* editctrl = xui_dynamic_cast(xui_drawer, m_editctrl);
	editctrl->ini_drawer(L"");
}

void xui_propedit_object::on_pickctrlclick( xui_component* sender, xui_method_mouse& args )
{
	m_propctrl->on_readyundo();
	m_propctrl->on_editvalue(this);
}
//xui_method_explain(xui_propedit_object, on_pickctrlrenderself,	void		)( xui_component* sender, xui_method_args&  args )
//{
//	xui_rect2d<s32> rt     = m_pickctrl->get_renderrtabs();
//	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
//	xui_canvas::get_ins()->draw_circle(center, 5, m_pickctrl->get_sidecolor(), 0, 360);
//	rt.ax = center.x-1;
//	rt.ay = center.y-1;
//	rt.bx = center.x+2;
//	rt.by = center.y+2;
//	xui_canvas::get_ins()->fill_rectangle(rt, m_pickctrl->get_sidecolor());
//}