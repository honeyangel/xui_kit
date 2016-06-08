#include "NP2DSTransRef.h"
#include "xui_toggle.h"
#include "onity_resource.h"
#include "onity_propctrl_flip.h"

xui_implement_rtti(onity_propctrl_flip, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_flip)( xui_propdata* propdata )
: xui_propctrl()
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);

	m_horzctrl = new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_horzctrl,	set_parent		)(this);
	xui_method_ptrcall(m_horzctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_horzctrl,	set_corner		)(3);
	xui_method_ptrcall(m_horzctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzctrl,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzctrl,	ini_drawer		)(onity_resource::icon_leftright, xui_vector<s32>(16));
	xui_method_ptrcall(m_horzctrl,	xm_toggleclick	) += new xui_method_member<xui_method_args, onity_propctrl_flip>(this, &onity_propctrl_flip::on_toggleclick);

	m_vertctrl = new xui_toggle(xui_vector<s32>(18), TOGGLE_BUTTON);
	xui_method_ptrcall(m_vertctrl,	set_parent		)(this);
	xui_method_ptrcall(m_vertctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_vertctrl,	set_corner		)(3);
	xui_method_ptrcall(m_vertctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertctrl,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertctrl,	ini_drawer		)(onity_resource::icon_topbottom, xui_vector<s32>(16));
	xui_method_ptrcall(m_vertctrl,	xm_toggleclick	) += new xui_method_member<xui_method_args, onity_propctrl_flip>(this, &onity_propctrl_flip::on_toggleclick);

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_horzctrl);
	m_widgetvec.push_back(m_vertctrl);
}

/*
//static
*/
xui_method_explain(onity_propctrl_flip, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_flip(propdata);
}

/*
//override
*/
xui_method_explain(onity_propctrl_flip, on_linkpropdata,	void			)( void )
{
	m_namectrl->set_text(m_propdata->get_name());
	m_horzctrl->ini_toggle(false);
	m_vertctrl->ini_toggle(false);

	bool same = true;
	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(m_propdata);
	f64 value = datanumber->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		u32 flip = (s32)value;
		if (flip & 0x01)
			m_horzctrl->ini_toggle(true);
		if (flip & 0x02)
			m_vertctrl->ini_toggle(true);
	}
}
xui_method_explain(onity_propctrl_flip, on_editvalue,		void			)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(onity_propctrl_flip, on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	//horzctrl
	pt.x  = rt.get_w()/3;
	pt.y  = rt.get_h()/2 - m_horzctrl->get_renderh()/2;
	m_horzctrl->on_perform_pt(pt);
	//vertctrl
	pt.x += m_horzctrl->get_renderw() + 4;
	m_vertctrl->on_perform_pt(pt);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w (rt.get_w()/3);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

/*
//event
*/
xui_method_explain(onity_propctrl_flip, on_toggleclick,		void			)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_horzctrl)
	{
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(m_propdatavec[i]);
			u32 flip = (u32)datanumber->get_value();
			if (m_horzctrl->was_push())flip |=   0x01;
			else                       flip &= (~0x01);

			datanumber->set_value((f64)flip);
		}
	}
	else
	if (sender == m_vertctrl)
	{
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(m_propdatavec[i]);
			u32 flip = (u32)datanumber->get_value();
			if (m_vertctrl->was_push())flip |=   0x02;
			else                       flip &= (~0x02);

			datanumber->set_value((f64)flip);
		}
	}
}