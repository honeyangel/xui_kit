#include "xui_propedit.h"
#include "xui_drawer.h"
#include "xui_dropbox.h"
#include "cocos_propdata_spineanim.h"

//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propdata_spineanim)( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, vec_func uservec, void* userptr )
: xui_propdata(kind, name, cocos_propctrl_spineanim::create)
, m_userget(userget)
, m_userset(userset)
, m_uservec(uservec)
, m_userptr(userptr)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_spineanim, get_value,			std::wstring	)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(cocos_propdata_spineanim, set_value,			void			)( const std::wstring& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}
xui_method_explain(cocos_propdata_spineanim, vec_value,			cocos_string_vec)( void )
{
	return (*m_uservec)(m_userptr);
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_spineanim, xui_propctrl_base);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_spineanim)( xui_propdata* propdata )
: xui_propctrl_base()
{
	xui_propedit_enum* editenum = new xui_propedit_enum(this, xui_propenum_map());
	xui_drawer*  namectrl = editenum->get_namectrl();								
	xui_control* enumctrl = editenum->get_editctrl();								
	namectrl->set_parent(this);													
	enumctrl->set_parent(this);													
	m_widgetvec.push_back(namectrl);											
	m_widgetvec.push_back(enumctrl);											
	m_propedit = editenum;															
}

/*
//static
*/
xui_method_explain(cocos_propctrl_spineanim, create,			xui_propctrl*	)( xui_propdata* data )
{
	return new cocos_propctrl_spineanim(data);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_spineanim, on_linkpropdata,	void			)( bool selfupdate /* = false */ )
{
	xui_propctrl_base::on_linkpropdata(selfupdate);								

	cocos_propdata_spineanim* dataanim = dynamic_cast<cocos_propdata_spineanim*>(m_propdata);						
	cocos_string_vec vec = dataanim->vec_value();
	if (selfupdate == false)
	{
		xui_dropbox* dropbox = xui_dynamic_cast(xui_dropbox, m_propedit->get_editctrl());
		dropbox->del_itemall();
		for (u32 i = 0; i < vec.size(); ++i)
			dropbox->add_item(vec[i]);
	}

	bool same = true;															
	std::wstring value = dataanim->get_value();									
	for (u32 i = 0; i < m_propdatavec.size(); ++i)								
	{																			
		cocos_propdata_spineanim* data = dynamic_cast<cocos_propdata_spineanim*>(m_propdatavec[i]);					
		if (data->get_value() != value)											
		{																		
			same = false;														
			break;																
		}																		
	}																			

	if (same)																	
	{
		for (u32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i] == value)
			{
				xui_propedit_enum* edit = (xui_propedit_enum*)m_propedit;
				edit->set_value(i);
				break;
			}
		}
	}																			
}
xui_method_explain(cocos_propctrl_spineanim, on_editvalue,		void			)( xui_propedit* sender )
{
	cocos_propdata_spineanim* dataanim = dynamic_cast<cocos_propdata_spineanim*>(m_propdata);
	cocos_string_vec vec = dataanim->vec_value();
	xui_propedit_enum* editenum = (xui_propedit_enum*)m_propedit;
	std::wstring value = vec[editenum->get_value()];
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_spineanim* data = dynamic_cast<cocos_propdata_spineanim*>(m_propdatavec[i]);
		data->set_value(value);
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_spineanim, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl_base::on_perform(args);
	xui_control* enumctrl = m_propedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	pt.x = rt.get_w() / 2;
	pt.y = rt.get_h() / 2 - enumctrl->get_renderh() / 2;
	enumctrl->on_perform_pt(pt);
	enumctrl->on_perform_w(rt.get_w() - pt.x);
}