#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "onity_proppath.h"
#include "onity_pathctrl.h"

xui_implement_rtti(onity_pathctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_pathctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	xui_menu* menu1 = xui_menu::create(80);
	m_auto		= menu1->add_item(NULL, L"Auto");
	m_never		= menu1->add_item(NULL, L"Never");
	m_immediate = menu1->add_item(NULL, L"Immediate");
	xui_method_ptrcall(m_auto,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_method_ptrcall(m_never,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_method_ptrcall(m_immediate,	xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_freetypeclick);
	xui_menu* menu2 = xui_menu::create(80);
	m_on		= menu2->add_item(NULL, L"On");
	m_off		= menu2->add_item(NULL, L"Off");
	xui_method_ptrcall(m_on,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_loadtypeclick);
	xui_method_ptrcall(m_off,		xm_click		) += new xui_method_member<xui_method_args, onity_pathctrl>(this, &onity_pathctrl::on_loadtypeclick);

	m_freetype	= new xui_toggle(xui_vector<s32>(128, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_freetype,	set_parent		)(this);
	xui_method_ptrcall(m_freetype,	set_corner		)(3);
	xui_method_ptrcall(m_freetype,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_freetype,	set_drawcolor	)(true);
	xui_method_ptrcall(m_freetype,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_freetype,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_freetype,	ini_drawer		)(L"Modify All FreeType");
	xui_method_ptrcall(m_freetype,	set_menu		)(menu1);

	m_loadtype	= new xui_toggle(xui_vector<s32>(128, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_loadtype,	set_parent		)(this);
	xui_method_ptrcall(m_loadtype,	set_corner		)(3);
	xui_method_ptrcall(m_loadtype,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_loadtype,	set_drawcolor	)(true);
	xui_method_ptrcall(m_loadtype,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_loadtype,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_loadtype,	ini_drawer		)(L"Modify All EntireLoad");
	xui_method_ptrcall(m_loadtype,	set_menu		)(menu2);

	m_widgetvec.push_back(m_freetype);
	m_widgetvec.push_back(m_loadtype);
}

/*
//static
*/
xui_method_explain(onity_pathctrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new onity_pathctrl(propkind);
}

/*
//callback
*/
xui_method_explain(onity_pathctrl, on_perform,			void			)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//freetype
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_freetype->get_renderh())/2 + height;
	m_freetype->on_perform_pt(pt);
	m_freetype->on_perform_w (rt.get_w()-indent);
	//loadtype
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_freetype->get_renderh())/2 + height*2;
	m_loadtype->on_perform_pt(pt);
	m_loadtype->on_perform_w (rt.get_w()-indent);
}

/*
//event
*/
xui_method_explain(onity_pathctrl, on_freetypeclick,	void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		onity_proppath* proppath = dynamic_cast<onity_proppath*>(m_propkindvec[i]);
	}
}
xui_method_explain(onity_pathctrl, on_loadtypeclick,	void			)( xui_component* sender, xui_method_args& args )
{

}

/*
//method
*/
xui_method_explain(onity_pathctrl, get_elsectrlsize,	s32				)( void )
{
	return 2*KIND_HEIGHT;
}
xui_method_explain(onity_pathctrl, get_prevctrlsize,	s32				)( void )
{
	return 2*KIND_HEIGHT;
}