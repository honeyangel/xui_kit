#include "2d/CCNode.h"
#include "ui/UILayoutComponent.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_numbbox.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "cocos_resource.h"
#include "cocos_propnodebase.h"
#include "cocos_propctrl_location.h"

//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propdata_location)( xui_propkind* kind, cocos_propnodebase* propnode, bool editsize )
: xui_propdata(kind, L"", cocos_propctrl_location::create)
, m_propnode(propnode)
, m_editsize(editsize)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_location, get_propnode,			cocos_propnodebase*	)( void )
{
	return m_propnode;
}
xui_method_explain(cocos_propdata_location, can_editsize,			bool				)( void )
{
	return m_editsize;
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_location, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_location)( xui_propdata* propdata )
: xui_propctrl()
{
	m_lnumctrl	= new xui_numbbox(xui_vector<s32>(48, 18), NT_FLOAT, 1.0, false);
	xui_method_ptrcall(m_lnumctrl, ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_lnumctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_lnumctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_lnumctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_lnumctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_lnumctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_lnumctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlnonfocus);
	xui_method_ptrcall(m_lnumctrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlgetfocus);
	xui_method_ptrcall(m_lnumctrl, xm_textchanged	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrltextchanged);

	m_rnumctrl	= new xui_numbbox(xui_vector<s32>(48, 18), NT_FLOAT, 1.0, false);
	xui_method_ptrcall(m_rnumctrl, ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_rnumctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_rnumctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_rnumctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_rnumctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_rnumctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_rnumctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlnonfocus);
	xui_method_ptrcall(m_rnumctrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlgetfocus);
	xui_method_ptrcall(m_rnumctrl, xm_textchanged	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrltextchanged);

	m_tnumctrl	= new xui_numbbox(xui_vector<s32>(48, 18), NT_FLOAT, 1.0, false);
	xui_method_ptrcall(m_tnumctrl, ini_component	)(ALIGNHORZ_C, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_tnumctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_tnumctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_tnumctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_tnumctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_tnumctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_tnumctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlnonfocus);
	xui_method_ptrcall(m_tnumctrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlgetfocus);
	xui_method_ptrcall(m_tnumctrl, xm_textchanged	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrltextchanged);

	m_bnumctrl	= new xui_numbbox(xui_vector<s32>(48, 18), NT_FLOAT, 1.0, false);
	xui_method_ptrcall(m_bnumctrl, ini_component	)(ALIGNHORZ_C, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_bnumctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_bnumctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_bnumctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 4, 2));
	xui_method_ptrcall(m_bnumctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_bnumctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_bnumctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlnonfocus);
	xui_method_ptrcall(m_bnumctrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrlgetfocus);
	xui_method_ptrcall(m_bnumctrl, xm_textchanged	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_numbctrltextchanged);

	m_ltogctrl	= new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	m_rtogctrl	= new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	m_ttogctrl	= new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	m_btogctrl	= new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_ltogctrl, ini_drawer		)(cocos_resource::icon_anchorl);
	xui_method_ptrcall(m_rtogctrl, ini_drawer		)(cocos_resource::icon_anchorr);
	xui_method_ptrcall(m_ttogctrl, ini_drawer		)(cocos_resource::icon_anchort);
	xui_method_ptrcall(m_btogctrl, ini_drawer		)(cocos_resource::icon_anchorb);
	xui_method_ptrcall(m_ltogctrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_rtogctrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_ttogctrl, ini_component	)(ALIGNHORZ_C, 0, 0);
	xui_method_ptrcall(m_btogctrl, ini_component	)(ALIGNHORZ_C, 0, 0);
	xui_method_ptrcall(m_ltogctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_rtogctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_ttogctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_btogctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_ltogctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);
	xui_method_ptrcall(m_rtogctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);
	xui_method_ptrcall(m_ttogctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);
	xui_method_ptrcall(m_btogctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);

	m_horzctrl	= new xui_toggle(xui_vector<s32>(48, 18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_horzctrl, ini_component	)(ALIGNHORZ_C, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_horzctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);
	xui_method_ptrcall(m_horzctrl, xm_renderself	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_horzctrlrenderself);
	m_vertctrl	= new xui_toggle(xui_vector<s32>(18, 48), TOGGLE_BUTTON);
	xui_method_ptrcall(m_vertctrl, ini_component	)(ALIGNHORZ_C, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_vertctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_toggctrlclick);
	xui_method_ptrcall(m_vertctrl, xm_renderself	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_vertctrlrenderself);

	m_editpane	= new xui_panel(xui_vector<s32>(190, 130));
	xui_method_ptrcall(m_editpane, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_editpane, set_parent		)(this);
	xui_method_ptrcall(m_editpane, set_drawcolor	)(false);
	xui_method_ptrcall(m_editpane, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_editpane, add_child		)(m_lnumctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_rnumctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_tnumctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_bnumctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_ltogctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_rtogctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_ttogctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_btogctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_horzctrl);
	xui_method_ptrcall(m_editpane, add_child		)(m_vertctrl);
	xui_method_ptrcall(m_editpane, xm_perform		) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_editpaneperform);
	xui_method_ptrcall(m_editpane, xm_renderself	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_editpanerenderself);

	m_viewpane	= new xui_panel(xui_vector<s32>(160, 100));
	xui_method_ptrcall(m_viewpane, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_viewpane, set_parent		)(this);
	xui_method_ptrcall(m_viewpane, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_viewpane, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_viewpane, xm_renderself	) += new xui_method_member<xui_method_args, cocos_propctrl_location>(this, &cocos_propctrl_location::on_viewpanerenderself);

	m_widgetvec.push_back(m_editpane);
	m_widgetvec.push_back(m_viewpane);

	xui_action_ctrl_impl<f32>* animctrl = new xui_action_ctrl_impl<f32>(this);
	animctrl->add_time(0);
	animctrl->add_time(2);
	animctrl->add_time(4);
	animctrl->add_data(0.5f);
	animctrl->add_data(1.0f);
	animctrl->add_data(0.5f);
	animctrl->set_loop(true);
	animctrl->set_play(true);
	m_animctrl = animctrl;
}

