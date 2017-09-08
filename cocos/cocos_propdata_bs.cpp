#include "xui_convas.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_numbbox.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_propview.h"
#include "cocos_propdata_bs.h"

//////////////////////////////////////////////////////////////////////////
xui_create_explain(cocos_propdata_bs)( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval )
: xui_propdata(kind, name, cocos_propctrl_bs::create)
, m_userget(userget)
, m_userset(userset)
, m_userptr(userptr)
, m_numbtype(numbtype)
, m_interval(interval)
{}
xui_method_explain(cocos_propdata_bs, get_numbtype,		u08				)( void ) const
{
	return m_numbtype;
}
xui_method_explain(cocos_propdata_bs, get_interval,		f64				)( void ) const
{
	return m_interval;
}
xui_method_explain(cocos_propdata_bs, get_value,		cocos_value_bs	)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(cocos_propdata_bs, set_value,		void			)( const cocos_value_bs& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_bs, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_bs)( xui_propdata* propdata )
: xui_propctrl()
{
	cocos_propdata_bs* databs = (cocos_propdata_bs*)propdata;
	xui_drawer*  namectrl = NULL;
	xui_control* editctrl = NULL;

	m_booledit = new xui_propedit_bool(this);
	namectrl = m_booledit->get_namectrl();
	editctrl = m_booledit->get_editctrl();
	xui_method_ptrcall(namectrl, set_parent		)(this);
	xui_method_ptrcall(editctrl, set_parent		)(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(editctrl);

	m_textedit = new xui_propedit_number(this, databs->get_numbtype(), databs->get_interval(), false);
	namectrl = m_textedit->get_namectrl();
	editctrl = m_textedit->get_editctrl();
	xui_method_ptrcall(namectrl, set_parent		)(this);
	xui_method_ptrcall(editctrl, set_parent		)(this);
	xui_method_ptrcall(namectrl, ini_component	)(true, false);
	xui_method_ptrcall(editctrl, ini_component	)(0, ALIGNVERT_C, 0);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(editctrl);
}

/*
//destructor
*/
xui_delete_explain(cocos_propctrl_bs)( void )
{
	delete m_booledit;
	delete m_textedit;
}

/*
//create
*/
xui_method_explain(cocos_propctrl_bs, create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new cocos_propctrl_bs(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_bs, on_linkpropdata,	void			)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		m_booledit->reset();
		m_textedit->reset();
		xui_drawer* namectrl = m_booledit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;															
	cocos_propdata_bs* databs = (cocos_propdata_bs*)m_propdata;										
	cocos_value_bs value = databs->get_value();											
	for (u32 i = 0; i < m_propdatavec.size(); ++i)								
	{																			
		cocos_propdata_bs* data = (cocos_propdata_bs*)m_propdatavec[i];							
		if (data->get_value() != value)											
		{																		
			same = false;														
			break;																
		}																		
	}																			
		
	if (same)																	
	{				
		m_booledit->set_value(value.flag);
		xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_textedit->get_editctrl());
		xui_method_ptrcall(textctrl, set_enable	)(value.flag);
		xui_method_ptrcall(textctrl, ini_textbox)(value.text);
	}																			
}
xui_method_explain(cocos_propctrl_bs, on_editvalue,		void			)( xui_propedit* sender )
{
	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, m_textedit->get_editctrl());
	textctrl->set_enable(m_booledit->get_value());

	cocos_value_bs value;
	value.flag = m_booledit->get_value();
	value.text = textctrl->get_text();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_bs* data = (cocos_propdata_bs*)m_propdatavec[i];
		data->set_value(value);
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_bs, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_drawer*  namectrl = m_booledit->get_namectrl();
	xui_control* boolctrl = m_booledit->get_editctrl();
	xui_control* textctrl = m_textedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	//boolctrl
	pt.x = rt.get_w() / 2;
	pt.y = rt.get_h() / 2 - boolctrl->get_renderh() / 2;
	boolctrl->on_perform_pt(pt);
	//textctrl
	pt.x = pt.x + boolctrl->get_renderw() + 4;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w() - pt.x);
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_w (rt.get_w() / 2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}