#include "xui_convas.h"
#include "xui_button.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_rect2d.h"

xui_implement_rtti(xui_propctrl_rect2d, xui_propctrl);
/*
//create
*/
xui_method_explain(xui_propctrl_rect2d, create, xui_propctrl*)( xui_propdata* propdata )
{
	return new xui_propctrl_rect2d(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_rect2d)( xui_propdata* propdata )
: xui_propctrl()
{
	m_render   = xui_rect2d<s32>(0, 0, 200, xui_propview::default_lineheight*2);

	xui_propdata_rect2d* datarect2d = dynamic_cast<xui_propdata_rect2d*>(propdata);
	m_subxedit = new xui_propedit_number(this, datarect2d->get_numbtype(), datarect2d->get_interval());
	m_subyedit = new xui_propedit_number(this, datarect2d->get_numbtype(), datarect2d->get_interval());
	m_subwedit = new xui_propedit_number(this, datarect2d->get_numbtype(), datarect2d->get_interval());
	m_subhedit = new xui_propedit_number(this, datarect2d->get_numbtype(), datarect2d->get_interval());
	m_namectrl = new xui_drawer(xui_vector<s32>(100, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_drawer*  subwname = m_subwedit->get_namectrl();
	xui_control* subwedit = m_subwedit->get_editctrl();
	xui_drawer*	 subhname = m_subhedit->get_namectrl();
	xui_control* subhedit = m_subhedit->get_editctrl();
	xui_method_ptrcall(subxname,	set_text		)(L"X");
	xui_method_ptrcall(subyname,	set_text		)(L"Y");
	xui_method_ptrcall(subwname,	set_text		)(L"W");
	xui_method_ptrcall(subhname,	set_text		)(L"H");
	xui_method_ptrcall(subxname,	set_parent		)(this);
	xui_method_ptrcall(subyname,	set_parent		)(this);
	xui_method_ptrcall(subxedit,	set_parent		)(this);
	xui_method_ptrcall(subyedit,	set_parent		)(this);
	xui_method_ptrcall(subwname,	set_parent		)(this);
	xui_method_ptrcall(subwedit,	set_parent		)(this);
	xui_method_ptrcall(subhname,	set_parent		)(this);
	xui_method_ptrcall(subhedit,	set_parent		)(this);
	m_widgetvec.push_back(subxname);
	m_widgetvec.push_back(subyname);
	m_widgetvec.push_back(subxedit);
	m_widgetvec.push_back(subyedit);
	m_widgetvec.push_back(subwname);
	m_widgetvec.push_back(subwedit);
	m_widgetvec.push_back(subhname);
	m_widgetvec.push_back(subhedit);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_rect2d)( void )
{
	delete m_subxedit;
	delete m_subyedit;
	delete m_subwedit;
	delete m_subhedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_rect2d, on_linkpropdata,	void)( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_subxedit->reset();
		m_subyedit->reset();
		m_subwedit->reset();
		m_subhedit->reset();
		m_namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	xui_propdata_rect2d* datarect2d = dynamic_cast<xui_propdata_rect2d*>(m_propdata);
	xui_rect2d<f64> value = datarect2d->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_rect2d* data = dynamic_cast<xui_propdata_rect2d*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_subxedit->set_value(value.get_pt().x);
		m_subyedit->set_value(value.get_pt().y);
		m_subwedit->set_value(value.get_sz().w);
		m_subhedit->set_value(value.get_sz().h);
	}
}
xui_method_explain(xui_propctrl_rect2d, on_editvalue,		void)( xui_propedit* sender )
{
	if (sender == m_subxedit)
	{
		f64 x = m_subxedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_rect2d* data = dynamic_cast<xui_propdata_rect2d*>(m_propdatavec[i]);
			xui_rect2d<f64> value = data->get_value();
			value.set_x(x);
			data->set_value(value);
		}
	}
	if (sender == m_subyedit)
	{
		f64 y = m_subyedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_rect2d* data = dynamic_cast<xui_propdata_rect2d*>(m_propdatavec[i]);
			xui_rect2d<f64> value = data->get_value();
			value.set_y(y);
			data->set_value(value);
		}
	}
	if (sender == m_subwedit)
	{
		f64 w = m_subwedit->get_value();
		if (w < 0.0)
		{
			w = 0.0;
			m_subwedit->set_value(w);
		}

		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_rect2d* data = dynamic_cast<xui_propdata_rect2d*>(m_propdatavec[i]);
			xui_rect2d<f64> value = data->get_value();
			value.set_w(w);
			data->set_value(value);
		}
	}
	if (sender == m_subhedit)
	{
		f64 h = m_subhedit->get_value();
		if (h < 0.0)
		{
			h = 0.0;
			m_subhedit->set_value(h);
		}
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_rect2d* data = dynamic_cast<xui_propdata_rect2d*>(m_propdatavec[i]);
			xui_rect2d<f64> value = data->get_value();
			value.set_h(h);
			data->set_value(value);
		}
	}
}

