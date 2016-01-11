#ifndef __xui_propedit_slider_h__
#define __xui_propedit_slider_h__

#include "xui_propedit.h"

class xui_propedit_slider : public xui_propedit
{
public:
	/*
	//constructor
	*/
	xui_propedit_slider( xui_propctrl* propctrl, f64 interval, f64 minvalue, f64 maxvalue );

	/*
	//destructor
	*/
	virtual ~xui_propedit_slider( void );

	/*
	//method
	*/
	xui_propedit_number*	get_editnumb			( void ) const;
	xui_slider*				get_spinctrl			( void ) const;

	/*
	//value
	*/
	f64						get_value				( void ) const;
	void					set_value				( f64 value );

	/*
	//virtual
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_spinctrlgetfocus		( xui_componet* sender, xui_method_args& args );
	void					on_spinctrlnonfocus		( xui_componet* sender, xui_method_args& args );
	void					on_namectrlmouserise	( xui_componet* sender, xui_method_mouse& args );
	void					on_textctrltextchanged	( xui_componet* sender, xui_method_args& args );
	void					on_textctrltextenter	( xui_componet* sender, xui_method_args& args );
	void					on_spinctrlscroll		( xui_componet* sender, xui_method_args& args );

protected:
	/*
	//method
	*/
	void					check_value				( void );

	/*
	//member
	*/
	f64						m_minvalue;
	f64						m_maxvalue;
	xui_propedit_number*	m_editnumb;
	xui_slider*				m_spinctrl;
};

#endif//__xui_propedit_slider_h__