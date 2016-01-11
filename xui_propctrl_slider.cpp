#include "xui_slider.h"
#include "xui_drawer.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_slider.h"

/*
//create
*/
xui_method_explain(xui_propctrl_slider, create, xui_propctrl*)( xui_propdata* propdata )
{
	return new xui_propctrl_slider(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_slider)( xui_propdata* propdata )
: xui_propctrl()
{
	m_type	   += "propctrlslider";
	m_backcolor = xui_colour(0.0f);

	xui_propdata_number* datanumber = (xui_propdata_number*)propdata;
	xui_propedit_slider* editslider = new xui_propedit_slider(this, datanumber->get_interval(), datanumber->get_minvalue(), datanumber->get_maxvalue());

	xui_drawer*  namectrl = editslider->get_editnumb()->get_namectrl();
	xui_control* textctrl = editslider->get_editnumb()->get_editctrl();
	xui_slider*  spinctrl = editslider->get_spinctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	spinctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);
	m_widgetvec.push_back(spinctrl);

	m_propedit = editslider;
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_slider)( void )
{
	delete m_propedit;
}

/*
//override
*/
xui_method_explain(xui_propctrl_slider, on_linkpropdata,	void			)( void )
{
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_editnumb()->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

	bool same = true;
	xui_propdata_number* datanumber = (xui_propdata_number*)m_propdata;
	f64 value = datanumber->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_number* data = (xui_propdata_number*)m_propdatavec[i];
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_propedit->set_value(value);
	}
}
xui_method_explain(xui_propctrl_slider, on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	xui_propedit_slider* editslider = (xui_propedit_slider*)m_propedit;
	xui_drawer*  namectrl = m_propedit->get_editnumb()->get_namectrl();
	xui_control* textctrl = m_propedit->get_editnumb()->get_editctrl();
	xui_slider*  spinctrl = m_propedit->get_spinctrl();
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//namectrl
	s32 indent = get_indent();
	pt.x = 0;
	pt.y = 0;
	sz.w = rt.get_sz().w/2;
	sz.h = rt.get_sz().h;
	namectrl->set_renderpt(pt, false);
	namectrl->set_rendersz(sz, false);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//textctrl
	pt.x = rt.bx - textctrl->get_renderw();
	pt.y = rt.get_sz().h/2 - textctrl->get_renderh()/2;
	textctrl->set_renderpt(pt, false);
	//spinctrl
	pt.x = rt.get_sz().w/2;
	pt.y = rt.get_sz().h/2 - spinctrl->get_renderh()/2;
	sz.w = rt.get_sz().w/2 - textctrl->get_renderw();
	sz.h = spinctrl->get_renderh();
	spinctrl->set_renderpt(pt, false);
	spinctrl->set_rendersz(sz, false);
}

/*
//event
*/
xui_method_explain(xui_propctrl_slider, on_editvalue,		void			)( xui_propedit* sender )
{
	f64 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_number* data = (xui_propdata_number*)m_propdatavec[i];
		data->set_value(value);
	}
}