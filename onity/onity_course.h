#ifndef __onity_course_h__
#define __onity_course_h__

#include "xui_window.h"

class onity_propcourse;
class onity_course : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_course( void );

	/*
	//method
	*/
	void				set_firstcourse			( onity_propcourse* editprop );

protected:
	/*
	//callback
	*/
	virtual void		on_accept				( xui_component* sender, xui_method_args& args );
	virtual void		on_cancel				( xui_component* sender, xui_method_args& args );

	/*
	//event
	*/
	void				on_coursetextmousemove	( xui_component* sender, xui_method_mouse&		args );
	void				on_courseviewkeybddown	( xui_component* sender, xui_method_keybd&		args );
	void				on_courseviewmousedown	( xui_component* sender, xui_method_mouse&		args );
	void				on_courseviewmousedclick( xui_component* sender, xui_method_mouse&		args );
	void				on_courseviewdragover	( xui_component* sender, xui_method_dragdrop&	args );
	void				on_courseviewdragdrop	( xui_component* sender, xui_method_dragdrop&	args );

	/*
	//method
	*/
	void				save_courseconfig		( void );

	/*
	//member
	*/
	xui_button*			m_run;
	xui_button*			m_close;
	xui_drawer*			m_coursetext;
	xui_treeview*		m_courseview;
	xui_panel*			m_bottom;
};

#endif//__onity_course_h__