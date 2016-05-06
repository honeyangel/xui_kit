#ifndef __onity_propctrl_pivot_h__
#define __onity_propctrl_pivot_h__

#include "xui_propctrl_vector.h"

class onity_propdata_pivot : public xui_propdata_vector
{
public:
	typedef xui_rect2d<f64>		(*get_rect)( void* userptr );

	/*
	//constructor
	*/
	onity_propdata_pivot( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, get_rect rectget, void* userptr, u08 numbtype, f64 interval = 1 )
	: xui_propdata_vector(kind, name, func, userget, userset, userptr, numbtype, interval)
	, m_rectget(rectget)
	{}

	/*
	//method
	*/
	xui_rect2d<f64>					get_boundbox			( void ) const
	{
		return (*m_rectget)(m_userptr);
	}

protected:
	/*
	//member
	*/
	get_rect						m_rectget;
};

class onity_propctrl_pivot : public xui_propctrl_vector
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*			create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_pivot( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void					on_linkpropdata			( void );

protected:
	/*
	//override
	*/
	virtual void					on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void							on_editctrltextchanged	( xui_component* sender, xui_method_args& args );
	void							on_enumctrlnonfocus		( xui_component* sender, xui_method_args& args );
	void							on_enumctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void							on_enumctrlselection	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_dropbox*					m_subxenum;
	xui_dropbox*					m_subyenum;
};

#endif//__onity_propctrl_pivot_h__