/*
//destructor
*/
xui_delete_explain(cocos_propctrl_location)( void )
{
	delete m_animctrl;
}

/*
//create
*/
xui_method_explain(cocos_propctrl_location, create,					xui_propctrl*		)( xui_propdata* propdata )
{
	return new cocos_propctrl_location(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_location, on_linkpropdata,		void				)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		xui_method_ptrcall(m_ltogctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_rtogctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_ttogctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_btogctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_horzctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_vertctrl, ini_toggle	)(false);
		xui_method_ptrcall(m_lnumctrl, set_enable	)(false);
		xui_method_ptrcall(m_rnumctrl, set_enable	)(false);
		xui_method_ptrcall(m_tnumctrl, set_enable	)(false);
		xui_method_ptrcall(m_bnumctrl, set_enable	)(false);
		xui_method_ptrcall(m_lnumctrl, ini_textbox	)(L"");
		xui_method_ptrcall(m_rnumctrl, ini_textbox	)(L"");
		xui_method_ptrcall(m_tnumctrl, ini_textbox	)(L"");
		xui_method_ptrcall(m_bnumctrl, ini_textbox	)(L"");
		xui_method_ptrcall(m_animctrl, set_time		)(0.0f);
	}

	bool editsize = true;
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		if (data->can_editsize() != editsize)
		{
			editsize = false;
			break;
		}
	}
	m_horzctrl->set_visible(editsize);
	m_vertctrl->set_visible(editsize);

	if (m_propdatavec.size() == 1)
	{
		cocos_propdata_location* datalocation = dynamic_cast<cocos_propdata_location*>(m_propdata);
		cocos_propnodebase* prop = datalocation->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
		cocos2d::ui::LayoutComponent::HorizontalEdge horzedge = component->getHorizontalEdge();
		cocos2d::ui::LayoutComponent::VerticalEdge   vertedge = component->getVerticalEdge();
		xui_method_ptrcall(m_ltogctrl, ini_toggle)(horzedge == cocos2d::ui::LayoutComponent::HorizontalEdge::Left  || horzedge == cocos2d::ui::LayoutComponent::HorizontalEdge::Center);
		xui_method_ptrcall(m_rtogctrl, ini_toggle)(horzedge == cocos2d::ui::LayoutComponent::HorizontalEdge::Right || horzedge == cocos2d::ui::LayoutComponent::HorizontalEdge::Center);
		xui_method_ptrcall(m_ttogctrl, ini_toggle)(vertedge == cocos2d::ui::LayoutComponent::VerticalEdge::Top     || vertedge == cocos2d::ui::LayoutComponent::VerticalEdge::Center);
		xui_method_ptrcall(m_btogctrl, ini_toggle)(vertedge == cocos2d::ui::LayoutComponent::VerticalEdge::Bottom  || vertedge == cocos2d::ui::LayoutComponent::VerticalEdge::Center);
		xui_method_ptrcall(m_horzctrl, ini_toggle)(component->isStretchWidthEnabled());
		xui_method_ptrcall(m_vertctrl, ini_toggle)(component->isStretchHeightEnabled());
		xui_method_ptrcall(m_lnumctrl, set_enable)(m_ltogctrl->was_push());
		xui_method_ptrcall(m_rnumctrl, set_enable)(m_rtogctrl->was_push());
		xui_method_ptrcall(m_tnumctrl, set_enable)(m_ttogctrl->was_push());
		xui_method_ptrcall(m_bnumctrl, set_enable)(m_btogctrl->was_push());

		char temp[32];
		sprintf(temp, "%.2f", component->getLeftMargin());
		m_lnumctrl->ini_textbox(xui_global::ascii_to_unicode(temp));
		sprintf(temp, "%.2f", component->getRightMargin());
		m_rnumctrl->ini_textbox(xui_global::ascii_to_unicode(temp));
		sprintf(temp, "%.2f", component->getTopMargin());
		m_tnumctrl->ini_textbox(xui_global::ascii_to_unicode(temp));
		sprintf(temp, "%.2f", component->getBottomMargin());
		m_bnumctrl->ini_textbox(xui_global::ascii_to_unicode(temp));
	}
}
xui_method_explain(cocos_propctrl_location, on_editvalue,			void				)( xui_propedit* sender )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propkind* kind = m_propdatavec[i]->get_kind();
		if (kind)
		{
			xui_method_propdata args;
			args.propdata = m_propdatavec[i];
			kind->xm_propchanged(this, args);
		}
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_location, on_invalid,				void				)( xui_method_args&		args )
{
	xui_vector<s32> sz;
	sz.w = m_border.ax + m_border.bx + m_editpane->get_renderw() + m_viewpane->get_renderw();
	sz.h = m_border.ay + m_border.by + m_editpane->get_renderh();
	sz.w = xui_max(sz.w, get_renderw());

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(cocos_propctrl_location, on_perform,				void				)( xui_method_args&		args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrtins();
	s32 x = m_border.ax;
	x += (rt.get_w()-m_editpane->get_renderw()-m_viewpane->get_renderw()) / 2;
	m_editpane->on_perform_x(x);
	x += m_editpane->get_renderw();
	m_viewpane->on_perform_x(x);

}
xui_method_explain(cocos_propctrl_location, on_updateself,			void				)( xui_method_update&	args )
{
	xui_propctrl::on_updateself(args);
	m_animctrl->update(args.delta);
}

/*
//event
*/
xui_method_explain(cocos_propctrl_location, on_numbctrlnonfocus,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_numbbox* numbbox = xui_dynamic_cast(xui_numbbox, sender);
	numbbox->set_sidecolor(xui_control::default_sidecolor);
}
xui_method_explain(cocos_propctrl_location, on_numbctrlgetfocus,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_numbbox* numbbox = xui_dynamic_cast(xui_numbbox, sender);
	numbbox->set_sidecolor(xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}
xui_method_explain(cocos_propctrl_location, on_numbctrltextchanged, void				)( xui_component* sender, xui_method_args& args )
{
	f64 value = 0.0;
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textctrl->get_text();
	std::wstringstream tmp(text.c_str());
	tmp >> value;

	set_margin(sender, value);
	on_editvalue(NULL);
}
xui_method_explain(cocos_propctrl_location, on_toggctrlclick,		void				)( xui_component* sender, xui_method_args& args )
{
	m_lnumctrl->set_enable(m_ltogctrl->was_push());
	m_rnumctrl->set_enable(m_rtogctrl->was_push());
	m_tnumctrl->set_enable(m_ttogctrl->was_push());
	m_bnumctrl->set_enable(m_btogctrl->was_push());
	if (sender == m_ltogctrl || sender == m_rtogctrl)
	{
		set_horzalign();
	}
	else
	if (sender == m_ttogctrl || sender == m_btogctrl)
	{
		set_vertalign();
	}
	else
	if (sender == m_horzctrl)
	{
		set_horzstretch();
	}
	else
	if (sender == m_vertctrl)
	{
		set_vertstretch();
	}

	on_editvalue(NULL);
}
xui_method_explain(cocos_propctrl_location, on_editpaneperform,		void				)( xui_component* sender, xui_method_args& args )
{
	m_ltogctrl->on_perform_x(m_lnumctrl->get_renderx()+m_lnumctrl->get_renderw());
	m_rtogctrl->on_perform_x(m_rnumctrl->get_renderx()-m_rtogctrl->get_renderw());
	m_ttogctrl->on_perform_y(m_tnumctrl->get_rendery()+m_tnumctrl->get_renderh());
	m_btogctrl->on_perform_y(m_bnumctrl->get_rendery()-m_btogctrl->get_renderh());
}
xui_method_explain(cocos_propctrl_location, on_editpanerenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_vector<s32> pt = m_editpane->get_screenpt();
	xui_rect2d<s32> rt;
	rt.ax  = m_horzctrl->get_renderx();
	rt.ay  = m_vertctrl->get_rendery();
	rt.set_w(m_horzctrl->get_renderw());
	rt.set_h(m_vertctrl->get_renderh());
	xui_convas::get_ins()->draw_rectangle(rt+pt, xui_control::default_sidecolor);
}
xui_method_explain(cocos_propctrl_location, on_viewpanerenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<f32>* animctrl = (xui_action_ctrl_impl<f32>*)m_animctrl;
	f32 s = animctrl->sample();

	xui_vector<s32> pt = m_viewpane->get_screenpt();
	xui_rect2d<s32> rt = m_viewpane->get_renderrtins();
	rt.set_w(rt.get_w()*s);
	rt.set_h(rt.get_h()*s);
	xui_convas::get_ins()->fill_rectangle(rt+pt, xui_button::default_movecolor);

	rt.ax += 4;
	rt.bx -= 4;
	rt.ay += 4;
	rt.by -= 4;
	s32 defsize = 12;
	if (m_horzctrl->was_push())
	{
		if (m_ltogctrl->was_push() == false ||
			m_rtogctrl->was_push() == false)
		{
			if (m_rtogctrl->was_push()) rt.ax = rt.bx - rt.get_w()/2;
			else						rt.bx = rt.ax + rt.get_w()/2;
		}
	}
	else
	{
		if (m_ltogctrl->was_push() &&
			m_rtogctrl->was_push())
		{
			rt.ax = rt.ax + (rt.get_w()-defsize) / 2;
			rt.bx = rt.ax + defsize;
		}
		else
		if (m_rtogctrl->was_push())
		{
			rt.ax = rt.bx - defsize;
		}
		else
		{
			rt.bx = rt.ax + defsize;
		}
	}

	if (m_vertctrl->was_push())
	{
		if (m_ttogctrl->was_push() == false ||
			m_btogctrl->was_push() == false)
		{
			if (m_ttogctrl->was_push()) rt.by = rt.ay + rt.get_h() / 2;
			else						rt.ay = rt.by - rt.get_h() / 2;
		}
	}
	else
	{
		if (m_ttogctrl->was_push() &&
			m_btogctrl->was_push())
		{
			rt.ay = rt.ay + (rt.get_h()-defsize)/2;
			rt.by = rt.ay + defsize;
		}
		else
		if (m_ttogctrl->was_push())
		{
			rt.by = rt.ay + defsize;
		}
		else
		{
			rt.ay = rt.by - defsize;
		}
	}

	xui_convas::get_ins()->fill_rectangle(rt+pt, xui_colour::white);
}
xui_method_explain(cocos_propctrl_location, on_horzctrlrenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_colour color = m_horzctrl->get_rendercolor();
	xui_rect2d<s32> rt = m_horzctrl->get_renderrtabs();
	xui_vector<s32> p1(rt.ax, rt.ay+rt.get_h()/2);
	xui_vector<s32> p2(rt.bx, rt.ay+rt.get_h()/2);
	xui_convas::get_ins()->draw_line(p1, p2, color);
	xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+6, p1.y-6), color);
	xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+6, p1.y+6), color);
	xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-6, p2.y-6), color);
	xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-6, p2.y+6), color);
}
xui_method_explain(cocos_propctrl_location, on_vertctrlrenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_colour color = m_vertctrl->get_rendercolor();
	xui_rect2d<s32> rt = m_vertctrl->get_renderrtabs();
	xui_vector<s32> p1(rt.ax+rt.get_w()/2, rt.ay);
	xui_vector<s32> p2(rt.ax+rt.get_w()/2, rt.by);
	xui_convas::get_ins()->draw_line(p1, p2, color);
	xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x-6, p1.y+6), color);
	xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+6, p1.y+6), color);
	xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-6, p2.y-6), color);
	xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x+6, p2.y-6), color);
}

