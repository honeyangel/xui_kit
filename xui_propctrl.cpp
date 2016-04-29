#include "xui_textbox.h"
#include "xui_dropbox.h"
#include "xui_toggle.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propedit_colour.h"
#include "xui_propctrl.h"

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl, xui_control);
/*
//constructor
*/
xui_create_explain(xui_propctrl)( void )
: xui_control(xui_vector<s32>(0, xui_propview::default_lineheight))
{
	m_propdata	= NULL;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl,		get_propdata,		const xui_propdata_vec&	)( void ) const
{
	return m_propdatavec;
}
xui_method_explain(xui_propctrl,		has_propdata,		bool					)( xui_propdata* propdata ) const
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		if (propdata == m_propdatavec[i])
			return true;
	}

	return false;
}
xui_method_explain(xui_propctrl,		set_propdata,		void					)( xui_propdata* propdata )
{
	m_propdata = propdata;
	m_propdatavec.clear();
	m_propdatavec.push_back(propdata);

	on_linkpropdata();
}
xui_method_explain(xui_propctrl,		set_propdata,		void					)( const xui_propdata_vec& propdata )
{
	m_propdata    = propdata.front();
	m_propdatavec = propdata;

	on_linkpropdata();
}

xui_method_explain(xui_propctrl,		add_ctrlelse,		void					)( xui_component* component )
{
	component->set_parent(this);
	m_widgetvec.push_back(component);
}
xui_method_explain(xui_propctrl,		get_propview,		xui_propview*			)( void )
{
	if (m_propdata)
		return m_propdata->get_kind()->get_root()->get_ctrl();

	return NULL;
}
xui_method_explain(xui_propctrl,		get_kindctrl,		xui_kindctrl*			)( void )
{
	if (m_propdata)
		return m_propdata->get_kind()->get_ctrl();

	return NULL;
}
xui_method_explain(xui_propctrl,		get_indent,			s32						)( void )
{
	s32 depth = 1;
	xui_kindctrl* kind = get_kindctrl();
	xui_component* root = m_parent;
	while (root && root != kind)
	{
		++depth;
		root = root->get_parent();
	}

	return depth * xui_propview::default_nodeindent;
}
xui_method_explain(xui_propctrl,		on_lock,			void					)( xui_method_args& args )
{
	xui_control::on_lock(args);
	m_maskcolor = xui_colour(1.0f, 0.7f);
}

//////////////////////////////////////////////////////////////////////////
//propctrl_base
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_base, xui_propctrl);
/*
//constructor
*/
xui_create_explain(xui_propctrl_base)( void )
: xui_propctrl()
{}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_base)( void )
{
	delete m_propedit;
}

/*
//override
*/
xui_method_explain(xui_propctrl_base,	on_linkpropdata,	void					)( void )
{
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());
}
xui_method_explain(xui_propctrl_base,	on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_drawer* namectrl = m_propedit->get_namectrl();

	xui_rect2d<s32> rt = get_renderrtins();
	s32 indent = get_indent();
	namectrl->on_perform_w(rt.get_w()/2);
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

#define xui_propctrl_implement_attach(name)										\
	xui_drawer*  namectrl = name->get_namectrl();								\
	xui_control* boolctrl = name->get_editctrl();								\
	namectrl->set_parent(this);													\
	boolctrl->set_parent(this);													\
	m_widgetvec.push_back(namectrl);											\
	m_widgetvec.push_back(boolctrl);											\
	m_propedit = name;															\

#define xui_propctrl_implement_link(class_name, propedit, propdata, datatype)	\
void	class_name::on_linkpropdata( void )										\
{																				\
	xui_propctrl_base::on_linkpropdata();										\
																				\
	bool same = true;															\
	propdata* data = (propdata*)m_propdata;										\
	datatype value = data->get_value();											\
	for (u32 i = 0; i < m_propdatavec.size(); ++i)								\
	{																			\
		propdata* temp = (propdata*)m_propdatavec[i];							\
		if (temp->get_value() != value)											\
		{																		\
			same = false;														\
			break;																\
		}																		\
	}																			\
																				\
	if (same)																	\
	{																			\
		propedit* edit = (propedit*)m_propedit;									\
		edit->set_value(value);													\
	}																			\
}

#define xui_propctrl_implement_edit(class_name, propedit, propdata, datatype)	\
void	class_name::on_editvalue( xui_propedit* sender )						\
{																				\
	propedit* edit = (propedit*)m_propedit;										\
	datatype value = edit->get_value();											\
	for (u32 i = 0; i < m_propdatavec.size(); ++i)								\
	{																			\
		propdata* data = (propdata*)m_propdatavec[i];							\
		data->set_value(value);													\
	}																			\
}

//////////////////////////////////////////////////////////////////////////
//propctrl_bool
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_bool, xui_propctrl_base);
/*
//constructor
*/
xui_create_explain(xui_propctrl_bool)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propdata_bool* databool = dynamic_cast<xui_propdata_bool*>(propdata);
	xui_propedit_bool* editbool = new xui_propedit_bool(this);
	xui_propctrl_implement_attach(editbool)
}
xui_propctrl_implement_link(xui_propctrl_bool, xui_propedit_bool, xui_propdata_bool, bool)
xui_propctrl_implement_edit(xui_propctrl_bool, xui_propedit_bool, xui_propdata_bool, bool)

