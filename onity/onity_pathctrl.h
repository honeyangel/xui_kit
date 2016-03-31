#ifndef __onity_pathctrl_h__
#define __onity_pathctrl_h__

#include "xui_kindctrl.h"

class onity_pathctrl : public xui_kindctrl
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
	onity_pathctrl( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void					on_freetypeclick	( xui_component* sender, xui_method_args& args );
	void					on_loadtypeclick	( xui_component* sender, xui_method_args& args );

	/*
	//override
	*/
	virtual s32				get_elsectrlsize	( void );
	virtual s32				get_prevctrlsize	( void );

	/*
	//member
	*/
	xui_toggle*				m_freetype;
	xui_toggle*				m_loadtype;
	xui_menuitem*			m_auto;
	xui_menuitem*			m_never;
	xui_menuitem*			m_immediate;
	xui_menuitem*			m_on;
	xui_menuitem*			m_off;
};

#endif//__onity_pathctrl_h__