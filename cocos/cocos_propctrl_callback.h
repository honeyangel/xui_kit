#ifndef __cocos_propctrl_callback_h__
#define __cocos_propctrl_callback_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propedit.h"

class cocos_value_callback
{
public:
	std::string		type;
	std::string		name;

	bool operator == ( const cocos_value_callback& other )
	{
		return type == other.type && name == other.name;
	}
	bool operator != ( const cocos_value_callback& other )
	{
		return type != other.type || name != other.name;
	}
};

class cocos_propnodewidget;
class cocos_propdata_callback : public xui_propdata
{
public:
	/*
	//constructor
	*/
	cocos_propdata_callback( xui_propkind* kind, cocos_propnodewidget* propnode );

	/*
	//method
	*/
	cocos_value_callback	get_value				( void );
	void					set_value				( const cocos_value_callback& value );

protected:
	/*
	//override
	*/
	virtual u08*			do_serialize			( void );
	virtual void			un_serialize			( u08* byte );

	/*
	//member
	*/
	cocos_propnodewidget*	m_propnode;
};

class cocos_propctrl_callback : public xui_propctrl
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
	cocos_propctrl_callback( xui_propdata* propdata );

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
	//event
	*/
	void					on_editctrlnonfocus		( xui_component* sender, xui_method_args& args );
	void					on_editctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void					on_typectrlselection	( xui_component* sender, xui_method_args& args );
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_dropbox*			m_typectrl;
	xui_textbox*			m_textctrl;
};

#endif//__cocos_propctrl_callback_h__