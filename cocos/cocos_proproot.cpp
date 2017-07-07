#include "xui_treenode.h"
#include "xui_treeview.h"
#include "cocos_treedata.h"
#include "cocos_timedata.h"
#include "cocos_proproot.h"

/*
//constructor
*/
xui_create_explain(cocos_proproot)( void )
: xui_proproot()
{}

/*
//destructor
*/
xui_delete_explain(cocos_proproot)( void )
{
	for (u32 i = 0; i < m_linkdatavec.size(); ++i)
	{
		cocos_treedata* treedata = dynamic_cast<cocos_treedata*>(m_linkdatavec[i]);
		cocos_timedata* timedata = dynamic_cast<cocos_timedata*>(m_linkdatavec[i]);
		if (treedata)   treedata->set_null();
		if (timedata)	timedata->set_null();
	}
}

/*
//method
*/
xui_method_explain(cocos_proproot, get_linkdata,	xui_treedata*	)( xui_treeview* treeview )
{
	if (m_linkdatavec.size() > 0)
	{
		if (treeview == NULL)
		{
			return m_linkdatavec.front();
		}
		else
		{
			for (u32 i = 0; i < m_linkdatavec.size(); ++i)
			{
				if (treeview == m_linkdatavec[i]->get_node()->get_parent())
					return m_linkdatavec[i];
			}
		}
	}

	return NULL;
}
xui_method_explain(cocos_proproot, add_linkdata,	void			)( xui_treedata* linkdata )
{
	m_linkdatavec.push_back(linkdata);
}
xui_method_explain(cocos_proproot, del_linkdata,	void			)( xui_treedata* linkdata )
{
	for (u32 i = 0; i < m_linkdatavec.size(); ++i)
	{
		if (m_linkdatavec[i] == linkdata)
		{
			m_linkdatavec.erase(m_linkdatavec.begin()+i);
			break;
		}
	}
}