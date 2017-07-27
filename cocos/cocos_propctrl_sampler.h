#ifndef __cocos_propctrl_sampler_h__
#define __cocos_propctrl_sampler_h__

#include "xui_propctrl_expand.h"
#include "cocos_propmaterial.h"

class cocos_propdata_sampler : public xui_propdata
{
public:
	/*
	//typedef
	*/
	typedef std::vector<cocos_propmaterial::sampler> 
		sampler_vec;

	/*
	//constructor
	*/
	cocos_propdata_sampler( xui_propkind* kind, const std::wstring& name, sampler_vec* vec );

	/*
	//destructor
	*/
	virtual ~cocos_propdata_sampler( void );

	/*
	//mdthod
	*/
	const xui_propdata_vec&	get_samplerdatavec		( void ) const;
	void					reset					( void );

	/*
	//override
	*/
	virtual void			non_ctrl				( void );

protected:
	/*
	//method
	*/
	void					add_samplerdata			( cocos_propmaterial::sampler* sampler_value );

	/*
	//member
	*/
	sampler_vec*			m_samplervec;
	xui_propdata_vec		m_propdatavec;
};

class cocos_propctrl_sampler : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*	create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_sampler( xui_propdata* propdata );

	/*
	//override
	*/
	virtual void			on_linkpropdata			( bool selfupdate = false );
	virtual void			on_editvalue			( xui_propedit* sender );

protected:
	/*
	//callback
	*/
	virtual void			on_renderback			( xui_method_args& args );
	virtual void			on_invalid				( xui_method_args& args );
	virtual void			on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void					on_middleinvalid		( xui_component* sender, xui_method_args& args );
	void					on_middleselection		( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*				m_header;
	xui_listview*			m_middle;
	xui_drawer*				m_nontip;
	xui_propctrl_expand*	m_expand;
};

#endif//__cocos_propctrl_sampler_h__