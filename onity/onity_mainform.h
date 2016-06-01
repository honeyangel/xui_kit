#ifndef __onity_mainform_h__
#define __onity_mainform_h__

#include "xui_window.h"

//transform operator anchor
enum
{
	TOA_PIVOT,
	TOA_CENTER,
};

//transform operator coordinate
enum 
{
	TOC_LOCAL,
	TOC_WORLD,
};

class onity_inspector;
class onity_project;
class onity_game;
class onity_animator;
class onity_console;
class onity_timeline;
class onity_mainform : public xui_window
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static onity_mainform*	get_ptr				( void );

	/*
	//constructor
	*/
	onity_mainform( void );

	/*
	//method
	*/
	onity_inspector*		get_inspector		( void );
	onity_project*			get_project			( void );
	onity_animator*			get_animator		( void );
	onity_game*				get_game			( void );
	onity_console*			get_console			( void );
	onity_timeline*			get_timeline		( void );

	/*
	//method
	*/
	void					set_pageshow		( xui_dockpage* page );

protected:
	/*
	//callback
	*/
	virtual void			on_load				( xui_method_args& args );

	/*
	//event
	*/
	void					on_paintdebug		( xui_component* sender, xui_method_args& args );
	void					on_clicktransform	( xui_component* sender, xui_method_args& args );
	void					on_clickanchor		( xui_component* sender, xui_method_args& args );
	void					on_clickcoordinate	( xui_component* sender, xui_method_args& args );
	void					on_clickdebug		( xui_component* sender, xui_method_args& args );
	void					on_clickwndmenu		( xui_component* sender, xui_method_args& args );
	void					on_clicksave		( xui_component* sender, xui_method_args& args );
	void					on_clickload		( xui_component* sender, xui_method_args& args );
	void					on_clickreset		( xui_component* sender, xui_method_args& args );
	void					on_mainviewinvalid	( xui_component* sender, xui_method_args& args );
	void					on_recentaccept		( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void					del_allview			( void );

	/*
	//member
	*/
	xui_panel*				m_toolpane;
	xui_dockview*			m_mainview;
	xui_toggle*				m_select;
	xui_toggle*				m_translate;
	xui_toggle*				m_scale;
	xui_toggle*				m_rotation;
	xui_button*				m_anchor;
	xui_button*				m_coordinate;
	xui_toggle*				m_run;
	xui_toggle*				m_pause;
	xui_button*				m_step;
	xui_toggle*				m_window;
	xui_menuitem*			m_hierarchy;
	xui_menuitem*			m_inspector;
	xui_menuitem*			m_project;
	xui_menuitem*			m_console;
	xui_menuitem*			m_timeline;
	xui_menuitem*			m_game;
	xui_menuitem*			m_scene;
	xui_menuitem*			m_animator;
	xui_menuitem*			m_save;
	xui_menuitem*			m_load;
	xui_menuitem*			m_reset;
};

#endif//__onity_mainform_h__