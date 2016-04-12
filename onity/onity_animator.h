#ifndef __onity_animator_h__
#define __onity_animator_h__

#include "xui_dockpage.h"

class onity_propcontroller;
class onity_paramview;
class onity_stateview;
class onity_animator : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_animator( void );

	/*
	//method
	*/
	onity_stateview*		get_stateview		( void );
	onity_paramview*		get_paramview		( void );
	onity_propcontroller*	get_editprop		( void );
	void					set_editprop		( onity_propcontroller* editprop );

protected:
	/*
	//event
	*/
	void					on_stateclick		( xui_component* sender, xui_method_args& args );
	void					on_retargetclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*				m_create;
	xui_toggle*				m_retarget;
	xui_menuitem*			m_state;
	xui_menuitem*			m_on;
	xui_menuitem*			m_off;
	xui_toolbar*			m_head;
	onity_stateview*		m_stateview;
	onity_paramview*		m_paramview;
	onity_propcontroller*	m_editprop;
};

#endif//__onity_animator_h__