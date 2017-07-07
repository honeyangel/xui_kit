#ifndef __cocos_filectrl_h__
#define __cocos_filectrl_h__

#include "xui_kindctrl.h"

class cocos_filectrl : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_kindctrl*	create				( xui_propkind* propkind );

	/*
	//constructor
	*/
	cocos_filectrl( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void					on_showfindclick	( xui_component* sender, xui_method_args& args );

	/*
	//override
	*/
	virtual s32				get_elsectrlsize	( void );
	virtual s32				get_prevctrlsize	( void );

	/*
	//member
	*/
	xui_button*				m_showfind;
};

#endif//__cocos_filectrl_h__