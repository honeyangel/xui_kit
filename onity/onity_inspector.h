#ifndef __onity_inspector_h__
#define __onity_inspector_h__

#include "xui_dockpage.h"
#include "xui_propview.h"

class onity_preview;
class onity_inspector : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_inspector( void );

	/*
	//method
	*/
	onity_preview*		get_preview			( void );
	void				set_proproot		( xui_proproot* proproot );
	void				set_proproot		( const xui_proproot_vec& proproot );

protected:
	/*
	//callback
	*/
	virtual void		on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void				on_sizectrlmousemove( xui_component* sender, xui_method_mouse& args );

	/*
	//member
	*/
	xui_propview*		m_propview;
	onity_preview*		m_drawview;
	xui_control*		m_sizectrl;
};

#endif//__onity_inspector_h__