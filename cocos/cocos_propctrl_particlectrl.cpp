#include "2d/CCParticleSystemQuad.h"

#include "xui_global.h"
#include "xui_button.h"
#include "xui_linebox.h"
#include "xui_propview.h"
#include "cocos_resource.h"
#include "cocos_propparticle.h"
#include "cocos_propnodeparticle.h"
#include "cocos_propctrl_particlectrl.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_particlectrl)( xui_propkind* kind, cocos_propnodebase* propnode )
: xui_propdata(kind, L"ParticleCtrl", cocos_propctrl_particlectrl::create)
, m_propnode(propnode)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_particlectrl, get_propnode,		cocos_propnodebase*	)( void )
{
	return m_propnode;
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_particlectrl, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_particlectrl)( xui_propdata* propdata )
: xui_propctrl()
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl, set_parent		)(this);
	xui_method_ptrcall(m_namectrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_namectrl, ini_drawer		)(L"Controller");	

	m_playctrl = new xui_button(xui_vector<s32>(24, 18));
	xui_method_ptrcall(m_playctrl, ini_drawer		)(cocos_resource::icon_play);
	xui_method_ptrcall(m_playctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_playctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_playctrl, set_corner		)(3);
	xui_method_ptrcall(m_playctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_playctrl, xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_propctrl_particlectrl>(this, &cocos_propctrl_particlectrl::on_buttonclick);
	m_stopctrl = new xui_button(xui_vector<s32>(24, 18));
	xui_method_ptrcall(m_stopctrl, ini_drawer		)(cocos_resource::icon_clear);
	xui_method_ptrcall(m_stopctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_stopctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_stopctrl, set_corner		)(3);
	xui_method_ptrcall(m_stopctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_stopctrl, xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_propctrl_particlectrl>(this, &cocos_propctrl_particlectrl::on_buttonclick);
	m_loadctrl = new xui_button(xui_vector<s32>(24, 18));
	xui_method_ptrcall(m_loadctrl, ini_drawer		)(cocos_resource::icon_add);
	xui_method_ptrcall(m_loadctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_loadctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_loadctrl, set_corner		)(3);
	xui_method_ptrcall(m_loadctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_loadctrl, xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_propctrl_particlectrl>(this, &cocos_propctrl_particlectrl::on_buttonclick);

	m_toolline = new xui_linebox(xui_vector<s32>(40, 18));
	xui_method_ptrcall(m_toolline, set_parent		)(this);
	xui_method_ptrcall(m_toolline, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_toolline, set_corner		)(3);
	xui_method_ptrcall(m_toolline, add_linectrl		)(m_playctrl);
	xui_method_ptrcall(m_toolline, add_linectrl		)(m_stopctrl);
	xui_method_ptrcall(m_toolline, add_linectrl		)(m_loadctrl);	

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_toolline);
}

/*
//create
*/
xui_method_explain(cocos_propctrl_particlectrl, create,				xui_propctrl*		)( xui_propdata* propdata )
{
	return new cocos_propctrl_particlectrl(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_particlectrl, on_linkpropdata,	void				)( bool selfupdate /* = false */ )
{}
xui_method_explain(cocos_propctrl_particlectrl, on_editvalue,		void				)( xui_propedit* sender )
{}

/*
//override
*/
xui_method_explain(cocos_propctrl_particlectrl, on_perform,			void				)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	s32 height = xui_propview::default_lineheight;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;

	//namectrl
	pt.x = rt.ax;
	pt.y = rt.ay;
	sz.w = rt.get_w() / 2;
	sz.h = height;
	m_namectrl->on_perform_pt(pt);
	m_namectrl->on_perform_sz(sz);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//toolline
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 - m_toolline->get_renderh()/2;
	m_toolline->on_perform_pt(pt);
}

/*
//event
*/
xui_method_explain(cocos_propctrl_particlectrl, on_buttonclick,		void				)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_particlectrl* data = dynamic_cast<cocos_propdata_particlectrl*>(m_propdatavec[i]);
		cocos_propnodeparticle* node = dynamic_cast<cocos_propnodeparticle*>(data->get_propnode());
		if (sender == m_playctrl)
		{
			node->play();
		}
		else
		if (sender == m_stopctrl)
		{
			node->stop();
		}
		else
		if (sender == m_loadctrl)
		{
			node->reload();
		}
	}
}