/*
//method
*/
xui_method_explain(cocos_propctrl_location, set_margin,				void				)( xui_component* sender, f64 value )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		cocos_propnodebase* prop = data->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());

		if		(sender == m_lnumctrl)component->setLeftMargin	(value);
		else if (sender == m_rnumctrl)component->setRightMargin	(value);
		else if (sender == m_tnumctrl)component->setTopMargin	(value);
		else if (sender == m_bnumctrl)component->setBottomMargin(value);

		component->refreshLayout();
	}
}
xui_method_explain(cocos_propctrl_location, set_horzalign,			void				)( void )
{
	cocos2d::ui::LayoutComponent::HorizontalEdge edge;
	if		(m_ltogctrl->was_push() && m_rtogctrl->was_push())	edge = cocos2d::ui::LayoutComponent::HorizontalEdge::Center;
	else if (m_ltogctrl->was_push())							edge = cocos2d::ui::LayoutComponent::HorizontalEdge::Left;
	else if (m_rtogctrl->was_push())							edge = cocos2d::ui::LayoutComponent::HorizontalEdge::Right;
	else														edge = cocos2d::ui::LayoutComponent::HorizontalEdge::None;

	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		cocos_propnodebase* prop = data->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
		component->setHorizontalEdge(edge);
		component->refreshLayout();
	}
}
xui_method_explain(cocos_propctrl_location, set_vertalign,			void				)( void )
{
	cocos2d::ui::LayoutComponent::VerticalEdge edge;
	if		(m_ttogctrl->was_push() && m_btogctrl->was_push())	edge = cocos2d::ui::LayoutComponent::VerticalEdge::Center;
	else if (m_ttogctrl->was_push())							edge = cocos2d::ui::LayoutComponent::VerticalEdge::Top;
	else if (m_btogctrl->was_push())							edge = cocos2d::ui::LayoutComponent::VerticalEdge::Bottom;
	else														edge = cocos2d::ui::LayoutComponent::VerticalEdge::None;

	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		cocos_propnodebase* prop = data->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
		component->setVerticalEdge(edge);
		component->refreshLayout();
	}
}
xui_method_explain(cocos_propctrl_location, set_horzstretch,		void				)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		cocos_propnodebase* prop = data->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
		component->setStretchWidthEnabled(m_horzctrl->was_push());
		component->refreshLayout();
	}
}
xui_method_explain(cocos_propctrl_location, set_vertstretch,		void				)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_location* data = dynamic_cast<cocos_propdata_location*>(m_propdatavec[i]);
		cocos_propnodebase* prop = data->get_propnode();
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
		component->setStretchHeightEnabled(m_vertctrl->was_push());
		component->refreshLayout();
	}
}