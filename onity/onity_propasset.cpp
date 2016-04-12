#include "xui_kindctrl.h"
#include "onity_savekind.h"
#include "onity_propfile.h"
#include "onity_propasset.h"

/*
//constructor
*/
xui_create_explain(onity_propasset)( onity_propfile* propfile )
: xui_proproot()
{
	m_savekind = new onity_savekind(this, propfile);
	add_propkind(m_savekind);
}