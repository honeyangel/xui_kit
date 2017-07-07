#ifndef __cocos_mainform_h__
#define __cocos_mainform_h__

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

//class onity_hierarchy;
class cocos_inspector;
class cocos_project;
//class onity_game;
//class onity_animator;
class cocos_console;
class cocos_timeline;
class cocos_scene;
//class onity_course;
class cocos_mainform : public xui_window
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static cocos_mainform*		get_ptr				( void );
	static std::string			get_pagename		( xui_dockpage* page );
	static xui_dockpage*		get_pagectrl		( const std::string& name );

	/*
	//constructor
	*/
	cocos_mainform( void );

	/*
	//method
	*/
	//onity_hierarchy*			get_hierarchy		( void );
	cocos_inspector*			get_inspector		( void );
	cocos_project*				get_project			( void );
	//onity_animator*				get_animator		( void );
	//onity_game*					get_game			( void );
	cocos_scene*				get_scene			( void );
	cocos_console*				get_console			( void );
	cocos_timeline*				get_timeline		( void );
	//onity_course*				get_course			( void );
	void						add_backupfile		( xui_proproot* prop );
	u32							get_backupfilenum	( void ) const;
	xui_proproot*				get_backupfile		( u32 index ) const;

	/*
	//method
	*/
	bool						was_gamerun			( void ) const;
	bool						was_gameplay		( void ) const;
	void						set_pageshow		( xui_dockpage* page );
	void						set_saveshow		( void );

protected:
	/*
	//callback
	*/
	virtual void				on_load				( xui_method_args&   args );
	virtual void				on_updateself		( xui_method_update& args );

	/*
	//event
	*/
	void						on_paintdebug		( xui_component* sender, xui_method_args&  args );
	void						on_clicktransform	( xui_component* sender, xui_method_args&  args );
	void						on_clickanchor		( xui_component* sender, xui_method_args&  args );
	void						on_clickcoordinate	( xui_component* sender, xui_method_args&  args );
	void						on_clickbuild		( xui_component* sender, xui_method_args&  args );
	void						on_clickdebug		( xui_component* sender, xui_method_args&  args );
	void						on_clickwndmenu		( xui_component* sender, xui_method_args&  args );
	void						on_clicksave		( xui_component* sender, xui_method_args&  args );
	void						on_clickload		( xui_component* sender, xui_method_args&  args );
	void						on_clickreset		( xui_component* sender, xui_method_args&  args );
	void						on_clicksaveall		( xui_component* sender, xui_method_args&  args );
	void						on_mainviewinvalid	( xui_component* sender, xui_method_args&  args );
	void						on_recentaccept		( xui_component* sender, xui_method_args&  args );
	void						on_backupaccept		( xui_component* sender, xui_method_args&  args );
	//void						on_configaccept		( xui_component* sender, xui_method_args&  args );
	//void						on_courseaccept		( xui_component* sender, xui_method_args&  args );
	void						on_globalkeybddown	( xui_component* sender, xui_method_keybd& args );

	/*
	//method
	*/
	void						del_allview			( void );

	/*
	//member
	*/
	xui_panel*					m_toolpane;
	xui_dockview*				m_mainview;
	xui_toggle*					m_select;
	xui_toggle*					m_translate;
	xui_toggle*					m_scale;
	xui_toggle*					m_rotation;
	xui_button*					m_anchor;
	xui_button*					m_coordinate;
	xui_button*					m_build;
	xui_toggle*					m_run;
	xui_toggle*					m_pause;
	xui_button*					m_step;
	xui_button*					m_saveall;
	xui_button*					m_setting;
	xui_toggle*					m_window;
	//xui_menuitem*				m_hierarchy;
	xui_menuitem*				m_inspector;
	xui_menuitem*				m_project;
	xui_menuitem*				m_console;
	xui_menuitem*				m_timeline;
	//xui_menuitem*				m_game;
	xui_menuitem*				m_scene;
	//xui_menuitem*				m_animator;
	xui_menuitem*				m_save;
	xui_menuitem*				m_load;
	xui_menuitem*				m_reset;
	s32							m_steptime;
	std::vector<xui_proproot*>	m_backupfiles;
};

#endif//__cocos_mainform_h__