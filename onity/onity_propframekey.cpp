#include "NP2DSFrameKey.h"

#include "xui_kindctrl.h"
#include "onity_propframekey.h"

/*
//constructor
*/
xui_create_explain(onity_propframekey)( onity_propfile* propfile, NP2DSFrameKey* framekey )
: onity_propleaf(propfile)
, m_framekey(framekey)
{
	m_basekind = new xui_propkind(this, L"FrameKey", "FrameKey", xui_kindctrl::create, NULL, true);
	add_propkind(m_basekind);
}

/*
//method
*/
xui_method_explain(onity_propframekey, get_framekey, NP2DSFrameKey*)( void )
{
	return m_framekey;
}