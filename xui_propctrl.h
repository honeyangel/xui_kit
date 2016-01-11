#ifndef __xui_propctrl_h__
#define __xui_propctrl_h__

#include "xui_control.h"
#include "xui_propdata.h"
#include "xui_propedit.h"

typedef std::vector<xui_propctrl*> xui_propctrl_vec;
class xui_propctrl : public xui_control
{
public:
	/*
	//constructor
	*/
	xui_propctrl( void );

	/*
	//propdata
	*/
	const xui_propdata_vec& get_propdata	( void ) const;
	void					set_propdata	( xui_propdata* propdata );
	void					set_propdata	( const xui_propdata_vec& propdata );
	virtual void			on_linkpropdata	( void ) = 0;
	virtual void			on_editvalue	( xui_propedit* sender ) = 0;

	/*
	//method
	*/
	s32						get_indent		( void );
	xui_kindctrl*			get_kindctrl	( void );
	xui_propview*			get_propview	( void );
	void					add_ctrlelse	( xui_componet* componet );

protected:
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
	virtual void			on_linkpropdata	( void );

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
public:																		\
	static xui_propctrl*	create			( xui_propdata* data )			\
	{																		\
		return new class_name(data);										\
	}																		\
																			\
	class_name( xui_propdata* data );										\
																			\
	virtual void			on_linkpropdata	( void );						\
	virtual void			on_editvalue	( xui_propedit* sender );		\
																			\
protected:																	\
	virtual void			on_perform		( xui_method_args& args );		\
};

xui_propctrl_declare(xui_propctrl_bool)
xui_propctrl_declare(xui_propctrl_enum)
xui_propctrl_declare(xui_propctrl_number)
xui_propctrl_declare(xui_propctrl_string)

///*object*/
//class xui_propctrl_object : public xui_propctrl
//{
//
//};
///*rect2d*/
//class xui_propctrl_rect2d : public xui_propctrl
//{
//
//};
///*colour*/
//class xui_propctrl_colour : public xui_propctrl
//{
//
//};

#endif//__xui_propctrl_h__