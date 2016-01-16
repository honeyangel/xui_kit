#ifndef __xui_linebox_h__
#define __xui_linebox_h__

#include "xui_control.h"

class xui_linebox : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_linebox( const xui_vector<s32>& size, xui_component* parent );

	/*
	//flow
	*/
	u08						get_flow			( void ) const;
	void					set_flow			( u08 flow );

	/*
	//ctrl
	*/
	void					add_linectrl		( xui_component* component );
	void					del_linectrl		( xui_component* component );
	virtual xui_rect2d<s32>	get_cornerrt		( xui_component* component ) const;

protected:
	/*
	//callback
	*/
	virtual void			on_invalid			( xui_method_args& args );
	virtual void			on_perform			( xui_method_args& args );

	/*
	//member
	*/
	u08						m_flow;
};

#endif//__xui_linebox_h__