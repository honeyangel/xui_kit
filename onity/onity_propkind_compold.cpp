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
#include "onity_proptempold.h"
#include "onity_propkind_compold.h"

//////////////////////////////////////////////////////////////////////////
//propkind
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propkind_compold)( xui_proproot* root, const std::wstring& name, xui_bitmap* icon, BreezeGame::Json::Value* node )
: xui_propkind(root, name, "ComponentOld", onity_kindctrl_compold::create, icon, true)
, m_node(node)
{
	std::vector<std::string> members = m_node->getMemberNames();
	for (u32 i = 0; i < members.size(); ++i)
	{
		std::string member = members[i];
		if (member == "ClassName" ||
			member == "Family")
			continue;

		BreezeGame::Json::Value* data = &((*m_node)[member]);
		if (data->isNumeric())
		{
			add_propdata(new xui_propdata_number_func(
				this,
				xui_global::ascii_to_unicode(member),
				xui_propctrl_number::create,
				get_number,
				set_number,
				data,
				NT_FLOAT));
		}
		else
		if (data->isBool())
		{
			add_propdata(new xui_propdata_bool(
				this, 
				xui_global::ascii_to_unicode(member), 
				xui_propctrl_bool::create, 
				get_bool, 
				set_bool, 
				data));
		}
		else
		if (data->isString())
		{
			add_propdata(new xui_propdata_string(
				this, 
				xui_global::ascii_to_unicode(member),
				xui_propctrl_string::create,
				get_string,
				set_string,
				data));
		}
	}
}

/*
//method
*/
xui_method_explain(onity_propkind_compold, get_node,			BreezeGame::Json::Value*)( void )
{
	return m_node;
}

/*
//static
*/
xui_method_explain(onity_propkind_compold, get_bool,			bool					)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return data->asBool();
}
xui_method_explain(onity_propkind_compold, set_bool,			void					)( void* userptr, bool value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(value);
}
xui_method_explain(onity_propkind_compold, get_number,			f64						)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return data->asDouble();
}
xui_method_explain(onity_propkind_compold, set_number,			void					)( void* userptr, f64  value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(value);
}
xui_method_explain(onity_propkind_compold, get_string,			std::wstring			)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return xui_global::ascii_to_unicode(data->asString());
}
xui_method_explain(onity_propkind_compold, set_string,			void					)( void* userptr, const std::wstring& value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(xui_global::unicode_to_ascii(value));
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
	return new onity_kindctrl_compold(NULL);
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
xui_method_explain(onity_kindctrl_compold, on_propkindchange,	void					)( void )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propctrl* propctrl = m_propctrlvec[i];
		xui_propdata_vec vec = propctrl->get_propdata();
		for (xui_propdata_vec::iterator itor = vec.begin(); itor != vec.end(); ++itor)
			(*itor)->non_ctrl();

		std::vector<xui_component*>::iterator itor = std::find(
			m_widgetvec.begin(), 
			m_widgetvec.end(), 
			propctrl);

		if (itor != m_widgetvec.end())
		{
			m_widgetvec.erase(itor);
			propctrl->set_parent(NULL);
			xui_desktop::get_ins()->move_recycle(propctrl);
		}
	}
	m_propctrlvec.clear();

	bool same = true;
	onity_propkind_compold* propkind = dynamic_cast<onity_propkind_compold*>(m_propkind);
	BreezeGame::Json::Value* node = propkind->get_node();
	for (u32 i = 1; i < m_propkindvec.size(); ++i)
	{
		onity_propkind_compold* tempkind = dynamic_cast<onity_propkind_compold*>(m_propkindvec[i]);
		if (tempkind->get_node() != node)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		const xui_propdata_vec& vec = m_propkind->get_propdata();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_prop_newctrl  func = vec[i]->get_func();
			xui_propctrl* propctrl = (*func)(vec[i]);
			propctrl->refresh();
			propctrl->set_parent(this);
			m_widgetvec.push_back(propctrl);
			m_propctrlvec.push_back(propctrl);
		}

		xui_kindctrl::on_propkindchange();
	}
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