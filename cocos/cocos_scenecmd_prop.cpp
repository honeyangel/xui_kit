#include "cocos_proproot.h"
#include "cocos_scenecmd_prop.h"

/*
//constructor
*/
xui_create_explain(cocos_scenecmd_prop)( cocos_proproot* root, xui_propdata* data )
: m_root(root)
, m_path(data->get_path())
, m_undo(data->get_byte())
, m_redo(NULL)
{}

/*
//destructor
*/
xui_delete_explain(cocos_scenecmd_prop)( void )
{
	delete [] m_undo;
	delete [] m_redo;
}

/*
//override
*/
xui_method_explain(cocos_scenecmd_prop, undo, void)( void )
{
	xui_propdata* data = m_root->get_propdata(m_path);
	if (data)
	{
		data->backups();
		delete m_redo;
		m_redo = data->get_byte();

		data->restore(m_undo);
		data->backups();
	}
}
xui_method_explain(cocos_scenecmd_prop, redo, void)( void )
{
	xui_propdata* data = m_root->get_propdata(m_path);
	if (data)
	{
		data->restore(m_redo);
	}
}