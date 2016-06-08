#include "xui_global.h"
#include "xui_convas.h"
#include "xui_button.h"
#include "xui_desktop.h"
#include "xui_propview.h"
#include "xui_propctrl_object.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_object
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_object, xui_propctrl);
/*
//create
*/
xui_method_explain(xui_propctrl_object, create,					xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_object(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_object)( xui_propdata* propdata )
: xui_propctrl()
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(propdata);
	xui_prop_geticon iconfunc = dataobject->get_iconfunc();
	xui_bitmap* icon = (iconfunc == NULL) ? NULL : iconfunc(propdata);

	xui_propedit_object* editobject = new xui_propedit_object(this, icon);
	xui_drawer*  namectrl = editobject->get_namectrl();
	xui_control* textctrl = editobject->get_editctrl();
	xui_drawer*  pickctrl = editobject->get_pickctrl();
	textctrl->xm_mouseclick		+= new xui_method_member<xui_method_mouse,	  xui_propctrl_object>(this, &xui_propctrl_object::on_textctrlclick);
	textctrl->xm_mousedragenter += new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragenter);
	textctrl->xm_mousedragleave += new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragleave);
	textctrl->xm_mousedragover  += new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragover);
	textctrl->xm_mousedragdrop  += new xui_method_member<xui_method_dragdrop, xui_propctrl_object>(this, &xui_propctrl_object::on_textctrldragdrop);
	xui_method_ptrcall(namectrl, set_parent)(this);
	xui_method_ptrcall(textctrl, set_parent)(this);
	xui_method_ptrcall(pickctrl, set_parent)(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);
	m_widgetvec.push_back(pickctrl);

	m_propedit = editobject;
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_object)( void )
{
	delete m_propedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_object, on_linkpropdata,		void			)( void )
{
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

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
		if (dataobject->get_value())
		{
			xui_prop_getname namefunc = dataobject->get_namefunc();
			if (namefunc)
				textctrl->set_text(namefunc(dataobject));
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
xui_method_explain(xui_propctrl_object, on_editvalue,			void			)( xui_propedit* sender )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	xui_prop_newpick pickfunc = dataobject->get_pickfunc();
	if (pickfunc)
	{
		xui_object_pickwnd* wnd = pickfunc();
		xui_method_ptrcall(wnd, set_propctrl)(this);
		xui_method_ptrcall(wnd, set_modal	)(true);
		xui_method_ptrcall(wnd, set_visible	)(true);
		xui_method_ptrcall(wnd, set_value	)(dataobject->get_value());
	}
}

/*
//override
*/
xui_method_explain(xui_propctrl_object, on_perform,				void			)( xui_method_args& args )
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
	pt.y = xui_propview::default_lineheight/2 - pickctrl->get_renderh()/2;
	pickctrl->on_perform_pt(pt);
	//textctrl
	pt.x = rt.get_w()/3;
	pt.y = xui_propview::default_lineheight/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w() - pt.x - pickctrl->get_renderw());
	//namectrl
	s32 indent = get_indent();
	namectrl->on_perform_w (rt.get_w()/3);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

/*
//event
*/
xui_method_explain(xui_propctrl_object, on_textctrlclick,		void			)( xui_component* sender, xui_method_mouse&	  args )
{
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

	if (same)
	{
		dataobject->xm_click(sender, args);
	}
}
xui_method_explain(xui_propctrl_object, on_textctrldragenter,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
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
xui_method_explain(xui_propctrl_object, on_textctrldragleave,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		xui_control* textctrl = m_propedit->get_editctrl();
		textctrl->set_backcolor(xui_colour::darkgray);
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			data->old_value();
		}

		on_linkpropdata();
	}
}
xui_method_explain(xui_propctrl_object, on_textctrldragover,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		args.allow = true;
	}
}
xui_method_explain(xui_propctrl_object, on_textctrldragdrop,	void			)( xui_component* sender, xui_method_dragdrop& args )
{
	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	if (dataobject->has_droptype(args.type))
	{
		xui_control* textctrl = m_propedit->get_editctrl();
		textctrl->set_backcolor(xui_colour::darkgray);
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
xui_implement_rtti(xui_object_pickwnd, xui_window);
/*
//constructor
*/
xui_create_explain(xui_object_pickwnd)( void )
: xui_window(xui_vector<s32>(320, 240))
, m_propctrl(NULL)
{}

/*
//method
*/
xui_method_explain(xui_object_pickwnd,	get_propctrl,	xui_propctrl*	)( void )
{
	return m_propctrl;
}
xui_method_explain(xui_object_pickwnd,	set_propctrl,	void			)( xui_propctrl* propctrl )
{
	m_propctrl = propctrl;
}
xui_method_explain(xui_object_pickwnd,	on_accept,		void			)( xui_component* sender, xui_method_args& args )
{
	void* value = get_value();
	xui_propdata_vec vec = m_propctrl->get_propdata();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(vec[i]);
		data->set_value(value);
	}

	m_propctrl->on_linkpropdata();
	set_visible(false);
}
xui_method_explain(xui_object_pickwnd,	on_cancel,		void			)( xui_component* sender, xui_method_args& args )
{
	set_visible(false);
}