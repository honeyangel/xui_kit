#ifndef __xui_linebox_h__
#define __xui_linebox_h__

#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_textbox.h"

class xui_linebox : public xui_control
{
public:
	/*
	//constructor
	*/
	xui_linebox( const std::string& name, const xui_rect2d<s32>& rect );

	/*
	//flow
	*/
	u08						get_flow			( void ) const;
	void					set_flow			( u08 flow );

	/*
	//ctrl
	*/
	void					add_linectrl		( xui_component* componet );
	void					del_linectrl		( xui_component* componet );
	virtual xui_rect2d<s32>	get_cornerrt		( xui_component* componet ) const;

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

//class xui_linebutton : public xui_button
//{
//public:
//	/*
//	//constructor
//	*/
//	xui_linebutton( const std::string& name, const xui_rect2d<s32>& rect )
//	: xui_button(name, rect)
//	{
//		m_type += "linebutton";
//	}
//
//	/*
//	//virtual
//	*/
//	virtual xui_rect2d<s32>	get_cornerrt( void ) const
//	{
//		xui_linebox* linebox = (xui_linebox*)m_parent;
//		return linebox->get_linectrlcorner((xui_component*)this);
//	}
//};
//
//class xui_linetoggle : public xui_toggle
//{
//public:
//	/*
//	//constructor
//	*/
//	xui_linetoggle( const std::string& name, const xui_rect2d<s32>& rect )
//	: xui_toggle(name, rect)
//	{
//		m_type += "linetoggle";
//	}
//
//	/*
//	//virtual
//	*/
//	virtual xui_rect2d<s32>	get_cornerrt( void ) const
//	{
//		xui_linebox* linebox = (xui_linebox*)m_parent;
//		return linebox->get_linectrlcorner((xui_component*)this);
//	}
//};
//
//class xui_linetextbox : public xui_textbox
//{
//public:
//	/*
//	//constructor
//	*/
//	xui_linetextbox( const std::string& name, const xui_rect2d<s32>& rect )
//	: xui_textbox(name, rect)
//	{
//		m_type += "linetextbox";
//	}
//
//	/*
//	//virtual
//	*/
//	virtual xui_rect2d<s32>	get_cornerrt( void ) const
//	{
//		xui_linebox* linebox = (xui_linebox*)m_parent;
//		return linebox->get_linectrlcorner((xui_component*)this);
//	}
//};

#endif//__xui_linebox_h__