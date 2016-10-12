#ifndef __onity_gradpane_h__
#define __onity_gradpane_h__

#include "xui_drawer.h"

class onity_gradpane : public xui_drawer
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_gradpane( s32 flowstyle );

	/*
	//destructor
	*/
	virtual ~onity_gradpane( void );

	/*
	//method
	*/
	const std::vector<s32>&	get_lines			( void ) const;
	void					del_lines			( void );
	s32						get_trans			( void ) const;
	void					set_trans			( s32 trans );
	f64						get_ratio			( void ) const;
	void					set_ratio			( f64 ratio );

protected:
	/*
	//override
	*/
	virtual void			on_mousedown		( xui_method_mouse& args );
	virtual void			on_mousemove		( xui_method_mouse& args );
	virtual void			on_mouserise		( xui_method_mouse& args );
	virtual void			on_mousedoubleclick	( xui_method_mouse& args );
	virtual void			on_renderself		( xui_method_args&  args );

	/*
	//event
	*/
	void					on_menuclick		( xui_component* sender, xui_method_args& args );
	void					on_acceptsetval		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	s32						m_trans;
	f64						m_ratio;
	s32						m_flowstyle;
	u32						m_downindex;
	std::vector<s32>		m_linearray;
	xui_menu*				m_rightmenu;
	xui_menuitem*			m_setval;
	xui_menuitem*			m_delete;
	xui_menuitem*			m_delall;
};

#endif//__onity_gradpane_h__