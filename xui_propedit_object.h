#ifndef __xui_propedit_object_h__
#define __xui_propedit_object_h__

#include "xui_propedit.h"

class xui_propedit_object : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	xui_propedit_object( xui_propctrl* propctrl, xui_bitmap* icon );

	/*
	//method
	*/
	xui_drawer*		get_pickctrl			( void ) const;

	/*
	//virtual
	*/
	virtual void	reset					( void );

protected:
	/*
	//event
	*/
	void			on_pickctrlclick		( xui_componet* sender, xui_method_mouse& args );
	void			on_pickctrlrenderself	( xui_componet* sender, xui_method_args&  args );

	/*
	//member
	*/
	xui_drawer*		m_pickctrl;
};

#endif//__xui_propedit_object_h__