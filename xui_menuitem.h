#ifndef __xui_menuitem_h__
#define __xui_menuitem_h__

#include "xui_drawer.h"

class xui_menuitem : public xui_drawer
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_menuitem( void );

	/*
	//destructor
	*/
	virtual ~xui_menuitem( void );

	/*
	//method
	*/
	s32							get_maxwidth	( void );
	xui_menu*					get_submenu		( void );
	void						set_submenu		( xui_menu* submenu );

	/*
	//override
	*/
	virtual xui_colour			get_rendercolor	( void ) const;

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_click;

protected:
	/*
	//callback
	*/
	virtual void				on_nonfocus		( xui_method_args&  args );
	virtual void				on_renderself	( xui_method_args&  args );
	virtual void				on_mouseenter	( xui_method_mouse& args );
	virtual void				on_mouseclick	( xui_method_mouse& args );

	/*
	//member
	*/
	xui_menu*					m_submenu;
};

#endif//__xui_menuitem_h__