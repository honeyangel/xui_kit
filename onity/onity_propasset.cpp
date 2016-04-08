#include "xui_kindctrl.h"
#include "onity_savekind.h"
#include "onity_propfile.h"
#include "onity_propasset.h"

/*
//constructor
*/
xui_create_explain(onity_propasset)( void )
: xui_proproot()
, m_savekind(NULL)
{}

/*
//virtual
*/
xui_method_explain(onity_propasset, on_propchanged, void)( xui_component* sender, xui_method_propdata& args )
{
	if (m_savekind)
	{
		onity_propfile* propfile = m_savekind->get_propfile();
		propfile->set_modify(true);

		xui_kindctrl* kindctrl = m_savekind->get_ctrl();
		if (kindctrl)
			kindctrl->refresh();
	}
}