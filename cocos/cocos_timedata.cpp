#include "cocos_propleaf.h"
#include "cocos_timedata.h"

/*
//constructor
*/
xui_create_explain(cocos_timedata)( xui_bitmap* icon, xui_proproot* prop )
: xui_timedata()
, m_prop(prop)
{
	m_icon = icon;

	cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(m_prop);
	if (propleaf)   propleaf->add_linkdata(this);
}

/*
//destructor
*/
xui_delete_explain(cocos_timedata)( void )
{
	cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(m_prop);
	if (propleaf)   propleaf->del_linkdata(this);
}

/*
//method
*/
xui_method_explain(cocos_timedata, get_prop, xui_proproot*	)( void )
{
	return m_prop;
}
xui_method_explain(cocos_timedata, set_null, void			)( void )
{
	m_prop = NULL;
}