/*
//override
*/
xui_method_explain(xui_propctrl_rect2d, on_perform,			void)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_drawer*	 subwname = m_subwedit->get_namectrl();
	xui_control* subwedit = m_subwedit->get_editctrl();
	xui_drawer*	 subhname = m_subhedit->get_namectrl();
	xui_control* subhedit = m_subhedit->get_editctrl();

	s32 xwnamewidth = 16;//xui_max(subxname->get_renderw(), subwname->get_renderw());
	s32 yhnamewidth = 16;//xui_max(subyname->get_renderw(), subhname->get_renderw());
	s32 xyeditwidth = (rt.get_w() - rt.get_w()/2 - 12 - xwnamewidth - yhnamewidth) / 2;
	//subxname
	pt.x = rt.get_w()/2;
	pt.y = 0;
	subxname->on_perform_pt(pt);
	subxname->on_perform_w (xwnamewidth);
	subxname->on_perform_h (rt.get_h()/2);
	//subxedit
	pt.x = pt.x + subxname->get_renderw() + 4;
	pt.y = rt.get_h()/4 - subxedit->get_renderh()/2;
	subxedit->on_perform_pt(pt);
	subxedit->on_perform_w (xyeditwidth);
	//subyname
	pt.x = pt.x + subxedit->get_renderw() + 4;
	pt.y = 0;
	subyname->on_perform_pt(pt);
	subyname->on_perform_w (yhnamewidth);
	subyname->on_perform_h (rt.get_h()/2);
	//subyedit
	pt.x = pt.x + subyname->get_renderw() + 4;
	pt.y = rt.get_h()/4 - subyedit->get_renderh()/2;
	subyedit->on_perform_pt(pt);
	subyedit->on_perform_w (rt.get_w() - pt.x);

	s32 wheditwidth = (rt.get_w() - rt.get_w()/2 - 12 - xwnamewidth - yhnamewidth) / 2;
	//subwname
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2;
	subwname->on_perform_pt(pt);
	subwname->on_perform_w (xwnamewidth);
	subwname->on_perform_h (rt.get_h()/2);
	//subwedit
	pt.x = pt.x + subwname->get_renderw() + 4;
	pt.y = rt.get_h()/2 + rt.get_h()/4 - subwedit->get_renderh()/2;
	subwedit->on_perform_pt(pt);
	subwedit->on_perform_w (wheditwidth);
	//subhname
	pt.x = pt.x + subwedit->get_renderw() + 4;
	pt.y = rt.get_h()/2;
	subhname->on_perform_pt(pt);
	subhname->on_perform_w (yhnamewidth);
	subhname->on_perform_h (rt.get_h()/2);
	//subhedit
	pt.x = pt.x + subhname->get_renderw() + 4;
	pt.y = rt.get_h()/2 + rt.get_h()/4 - subhedit->get_renderh()/2;
	subhedit->on_perform_pt(pt);
	subhedit->on_perform_w (rt.get_w() - pt.x);

	//namectrl
	m_namectrl->on_perform_w(rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(get_indent(), 0));
}