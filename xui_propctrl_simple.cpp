#include "xui_canvas.h"
#include "xui_drawer.h"
#include "xui_toggle.h"
#include "xui_propview.h"
#include "xui_propctrl_simple.h"

xui_implement_rtti(xui_propctrl_simple, xui_propctrl)

xui_propctrl* xui_propctrl_simple::create( xui_propdata* propdata )
{
	return new xui_propctrl_simple(propdata);
}

xui_propctrl_simple::xui_propctrl_simple( xui_propdata* propdata )
: xui_propctrl()
{
	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	m_namectrl->set_parent(this);
	m_namectrl->set_textalign(k_textalign_lc);
	m_widgetvec.push_back(m_namectrl);

	//prop
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(propdata);
	const xui_propdata_vec& vec = dataexpand->get_subprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_propdata*      data = vec[i];
		xui_propedit_base* edit = new_propedit(data);
		if (edit)
		{
			xui_drawer*  namectrl = edit->get_namectrl();
			xui_control* editctrl = edit->get_editctrl();
			namectrl->set_text(data->get_name());
			namectrl->set_parent(this);
			editctrl->set_parent(this);
			m_widgetvec.push_back(namectrl);
			m_widgetvec.push_back(editctrl);
		}

		m_propeditvec.push_back(edit);
	}
}

xui_propctrl_simple::~xui_propctrl_simple( void )
{
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
		delete m_propeditvec[i];
}

void xui_propctrl_simple::on_linkpropdata( bool selfupdate )
{
	if (selfupdate == false)
		m_namectrl->set_text(m_propdata->get_name());

	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
	{
		xui_propedit_base* edit = m_propeditvec[i];
		if (edit == NULL)
			continue;

		if (selfupdate == false)
			edit->reset();

		xui_propdata_vec vec = dataexpand->get_subprop();
		set_samedata(vec[i], get_propdataall(i), edit);
	}
}

void xui_propctrl_simple::on_editvalue( xui_propedit* sender )
{
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
	{
		xui_propedit_base* edit = m_propeditvec[i];
		if (edit == sender)
		{
			set_editdata(edit, get_propdataall(i));
			break;
		}
	}
}

void xui_propctrl_simple::on_invalid( xui_method_args& args )
{
	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::k_default_lineheight * (s32)m_propeditvec.size();

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

void xui_propctrl_simple::on_perform( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;
	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//name
	m_namectrl->on_perform_w(rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));

	s32 max_width = 0;
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
	{
		xui_propedit_base* edit = m_propeditvec[i];
		if (edit == NULL)
			continue;

		s32 width = edit->get_namectrl()->get_renderw();
		max_width = xui_max(max_width, width);
	}
	max_width += 2;
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
	{
		xui_propedit_base* edit = m_propeditvec[i];
		if (edit == NULL)
			continue;

		xui_drawer*  namectrl = edit->get_namectrl();
		xui_control* editctrl = edit->get_editctrl();
		pt.x = rt.get_w()/2;
		pt.y = i*height;
		namectrl->on_perform_pt(pt);
		namectrl->on_perform_w (max_width);
		pt.x = rt.get_w()/2 + max_width;
		pt.y = i*height + height/2 - editctrl->get_renderh()/2;
		editctrl->on_perform_pt(pt);
		editctrl->on_perform_w (xui_issub_kindof(xui_toggle, editctrl) ? editctrl->get_renderw() : (rt.get_w() - pt.x));
	}
}

xui_propedit_base* xui_propctrl_simple::new_propedit( xui_propdata* propdata )
{
	xui_propdata_bool*   databool = dynamic_cast<xui_propdata_bool*  >(propdata);
	xui_propdata_enum*   dataenum = dynamic_cast<xui_propdata_enum*  >(propdata);
	xui_propdata_number* datanumb = dynamic_cast<xui_propdata_number*>(propdata);
	xui_propdata_string* datatext = dynamic_cast<xui_propdata_string*>(propdata);
	if (databool)	return new xui_propedit_bool  (this);
	if (dataenum)	return new xui_propedit_enum  (this, dataenum->get_textmap());
	if (datatext)	return new xui_propedit_string(this);
	if (datanumb)	return new xui_propedit_number(this, datanumb->get_numbtype(), datanumb->get_interval(), false);

	return NULL;
}

