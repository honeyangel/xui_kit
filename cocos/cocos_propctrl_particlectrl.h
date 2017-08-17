#ifndef __cocos_propctrl_particlectrl_h__
#define __cocos_propctrl_particlectrl_h__

#include "xui_propctrl.h"
#include "xui_propdata.h"

class cocos_propnodebase;
class cocos_propdata_particlectrl : public xui_propdata
{
public:
	/*
	//constructor
	*/
	cocos_propdata_particlectrl( xui_propkind* kind, cocos_propnodebase* propnode );

	/*
	//method
	*/
	cocos_propnodebase*		get_propnode	( void );

protected:
	/*
	//member
	*/
	cocos_propnodebase*		m_propnode;
};

class cocos_propctrl_particlectrl : public xui_propctrl
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
	cocos_propctrl_particlectrl( xui_propdata* propdata );

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
	void					on_buttonclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_linebox*			m_toolline;
	xui_button*				m_playctrl;
	xui_button*				m_stopctrl;
	xui_button*				m_loadctrl;
};

#endif//__cocos_propctrl_particlectrl_h__