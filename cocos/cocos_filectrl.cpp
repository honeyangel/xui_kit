#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "cocos_filedata.h"
#include "cocos_propfile.h"
#include "cocos_filectrl.h"

xui_implement_rtti(cocos_filectrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(cocos_filectrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_showfind	= new xui_button(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_showfind,	xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_filectrl>(this, &cocos_filectrl::on_showfindclick);
	xui_method_ptrcall(m_showfind,	set_parent		)(this);
	xui_method_ptrcall(m_showfind,	set_corner		)(3);
	xui_method_ptrcall(m_showfind,	set_borderrt	)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_showfind,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_showfind,	set_drawcolor	)(true);
	xui_method_ptrcall(m_showfind,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_showfind,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_showfind,	ini_drawer		)(L"Show in Finder");
	m_widgetvec.push_back(m_showfind);
}

/*
//static
*/
xui_method_explain(cocos_filectrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new cocos_filectrl(propkind);
}

/*
//callback
*/
xui_method_explain(cocos_filectrl, on_perform,			void			)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//showfind
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_showfind->get_renderh())/2 + height;
	m_showfind->on_perform_pt(pt);
	m_showfind->on_perform_w (rt.get_w()-indent);
}

/*
//event
*/
xui_method_explain(cocos_filectrl, on_showfindclick,	void			)( xui_component* sender, xui_method_args& args )
{
	cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(m_propkind->get_root());
	xui_global::set_showfind(propfile->get_fullname());
}

/*
//override
*/
xui_method_explain(cocos_filectrl, get_elsectrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}
xui_method_explain(cocos_filectrl, get_prevctrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}