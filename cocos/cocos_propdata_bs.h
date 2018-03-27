#ifndef __cocos_propdata_bs_h__
#define __cocos_propdata_bs_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propedit.h"

class cocos_value_bs
{
public:
	cocos_value_bs( void )
	: flag(false)
	{}
	cocos_value_bs( bool _flag, const std::wstring& _text )
	: flag(_flag)
	, text(_text)
	{}

	bool operator == ( const cocos_value_bs& other )
	{
		return (flag == other.flag &&
				text == other.text);
	}
	bool operator != ( const cocos_value_bs& other )
	{
		return (flag != other.flag ||
				text != other.text);
	}

	bool			flag;
	std::wstring	text;
};

class cocos_propdata_bs : public xui_propdata
{
public:
	typedef cocos_value_bs	(*get_func)( void* userptr );
	typedef void			(*set_func)( void* userptr, const cocos_value_bs& value );

	/*
	//constructor
	*/
	cocos_propdata_bs( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval );

	/*
	//method
	*/
	u08						get_numbtype			( void ) const;
	f64						get_interval			( void ) const;
	virtual cocos_value_bs	get_value				( void ) const;
	virtual void			set_value				( const cocos_value_bs& value );

protected:
	/*
	//override
	*/
	virtual u08*			do_serialize			( void );
	virtual void			un_serialize			( u08* byte );

	/*
	//member
	*/
	get_func				m_userget;
	set_func				m_userset;
	void*					m_userptr;
	u08						m_numbtype;
	f64						m_interval;
};

class cocos_propctrl_bs : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_bs( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~cocos_propctrl_bs( void );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_perform				( xui_method_args& args );

	/*
	//member
	*/
	xui_propedit_bool*		m_booledit;
	xui_propedit_number*	m_textedit;
};

#endif//__cocos_propdata_bs_h__