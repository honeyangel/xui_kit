#include "xui_propview.h"
#include "xui_proproot.h"

/*
//constructor
*/
xui_create_explain(xui_proproot)( void )
: m_ctrl(NULL)
{}

/*
//destructor
*/
xui_delete_explain(xui_proproot)( void )
{
	if (m_ctrl)
		m_ctrl->del_proproot(this);

	for (u32 i = 0; i < m_propkind.size(); ++i)
		delete m_propkind[i];
}

/*
//method
*/
xui_method_explain(xui_proproot, get_propkind,	const xui_propkind_vec&	)( void ) const
{
	return m_propkind;
}
xui_method_explain(xui_proproot, get_propkind,	xui_propkind*			)( const std::string& type )
{
	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i]->get_type() == type)
			return m_propkind[i];
	}

	return NULL;
}
xui_method_explain(xui_proproot, has_propkind,	bool					)( xui_propkind* propkind ) const
{
	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i]->get_func() == propkind->get_func() &&
			m_propkind[i]->get_type() == propkind->get_type())
			return true;
	}

	return false;
}
xui_method_explain(xui_proproot, add_propkind,	void					)( xui_propkind* propkind )
{
	if (propkind->was_tail())
	{
		m_propkind.push_back(propkind);
	}
	else
	{
		xui_propkind_vec::iterator itor = m_propkind.begin();
		for (; itor != m_propkind.end(); ++itor)
		{
			if ((*itor)->was_tail())
				break;
		}

		m_propkind.insert(itor, propkind);
	}
}
xui_method_explain(xui_proproot, del_propkind,	void					)( xui_propkind* propkind )
{
	for (u32 i = 0; i < m_propkind.size(); ++i)
	{
		if (m_propkind[i] == propkind)
		{
			delete m_propkind[i];
			m_propkind.erase(m_propkind.begin()+i);
			break;
		}
	}
}
xui_method_explain(xui_proproot, non_ctrl,		void					)( void )
{
	m_ctrl = NULL;
	for (u32 i = 0; i < m_propkind.size(); ++i)
		m_propkind[i]->non_ctrl();
}
xui_method_explain(xui_proproot, get_ctrl,		xui_propview*			)( void )
{
	return m_ctrl;
}
xui_method_explain(xui_proproot, set_ctrl,		void					)( xui_propview* propview )
{
	m_ctrl = propview;
}

/*
//virtual
*/
xui_method_explain(xui_proproot, on_detach,		void					)( xui_proproot* proproot )
{
	xui_proproot_vec vec;
	if (proproot)    
	{
		vec.push_back(proproot);
	}

	on_detach(vec);
}
xui_method_explain(xui_proproot, on_detach,		void					)( const xui_proproot_vec& proproot )
{

}
xui_method_explain(xui_proproot, on_attach,		void					)( void )
{

}