#include "NP2DSTransRef.h"

#include "xui_global.h"
#include "xui_drawer.h"
#include "xui_propview.h"
#include "onity_propctrl_sceneparam.h"

xui_implement_rtti(onity_propctrl_sceneparam, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_sceneparam)( void )
: xui_propctrl()
{
	m_nameedit = new xui_propedit_string(this);
	m_textedit = new xui_propedit_string(this);
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl,	set_parent		)(this);
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	xui_drawer*  namename = m_nameedit->get_namectrl();
	xui_control* nameedit = m_nameedit->get_editctrl();
	xui_drawer*  textname = m_textedit->get_namectrl();
	xui_control* textedit = m_textedit->get_editctrl();
	xui_method_ptrcall(namename,	set_text		)(L"Name");
	xui_method_ptrcall(textname,	set_text		)(L"Text");
	xui_method_ptrcall(namename,	set_parent		)(this);
	xui_method_ptrcall(textname,	set_parent		)(this);
	xui_method_ptrcall(nameedit,	set_parent		)(this);
	xui_method_ptrcall(textedit,	set_parent		)(this);
	m_widgetvec.push_back(namename);
	m_widgetvec.push_back(textname);
	m_widgetvec.push_back(nameedit);
	m_widgetvec.push_back(textedit);
}

/*
//destructor
*/
xui_delete_explain(onity_propctrl_sceneparam)( void )
{
	delete m_nameedit;
	delete m_textedit;
}

/*
//create
*/
xui_method_explain(onity_propctrl_sceneparam, create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_sceneparam;
}

/*
//propdata
*/
xui_method_explain(onity_propctrl_sceneparam, on_linkpropdata,	void			)( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_nameedit->reset();
		m_textedit->reset();
		m_namectrl->set_text(m_propdata->get_name());
	}

	bool samename = true;
	bool sametext = true;
	onity_propdata_sceneparam* dataparam = dynamic_cast<onity_propdata_sceneparam*>(m_propdata);
	std::string name = dataparam->get_param()->name;
	std::string text = dataparam->get_param()->text;
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		onity_propdata_sceneparam* data = dynamic_cast<onity_propdata_sceneparam*>(m_propdatavec[i]);
		if (data->get_param()->name != name)
			samename = false;
		if (data->get_param()->text != text)
			sametext = false;
	}

	if (samename) m_nameedit->set_value(xui_global::ascii_to_unicode(name));
	if (sametext) m_textedit->set_value(xui_global::ascii_to_unicode(text));
}
xui_method_explain(onity_propctrl_sceneparam, on_editvalue,		void			)( xui_propedit* sender )
{
	if (sender == m_nameedit)
	{
		std::string name = xui_global::unicode_to_ascii(m_nameedit->get_value());
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			onity_propdata_sceneparam* data = dynamic_cast<onity_propdata_sceneparam*>(m_propdatavec[i]);
			data->get_param()->name = name;
		}
	}
	if (sender == m_textedit)
	{
		std::string text = xui_global::unicode_to_ascii(m_textedit->get_value());
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			onity_propdata_sceneparam* data = dynamic_cast<onity_propdata_sceneparam*>(m_propdatavec[i]);
			data->get_param()->text = text;
		}
	}
}

/*
//override
*/
xui_method_explain(onity_propctrl_sceneparam, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	xui_drawer*  namename = m_nameedit->get_namectrl();
	xui_control* nameedit = m_nameedit->get_editctrl();
	xui_drawer*  textname = m_textedit->get_namectrl();
	xui_control* textedit = m_textedit->get_editctrl();
	s32 namewidth = xui_max(namename->get_renderw(), textname->get_renderw());
	s32 editwidth = (rt.get_w() - rt.get_w()/2 - 12 - 2*namewidth) / 2;
	//subxname
	pt.x = rt.get_w()/2;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	namename->on_perform_pt(pt);
	namename->on_perform_sz(sz);
	//subxedit
	pt.x = pt.x + namename->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - nameedit->get_renderh()/2;
	sz.w = editwidth;
	sz.h = nameedit->get_renderh();
	nameedit->on_perform_pt(pt);
	nameedit->on_perform_sz(sz);
	//subyname
	pt.x = pt.x + nameedit->get_renderw() + 4;
	pt.y = 0;
	sz.w = namewidth;
	sz.h = xui_propview::default_lineheight;
	textname->on_perform_pt(pt);
	textname->on_perform_sz(sz);
	//subyedit
	pt.x = pt.x + textname->get_renderw() + 4;
	pt.y = xui_propview::default_lineheight/2 - textedit->get_renderh()/2;
	sz.w = rt.get_w() - pt.x;
	sz.h = textedit->get_renderh();
	textedit->on_perform_pt(pt);
	textedit->on_perform_sz(sz);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w(rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}