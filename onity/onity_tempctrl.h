#ifndef __onity_tempctrl_h__
#define __onity_tempctrl_h__

#include "xui_kindctrl.h"

class onity_tempctrl : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_kindctrl*	create				( xui_propkind* propkind );

	/*
	//constructor
	*/
	onity_tempctrl( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_updateself		( xui_method_update& args );
	virtual void			on_perform			( xui_method_args&   args );

	/*
	//event
	*/
	void					on_clickselect		( xui_component* sender, xui_method_args& args );
	void					on_clickapply		( xui_component* sender, xui_method_args& args );
	void					on_clickrevert		( xui_component* sender, xui_method_args& args );

	/*
	//override
	*/
	virtual s32				get_elsectrlsize	( void );
	virtual s32				get_prevctrlsize	( void );

	/*
	//member
	*/
	xui_button*				m_select;
	xui_button*				m_apply;
	xui_button*				m_revert;
};

#endif//__onity_tempctrl_h__