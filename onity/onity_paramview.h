#ifndef __onity_paramview_h__
#define __onity_paramview_h__

#include "xui_control.h"

class NP2DSParam;
class NP2DSStateCtrl;
class onity_param;
class onity_paramview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_paramview( void );

	/*
	//method
	*/
	void						set_editfile		( NP2DSStateCtrl* editfile );
	void						add_paramctrl		( NP2DSParam* param );
	void						del_paramctrl		( NP2DSParam* param );
	void						del_paramctrlall	( void );

	/*
	//virtual
	*/
	virtual xui_rect2d<s32>		get_cornerrt		( void ) const;

protected:
	/*
	//callback
	*/
	virtual void				on_invalid			( xui_method_args& args );
	virtual void				on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void						on_addrenderself	( xui_component* sender, xui_method_args& args );
	void						on_menuitemclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*					m_title;
	xui_toggle*					m_add;
	xui_menuitem*				m_bool;
	xui_menuitem*				m_int;
	xui_menuitem*				m_float;
	std::vector<onity_param*>	m_paramctrl;
	NP2DSStateCtrl*				m_editfile;
};

#endif//__onity_paramview_h__