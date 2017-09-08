#include "2d/CCParticleSystemQuad.h"
#include "2d/CCSprite.h"
#include "ui/UIWidget.h"

#include "xui_global.h"
#include "xui_toggle.h"
#include "xui_linebox.h"
#include "xui_propview.h"
#include "cocos_resource.h"
#include "cocos_propnodesprite.h"
#include "cocos_propctrl_flip.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_flip)( xui_propkind* kind, cocos_propnodebase* propnode )
: xui_propdata(kind, L"Flip", cocos_propctrl_flip::create)
, m_propnode(propnode)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_flip, get_propnode,		cocos_propnodebase*	)( void )
{
	return m_propnode;
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_flip, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_flip)( xui_propdata* propdata )
: xui_propctrl()
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl, set_parent	)(this);
	xui_method_ptrcall(m_namectrl, set_textalign)(TEXTALIGN_LC);
	xui_method_ptrcall(m_namectrl, ini_drawer	)(L"Flip");

	m_horzctrl = new xui_toggle(xui_vector<s32>(24, 18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_horzctrl, ini_drawer		)(cocos_resource::icon_leftright);
	xui_method_ptrcall(m_horzctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_horzctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_horzctrl, set_corner		)(3);
	xui_method_ptrcall(m_horzctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_flip>(this, &cocos_propctrl_flip::on_toggleclick);
	m_vertctrl = new xui_toggle(xui_vector<s32>(24, 18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_vertctrl, ini_drawer		)(cocos_resource::icon_topbottom);
	xui_method_ptrcall(m_vertctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_vertctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_vertctrl, set_corner		)(3);
	xui_method_ptrcall(m_vertctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertctrl, xm_toggleclick	) += new xui_method_member<xui_method_args, cocos_propctrl_flip>(this, &cocos_propctrl_flip::on_toggleclick);

	m_toolline = new xui_linebox(xui_vector<s32>(40, 18));
	xui_method_ptrcall(m_toolline, set_parent		)(this);
	xui_method_ptrcall(m_toolline, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_toolline, set_corner		)(3);
	xui_method_ptrcall(m_toolline, add_linectrl		)(m_horzctrl);
	xui_method_ptrcall(m_toolline, add_linectrl		)(m_vertctrl);

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_toolline);
}

/*
//create
*/
xui_method_explain(cocos_propctrl_flip, create,				xui_propctrl*		)( xui_propdata* propdata )
{
	return new cocos_propctrl_flip(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_flip, on_linkpropdata,	void				)( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_horzctrl->ini_toggle(false);
		m_vertctrl->ini_toggle(false);
	}

	if (m_propdatavec.size() == 1)
	{
		cocos_propdata_flip* dataflip	= dynamic_cast<cocos_propdata_flip*>(m_propdata);
		cocos_propnodebase*  propnode	= dataflip->get_propnode();
		cocos2d::Sprite*	 sprite		= dynamic_cast<cocos2d::Sprite*>(propnode->get_node());
		cocos2d::ui::Widget* widget		= dynamic_cast<cocos2d::ui::Widget*>(propnode->get_node());
		if (sprite)
		{
			m_horzctrl->ini_toggle(sprite->isFlippedX());
			m_vertctrl->ini_toggle(sprite->isFlippedY());
		}
		if (widget)
		{
			m_horzctrl->ini_toggle(widget->isFlippedX());
			m_vertctrl->ini_toggle(widget->isFlippedY());
		}
	}
}
xui_method_explain(cocos_propctrl_flip, on_editvalue,		void				)( xui_propedit* sender )
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
xui_method_explain(cocos_propctrl_flip, on_perform,			void				)( xui_method_args& args )
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
	pt.x = rt.get_w() / 2;
	pt.y = rt.ay + height / 2 - m_toolline->get_renderh() / 2;
	m_toolline->on_perform_pt(pt);
}

/*
//event
*/
xui_method_explain(cocos_propctrl_flip, on_toggleclick,		void				)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_flip* data	= dynamic_cast<cocos_propdata_flip*>(m_propdatavec[i]);
		cocos_propnodebase*  prop	= dynamic_cast<cocos_propnodebase*>(data->get_propnode());
		cocos2d::Sprite*	 sprite = dynamic_cast<cocos2d::Sprite*>(prop->get_node());
		cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(prop->get_node());
		if (sender == m_horzctrl)
		{
			if (sprite)
				sprite->setFlippedX(m_horzctrl->was_push());
			if (widget)
				widget->setFlippedX(m_horzctrl->was_push());
		}
		else
		if (sender == m_vertctrl)
		{
			if (sprite)
				sprite->setFlippedY(m_vertctrl->was_push());
			if (widget)
				widget->setFlippedY(m_vertctrl->was_push());
		}
	}

	on_editvalue(NULL);
}