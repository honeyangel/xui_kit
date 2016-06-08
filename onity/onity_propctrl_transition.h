#ifndef __onity_propctrl_transition_h__
#define __onity_propctrl_transition_h__

#include "xui_propctrl_stdvec.h"

class NP2DSState;
class onity_propctrl_transition : public xui_propctrl_stdvec_root
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create				( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_transition( xui_propdata* propdata );

	/*
	//notify
	*/
	void					on_delstate			( NP2DSState* state );
};

#endif//__onity_propctrl_transition_h__