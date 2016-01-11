#include "xui_convas.h"
#include "xui_button.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_rect2d.h"

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
	m_type     += "propctrlrect2d";
	m_render    = xui_rect2d<s32>(0, 0, 100, xui_propview::LINE_HEIGHT*2);
	m_backcolor = xui_colour(0.0f);

	xui_propdata_rect2d* datarect2d = (xui_propdata_rect2d*)propdata;
	m_subxedit = new xui_propedit_number(this, datarect2d->get_interval());
	m_subyedit = new xui_propedit_number(this, datarect2d->get_interval());
	m_subwedit = new xui_propedit_number(this, datarect2d->get_interval());
	m_subhedit = new xui_propedit_number(this, datarect2d->get_interval());
	m_namectrl = new xui_drawer("", xui_rect2d<s32>(0, 0, 128, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_backcolor	)(xui_colour(0.0f));
	xui_method_ptrcall(m_namectrl,	set_font		)(xui_family("Arial", 16, false));
	xui_method_ptrcall(m_namectrl,	set_textcolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TA_LC);
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
xui_method_explain(xui_propctrl_rect2d, on_linkpropdata,	void)( void )
{
	m_subxedit->reset();
	m_subyedit->reset();
	m_subwedit->reset();
	m_subhedit->reset();
	m_namectrl->set_text(m_propdata->get_name());

	bool same = true;
	xui_propdata_rect2d* datarect2d = (xui_propdata_rect2d*)m_propdata;
	xui_rect2d<f64> value = datarect2d->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_rect2d* data = (xui_propdata_rect2d*)m_propdatavec[i];
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
			xui_propdata_rect2d* data = (xui_propdata_rect2d*)m_propdatavec[i];
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
			xui_propdata_rect2d* data = (xui_propdata_rect2d*)m_propdatavec[i];
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
			xui_propdata_rect2d* data = (xui_propdata_rect2d*)m_propdatavec[i];
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
			xui_propdata_rect2d* data = (xui_propdata_rect2d*)m_propdatavec[i];
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

	s32 xyeditwidth = (rt.get_sz().w/2 - 18 - subxname->get_renderw() - subyname->get_renderw()) / 2;
	//subxname
	pt.x = rt.get_sz().w/2;
	pt.y = 0;
	sz.w = subxname->get_renderw();
	sz.h = rt.get_sz().h/2;
	subxname->set_renderpt(pt, false);
	subxname->set_rendersz(sz, false);
	//subxedit
	pt.x = pt.x + subxname->get_renderw() + 6;
	pt.y = rt.get_sz().h/4 - subxedit->get_renderh()/2;
	sz.w = xyeditwidth;
	sz.h = subxedit->get_renderh();
	subxedit->set_renderpt(pt, false);
	subxedit->set_rendersz(sz, false);
	//subyname
	pt.x = pt.x + subxedit->get_renderw() + 6;
	pt.y = 0;
	sz.w = subyname->get_renderw();
	sz.h = rt.get_sz().h/2;
	subyname->set_renderpt(pt, false);
	subyname->set_rendersz(sz, false);
	//subyedit
	pt.x = pt.x + subyname->get_renderw() + 6;
	pt.y = rt.get_sz().h/4 - subyedit->get_renderh()/2;
	sz.w = xyeditwidth;
	sz.h = subyedit->get_renderh();
	subyedit->set_renderpt(pt, false);
	subyedit->set_rendersz(sz, false);

	s32 wheditwidth = (rt.get_sz().w/2 - 18 - subwname->get_renderw() - subhname->get_renderw()) / 2;
	//subwname
	pt.x = rt.get_sz().w/2;
	pt.y = rt.get_sz().h/2;
	sz.w = subwname->get_renderw();
	sz.h = rt.get_sz().h/2;
	subwname->set_renderpt(pt, false);
	subwname->set_rendersz(sz, false);
	//subxedit
	pt.x = pt.x + subwname->get_renderw() + 6;
	pt.y = rt.get_sz().h/2 + rt.get_sz().h/4 - subwedit->get_renderh()/2;
	sz.w = wheditwidth;
	sz.h = subxedit->get_renderh();
	subwedit->set_renderpt(pt, false);
	subwedit->set_rendersz(sz, false);
	//subyname
	pt.x = pt.x + subwedit->get_renderw() + 6;
	pt.y = rt.get_sz().h/2;
	sz.w = subhname->get_renderw();
	sz.h = rt.get_sz().h/2;
	subhname->set_renderpt(pt, false);
	subhname->set_rendersz(sz, false);
	//subyedit
	pt.x = pt.x + subhname->get_renderw() + 6;
	pt.y = rt.get_sz().h/2 + rt.get_sz().h/4 - subhedit->get_renderh()/2;
	sz.w = wheditwidth;
	sz.h = subhedit->get_renderh();
	subhedit->set_renderpt(pt, false);
	subhedit->set_rendersz(sz, false);

	//namectrl
	pt.x = 0;
	pt.y = 0;
	sz.w = rt.get_sz().w/2;
	sz.h = m_namectrl->get_renderh();
	m_namectrl->set_renderpt(pt, false);
	m_namectrl->set_rendersz(sz, false);
	m_namectrl->set_textoffset(xui_vector<s32>(get_indent(), 0));
}