void xui_propctrl_simple::set_samedata( xui_propdata* propdata, const xui_propdata_vec& vec, xui_propedit* propedit )
{
	xui_propdata_bool*   databool = dynamic_cast<xui_propdata_bool*  >(propdata);
	xui_propdata_enum*   dataenum = dynamic_cast<xui_propdata_enum*  >(propdata);
	xui_propdata_string* datatext = dynamic_cast<xui_propdata_string*>(propdata);
	xui_propdata_number* datanumb = dynamic_cast<xui_propdata_number*>(propdata);

	bool		 valuebool = (databool == NULL) ? false : databool->get_value();
	s32			 valueenum = (dataenum == NULL) ? 0     : dataenum->get_value();
	std::wstring valuetext = (datatext == NULL) ? L""   : datatext->get_value();
	f64			 valuenumb = (datanumb == NULL) ? 0     : datanumb->get_value();

	bool same = true;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (databool)
		{
			xui_propdata_bool*   data = dynamic_cast<xui_propdata_bool*  >(vec[i]);
			if (data->get_value() != valuebool)
			{
				same = false;
				break;
			}
		}
		if (dataenum)
		{
			xui_propdata_enum*   data = dynamic_cast<xui_propdata_enum*  >(vec[i]);
			if (data->get_value() != valueenum)
			{
				same = false;
				break;
			}
		}
		if (datatext)
		{
			xui_propdata_string* data = dynamic_cast<xui_propdata_string*>(vec[i]);
			if (data->get_value() != valuetext)
			{
				same = false;
				break;
			}
		}
		if (datanumb)
		{
			xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(vec[i]);
			if (data->get_value() != valuenumb)
			{
				same = false;
				break;
			}
		}
	}

	if (same)
	{
		if (databool)
		{
			dynamic_cast<xui_propedit_bool*  >(propedit)->set_value(valuebool);
		}
		if (dataenum)
		{
			dynamic_cast<xui_propedit_enum*  >(propedit)->set_value(valueenum);
		}
		if (datatext)
		{
			dynamic_cast<xui_propedit_string*>(propedit)->set_value(valuetext);
		}
		if (datanumb)
		{
			dynamic_cast<xui_propedit_number*>(propedit)->set_value(valuenumb);
		}
	}
}

void xui_propctrl_simple::set_editdata( xui_propedit* propedit, const xui_propdata_vec& vec )
{
	xui_propedit_bool*   editbool = dynamic_cast<xui_propedit_bool*  >(propedit);
	xui_propedit_enum*   editenum = dynamic_cast<xui_propedit_enum*  >(propedit);
	xui_propedit_string* edittext = dynamic_cast<xui_propedit_string*>(propedit);
	xui_propedit_number* editnumb = dynamic_cast<xui_propedit_number*>(propedit);

	bool		 valuebool = (editbool == NULL) ? false : editbool->get_value();
	s32			 valueenum = (editenum == NULL) ? 0     : editenum->get_value();
	std::wstring valuetext = (edittext == NULL) ? L""   : edittext->get_value();
	f64			 valuenumb = (editnumb == NULL) ? 0     : editnumb->get_value();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (editbool)
		{
			dynamic_cast<xui_propdata_bool*  >(vec[i])->set_value(valuebool);
		}
		if (editenum)
		{
			dynamic_cast<xui_propdata_enum*  >(vec[i])->set_value(valueenum);
		}
		if (edittext)
		{
			dynamic_cast<xui_propdata_string*>(vec[i])->set_value(valuetext);
		}
		if (editnumb)
		{
			dynamic_cast<xui_propdata_number*>(vec[i])->set_value(valuenumb);
		}
	}
}

xui_propdata_vec xui_propctrl_simple::get_propdataall( u32 index )
{
	xui_propdata_vec result;
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdatavec[i]);
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		result.push_back(vec[index]);
	}

	return result;
}
