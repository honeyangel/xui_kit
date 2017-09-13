#ifndef __cocos_propdata_unitvec_h__
#define __cocos_propdata_unitvec_h__

#include "xui_propctrl.h"
#include "cocos_propedit_unit.h"

class cocos_value_unitvec
{
public:
	cocos_value_unitvec( void )
	{}
	cocos_value_unitvec( const cocos_value_unit& _x, const cocos_value_unit& _y )
	: x(_x)
	, y(_y)
	{}

	bool operator == ( const cocos_value_unitvec& other )
	{ return x == other.x && y == other.y; }
	bool operator != ( const cocos_value_unitvec& other )
	{ return x != other.x || y != other.y; }

	union
	{
		struct  
		{
			cocos_value_unit x;
			cocos_value_unit y;
		};
		struct  
		{
			cocos_value_unit w;
			cocos_value_unit h;
		};
	};
};

class cocos_propdata_unitvec : public xui_propdata
{
public:
	typedef cocos_value_unitvec	(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const cocos_value_unitvec& value );

	/*
	//constructor
	*/
	cocos_propdata_unitvec( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, get_func userdef, void* userptr );

	/*
	//method
	*/
	bool						def_exist		( void );
	virtual cocos_value_unitvec	get_value		( void ) const;
	virtual void				set_value		( const cocos_value_unitvec& value );
	virtual void				def_value		( void );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	get_func					m_userdef;
	void*						m_userptr;
};

class cocos_propctrl_unitvec : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_propctrl_unitvec( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~cocos_propctrl_unitvec( void );

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

	/*
	//member
	*/
	xui_drawer*					m_namectrl;
	cocos_propedit_unit*		m_subxedit;
	cocos_propedit_unit*		m_subyedit;
};

class cocos_propctrl_unitvecpt : public cocos_propctrl_unitvec
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*		create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_unitvecpt( xui_propdata* propdata );
};
class cocos_propctrl_unitvecsz : public cocos_propctrl_unitvec
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*		create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_unitvecsz( xui_propdata* propdata );

protected:
	/*
	//override
	*/
	virtual void				on_invalid		( xui_method_args& args );
	virtual void				on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void						on_defvalueclick( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*					m_defvalue;
};

#endif//__cocos_propdata_unitvec_h__