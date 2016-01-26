#include "xui_convas.h"
#include "xui_drawer.h"
#include "xui_toggle.h"
#include "xui_propview.h"
#include "xui_propctrl_simple.h"

xui_implement_rtti(xui_propctrl_simple, xui_propctrl);

/*
//create
*/
xui_method_explain(xui_propctrl_simple, create, xui_propctrl*)( xui_propdata* propdata )
{
	return new xui_propctrl_simple(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_simple)( xui_propdata* propdata )
: xui_propctrl()
{
	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20), this);
	xui_method_ptrcall(m_namectrl, set_textalign)(TA_LC);
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
			xui_method_ptrcall(namectrl, set_text	)(data->get_name());
			xui_method_ptrcall(namectrl, set_parent	)(this);
			xui_method_ptrcall(editctrl, set_parent	)(this);
			m_widgetvec.push_back(namectrl);
			m_widgetvec.push_back(editctrl);
		}

		m_propeditvec.push_back(edit);
	}
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_simple)( void )
{
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
		delete m_propeditvec[i];
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_simple, on_linkpropdata,	void				)( void )
{
	m_namectrl->set_text(m_propdata->get_name());

	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	for (u32 i = 0; i < m_propeditvec.size(); ++i)
	{
		xui_propedit_base* edit = m_propeditvec[i];
		if (edit == NULL)
			continue;

		edit->reset();

		xui_propdata_vec vec = dataexpand->get_subprop();
		set_samedata(vec[i], get_propdataall(i), edit);
	}
}
xui_method_explain(xui_propctrl_simple, on_editvalue,		void				)( xui_propedit* sender )
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

/*
//override
*/
xui_method_explain(xui_propctrl_simple, on_invalid,			void				)( xui_method_args& args )
{
	xui_propctrl::on_invalid(args);

	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::LINE_HEIGHT * (s32)m_propeditvec.size();

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}

}
xui_method_explain(xui_propctrl_simple, on_perform,			void				)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::LINE_HEIGHT;
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
		editctrl->on_perform_w (xui_issub_kindof(xui_toggle, editctrl) ? editctrl->get_renderw() : (rt.get_w()/2 - max_width));
	}
}

/*
//method
*/
xui_method_explain(xui_propctrl_simple, new_propedit,		xui_propedit_base*	)( xui_propdata* propdata )
{
	xui_propdata_bool*   databool   = dynamic_cast<xui_propdata_bool*  >(propdata);
	xui_propdata_enum*   dataenum   = dynamic_cast<xui_propdata_enum*  >(propdata);
	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(propdata);
	xui_propdata_string* datastring = dynamic_cast<xui_propdata_string*>(propdata);
	if (databool  )	return new xui_propedit_bool	(this);
	if (dataenum  )	return new xui_propedit_enum	(this, xui_method_ptrcall(dataenum,   get_textmap )());
	if (datastring)	return new xui_propedit_string	(this);
	if (datanumber)	return new xui_propedit_number	(this, xui_method_ptrcall(datanumber, get_interval)());

	return NULL;
}
xui_method_explain(xui_propctrl_simple, set_samedata,		void				)( xui_propdata* propdata, const xui_propdata_vec& vec, xui_propedit* propedit )
{
	xui_propdata_bool*   databool   = dynamic_cast<xui_propdata_bool*  >(propdata);
	xui_propdata_enum*   dataenum   = dynamic_cast<xui_propdata_enum*  >(propdata);
	xui_propdata_string* datastring = dynamic_cast<xui_propdata_string*>(propdata);
	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(propdata);

	bool		 valuebool   = (databool   == NULL) ? false : xui_method_ptrcall(databool,   get_value)();
	s32			 valueenum   = (dataenum   == NULL) ? 0     : xui_method_ptrcall(dataenum,   get_value)();
	std::wstring valuestring = (datastring == NULL) ? L""   : xui_method_ptrcall(datastring, get_value)();
	f64			 valuenumber = (datanumber == NULL) ? 0     : xui_method_ptrcall(datanumber, get_value)();

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
		if (datastring)
		{
			xui_propdata_string* data = dynamic_cast<xui_propdata_string*>(vec[i]);
			if (data->get_value() != valuestring)
			{
				same = false;
				break;
			}
		}
		if (datanumber)
		{
			xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(vec[i]);
			if (data->get_value() != valuenumber)
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
			xui_propedit_bool*   editbool   = dynamic_cast<xui_propedit_bool*  >(propedit);
			xui_method_ptrcall(editbool,   set_value)(valuebool);
		}
		if (dataenum)
		{
			xui_propedit_enum*   editenum   = dynamic_cast<xui_propedit_enum*  >(propedit);
			xui_method_ptrcall(editenum,   set_value)(valueenum);
		}
		if (datastring)
		{
			xui_propedit_string* editstring = dynamic_cast<xui_propedit_string*>(propedit);
			xui_method_ptrcall(editstring, set_value)(valuestring);
		}
		if (datanumber)
		{
			xui_propedit_number* editnumber = dynamic_cast<xui_propedit_number*>(propedit);
			xui_method_ptrcall(editnumber, set_value)(valuenumber);
		}
	}
}
xui_method_explain(xui_propctrl_simple, set_editdata,		void				)( xui_propedit* propedit, const xui_propdata_vec& vec )
{
	xui_propedit_bool*   editbool   = dynamic_cast<xui_propedit_bool*  >(propedit);
	xui_propedit_enum*   editenum   = dynamic_cast<xui_propedit_enum*  >(propedit);
	xui_propedit_string* editstring = dynamic_cast<xui_propedit_string*>(propedit);
	xui_propedit_number* editnumber = dynamic_cast<xui_propedit_number*>(propedit);

	bool		 valuebool   = (editbool   == NULL) ? false : xui_method_ptrcall(editbool,   get_value)();
	s32			 valueenum	 = (editenum   == NULL) ? 0     : xui_method_ptrcall(editenum,   get_value)();
	std::wstring valuestring = (editstring == NULL) ? L""   : xui_method_ptrcall(editstring, get_value)();
	f64			 valuenumber = (editnumber == NULL) ? 0     : xui_method_ptrcall(editnumber, get_value)();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (editbool)
		{
			xui_propdata_bool*   data = dynamic_cast<xui_propdata_bool*  >(vec[i]);
			data->set_value(valuebool);
		}
		if (editenum)
		{
			xui_propdata_enum*   data = dynamic_cast<xui_propdata_enum*  >(vec[i]);
			data->set_value(valueenum);
		}
		if (editstring)
		{
			xui_propdata_string* data = dynamic_cast<xui_propdata_string*>(vec[i]);
			data->set_value(valuestring);
		}
		if (editnumber)
		{
			xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(vec[i]);
			data->set_value(valuenumber);
		}
	}
}
xui_method_explain(xui_propctrl_simple, get_propdataall,	xui_propdata_vec	)( u32 index )
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
