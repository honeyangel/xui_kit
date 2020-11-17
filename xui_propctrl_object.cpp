#include "xui_global.h"
#include "xui_canvas.h"
#include "xui_button.h"
#include "xui_desktop.h"
#include "xui_propview.h"
#include "xui_propctrl_object.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_object
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_object, xui_propctrl)

xui_propctrl* xui_propctrl_object::create( xui_propdata* propdata )
{
	return new xui_propctrl_object(propdata);
}

xui_propctrl_object::xui_propctrl_object( xui_propdata* propdata )
: xui_propctrl()
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	xui_prop_geticon iconfunc = dataobject->get_iconfunc();
	xui_bitmap* icon = (iconfunc == NULL) ? NULL : iconfunc(propdata);

	xui_propedit_object* editobject = new xui_propedit_object(this, icon);
	xui_drawer*  namectrl = editobject->get_namectrl();
	xui_control* textctrl = editobject->get_editctrl();
	xui_drawer*  pickctrl = editobject->get_pickctrl();
	textctrl->xm_keybddown			+= new xui_method_member<xui_method_keybd,	  xui_propctrl_object>(this, &xui_propctrl_object::on_textctrlkeybddown);
	textctrl->xm_mousedoubleclick	+= new xui_method_member<xui_method_mouse,	  xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldoubleclick);
	textctrl->xm_mousedragenter		+= new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragenter);
	textctrl->xm_mousedragleave		+= new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragleave);
	textctrl->xm_mousedragover		+= new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragover);
	textctrl->xm_mousedragdrop		+= new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragdrop);
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	pickctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);
	m_widgetvec.push_back(pickctrl);

	m_propedit = editobject;
}

xui_propctrl_object::~xui_propctrl_object( void )
{
	delete m_propedit;
}

void xui_propctrl_object::on_linkpropdata( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_propedit->reset();
		xui_drawer* namectrl = m_propedit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());
	}

	bool same = true;
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	void* value = dataobject->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	xui_drawer* textctrl = xui_dynamic_cast(xui_drawer, m_propedit->get_editctrl());
	if (same)
	{
		if (value)
		{
			xui_prop_getname namefunc = dataobject->get_namefunc();
			if (namefunc)
				textctrl->ini_drawer(namefunc(dataobject));
			xui_prop_geticon iconfunc = dataobject->get_iconfunc();
			if (iconfunc)
				textctrl->ini_drawer(iconfunc(dataobject), xui_vector<s32>(16));
		}
		else
		{
			std::wstring text;
			text += L"None";
			textctrl->set_text(text);
		}
	}
	else
	{
		textctrl->set_text(L"Multi-Value");
	}
}

void xui_propctrl_object::on_editvalue( xui_propedit* sender )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	xui_prop_newpick pickfunc = dataobject->get_pickfunc();
	if (pickfunc)
	{
		xui_object_pickwnd* wnd = pickfunc();
		wnd->set_propctrl(this);
		wnd->set_visible(true);
		wnd->set_value(dataobject->get_value());
	}
}

void xui_propctrl_object::on_perform( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	xui_propedit_object* editobject = (xui_propedit_object*)m_propedit;
	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* textctrl = m_propedit->get_editctrl();
	xui_drawer*  pickctrl = m_propedit->get_pickctrl();
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	//pickctrl
	pt.x = rt.bx - pickctrl->get_renderw();
	pt.y = xui_propview::k_default_lineheight/2 - pickctrl->get_renderh()/2;
	pickctrl->on_perform_pt(pt);
	//textctrl
	pt.x = rt.get_w()/2;
	pt.y = xui_propview::k_default_lineheight/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w() - pt.x - pickctrl->get_renderw());
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_w (rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

void xui_propctrl_object::on_textctrlkeybddown( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == k_key_delete)
	{
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			data->set_value(NULL);
			data->syn_value();
		}

		on_linkpropdata();
	}
}

void xui_propctrl_object::on_textctrldoubleclick( xui_component* sender, xui_method_mouse& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	void* value = dataobject->get_value();

	bool same = (value != NULL);
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		xui_method_args other_args;
		dataobject->xm_doubleclick(sender, other_args);
	}
}

void xui_propctrl_object::on_textctrldragenter( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		on_readyundo();

		xui_control* textctrl = m_propedit->get_editctrl();
		textctrl->set_backcolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			data->set_value(args.data);
		}

		on_linkpropdata();
	}
}

void xui_propctrl_object::on_textctrldragleave( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		xui_control* textctrl = m_propedit->get_editctrl();
		textctrl->set_backcolor(xui_colour::k_darkgray);
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			data->old_value();
		}

		on_linkpropdata();
	}
}

void xui_propctrl_object::on_textctrldragover( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		args.allow = true;
	}
}

void xui_propctrl_object::on_textctrldragdrop( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		xui_control* textctrl = m_propedit->get_editctrl();
		textctrl->set_backcolor(xui_colour::k_darkgray);
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			data->syn_value();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//pickwnd
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_object_pickwnd, xui_window)

xui_object_pickwnd::xui_object_pickwnd( void )
: xui_window(xui_vector<s32>(320, 240), true)
, m_propctrl(NULL)
{}

xui_propctrl* xui_object_pickwnd::get_propctrl( void )
{
	return m_propctrl;
}

void xui_object_pickwnd::set_propctrl( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}

void xui_object_pickwnd::on_accept( xui_component* sender, xui_method_args& args )
{
	void* value = get_value();
	xui_propdata_vec vec = m_propctrl->get_propdata();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(vec[i]);
		data->set_value(value);
		data->syn_value();
	}

	m_propctrl->on_linkpropdata();
	set_visible(false);
}

void xui_object_pickwnd::on_cancel( xui_component* sender, xui_method_args& args )
{
	set_visible(false);
}