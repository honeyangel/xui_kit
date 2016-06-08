#include "NP2DSState.h"
#include "NP2DSTransition.h"

#include "xui_convas.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_propview.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "onity_state.h"
#include "onity_propstate.h"
#include "onity_proptransition.h"
#include "onity_propctrl_transition.h"

xui_implement_rtti(onity_propctrl_transition, xui_propctrl_stdvec_root);

/*
//constructor
*/
xui_create_explain(onity_propctrl_transition)( xui_propdata* propdata )
: xui_propctrl_stdvec_root(propdata)
{}

/*
//static
*/
xui_method_explain(onity_propctrl_transition, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_transition(propdata);
}

/*
//notify
*/
xui_method_explain(onity_propctrl_transition, on_delstate,			void			)( NP2DSState* state )
{
	for (s32 i = (s32)m_middle->get_itemcount()-1; i >= 0; --i)
	{
		xui_listitem* item = m_middle->get_item((u32)i);
		onity_proptransition* proptransition = (onity_proptransition*)item->get_data();
		if (proptransition->get_transition()->GetNextState() == state)
			m_middle->del_item(item);
	}

	xui_method_ptrcall(m_middle, refresh	)();
	xui_method_ptrcall(m_delete, set_enable	)(m_middle->get_selecteditem().size() > 0);
	xui_method_ptrcall(m_nontip, set_visible)(m_middle->get_itemcount() == 0);
}