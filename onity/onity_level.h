#ifndef __onity_level_h__
#define __onity_level_h__

#include "xui_window.h"

class onity_propcourse;
class onity_level : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_level( void );

	/*
	//method
	*/
	void				set_firstlevel			( onity_propcourse* editprop );

protected:
	/*
	//callback
	*/
	virtual void		on_accept				( xui_component* sender, xui_method_args& args );
	virtual void		on_cancel				( xui_component* sender, xui_method_args& args );

	/*
	//event
	*/
	void				on_leveltextmousemove	( xui_component* sender, xui_method_mouse&		args );
	void				on_levelviewkeybddown	( xui_component* sender, xui_method_keybd&		args );
	void				on_levelviewmousedown	( xui_component* sender, xui_method_mouse&		args );
	void				on_levelviewmousedclick	( xui_component* sender, xui_method_mouse&		args );
	void				on_levelviewdragover	( xui_component* sender, xui_method_dragdrop&	args );
	void				on_levelviewdragdrop	( xui_component* sender, xui_method_dragdrop&	args );

	/*
	//method
	*/
	void				save_levelconfig		( void );

	/*
	//member
	*/
	xui_button*			m_run;
	xui_button*			m_close;
	xui_drawer*			m_leveltext;
	xui_treeview*		m_levelview;
	xui_panel*			m_bottom;
};

#endif//__onity_level_h__