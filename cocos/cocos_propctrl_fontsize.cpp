#include "xui_global.h"
#include "xui_dropbox.h"
#include "xui_drawer.h"
#include "cocos_propctrl_fontsize.h"

xui_implement_rtti(cocos_propctrl_fontsize, xui_propctrl_base);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_fontsize)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propenum_map textmap;
	for (s32 i = 0; i < 20; ++i)
	{
		std::wstringstream text;
		text << (i*2+12);
		textmap[i] = text.str();
	}

	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(propdata);
	m_propedit = new xui_propedit_enum(this, textmap);
	xui_drawer*  namectrl = m_propedit->get_namectrl();								
	xui_control* textctrl = m_propedit->get_editctrl();								
	xui_method_ptrcall(namectrl, set_parent)(this);													
	xui_method_ptrcall(textctrl, set_parent)(this);													
	m_widgetvec.push_back(namectrl);											
	m_widgetvec.push_back(textctrl);											
}

/*
//create
*/
xui_method_explain(cocos_propctrl_fontsize, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new cocos_propctrl_fontsize(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_fontsize, on_linkpropdata,	void			)( bool selfupdate /* = false */ )
{
	xui_propctrl_base::on_linkpropdata(selfupdate);						

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
		xui_propedit_enum* edit = (xui_propedit_enum*)m_propedit;							
		edit->set_value(((s32)value-12) / 2);											
	}																
}
xui_method_explain(cocos_propctrl_fontsize, on_editvalue,		void			)( xui_propedit* sender )
{
	xui_propedit_enum* edit = (xui_propedit_enum*)m_propedit;							
	u32 value = edit->get_value();								
	for (u32 i = 0; i < m_propdatavec.size(); ++i)					
	{																
		xui_propdata_number* data = dynamic_cast<xui_propdata_number*>(m_propdatavec[i]);					
		data->set_value(value*2+12);											
	}																
}

/*
//override
*/
xui_method_explain(cocos_propctrl_fontsize, on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* enumctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - enumctrl->get_renderh()/2;
	enumctrl->on_perform_pt(pt);
	enumctrl->on_perform_w(rt.get_w() - pt.x);
}