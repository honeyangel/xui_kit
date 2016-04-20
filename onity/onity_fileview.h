#ifndef __onity_fileview_h__
#define __onity_fileview_h__

#include "xui_control.h"

class onity_tileview;
class onity_fileview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_fileview( void );

	/*
	//method
	*/
	xui_treeview*					get_lineview			( void );
	onity_tileview*					get_tileview			( void );
	void							clear					( void );

	/*
	//method
	*/
	xui_method<xui_method_mouse>	xm_lineviewdoubleclk;
	xui_method<xui_method_args >	xm_lineviewselection;

protected:
	/*
	//event
	*/
	void							on_lineviewshow			( xui_component* sender, xui_method_args&  args );
	void							on_tileviewshow			( xui_component* sender, xui_method_args&  args );
	void							on_lineviewselection	( xui_component* sender, xui_method_args&  args );
	void							on_lineviewdoubleclk	( xui_component* sender, xui_method_mouse& args );

	/*
	//member
	*/
	xui_treeview*					m_lineview;
	onity_tileview*					m_tileview;
};

#endif//__onity_fileview_h__