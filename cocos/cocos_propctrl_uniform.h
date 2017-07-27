#ifndef __cocos_propctrl_uniform_h__
#define __cocos_propctrl_uniform_h__

#include "xui_propctrl_expand.h"
#include "cocos_propmaterial.h"

class cocos_propdata_uniform : public xui_propdata
{
public:
	/*
	//typedef
	*/
	typedef std::vector<cocos_propmaterial::uniform> 
		uniform_vec;

	/*
	//constructor
	*/
	cocos_propdata_uniform( xui_propkind* kind, const std::wstring& name, uniform_vec* vec );

	/*
	//destructor
	*/
	virtual ~cocos_propdata_uniform( void );

	/*
	//mdthod
	*/
	const xui_propdata_vec&	get_uniformdatavec		( void ) const;
	void					reset					( void );

	/*
	//override
	*/
	virtual void			non_ctrl				( void );

protected:
	/*
	//method
	*/
	xui_propdata*			add_uniformdata			( cocos_propmaterial::uniform* uniform_value );
	xui_propdata*			new_float				( cocos_propmaterial::uniform* uniform_value );
	xui_propdata*			new_vec2				( cocos_propmaterial::uniform* uniform_value );
	xui_propdata*			new_vec3				( cocos_propmaterial::uniform* uniform_value );
	xui_propdata*			new_vec4				( cocos_propmaterial::uniform* uniform_value );
	xui_propdata*			new_mat4				( cocos_propmaterial::uniform* uniform_value );

	/*
	//member
	*/
	uniform_vec*			m_uniformvec;
	xui_propdata_vec		m_propdatavec;
};

class cocos_propctrl_uniform : public xui_propctrl
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
	cocos_propctrl_uniform( xui_propdata* propdata );

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

#endif//__cocos_propctrl_uniform_h__