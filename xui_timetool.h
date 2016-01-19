#ifndef __xui_timetool_h__
#define __xui_timetool_h__

#include "xui_toolbar.h"

class xui_timetool : public xui_toolbar
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_timetool( xui_component* parent );

	/*
	//destructor
	*/
	virtual ~xui_timetool( void );

protected:
	/*
	//method
	*/
	void			on_buttonclick		( xui_component* sender, xui_method_args& args );
	void			on_toggleclick		( xui_component* sender, xui_method_args& args );
	void			on_buttonrenderself	( xui_component* sender, xui_method_args& args );
	void			on_togglerenderself	( xui_component* sender, xui_method_args& args );
	void			on_timertick		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*		m_head;
	xui_button*		m_prev;
	xui_toggle*		m_back;
	xui_toggle*		m_play;
	xui_button*		m_next;
	xui_button*		m_tail;
	xui_toggle*		m_loop;
	xui_timer*		m_playtimer;
};

#endif//__xui_timetool_h__