#ifndef __onity_savectrl_h__
#define __onity_savectrl_h__

#include "xui_kindctrl.h"

class onity_savectrl : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_kindctrl*	create			( xui_propkind* propkind );

	/*
	//constructor
	*/
	onity_savectrl( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void					on_saveclick	( xui_component* sender, xui_method_args& args );
	void					on_loadclick	( xui_component* sender, xui_method_args& args );

	/*
	//override
	*/
	virtual s32				get_elsectrlsize	( void );
	virtual s32				get_prevctrlsize	( void );

	/*
	//member
	*/
	xui_button*				m_save;
	xui_button*				m_load;
};

#endif//__onity_propasset_h__