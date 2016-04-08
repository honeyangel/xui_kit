#include "onity_propfile.h"
#include "onity_savekind.h"
#include "onity_savectrl.h"

xui_implement_rtti(onity_savectrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_savectrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_save = new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_save,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_savectrl>(this, &onity_savectrl::on_saveclick);
	xui_method_ptrcall(m_save,	set_parent		)(this);
	xui_method_ptrcall(m_save,	set_corner		)(3);
	xui_method_ptrcall(m_save,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_save,	set_drawcolor	)(true);
	xui_method_ptrcall(m_save,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_save,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_save,	ini_drawer		)(L"Apply");

	m_load = new xui_button(xui_vector<s32>(80, 20));
	xui_method_ptrcall(m_load,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_savectrl>(this, &onity_savectrl::on_loadclick);
	xui_method_ptrcall(m_load,	set_parent		)(this);
	xui_method_ptrcall(m_load,	set_corner		)(3);
	xui_method_ptrcall(m_load,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_load,	set_drawcolor	)(true);
	xui_method_ptrcall(m_load,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_load,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_load,	ini_drawer		)(L"Revert");

	m_widgetvec.push_back(m_save);
	m_widgetvec.push_back(m_load);
}

/*
//static
*/
xui_method_explain(onity_savectrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new onity_savectrl(propkind);
}

/*
//callback
*/
xui_method_explain(onity_savectrl, on_invalid,			void			)( xui_method_args& args )
{
	xui_kindctrl::on_invalid(args);
	bool modify = false;
	onity_savekind* savekind = dynamic_cast<onity_savekind*>(m_propkind);
	if (savekind)
	{
		onity_propfile* propfile = savekind->get_propfile();
		modify = propfile->was_modify();
	}

	m_save->set_enable(modify);
	m_load->set_enable(modify);
}
xui_method_explain(onity_savectrl, on_perform,			void			)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//load
	pt.x  = rt.bx - m_load->get_renderw();
	pt.y  = rt.ay + (height-m_load->get_renderh())/2;
	m_load->on_perform_pt(pt);
	//save
	pt.x -= m_save->get_renderw()+4;
	m_save->on_perform_pt(pt);
}

/*
//event
*/
xui_method_explain(onity_savectrl, on_saveclick,		void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		onity_savekind* savekind = dynamic_cast<onity_savekind*>(m_propkindvec[i]);
		onity_propfile* propfile = savekind->get_propfile();
		if (propfile->was_modify())
			propfile->save();
	}

	m_save->set_enable(false);
	m_load->set_enable(false);
}
xui_method_explain(onity_savectrl, on_loadclick,		void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		onity_savekind* savekind = dynamic_cast<onity_savekind*>(m_propkindvec[i]);
		onity_propfile* propfile = savekind->get_propfile();
		if (propfile->was_modify())
			propfile->load();
	}

	m_save->set_enable(false);
	m_load->set_enable(false);
}

/*
//override
*/
xui_method_explain(onity_savectrl, get_elsectrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}
xui_method_explain(onity_savectrl, get_prevctrlsize,	s32				)( void )
{
	return KIND_HEIGHT;
}