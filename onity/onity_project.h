#ifndef __onity_project_h__
#define __onity_project_h__

#include "xui_dockpage.h"

class onity_project : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_project( void );

	/*
	//method
	*/
	void			ini_pathtree		( void );

	/*
	//notify
	*/
	void			on_pathrename		( const std::wstring& last, const std::wstring& curr, const std::wstring& text );

protected:
	/*
	//event
	*/
	void			on_clearclick		( xui_component* sender, xui_method_args&  args );
	void			on_searchtextchanged( xui_component* sender, xui_method_args&  args );
	void			on_searchtextenter	( xui_component* sender, xui_method_args&  args );
	void			on_headperform		( xui_component* sender, xui_method_args&  args );
	void			on_fillperform		( xui_component* sender, xui_method_args&  args );
	void			on_fillrenderelse	( xui_component* sender, xui_method_args&  args );
	void			on_sizectrlmousemove( xui_component* sender, xui_method_mouse& args );
	void			on_lineviewdclick	( xui_component* sender, xui_method_mouse& args );
	void			on_treeviewselection( xui_component* sender, xui_method_args&  args );
	void			on_folderclick		( xui_component* sender, xui_method_args&  args );
	void			on_controllerclick	( xui_component* sender, xui_method_args&  args );
	void			on_pathitemclick	( xui_component* sender, xui_method_args&  args );
	void			on_pathtogglerender	( xui_component* sender, xui_method_args&  args );
	void			on_sliderscroll		( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void			refresh_lineview	( void );
	void			refresh_pathpane	( void );

	/*
	//member
	*/
	xui_panel*		m_head;
	xui_toggle*		m_create;
	xui_menuitem*	m_folder;
	xui_menuitem*	m_controller;
	xui_textbox*	m_search;
	xui_button*		m_clear;

	xui_panel*		m_fill;
	xui_control*	m_sizectrl;
	xui_treeview*	m_pathview;
	xui_treeview*	m_lineview;
	xui_panel*		m_drawview;
	xui_panel*		m_pathpane;
	xui_panel*		m_toolpane;
	xui_slider*		m_slider;
	xui_drawer*		m_status;
};

#endif//__onity_project_h__