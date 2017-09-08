#include "ui/UIWidget.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_dropbox.h"
#include "xui_propview.h"
#include "cocos_propnodewidget.h"
#include "cocos_propctrl_callback.h"

//////////////////////////////////////////////////////////////////////////
xui_create_explain(cocos_propdata_callback)( xui_propkind* kind, cocos_propnodewidget* propnode )
: xui_propdata(kind, L"", cocos_propctrl_callback::create)
, m_propnode(propnode)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_callback, get_propnode,			cocos_propnodewidget*	)( void )
{
	return m_propnode;
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_callback, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_callback)( xui_propdata* propdata )
: xui_propctrl()
{
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_namectrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_namectrl, set_parent		)(this);
	xui_method_ptrcall(m_namectrl, ini_drawer		)(L"Callback");
	xui_method_ptrcall(m_namectrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlnonfocus);
	xui_method_ptrcall(m_namectrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlgetfocus);

	m_typectrl = new xui_dropbox(xui_vector<s32>(80, 18), NULL);
	xui_method_ptrcall(m_typectrl, set_parent		)(this);
	xui_method_ptrcall(m_typectrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_typectrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_typectrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_typectrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_typectrl, set_corner		)(3);
	xui_method_ptrcall(m_typectrl, set_readonly		)(true);
	xui_method_ptrcall(m_typectrl, add_item			)(L"None");
	xui_method_ptrcall(m_typectrl, add_item			)(L"Touch");
	xui_method_ptrcall(m_typectrl, add_item			)(L"Click");
	xui_method_ptrcall(m_typectrl, add_item			)(L"Event");
	xui_method_ptrcall(m_typectrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlnonfocus);
	xui_method_ptrcall(m_typectrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlgetfocus);
	xui_method_ptrcall(m_typectrl, xm_selection		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_typectrlselection);

	m_textctrl = new xui_textbox(xui_vector<s32>(64, 18));
	xui_method_ptrcall(m_textctrl, set_parent		)(this);
	xui_method_ptrcall(m_textctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_textctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_textctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_textctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_textctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlnonfocus);
	xui_method_ptrcall(m_textctrl, xm_getfocus		) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_editctrlgetfocus);
	xui_method_ptrcall(m_textctrl, xm_textchanged	) += new xui_method_member<xui_method_args, cocos_propctrl_callback>(this, &cocos_propctrl_callback::on_textctrltextchanged);

	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_typectrl);
	m_widgetvec.push_back(m_textctrl);
}

/*
//create
*/
xui_method_explain(cocos_propctrl_callback, create,					xui_propctrl*			)( xui_propdata* propdata )
{
	return new cocos_propctrl_callback(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_callback, on_linkpropdata,		void					)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		xui_method_ptrcall(m_typectrl, ini_dropbox	)(-1);
		xui_method_ptrcall(m_textctrl, ini_textbox	)(L"");
		xui_method_ptrcall(m_textctrl, set_enable	)(false);
	}

	bool same = true;
	cocos_propdata_callback* datacallback = dynamic_cast<cocos_propdata_callback*>(m_propdata);
	cocos2d::ui::Widget* datawidget = datacallback->get_propnode()->get_widget();
	std::string type = datawidget->getCallbackType();
	std::string name = datawidget->getCallbackName();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_callback* data = dynamic_cast<cocos_propdata_callback*>(m_propdatavec[i]);
		cocos2d::ui::Widget* widget = data->get_propnode()->get_widget();
		if (type != widget->getCallbackType() ||
			name != widget->getCallbackName())
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		u32 selectedindex = 0;
		if		(type == "Touch") selectedindex = 1;
		else if (type == "Click") selectedindex = 2;
		else if (type == "Event") selectedindex = 3;

		xui_method_ptrcall(m_typectrl, ini_dropbox	)(selectedindex);
		xui_method_ptrcall(m_textctrl, ini_textbox	)(xui_global::ascii_to_unicode(name));
		xui_method_ptrcall(m_textctrl, set_enable	)(selectedindex > 0);
	}
}
xui_method_explain(cocos_propctrl_callback, on_editvalue,			void					)( xui_propedit* sender )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propkind* kind = m_propdatavec[i]->get_kind();
		if (kind)
		{
			xui_method_propdata args;
			args.propdata = m_propdatavec[i];
			kind->xm_propchanged(this, args);
		}
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_callback, on_perform,				void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	//typectrl
	pt.x = rt.get_w()/2;
	pt.y = xui_propview::default_lineheight/2 - m_typectrl->get_renderh()/2;
	m_typectrl->on_perform_pt(pt);
	//textctrl
	pt.x = pt.x + m_typectrl->get_renderw() + 4;
	m_textctrl->on_perform_pt(pt);
	m_textctrl->on_perform_w (rt.get_w() - pt.x);
	//namectrl
	s32 indent = get_indent();
	m_namectrl->on_perform_w (rt.get_w()/2);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

/*
//event
*/
xui_method_explain(cocos_propctrl_callback, on_editctrlnonfocus,	void					)( xui_component* sender, xui_method_args& args )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 0.8f));
	m_typectrl->set_sidecolor(xui_control::default_sidecolor);
	m_textctrl->set_sidecolor(xui_control::default_sidecolor);
}
xui_method_explain(cocos_propctrl_callback, on_editctrlgetfocus,	void					)( xui_component* sender, xui_method_args& args )
{
	m_namectrl->set_textcolor(xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	m_typectrl->set_sidecolor(xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	m_textctrl->set_sidecolor(xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}
xui_method_explain(cocos_propctrl_callback, on_typectrlselection,	void					)( xui_component* sender, xui_method_args& args )
{
	std::string type = xui_global::unicode_to_ascii(m_typectrl->get_text());

	m_textctrl->set_enable(m_typectrl->get_selectedindex() > 0);
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_callback* data = dynamic_cast<cocos_propdata_callback*>(m_propdatavec[i]);
		cocos2d::ui::Widget* widget = data->get_propnode()->get_widget();
		widget->setCallbackType(type);
	}

	on_editvalue(NULL);
}
xui_method_explain(cocos_propctrl_callback, on_textctrltextchanged, void					)( xui_component* sender, xui_method_args& args )
{
	std::string name = xui_global::unicode_to_ascii(m_textctrl->get_text());
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_callback* data = dynamic_cast<cocos_propdata_callback*>(m_propdatavec[i]);
		cocos2d::ui::Widget* widget = data->get_propnode()->get_widget();
		widget->setCallbackName(name);
	}

	on_editvalue(NULL);
}