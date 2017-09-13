#include "xui_drawer.h"
#include "xui_toggle.h"
#include "xui_propview.h"
#include "cocos_propdata_unitvec.h"

//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propdata_unitvec)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, get_func userdef, void* userptr )
: xui_propdata(kind, name, func)
, m_userget(userget)
, m_userset(userset)
, m_userdef(userdef)
, m_userptr(userptr)
{
	set_edit(m_userset != NULL);
}

/*
//method
*/
xui_method_explain(cocos_propdata_unitvec,		def_exist,			bool				)( void )
{
	return m_userdef != NULL;
}
xui_method_explain(cocos_propdata_unitvec,		get_value,			cocos_value_unitvec	)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(cocos_propdata_unitvec,		set_value,			void				)( const cocos_value_unitvec& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}
xui_method_explain(cocos_propdata_unitvec,		def_value,			void				)( void )
{
	if (m_userdef)
	{
		cocos_value_unitvec value = (*m_userdef)(m_userptr);
		set_value(value);
	}
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_unitvec, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_unitvec)( xui_propdata* propdata )
: xui_propctrl()
{
	cocos_propdata_unitvec* dataunitvec = dynamic_cast<cocos_propdata_unitvec*>(propdata);
	m_subxedit = new cocos_propedit_unit(this);
	m_subyedit = new cocos_propedit_unit(this);
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_toggle*	 subxmenu = m_subxedit->get_menuctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_toggle*  subymenu = m_subyedit->get_menuctrl();
	xui_method_ptrcall(subxname,	set_parent		)(this);
	xui_method_ptrcall(subyname,	set_parent		)(this);
	xui_method_ptrcall(subxedit,	set_parent		)(this);
	xui_method_ptrcall(subyedit,	set_parent		)(this);
	xui_method_ptrcall(subxmenu,	set_parent		)(this);
	xui_method_ptrcall(subymenu,	set_parent		)(this);
	m_widgetvec.push_back(subxname);
	m_widgetvec.push_back(subyname);
	m_widgetvec.push_back(subxedit);
	m_widgetvec.push_back(subyedit);
	m_widgetvec.push_back(subxmenu);
	m_widgetvec.push_back(subymenu);
}

/*
//destructor
*/
xui_delete_explain(cocos_propctrl_unitvec)( void )
{
	delete m_subxedit;
	delete m_subyedit;
}

/*
//create
*/
//xui_method_explain(cocos_propctrl_unitvec, create,			xui_propctrl*		)( xui_propdata* propdata )
//{
//	return new cocos_propctrl_unitvec(propdata);
//}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_unitvec,		on_linkpropdata,	void				)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		m_subxedit->reset();
		m_subyedit->reset();
		m_namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	cocos_propdata_unitvec* dataunitvec = dynamic_cast<cocos_propdata_unitvec*>(m_propdata);
	cocos_value_unitvec value = dataunitvec->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_unitvec* data = dynamic_cast<cocos_propdata_unitvec*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_subxedit->set_value(value.x);
		m_subyedit->set_value(value.y);
	}
}
xui_method_explain(cocos_propctrl_unitvec,		on_editvalue,		void				)( xui_propedit* sender )
{
	if (sender == m_subxedit)
	{
		cocos_value_unit x = m_subxedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			cocos_propdata_unitvec* data = dynamic_cast<cocos_propdata_unitvec*>(m_propdatavec[i]);
			cocos_value_unitvec value = data->get_value();
			value.x = x;
			data->set_value(value);
		}

		//if (m_subxedit->get_editctrl()->has_focus() == false)
		//	on_linkpropdata();
	}
	if (sender == m_subyedit)
	{
		cocos_value_unit y = m_subyedit->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			cocos_propdata_unitvec* data = dynamic_cast<cocos_propdata_unitvec*>(m_propdatavec[i]);
			cocos_value_unitvec value = data->get_value();
			value.y = y;
			data->set_value(value);
		}

		//if (m_subyedit->get_editctrl()->has_focus() == false)
		//	on_linkpropdata();
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_unitvec,		on_perform,			void				)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_toggle*	 subxmenu = m_subxedit->get_menuctrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_toggle*  subymenu = m_subyedit->get_menuctrl();
	s32 namewidth = 16;//xui_max(subxname->get_renderw(), subyname->get_renderw());
	s32 editwidth = (rt.get_w() - rt.get_w()/2 - 12 - 2*namewidth) / 2;
	//subxname
	pt.x = rt.get_w() / 2;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	subxname->on_perform_pt(pt);
	subxname->on_perform_sz(sz);
	//subxedit
	pt.x = pt.x + subxname->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - subxedit->get_renderh()/2;
	sz.w = editwidth;
	sz.h = subxedit->get_renderh();
	subxedit->on_perform_pt(pt);
	subxedit->on_perform_sz(sz);
	//subyname
	pt.x = pt.x + subxedit->get_renderw() + 4;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	subyname->on_perform_pt(pt);
	subyname->on_perform_sz(sz);
	//subyedit
	pt.x = pt.x + subyname->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - subyedit->get_renderh()/2;
	sz.w = rt.get_w() - pt.x;
	sz.h = subyedit->get_renderh();
	subyedit->on_perform_pt(pt);
	subyedit->on_perform_sz(sz);
	//subxmenu
	pt.x = subxedit->get_renderx() + subxedit->get_renderw() - subxmenu->get_renderw();
	sz.w = subxmenu->get_renderw();
	subxmenu->on_perform_pt(pt);
	subxmenu->on_perform_sz(sz);
	//subymenu
	pt.x = subyedit->get_renderx() + subyedit->get_renderw() - subymenu->get_renderw();
	sz.w = subymenu->get_renderw();
	subymenu->on_perform_pt(pt);
	subymenu->on_perform_sz(sz);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w(rt.get_w() / 2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_unitvecpt, cocos_propctrl_unitvec);
