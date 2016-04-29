#ifndef __xui_propedit_colour_h__
#define __xui_propedit_colour_h__

#include "xui_propedit.h"

class xui_propedit_colour : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_colour( xui_propctrl* propctrl );

	/*
	//method
	*/
	xui_colour				get_value				( void ) const;
	void					set_value				( const xui_colour& value );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_textctrltextenter	( xui_component* sender, xui_method_args&  args );
};

#endif//__xui_propedit_stdvec_h__