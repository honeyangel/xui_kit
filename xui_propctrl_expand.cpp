#include "xui_canvas.h"
#include "xui_drawer.h"
#include "xui_numbbox.h"
#include "xui_desktop.h"
#include "xui_propview.h"
#include "xui_propctrl_expand.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_expand
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand, xui_propctrl)

xui_propctrl* xui_propctrl_expand::create( xui_propdata* propdata )
{
	return new xui_propctrl_expand(propdata);
}

xui_propctrl_expand::xui_propctrl_expand( xui_propdata* propdata )
: xui_propctrl()
{
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(propdata);

	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	m_namectrl->set_parent(this);
	m_namectrl->set_textalign(k_textalign_lc);
	m_widgetvec.push_back(m_namectrl);

	//plus
	m_propplus = new xui_plusctrl(k_plusrender_normal, (dataexpand == NULL || dataexpand->can_subfold() == false));
	m_propplus->set_parent(this);
	m_propplus->xm_expand += new xui_method_member<xui_method_args, xui_propctrl_expand>(this, &xui_propctrl_expand::on_propexpand);
	m_widgetvec.push_back(m_propplus);

	//prop
	if (dataexpand)
	{
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_prop_newctrl  func = vec[i]->get_func();
			xui_propctrl* propctrl = (*func)(vec[i]);
			propctrl->refresh();
			propctrl->set_parent(this);
			m_widgetvec.push_back(propctrl);
			m_propctrlvec.push_back(propctrl);
		}
	}
}

bool xui_propctrl_expand::was_expanded( void ) const
{
	return m_propplus->was_expanded();
}

void xui_propctrl_expand::set_expanded( bool flag )
{
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	if (dataexpand->can_subfold())
	{
		m_propplus->set_expanded(flag);
	}
}

void xui_propctrl_expand::on_linkpropdata( bool selfupdate )
{
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	if (selfupdate == false)
	{
		m_namectrl->set_text(m_propdata->get_name());

		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propdata_vec propdataall = get_propdataall(i);
			xui_propctrl* propctrl = m_propctrlvec[i];
			propctrl->set_propdata(propdataall);

			for (xui_propdata_vec::iterator itor = propdataall.begin(); itor != propdataall.end(); ++itor)
			{
				(*itor)->set_ctrl(propctrl);
			}
		}
	}

	bool plusvisible = false;
	const xui_propdata_vec& vec = dataexpand->get_subprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (vec[i]->can_show() && dataexpand->can_subfold())
		{
			plusvisible = true;
			break;
		}
	}
	m_propplus->set_visible(plusvisible);
}

void xui_propctrl_expand::on_editvalue( xui_propedit* sender )
{}

void xui_propctrl_expand::on_invalid( xui_method_args& args )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		m_propctrlvec[i]->set_visible(false);

	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::k_default_lineheight + m_border.ay + m_border.by;

	if (m_propdata)
	{
		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		bool plusvisible = false;
		for (u32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i]->can_show())
			{
				plusvisible = true;
				break;
			}
		}
		m_propplus->set_visible(plusvisible);

		if (m_propplus->was_expanded())
		{
			for (u32 i = 0; i < m_propctrlvec.size(); ++i)
			{
				xui_propctrl* propctrl = m_propctrlvec[i];
				propctrl->set_enable (vec[i]->can_edit());
				propctrl->set_visible(vec[i]->can_show());
				if (vec[i]->can_show() == false)
					continue;

				sz.h += propctrl->get_renderh();
			}
		}
	}

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

void xui_propctrl_expand::on_perform( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;
	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//name
	m_namectrl->on_perform_y (rt.ay);
	m_namectrl->on_perform_w (rt.get_w());
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//plus
	pt.x = (indent-m_propplus->get_renderw());
	pt.y = rt.ay + (height-m_propplus->get_renderh())/2;
	m_propplus->on_perform_pt(pt);
	if (m_propplus->was_expanded())
	{
		pt.x = 0;
		pt.y = rt.ay + height;
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			if (propctrl->was_visible() == false)
				continue;

			propctrl->on_perform_pt(pt);
			propctrl->on_perform_w (rt.get_w());
			pt.y += propctrl->get_renderh();
		}
	}
}

