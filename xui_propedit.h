#ifndef __xui_propedit_h__
#define __xui_propedit_h__

#include "xui_propdata.h"
#include "xui_control.h"

class xui_propedit
{
public:
	/*
	//constructor
	*/
	xui_propedit( xui_propctrl* propctrl );

	/*
	//virtual
	*/
	virtual void			reset					( void ) = 0;

protected:
	/*
	//member
	*/
	xui_propctrl*			m_propctrl;
};

class xui_propedit_base : public xui_propedit
{
public:
	/*
	//constructor
	*/
	xui_propedit_base( xui_propctrl* propctrl );

	/*
	//method
	*/
	xui_drawer*				get_namectrl			( void ) const;
	xui_control*			get_editctrl			( void ) const;

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	virtual void			on_editctrlnonfocus		( xui_component* sender, xui_method_args& args );
	virtual void			on_editctrlgetfocus		( xui_component* sender, xui_method_args& args );
	virtual void			on_namectrltextchanged	( xui_component* sender, xui_method_args& args );

protected:
	/*
	//member
	*/
	xui_drawer*				m_namectrl;
	xui_control*			m_editctrl;
};

class xui_propedit_bool : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_bool( xui_propctrl* propctrl );

	/*
	//method
	*/
	bool					get_value				( void ) const;
	void					set_value				( bool value );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	virtual void			on_editctrlnonfocus		( xui_component* sender, xui_method_args&  args );
	virtual void			on_editctrlgetfocus		( xui_component* sender, xui_method_args&  args );
	void					on_boolctrlclick		( xui_component* sender, xui_method_args&  args );
};

class xui_propedit_enum : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_enum( xui_propctrl* propctrl, const xui_propenum_map& textmap );

	/*
	//method
	*/
	u32						get_value				( void ) const;
	void					set_value				( u32 index );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_enumctrlselection	( xui_component* sender, xui_method_args&  args );
};

class xui_propedit_number : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_number( xui_propctrl* propctrl, u08 numbtype, f64 interval, bool showarrow );

	/*
	//method
	*/
	f64						get_interval			( void ) const;
	f64						get_value				( void ) const;
	void					set_value				( f64 value );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args&  args );
	//void					on_textctrlmousewheel	( xui_component* sender, xui_method_mouse& args );
	void					on_namectrlmousemove	( xui_component* sender, xui_method_mouse& args );

protected:
	/*
	//member
	*/
	f64						m_interval;
};

class xui_propedit_string : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_string( xui_propctrl* propctrl );

	/*
	//value
	*/
	const std::wstring&		get_value	( void ) const;
	void					set_value	( const std::wstring& value );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args&  args );
};

#endif//__xui_propedit_h__