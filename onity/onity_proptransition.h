#ifndef __onity_proptransition_h__
#define __onity_proptransition_h__

#include "xui_proproot.h"
#include "xui_kindctrl.h"

class NP2DSParam;
class NP2DSTransition;
class onity_proptransition : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_proptransition( NP2DSTransition* transition );

	/*
	//method
	*/
	NP2DSTransition*	get_transition		( void );

	/*
	//notify
	*/
	void				on_delparam			( NP2DSParam* param );
	void				on_addparam			( NP2DSParam* param );

protected:
	/*
	//member
	*/
	xui_propdata*		m_condition;
	NP2DSTransition*	m_transition;
};

#endif//__onity_proptransition_h__