void xui_propctrl_expand::on_propexpand( xui_component* sender, xui_method_args& args )
{
	invalid();
}

xui_propdata_vec xui_propctrl_expand::get_propdataall( u32 index )
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

//////////////////////////////////////////////////////////////////////////
//propctrl_expandplus
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand_plus, xui_propctrl_expand)

xui_propctrl* xui_propctrl_expand_plus::create( xui_propdata* propdata )
{
	return new xui_propctrl_expand_plus();
}

xui_propctrl_expand_plus::xui_propctrl_expand_plus( void )
: xui_propctrl_expand(NULL)
{}

void xui_propctrl_expand_plus::on_linkpropdata( bool selfupdate )
{
	if (selfupdate == false)
	{
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			std::vector<xui_component*>::iterator itor = std::find(m_widgetvec.begin(), m_widgetvec.end(), m_propctrlvec[i]);
			if (itor != m_widgetvec.end())
			{
				m_widgetvec.erase(itor);
				m_propctrlvec[i]->set_parent(NULL);
				xui_desktop::get_ins()->move_recycle(m_propctrlvec[i]);
			}
		}
		m_propctrlvec.clear();

		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
		if (dataexpand)
		{
			m_namectrl->set_text(m_propdata->get_name());
			m_propplus->set_visible(dataexpand->can_subfold());
		}

		if (m_propdatavec.size() == 1)
		{
			//propctrl
			const xui_propdata_vec& vec = dataexpand->get_subprop();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				xui_prop_newctrl  func = vec[i]->get_func();
				xui_propctrl* propctrl = (*func)(vec[i]);
				propctrl->refresh();
				propctrl->set_parent(this);
				m_widgetvec.push_back(propctrl);
				m_propctrlvec.push_back(propctrl);
			}

			//propdata
			for (u32 i = 0; i < m_propctrlvec.size(); ++i)
			{
				xui_propdata_vec propdataall = get_propdataall(i);
				xui_propctrl* propctrl = m_propctrlvec[i];
				propctrl->set_propdata(propdataall);

				for (xui_propdata_vec::iterator itor = propdataall.begin(); itor != propdataall.end(); ++itor)
				{
					(*itor)->set_ctrl(propctrl);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//propctrl_expandstring
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand_number, xui_propctrl_expand)

xui_propctrl* xui_propctrl_expand_number::create( xui_propdata* propdata )
{
	return new xui_propctrl_expand_number(propdata);
}

xui_propctrl_expand_number::xui_propctrl_expand_number( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(propdata);
	xui_propedit_number* editnumber = new xui_propedit_number(this, datanumber->get_numbtype(), datanumber->get_interval(), datanumber->get_numbtype() != k_nt_float);

	xui_drawer*  namectrl = editnumber->get_namectrl();
	xui_control* textctrl = editnumber->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.insert(m_widgetvec.begin(), namectrl);
	m_widgetvec.insert(m_widgetvec.begin(), textctrl);

	m_propedit = editnumber;
	m_namectrl->set_visible(false);
}

xui_propctrl_expand_number::~xui_propctrl_expand_number( void )
{
	delete m_propedit;
}

void xui_propctrl_expand_number::on_linkpropdata( bool selfupdate )
{
	xui_propctrl_expand::on_linkpropdata(selfupdate);
	if (selfupdate == false)
	{
		m_propedit->reset();
		xui_drawer* namectrl = m_propedit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());
	}

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
		m_propedit->set_value(value);
	}
}

void xui_propctrl_expand_number::on_editvalue( xui_propedit* sender )
{
	f64 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(m_propdatavec[i]);
		data->set_value(value);
	}

	refresh();
}

void xui_propctrl_expand_number::on_perform( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* textctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//textctrl
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w() - pt.x);
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_y (rt.ay);
	namectrl->on_perform_w (rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

//////////////////////////////////////////////////////////////////////////
//propctrl_expandbool
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand_bool, xui_propctrl_expand)

xui_propctrl* xui_propctrl_expand_bool::create( xui_propdata* propdata )
{
	return new xui_propctrl_expand_bool(propdata);
}

