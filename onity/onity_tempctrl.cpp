#include "xui_button.h"
#include "xui_linebox.h"
#include "onity_propentitytemp.h"
#include "onity_tempctrl.h"

xui_implement_rtti(onity_tempctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_tempctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_select = new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_select,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickselect);
	xui_method_ptrcall(m_select,	set_drawcolor	)(true);
	xui_method_ptrcall(m_select,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_select,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_select,	ini_drawer		)(L"Select");

	m_apply	 = new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_apply,		xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickapply);
	xui_method_ptrcall(m_apply,		set_drawcolor	)(true);
	xui_method_ptrcall(m_apply,		set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_apply,		set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_apply,		ini_drawer		)(L"Apply");

	m_revert = new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_revert,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickrevert);
	xui_method_ptrcall(m_revert,	set_drawcolor	)(true);
	xui_method_ptrcall(m_revert,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_revert,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_revert,	ini_drawer		)(L"Revert");

	xui_linebox* linebox = new xui_linebox(xui_vector<s32>(0, 20), FLOWSTYLE_H);
	xui_method_ptrcall(linebox,		set_parent		)(this);
	xui_method_ptrcall(linebox,		set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(linebox,		set_corner		)(3);
	xui_method_ptrcall(linebox,		ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(linebox,		add_linectrl	)(m_select);
	xui_method_ptrcall(linebox,		add_linectrl	)(m_apply);
	xui_method_ptrcall(linebox,		add_linectrl	)(m_revert);
	m_widgetvec.push_back(linebox);
}

/*
//static
*/
xui_method_explain(onity_tempctrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new onity_tempctrl(propkind);
}

/*
//callback
*/
xui_method_explain(onity_tempctrl, on_updateself,		void			)( xui_method_update& args )
{
	xui_kindctrl::on_updateself(args);
	onity_propentitytemp*  proptemp = dynamic_cast<onity_propentitytemp*>(m_propkind->get_root());
	Omiga::EntityTemplate* temp     = proptemp->get_template();

	xui_method_ptrcall(m_select, set_enable)(temp != NULL);
	xui_method_ptrcall(m_apply,  set_enable)(temp != NULL && proptemp->was_modify());
	xui_method_ptrcall(m_revert, set_enable)(temp != NULL && proptemp->was_modify());
}
xui_method_explain(onity_tempctrl, on_perform,			void			)( xui_method_args&   args )
{
	xui_kindctrl::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	xui_method_ptrcall(m_select, set_renderw)(rt.get_w()/3);
	xui_method_ptrcall(m_apply,	 set_renderw)(rt.get_w()/3);
	xui_method_ptrcall(m_revert, set_renderw)(rt.get_w()-m_select->get_renderw()-m_apply->get_renderw());
}

/*
//event
*/
xui_method_explain(onity_tempctrl, on_clickselect,		void			)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_tempctrl, on_clickapply,		void			)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_tempctrl, on_clickrevert,		void			)( xui_component* sender, xui_method_args& args )
{

}

/*
//override
*/
xui_method_explain(onity_tempctrl, get_elsectrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}
xui_method_explain(onity_tempctrl, get_prevctrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}