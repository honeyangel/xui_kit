#ifndef __onity_selector_h__
#define __onity_selector_h__

#include "xui_propctrl_object.h"

class onity_fileview;
class onity_preview;
class onity_selector : public xui_object_pickwnd
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_object_pickwnd*	get_ptr					( void );

	/*
	//constructor
	*/
	onity_selector( void );

	/*
	//override
	*/
	virtual void*				get_value				( void );
	virtual void				set_value				( void* value );

protected:
	/*
	//callback
	*/
	virtual void				on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void						on_searchtextchanged	( xui_component* sender, xui_method_args&  args );
	void						on_backpathclick		( xui_component* sender, xui_method_args&  args );
	void						on_assetclick			( xui_component* sender, xui_method_args&  args );
	void						on_sceneclick			( xui_component* sender, xui_method_args&  args );
	void						on_sliderscroll			( xui_component* sender, xui_method_args&  args );
	void						on_fileviewselection	( xui_component* sender, xui_method_args&  args );
	void						on_fileviewdoubleclk	( xui_component* sender, xui_method_mouse& args );
	void						on_sizectrlmousemove	( xui_component* sender, xui_method_mouse& args );
	void						on_clearclick			( xui_component* sender, xui_method_args&  args );
	void						on_headerperform		( xui_component* sender, xui_method_args&  args );
	void						on_middleperform		( xui_component* sender, xui_method_args&  args );
	void						on_bottomperform		( xui_component* sender, xui_method_args&  args );

	/*
	//method
	*/
	void						refresh_fileview		( void );
	void						refresh_backpath		( void );
	std::wstring				convert_dropsuff		( const std::string& droptype );

	/*
	//member
	*/
	xui_drawer*					m_title;
	xui_textbox*				m_search;
	xui_button*					m_clear;
	xui_panel*					m_header;
	xui_panel*					m_middle;
	xui_toggle*					m_asset;
	xui_toggle*					m_scene;
	xui_slider*					m_slider;
	xui_linebox*				m_filter;
	xui_button*					m_backpath;
	onity_fileview*				m_fileview;
	xui_control*				m_sizectrl;
	onity_preview*				m_drawview;
	xui_panel*					m_bottom;
	xui_button*					m_accept;
	xui_button*					m_cancel;
};

#endif//__onity_selector_h__