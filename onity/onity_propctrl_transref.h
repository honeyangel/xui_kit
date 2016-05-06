#ifndef __onity_propctrl_transref_h__
#define __onity_propctrl_transref_h__

#include "xui_propdata.h"
#include "xui_propctrl_object.h"

class onity_proptransref;
class onity_propdata_transref : public xui_propdata_object_func
{
public:
	/*
	//typedef
	*/
	typedef xui_proproot_vec	(*get_total)( void* userptr );

	/*
	//constructor
	*/
	onity_propdata_transref(
		xui_propkind*			kind,
		const std::wstring&		name,
		xui_prop_newctrl		func,
		const std::string&		droptype,
		onity_proptransref*		proptransref,
		get_total				usergettotal,
		void*					userptrtotal );

	/*
	//method
	*/
	onity_proptransref*			get_proptransref( void );
	xui_proproot_vec			get_proptotal	( void );

protected:
	/*
	//static
	*/
	static xui_bitmap*			get_icon		( xui_propdata* propdata );
	static std::wstring			get_name		( xui_propdata* propdata );
	static void*				get_asset		( void* userptr );
	static void					set_asset		( void* userptr, void* value );		

	/*
	//member
	*/
	onity_proptransref*			m_proptransref;
	get_total					m_usergettotal;
	void*						m_userptrtotal;
};

class onity_propctrl_transref : public xui_propctrl_object
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create				( xui_propdata* propdata );

	/*
	//constructor
	*/
	onity_propctrl_transref( xui_propdata* propdata );

protected:
	/*
	//event
	*/
	void						on_editctrlclick	( xui_component* sender, xui_method_mouse& args );
};

#endif//__onity_propctrl_transref_h__