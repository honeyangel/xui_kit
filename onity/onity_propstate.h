#ifndef __onity_propstate_h__
#define __onity_propstate_h__

#include "xui_kindctrl.h"
#include "onity_propasset.h"

class NP2DSState;
class onity_state;
class onity_proptransition;
class onity_propstate : public onity_propasset
{
public:
	/*
	//constructor
	*/
	onity_propstate( onity_propfile* propfile, onity_state* statectrl );

	/*
	//destructor
	*/
	virtual ~onity_propstate( void );

	/*
	//method
	*/
	const xui_proproot_vec&				get_proptransitions	( void ) const;
	onity_proptransition*				get_proptransition	( NP2DSTransition* transition );
	void								add_transition		( NP2DSTransition* transition );
	void								del_transition		( NP2DSTransition* transition );
	onity_state*						get_statectrl		( void );

	/*
	//notify
	*/
	void								on_delstate			( NP2DSState* state );
	void								on_delparam			( NP2DSParam* param );
	void								on_addparam			( NP2DSParam* param );

protected:
	/*
	//static
	*/
	static bool							get_retarget		( void* userptr );
	static void							set_retarget		( void* userptr, bool  value );
	static void*						get_actor			( void* userptr );
	static void							set_actor			( void* userptr, void* value );
	static xui_bitmap*					get_actoricon		( xui_propdata* propdata );
	static std::wstring					get_actorname		( xui_propdata* propdata );
	static xui_proproot_vec				get_transitions		( void* userptr );
	static void							del_transition		( void* userptr, xui_proproot* root );

	/*
	//event
	*/
	void								on_namechanged		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_propkind*						m_statekind;
	onity_state*						m_statectrl;
	xui_propdata*						m_retarget;
	xui_propdata*						m_actor;
	xui_propdata*						m_transition;
	std::vector<xui_proproot*>			m_proptransition;
};

#endif//__onity_propstate_h__