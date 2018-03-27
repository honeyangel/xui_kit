#include "cocos_proproot.h"
#include "cocos_scenecmd_list.h"

/*
//constructor
*/
xui_create_explain(cocos_scenecmd_list)( void )
{}

/*
//destructor
*/
xui_delete_explain(cocos_scenecmd_list)( void )
{
	for (std::list<cocos_scenecmd*>::iterator itor = m_cmdlist.begin(); itor != m_cmdlist.end(); ++itor)
	{
		delete (*itor);
	}

	m_cmdlist.clear();
}

/*
//method
*/
xui_method_explain(cocos_scenecmd_list, push, void)( cocos_scenecmd* cmd )
{
	m_cmdlist.push_back(cmd);
}

/*
//override
*/
xui_method_explain(cocos_scenecmd_list, undo, void)( void )
{
	for (std::list<cocos_scenecmd*>::iterator itor = m_cmdlist.begin(); itor != m_cmdlist.end(); ++itor)
	{
		(*itor)->undo();
	}
}
xui_method_explain(cocos_scenecmd_list, redo, void)( void )
{
	for (std::list<cocos_scenecmd*>::iterator itor = m_cmdlist.begin(); itor != m_cmdlist.end(); ++itor)
	{
		(*itor)->redo();
	}
}