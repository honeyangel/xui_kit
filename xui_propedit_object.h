#ifndef __xui_propedit_object_h__
#define __xui_propedit_object_h__

#include "xui_propedit.h"

class xui_propedit_object : public xui_propedit_base
{
public:
	xui_propedit_object( xui_propctrl* propctrl, xui_bitmap* icon );

	xui_drawer*		get_pickctrl			( void ) const;
	virtual void	reset					( void );

protected:
	void			on_pickctrlclick		( xui_component* sender, xui_method_mouse& args );
	//void			on_pickctrlrenderself	( xui_component* sender, xui_method_args&  args );

	xui_drawer*		m_pickctrl;
};

#endif//__xui_propedit_object_h__