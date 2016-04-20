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

///*
////override
//*/
//xui_method_explain(onity_propctrl_transition, on_linkpropdata,		void			)( void )
//{
//	onity_propdata_transition* datatransition = dynamic_cast<onity_propdata_transition*>(m_propdata);
//	onity_state* statectrl = datatransition->get_statectrl();
//	const NP2DSState::TransitionVec& vec = statectrl->get_state()->GetTransitionVec();
//
//	xui_method_ptrcall(m_sublst, del_itemall)();
//	for (u32 i = 0; i < vec.size(); ++i)
//	{
//		NP2DSTransition* transition = vec[i];
//		std::string text;
//		text += transition->GetSelfState()->GetName();
//		text += " >> ";
//		text += transition->GetNextState()->GetName();
//		xui_listitem * item = m_sublst->add_item(xui_global::ascii_to_unicode(text));
//		item->set_data(transition);
//	}
//	xui_method_ptrcall(m_sublst, refresh	)();
//	xui_method_ptrcall(m_delete, set_enable	)(false);
//	xui_method_ptrcall(m_nontip, set_visible)(vec.size() == 0);
//}

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

///*
////event
//*/
//xui_method_explain(onity_propctrl_transition, on_deleteclick,		void			)( xui_component* sender, xui_method_args& args )
//{
//	std::vector<xui_listitem*> vec = m_sublst->get_selecteditem();
//	if (vec.size() > 0)
//	{
//		onity_propdata_transition* datatransition = dynamic_cast<onity_propdata_transition*>(m_propdata);
//
//		NP2DSTransition* transition = (NP2DSTransition*)vec[0]->get_data();
//		onity_state*	 statectrl  = datatransition->get_statectrl();
//		statectrl->get_propstate()->del_transition(transition);
//		statectrl->get_state()->DelTransition(transition);
//
//		on_linkpropdata();
//	}
//}