xui_propctrl_expand_bool::xui_propctrl_expand_bool( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propedit_bool* editbool = new xui_propedit_bool(this);

	xui_drawer*  namectrl = editbool->get_namectrl();
	xui_control* textctrl = editbool->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.insert(m_widgetvec.begin(), namectrl);
	m_widgetvec.insert(m_widgetvec.begin(), textctrl);

	m_propedit = editbool;
	m_namectrl->set_visible(false);
}

xui_propctrl_expand_bool::~xui_propctrl_expand_bool( void )
{
	delete m_propedit;
}

void xui_propctrl_expand_bool::on_linkpropdata( bool selfupdate )
{
	xui_propctrl_expand::on_linkpropdata(selfupdate);
	if (selfupdate == false)
	{
		m_propedit->reset();
		xui_drawer* namectrl = m_propedit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	xui_propdata_bool* databool = dynamic_cast<xui_propdata_bool*>(m_propdata);
	bool value = databool->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_bool* data = dynamic_cast<xui_propdata_bool*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		if (selfupdate == false || m_propedit->get_value() != value)
		{
			xui_expandvary* dataexpand = dynamic_cast<xui_expandvary*>(m_propdata);
			dataexpand->syn_subprop();
			refresh();
		}

		m_propedit->set_value(value);
	}
}

void xui_propctrl_expand_bool::on_editvalue( xui_propedit* sender )
{
	bool value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_bool* data = dynamic_cast<xui_propdata_bool*>(m_propdatavec[i]);
		data->set_value(value);
	}

	refresh();
}

void xui_propctrl_expand_bool::on_perform( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* boolctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//boolctrl
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 - boolctrl->get_renderh()/2;
	boolctrl->on_perform_pt(pt);
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_y (rt.ay);
	namectrl->on_perform_w (rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

//////////////////////////////////////////////////////////////////////////
//propctrl_expandenum
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand_enum, xui_propctrl_expand)

xui_propctrl* xui_propctrl_expand_enum::create( xui_propdata* propdata )
{
	return new xui_propctrl_expand_enum(propdata);
}

xui_propctrl_expand_enum::xui_propctrl_expand_enum( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propdata_enum* dataenum = dynamic_cast<xui_propdata_enum*>(propdata);
	xui_propedit_enum* editenum = new xui_propedit_enum(this, dataenum->get_textmap());

	xui_drawer*  namectrl = editenum->get_namectrl();
	xui_control* textctrl = editenum->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.insert(m_widgetvec.begin(), namectrl);
	m_widgetvec.insert(m_widgetvec.begin(), textctrl);

	m_propedit = editenum;
	m_namectrl->set_visible(false);
}

xui_propctrl_expand_enum::~xui_propctrl_expand_enum( void )
{
	delete m_propedit;
}

void xui_propctrl_expand_enum::on_linkpropdata( bool selfupdate )
{
	xui_propctrl_expand::on_linkpropdata(selfupdate);
	if (selfupdate == false)
	{
		m_propedit->reset();
		xui_drawer* namectrl = m_propedit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	xui_propdata_enum* dataenum = dynamic_cast<xui_propdata_enum*>(m_propdata);
	u32 value = dataenum->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_enum* data = dynamic_cast<xui_propdata_enum*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		if (selfupdate == false || m_propedit->get_value() != value)
		{
			xui_expandvary* dataexpand = dynamic_cast<xui_expandvary*>(m_propdata);
			dataexpand->syn_subprop();
			refresh();
		}

		m_propedit->set_value(value);
	}
}

void xui_propctrl_expand_enum::on_editvalue( xui_propedit* sender )
{
	u32 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_enum* data = dynamic_cast<xui_propdata_enum*>(m_propdatavec[i]);
		data->set_value(value);
	}

	refresh();
}

void xui_propctrl_expand_enum::on_perform( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* enumctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//enumctrl
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 - enumctrl->get_renderh()/2;
	enumctrl->on_perform_pt(pt);
	enumctrl->on_perform_w (rt.get_w() - pt.x);
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_y (rt.ay);
	namectrl->on_perform_w (rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}