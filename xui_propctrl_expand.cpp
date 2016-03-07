#include "xui_convas.h"
#include "xui_drawer.h"
#include "xui_desktop.h"
#include "xui_propview.h"
#include "xui_propctrl_expand.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_expand
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand, xui_propctrl);
/*
//create
*/
xui_method_explain(xui_propctrl_expand,			create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand)( xui_propdata* propdata )
: xui_propctrl()
{
	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl, set_parent	)(this);
	xui_method_ptrcall(m_namectrl, set_textalign)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	//plus
	m_propplus = new xui_plusctrl(PLUSRENDER_NORMAL, true);
	xui_method_ptrcall(m_propplus, set_parent	)(this);
	m_propplus->xm_expand += new xui_method_member<xui_method_args, xui_propctrl_expand>(this, &xui_propctrl_expand::on_propexpand);
	m_widgetvec.push_back(m_propplus);

	//prop
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(propdata);
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

/*
//method
*/
xui_method_explain(xui_propctrl_expand,			was_expanded,		bool			)( void ) const
{
	return m_propplus->was_expanded();
}
xui_method_explain(xui_propctrl_expand,			set_expanded,		void			)( bool flag )
{
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	if (dataexpand->can_subfold())
	{
		m_propplus->set_expanded(flag);
	}
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand,			on_linkpropdata,	void			)( void )
{
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
	xui_method_ptrcall(m_namectrl, set_text		)(m_propdata->get_name());
	xui_method_ptrcall(m_propplus, set_visible	)(dataexpand->can_subfold());

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
xui_method_explain(xui_propctrl_expand,			on_editvalue,		void			)( xui_propedit* sender )
{}

/*
//override
*/
xui_method_explain(xui_propctrl_expand,			on_invalid,			void			)( xui_method_args& args )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		m_propctrlvec[i]->set_visible(false);

	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::default_lineheight + m_border.ay + m_border.by;

	if (m_propdata && m_propplus->was_expanded())
	{
		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			xui_method_ptrcall(propctrl, set_enable )(vec[i]->can_edit());
			xui_method_ptrcall(propctrl, set_visible)(vec[i]->can_show());
			if (vec[i]->can_show() == false)
				continue;

			sz.h += propctrl->get_renderh();
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
xui_method_explain(xui_propctrl_expand,			on_perform,			void			)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::default_lineheight;
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

/*
//event
*/
xui_method_explain(xui_propctrl_expand,			on_propexpand,		void			)( xui_component* sender, xui_method_args& args )
{
	invalid();
}

/*
//method
*/
xui_method_explain(xui_propctrl_expand,			get_propdataall,	xui_propdata_vec)( u32 index )
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
xui_implement_rtti(xui_propctrl_expand_plus, xui_propctrl_expand);
/*
//create
*/
xui_method_explain(xui_propctrl_expand_plus,	create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand_plus(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand_plus)( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{}

/*
//override
*/
xui_method_explain(xui_propctrl_expand_plus,	on_linkpropdata,	void			)( void )
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
	xui_method_ptrcall(m_namectrl, set_text		)(m_propdata->get_name());
	xui_method_ptrcall(m_propplus, set_visible	)(dataexpand->can_subfold());

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

//////////////////////////////////////////////////////////////////////////
//propctrl_expandbool
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_expand_bool, xui_propctrl_expand);
/*
//create
*/
xui_method_explain(xui_propctrl_expand_bool,	create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand_bool(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand_bool)( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propedit_bool* editbool = new xui_propedit_bool(this);

	xui_drawer*  namectrl = editbool->get_namectrl();
	xui_control* textctrl = editbool->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);

	m_propedit = editbool;
	m_namectrl->set_visible(false);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_expand_bool)( void )
{
	delete m_propedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand_bool,	on_linkpropdata,	void			)( void )
{
	xui_propctrl_expand::on_linkpropdata();
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

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
		m_propedit->set_value(value);
	}
}
xui_method_explain(xui_propctrl_expand_bool,	on_editvalue,		void			)( xui_propedit* sender )
{
	bool value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_bool* data = dynamic_cast<xui_propdata_bool*>(m_propdatavec[i]);
		data->set_value(value);
	}

	refresh();
}

/*
//override
*/
xui_method_explain(xui_propctrl_expand_bool,	on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::default_lineheight;

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
xui_implement_rtti(xui_propctrl_expand_enum, xui_propctrl_expand);
/*
//create
*/
xui_method_explain(xui_propctrl_expand_enum,	create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand_enum(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand_enum)( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propdata_enum* dataenum = dynamic_cast<xui_propdata_enum*>(propdata);
	xui_propedit_enum* editenum = new xui_propedit_enum(this, dataenum->get_textmap());

	xui_drawer*  namectrl = editenum->get_namectrl();
	xui_control* textctrl = editenum->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);

	m_propedit = editenum;
	m_namectrl->set_visible(false);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_expand_enum)( void )
{
	delete m_propedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand_enum,	on_linkpropdata,	void			)( void )
{
	xui_propctrl_expand::on_linkpropdata();
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

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
		m_propedit->set_value(value);
	}
}
xui_method_explain(xui_propctrl_expand_enum,	on_editvalue,		void			)( xui_propedit* sender )
{
	u32 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_enum* data = dynamic_cast<xui_propdata_enum*>(m_propdatavec[i]);
		data->set_value(value);
	}

	refresh();
}

/*
//override
*/
xui_method_explain(xui_propctrl_expand_enum,	on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::default_lineheight;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* enumctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//enumctrl
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 - enumctrl->get_renderh()/2;
	enumctrl->on_perform_pt(pt);
	enumctrl->on_perform_w (rt.get_w()/2);
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_y (rt.ay);
	namectrl->on_perform_w (rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}