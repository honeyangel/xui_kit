#ifndef __onity_animator_h__
#define __onity_animator_h__

#include "xui_dockpage.h"

class NP2DSState;
class NP2DSParam;
class NP2DSStateCtrl;
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
	NP2DSStateCtrl*		get_editfile		( void );
	void				set_editfile		( NP2DSStateCtrl* editfile );
	void				del_param			( NP2DSParam* param );
	void				del_state			( NP2DSState* state );

protected:
	/*
	//event
	*/
	void				on_stateclick		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*			m_create;
	xui_menuitem*		m_state;
	xui_panel*			m_head;
	onity_stateview*	m_stateview;
	onity_paramview*	m_paramview;
	NP2DSStateCtrl*		m_editfile;
};

#endif//__onity_animator_h__