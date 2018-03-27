#ifndef __cocos_propctrl_flip_h__
#define __cocos_propctrl_flip_h__

#include "xui_propctrl.h"
#include "xui_propdata.h"

class cocos_value_flip
{
public:
	bool	horzflip;
	bool	vertflip;
};

class cocos_propnodebase;
class cocos_propdata_flip : public xui_propdata
{
public:
	/*
	//constructor
	*/
	cocos_propdata_flip( xui_propkind* kind, cocos_propnodebase* propnode );

	/*
	//method
	*/
	cocos_value_flip		get_value		( void );
	void					set_value		( cocos_value_flip& value );

protected:
	/*
	//override
	*/
	virtual u08*			do_serialize	( void );
	virtual void			un_serialize	( u08* byte );

	/*
	//member
	*/
	cocos_propnodebase*		m_propnode;
};

class cocos_propctrl_flip : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*	create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_flip( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( bool selfupdate = false );
	virtual void			on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void			on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void					on_toggleclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_linebox*			m_toolline;
	xui_toggle*				m_horzctrl;
	xui_toggle*				m_vertctrl;
};

#endif//__cocos_propctrl_flip_h__