#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_numbbox.h"
#include "xui_propctrl.h"
#include "xui_propview.h"
#include "xui_textbox.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_linebox.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_propjsones.h"
#include "onity_proptempold.h"
#include "onity_propctrl_compold.h"
#include "onity_propkind_compold.h"

//////////////////////////////////////////////////////////////////////////
//propkind
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propkind_compold)( xui_proproot* root, const std::wstring& name, const std::string& type, xui_bitmap* icon, BreezeGame::Json::Value* node )
: xui_propkind(root, name, type, onity_kindctrl_compold::create, icon, true)
, m_node(node)
{
	xm_namechanged += new xui_method_member<xui_method_args, onity_propkind_compold>(this, &onity_propkind_compold::on_namechanged);
	add_propdata(new xui_propdata(this, L"", onity_propctrl_compattr::create));
}

/*
//method
*/
xui_method_explain(onity_propkind_compold, get_node,			BreezeGame::Json::Value*)( void )
{
	return m_node;
}

/*
//event
*/
xui_method_explain(onity_propkind_compold, on_namechanged,		void					)( xui_component* sender, xui_method_args& args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textbox->get_text();
	if (text.length() > 0)
	{
		(*m_node)["ClassName"] = xui_global::unicode_to_ascii(text);

		xui_method_propdata  other_args;
		xm_propchanged(NULL, other_args);
	}
}

//////////////////////////////////////////////////////////////////////////
//kindctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_kindctrl_compold, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_kindctrl_compold)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_killctrl = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_killctrl, xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_kindctrl_compold>(this, &onity_kindctrl_compold::on_killctrlclick);
	xui_method_ptrcall(m_killctrl, set_parent		)(this);
	xui_method_ptrcall(m_killctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_killctrl, ini_drawer		)(onity_resource::icon_delete);
	m_widgetvec.push_back(m_killctrl);
}

/*
//static
*/
xui_method_explain(onity_kindctrl_compold, create,				xui_kindctrl*			)( xui_propkind* propkind )
{
	return new onity_kindctrl_compold(propkind);
}

/*
//override
*/
xui_method_explain(onity_kindctrl_compold, on_perform,			void					)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	m_killctrl->on_perform_x(m_flagctrl->get_renderx()-4-m_killctrl->get_renderw());
	m_killctrl->on_perform_y(m_flagctrl->get_rendery());
	m_namectrl->on_perform_w(m_namectrl->get_renderw()-8-m_killctrl->get_renderw());
}

/*
//event
*/
xui_method_explain(onity_kindctrl_compold, on_killctrlclick,	void					)( xui_component* sender, xui_method_args& args )
{
	onity_proptempold* proptemp = dynamic_cast<onity_proptempold*>(m_propkind->get_root());
	proptemp->del_component(m_propkind);
}

xui_implement_rtti(onity_kindctrl_compadd, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_kindctrl_compadd)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	xui_menu*     menu		= xui_menu::create(80);
	xui_menuitem* visual	= menu->add_item(NULL, L"Visual");
	xui_menuitem* transform = menu->add_item(NULL, L"Transform");
	xui_menuitem* ai		= menu->add_item(NULL, L"AI");
	xui_menuitem* physics	= menu->add_item(NULL, L"Physics");
	xui_menuitem* action	= menu->add_item(NULL, L"InterAction");
	xui_menuitem* gui		= menu->add_item(NULL, L"GUI");
	xui_method_ptrcall(visual,		xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);
	xui_method_ptrcall(transform,	xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);
	xui_method_ptrcall(ai,			xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);
	xui_method_ptrcall(physics,		xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);
	xui_method_ptrcall(action,		xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);
	xui_method_ptrcall(gui,			xm_click		) += new xui_method_member<xui_method_args, onity_kindctrl_compadd>(this, &onity_kindctrl_compadd::on_menuitemclick);

	m_menuctrl	= new xui_toggle(xui_vector<s32>(128, 18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_menuctrl,	set_parent		)(this);
	xui_method_ptrcall(m_menuctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_menuctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_menuctrl,	set_corner		)(3);
	xui_method_ptrcall(m_menuctrl,	set_borderrt	)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_menuctrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_menuctrl,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_menuctrl,	ini_drawer		)(L"Add Component");
	xui_method_ptrcall(m_menuctrl,	set_menu		)(menu);

	m_namectrl	= new xui_textbox(xui_vector<s32>(128, 18));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_namectrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_namectrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_namectrl,	set_borderrt	)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);

	m_widgetvec.push_back(m_menuctrl);
	m_widgetvec.push_back(m_namectrl);
}

/*
//static
*/
xui_method_explain(onity_kindctrl_compadd, create,				xui_kindctrl*			)( xui_propkind* propkind )
{
	return new onity_kindctrl_compadd(propkind);
}

/*
//callback
*/
xui_method_explain(onity_kindctrl_compadd, on_perform,			void					)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//linectrl
	pt.x = rt.ax +  indent;
	pt.y = rt.ay + (height-m_menuctrl->get_renderh())/2;
	m_menuctrl->on_perform_pt(pt);
	m_menuctrl->on_perform_w (rt.get_w()/2 - indent - 8);
	pt.x = rt.get_w()/2;
	m_namectrl->on_perform_pt(pt);
	m_namectrl->on_perform_w (rt.get_w()/2);
}

/*
//override
*/
xui_method_explain(onity_kindctrl_compadd, get_elsectrlsize,	s32						)( void )
{
	return KIND_HEIGHT;
}
xui_method_explain(onity_kindctrl_compadd, get_prevctrlsize,	s32						)( void )
{
	return KIND_HEIGHT;
}

/*
//event
*/
xui_method_explain(onity_kindctrl_compadd, on_menuitemclick,	void					)( xui_component* sender, xui_method_args&	 args )
{
	xui_menuitem* item = xui_dynamic_cast(xui_menuitem, sender);
	std::string   type = xui_global::unicode_to_ascii(item->get_text());
	std::string	  name = xui_global::unicode_to_ascii(m_namectrl->get_text());

	if (name.length() == 0)
	{
		xui_desktop::get_ins()->show_message(L"Please input component name!", 1);
		return;
	}

	onity_proptempold* proptemp = dynamic_cast<onity_proptempold*>(m_propkind->get_root());
	proptemp->add_component(type, name);
	m_namectrl->ini_drawer(L"");
}