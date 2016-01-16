#ifndef __xui_toolbar_h__
#define __xui_toolbar_h__

#include "xui_control.h"

class xui_toolbar : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_toolbar( const xui_vector<s32>& size, xui_component* parent );

	/*
	//property
	*/
	u08				get_flow	( void ) const;
	void			set_flow	( u08 flow );
	s32				get_grap	( void ) const;
	void			set_grap	( s32 grap );

	/*
	//item
	*/
	void			add_item	( xui_component* component );
	void			del_item	( xui_component* component );

protected:
	/*
	//callback
	*/
	virtual void	on_invalid	( xui_method_args& args );
	virtual void	on_perform	( xui_method_args& args );

	/*
	//member
	*/
	u08				m_flow;
	s32				m_grap;
};

#endif//__xui_toolbar_h__