/*
//constructor
*/
xui_create_explain(cocos_propctrl_unitvecpt)( xui_propdata* propdata )
: cocos_propctrl_unitvec(propdata)
{
	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	subxname->set_text(L"X");
	subyname->set_text(L"Y");
}
/*
//create
*/
xui_method_explain(cocos_propctrl_unitvecpt,	create,				xui_propctrl*		)( xui_propdata* propdata )
{
	return new cocos_propctrl_unitvecpt(propdata);
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_unitvecsz, cocos_propctrl_unitvec);
/*
//constructor
*/
xui_create_explain(cocos_propctrl_unitvecsz)( xui_propdata* propdata )
: cocos_propctrl_unitvec(propdata)
{
	cocos_propdata_unitvec* dataunitvec = dynamic_cast<cocos_propdata_unitvec*>(propdata);
	bool showbutton = dataunitvec->def_exist();

	xui_drawer*  subxname = m_subxedit->get_namectrl();
	xui_drawer*  subyname = m_subyedit->get_namectrl();
	subxname->set_text(L"W");
	subyname->set_text(L"H");

	m_defvalue = new xui_button(xui_vector<s32>(100, 18));
	xui_method_ptrcall(m_defvalue, set_parent		)(this);
	xui_method_ptrcall(m_defvalue, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_defvalue, set_drawcolor	)(true);
	xui_method_ptrcall(m_defvalue, set_corner		)(3);
	xui_method_ptrcall(m_defvalue, set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_defvalue, ini_component	)(true, showbutton);
	xui_method_ptrcall(m_defvalue, ini_drawer		)(L"Default Size");
	xui_method_ptrcall(m_defvalue, xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_propctrl_unitvecsz>(this, &cocos_propctrl_unitvecsz::on_defvalueclick);
	m_widgetvec.push_back(m_defvalue);
}
/*
//create
*/
xui_method_explain(cocos_propctrl_unitvecsz,	create,				xui_propctrl*		)( xui_propdata* propdata )
{
	return new cocos_propctrl_unitvecsz(propdata);
}

/*
//override
*/
xui_method_explain(cocos_propctrl_unitvecsz,	on_invalid,			void				)( xui_method_args& args )
{
	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::default_lineheight * (m_defvalue->was_visible()?2:1) + m_border.ay + m_border.by;

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(cocos_propctrl_unitvecsz,	on_perform,			void				)( xui_method_args& args )
{
	cocos_propctrl_unitvec::on_perform(args);

	if (m_defvalue->was_visible())
	{
		xui_rect2d<s32> rt = get_renderrt();
		xui_vector<s32> pt;
		//defvalue
		pt.x = rt.get_w()/2 + 20;
		pt.y = xui_propview::default_lineheight + (xui_propview::default_lineheight - m_defvalue->get_renderh()) / 2;
		m_defvalue->on_perform_pt(pt);
	}
}

/*
//event
*/
xui_method_explain(cocos_propctrl_unitvecsz,	on_defvalueclick,	void				)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_unitvec* data = dynamic_cast<cocos_propdata_unitvec*>(m_propdatavec[i]);
		data->def_value();
	}

	on_linkpropdata();
}