/*
//override
*/
xui_method_explain(xui_propctrl_bool,	on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* boolctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - boolctrl->get_renderh()/2;
	boolctrl->on_perform_pt(pt);
}

//////////////////////////////////////////////////////////////////////////
//propctrl_enum
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_enum, xui_propctrl_base);
/*
//constructor
*/
xui_create_explain(xui_propctrl_enum)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propdata_enum* dataenum = dynamic_cast<xui_propdata_enum*>(propdata);
	xui_propedit_enum* editenum = new xui_propedit_enum(this, dataenum->get_textmap());
	xui_propctrl_implement_attach(editenum)
}
xui_propctrl_implement_link(xui_propctrl_enum, xui_propedit_enum, xui_propdata_enum, u32)
xui_propctrl_implement_edit(xui_propctrl_enum, xui_propedit_enum, xui_propdata_enum, u32)

/*
//override
*/
xui_method_explain(xui_propctrl_enum,	on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* enumctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - enumctrl->get_renderh()/2;
	enumctrl->on_perform_pt(pt);
	enumctrl->on_perform_w (rt.get_w()/2);
}

//////////////////////////////////////////////////////////////////////////
//propctrl_number
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_number, xui_propctrl_base);
/*
//constructor
*/
xui_create_explain(xui_propctrl_number)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propdata_number* datanumber = dynamic_cast<xui_propdata_number*>(propdata);
	xui_propedit_number* editnumber = new xui_propedit_number(this, datanumber->get_numbtype(), datanumber->get_interval());
	xui_propctrl_implement_attach(editnumber)
}
xui_propctrl_implement_link(xui_propctrl_number, xui_propedit_number, xui_propdata_number, f64)
xui_propctrl_implement_edit(xui_propctrl_number, xui_propedit_number, xui_propdata_number, f64)

/*
//override
*/
xui_method_explain(xui_propctrl_number, on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* textctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w()/2);
}

//////////////////////////////////////////////////////////////////////////
//propctrl_string
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_string, xui_propctrl_base);
/*
//constructor
*/
xui_create_explain(xui_propctrl_string)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propdata_string* datastring = dynamic_cast<xui_propdata_string*>(propdata);
	xui_propedit_string* editstring = new xui_propedit_string(this);
	xui_propctrl_implement_attach(editstring)
}
xui_propctrl_implement_link(xui_propctrl_string, xui_propedit_string, xui_propdata_string, std::wstring)
xui_propctrl_implement_edit(xui_propctrl_string, xui_propedit_string, xui_propdata_string, std::wstring)

/*
//override
*/
xui_method_explain(xui_propctrl_string, on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* textctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w()/2);
}

//////////////////////////////////////////////////////////////////////////
//propctrl_colour
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_colour, xui_propctrl_base);
/*
//constructor
*/
xui_create_explain(xui_propctrl_colour)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propdata_colour* datacolour = dynamic_cast<xui_propdata_colour*>(propdata);
	xui_propedit_colour* editcolour = new xui_propedit_colour(this);
	xui_propctrl_implement_attach(editcolour)
}
xui_propctrl_implement_link(xui_propctrl_colour, xui_propedit_colour, xui_propdata_colour, xui_colour)
xui_propctrl_implement_edit(xui_propctrl_colour, xui_propedit_colour, xui_propdata_colour, xui_colour)

/*
//override
*/
xui_method_explain(xui_propctrl_colour, on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* textctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w()/2;
	pt.y = rt.get_h()/2 - textctrl->get_renderh()/2;
	textctrl->on_perform_pt(pt);
	textctrl->on_perform_w (rt.get_w()/2);
}
