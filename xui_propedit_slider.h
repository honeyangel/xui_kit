#ifndef __xui_propedit_slider_h__
#define __xui_propedit_slider_h__

#include "xui_propedit.h"

class xui_propedit_slider : public xui_propedit
{
public:
	xui_propedit_slider( xui_propctrl* propctrl, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue );
	virtual ~xui_propedit_slider( void );

	xui_propedit_number*	get_editnumb			( void ) const;
	xui_slider*				get_spinctrl			( void ) const;
	f64						get_value				( void ) const;
	void					set_value				( f64 value );

	virtual void			reset					( void );
	void					on_spinctrlgetfocus		( xui_component* sender, xui_method_args& args );
	void					on_spinctrlnonfocus		( xui_component* sender, xui_method_args& args );
	void					on_namectrlmouserise	( xui_component* sender, xui_method_mouse& args );
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args& args );
	void					on_textctrltextenter	( xui_component* sender, xui_method_args& args );
	void					on_spinctrlscroll		( xui_component* sender, xui_method_args& args );

protected:
	void					check_value				( void );

	f64						m_minvalue;
	f64						m_maxvalue;
	xui_propedit_number*	m_editnumb;
	xui_slider*				m_spinctrl;
};

#endif//__xui_propedit_slider_h__