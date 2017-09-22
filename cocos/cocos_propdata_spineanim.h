#ifndef __cocos_propdata_spineanim_h__
#define __cocos_propdata_spineanim_h__

#include "xui_propctrl.h"

typedef std::vector<std::wstring> cocos_string_vec;
class cocos_propdata_spineanim : public xui_propdata
{
public:
	typedef cocos_string_vec	(*vec_func)( void* userptr );
	typedef std::wstring		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const std::wstring& value );

	/*
	//constructor
	*/
	cocos_propdata_spineanim( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, vec_func uservec, void* userptr );

	/*
	//method
	*/
	virtual std::wstring		get_value		( void ) const;
	virtual void				set_value		( const std::wstring& value );
	virtual cocos_string_vec	vec_value		( void );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	vec_func					m_uservec;
	void*						m_userptr;
};

class cocos_propctrl_spineanim : public xui_propctrl_base
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create			( xui_propdata* data );

	/*
	//constructor
	*/
	cocos_propctrl_spineanim( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void				on_linkpropdata	( bool selfupdate = false );
	virtual void				on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void				on_perform		( xui_method_args& args );
};

#endif//__cocos_propdata_spineanim_h__