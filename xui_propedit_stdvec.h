#ifndef __xui_propedit_stdvec_h__
#define __xui_propedit_stdvec_h__

#include "xui_propedit.h"

class xui_propedit_stdvec : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_stdvec( xui_propctrl* propctrl );

	/*
	//method
	*/
	u32						get_value				( void ) const;
	void					set_value				( u32 value );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_textctrltextenter	( xui_componet* sender, xui_method_args&  args );
};

#endif//__xui_propedit_stdvec_h__