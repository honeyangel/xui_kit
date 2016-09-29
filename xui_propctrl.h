#ifndef __xui_propctrl_h__
#define __xui_propctrl_h__

#include "xui_control.h"
#include "xui_propdata.h"
#include "xui_propedit.h"

typedef std::vector<xui_propctrl*> xui_propctrl_vec;
class xui_propctrl : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_propctrl( void );

	/*
	//propdata
	*/
	const xui_propdata_vec& get_propdata	( void ) const;
	bool					has_propdata	( xui_propdata* propdata ) const;
	void					set_propdata	( xui_propdata* propdata );
	void					set_propdata	( const xui_propdata_vec& propdata );
	void					del_propdata	( void );
	virtual void			on_linkpropdata	( bool selfupdate = false ) = 0;
	virtual void			on_editvalue	( xui_propedit* sender ) = 0;

	/*
	//method
	*/
	s32						get_indent		( void );
	xui_kindctrl*			get_kindctrl	( void );
	xui_propview*			get_propview	( void );
	xui_component*			get_ctrlelse	( void );
	void					add_ctrlelse	( xui_component* component );

protected:
	/*
	//callback
	*/
	virtual void			on_lock			( xui_method_args&   args );
	virtual void			on_updateself	( xui_method_update& args );

	/*
	//member
	*/
	xui_propdata*			m_propdata;
	xui_propdata_vec		m_propdatavec;
};

/*
//base
*/
class xui_propctrl_base : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_propctrl_base( void );

	/*
	//destructor
	*/
	virtual ~xui_propctrl_base( void );

	/*
	//propdata
	*/
	virtual void			on_linkpropdata	( bool selfupdate = false );

protected:
	/*
	//override
	*/
	virtual void			on_perform		( xui_method_args& args );

	/*
	//member
	*/
	xui_propedit_base*		m_propedit;
};

#define xui_propctrl_declare(class_name)									\
class class_name : public xui_propctrl_base									\
{																			\
	xui_declare_rtti														\
																			\
public:																		\
	static xui_propctrl*	create			( xui_propdata* data )			\
	{																		\
		return new class_name(data);										\
	}																		\
																			\
	class_name( xui_propdata* data );										\
																			\
	virtual void			on_linkpropdata	( bool selfupdate = false );		\
	virtual void			on_editvalue	( xui_propedit* sender );		\
																			\
protected:																	\
	virtual void			on_perform		( xui_method_args& args );	\
};

xui_propctrl_declare(xui_propctrl_bool)
xui_propctrl_declare(xui_propctrl_enum)
xui_propctrl_declare(xui_propctrl_number)
xui_propctrl_declare(xui_propctrl_string)
xui_propctrl_declare(xui_propctrl_colour)

#endif//__xui_propctrl_h__