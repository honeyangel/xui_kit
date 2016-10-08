#include "NPFileName.h"

#include "xui_global.h"
#include "xui_button.h"
#include "xui_linebox.h"
#include "xui_textbox.h"
#include "xui_treenode.h"
#include "xui_propview.h"
#include "xui_treeview.h"
#include "onity_propentitytemp.h"
#include "onity_propjsones.h"
#include "onity_propjsonestemp.h"
#include "onity_mainform.h"
#include "onity_project.h"
#include "onity_hierarchy.h"
#include "onity_treedata.h"
#include "onity_tempctrl.h"

xui_implement_rtti(onity_tempctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_tempctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_select	= new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_select,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickselect);
	xui_method_ptrcall(m_select,	set_drawcolor	)(true);
	xui_method_ptrcall(m_select,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_select,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_select,	ini_drawer		)(L"Select");

	m_apply		= new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_apply,		xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickapply);
	xui_method_ptrcall(m_apply,		set_drawcolor	)(true);
	xui_method_ptrcall(m_apply,		set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_apply,		set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_apply,		ini_drawer		)(L"Apply");

	m_revert	= new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_revert,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_tempctrl>(this, &onity_tempctrl::on_clickrevert);
	xui_method_ptrcall(m_revert,	set_drawcolor	)(true);
	xui_method_ptrcall(m_revert,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_revert,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_revert,	ini_drawer		)(L"Revert");

	m_linebox	= new xui_linebox(xui_vector<s32>(0, 20), FLOWSTYLE_H);
	xui_method_ptrcall(m_linebox,	set_parent		)(this);
	xui_method_ptrcall(m_linebox,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_linebox,	set_corner		)(3);
	xui_method_ptrcall(m_linebox,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_linebox,	add_linectrl	)(m_select);
	xui_method_ptrcall(m_linebox,	add_linectrl	)(m_apply);
	xui_method_ptrcall(m_linebox,	add_linectrl	)(m_revert);
	m_widgetvec.push_back(m_linebox);
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

	bool enable = (onity_mainform::get_ptr()->was_gamerun() == false);
	xui_method_ptrcall(m_select,	set_enable)(temp != NULL);
	xui_method_ptrcall(m_apply,		set_enable)(enable && temp != NULL && proptemp->was_modify());
	xui_method_ptrcall(m_revert,	set_enable)(enable && temp != NULL && proptemp->was_modify());
}
xui_method_explain(onity_tempctrl, on_perform,			void			)( xui_method_args&   args )
{
	xui_kindctrl::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	xui_method_ptrcall(m_select,	on_perform_w)(rt.get_w()/3);
	xui_method_ptrcall(m_apply,		on_perform_w)(rt.get_w()/3);
	xui_method_ptrcall(m_revert,	on_perform_w)(rt.get_w()-m_select->get_renderw()-m_apply->get_renderw());
	xui_method_ptrcall(m_linebox,	refresh		)();
}

/*
//event
*/
xui_method_explain(onity_tempctrl, on_clickselect,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_propentitytemp*  proptemp = dynamic_cast<onity_propentitytemp*>(m_propkind->get_root());
	Omiga::EntityTemplate* temp     = proptemp->get_template();
	if (temp)
	{
		std::string full = temp->GetFileName();
		std::string path = NPFileNameHelper::PathName(full);
		std::string file = NPFileNameHelper::FileName(full);
		std::string name = temp->GetName();
		path = path.substr(0, path.length()-1);
		file = NPFileNameHelper::SafeName(file);

		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->loc_filenode(xui_global::ascii_to_unicode(path), xui_global::ascii_to_unicode(file), xui_global::ascii_to_unicode(name));
		onity_mainform::get_ptr()->set_pageshow(project);
	}
}
xui_method_explain(onity_tempctrl, on_clickapply,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_propentitytemp* proptemp = dynamic_cast<onity_propentitytemp*>(m_propkind->get_root());
	proptemp->save();

	Omiga::EntityTemplate* temp = proptemp->get_template();
	if (temp)
	{
		onity_hierarchy* hierarchy = onity_mainform::get_ptr()->get_hierarchy();
		std::vector<xui_treenode*> nodes = hierarchy->get_treeview()->get_upmostnodearray();
		for (u32 i = 0; i < nodes.size(); ++i)
		{
			xui_treenode*			node = nodes[i];
			onity_treedata*			data = (onity_treedata*)node->get_linkdata();
			onity_propentitytemp*	prop = dynamic_cast<onity_propentitytemp*>(data->get_prop());
			if (prop != proptemp && prop->get_template() == temp)
				prop->load();
		}

		xui_proproot_vec props;
		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->get_pathfile(L".json", props);
		for (u32 i = 0; i < props.size(); ++i)
		{
			onity_propjsones* propjson = dynamic_cast<onity_propjsones*>(props[i]);
			if (propjson)
			{
				onity_propjsonestemp* prop = dynamic_cast<onity_propjsonestemp*>(propjson->get_subprop(temp));
				if (prop)
				{
					prop->del_compkind();
					prop->add_compkind();
					propjson->set_modify(true);
					break;
				}
			}
		}
	}
}
xui_method_explain(onity_tempctrl, on_clickrevert,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_propentitytemp* proptemp = dynamic_cast<onity_propentitytemp*>(m_propkind->get_root());
	proptemp->load();

	Omiga::EntityTemplate* temp = proptemp->get_template();
	if (temp)
	{
		xui_propview* propview = proptemp->get_ctrl();
		propview->reset();
	}
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