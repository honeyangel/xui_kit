#ifndef __cocos_backup_h__
#define __cocos_backup_h__

#include "xui_window.h"

class cocos_backup : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_backup( void );

protected:
	/*
	//callback
	*/
	virtual void		on_updateself			( xui_method_update& args );
	virtual void		on_accept				( xui_component* sender, xui_method_args&  args );

	/*
	//event
	*/
	void				on_toggleclick			( xui_component* sender, xui_method_args&  args );
	void				on_buttonclick			( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void				load					( void );
	void				restore					( void );
	
	/*
	//member
	*/
	xui_drawer*			m_header;
	xui_treeview*		m_backup;
	xui_panel*			m_bottom;
	xui_toggle*			m_selectall;
	xui_drawer*			m_selecttxt;
	xui_button*			m_accept;
	xui_button*			m_cancel;
};

#endif//__cocos_restore_h__