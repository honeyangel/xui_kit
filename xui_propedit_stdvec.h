#ifndef __xui_propedit_stdvec_h__
#define __xui_propedit_stdvec_h__

#include "xui_propedit.h"

class xui_propedit_stdvec : public xui_propedit_base
{
public:
	xui_propedit_stdvec( xui_propctrl* propctrl );

	u32						get_value				( void ) const;
	void					set_value				( u32 value );
	virtual void			reset					( void );
	void					on_textctrltextenter	( xui_component* sender, xui_method_args&  args );
};

#endif//__xui_propedit_stdvec_h__