#ifndef __onity_selector_h__
#define __onity_selector_h__

#include "xui_propctrl_object.h"

class onity_fileview;
class onity_preview;
class onity_selector : public xui_pickwnd
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_pickwnd*		get_ptr		( void );

	/*
	//constructor
	*/
	onity_selector( void );

	/*
	//override
	*/
	virtual void*			get_value	( void );
	virtual void			set_value	( void* value );

protected:
	/*
	//member
	*/
	xui_drawer*				m_title;
	xui_textbox*			m_search;
	xui_panel*				m_middle;
	xui_toggle*				m_asset;
	xui_toggle*				m_scene;
	xui_slider*				m_slider;
	onity_fileview*			m_fileview;
	xui_control*			m_sizectrl;
	onity_preview*			m_drawview;
	xui_panel*				m_bottom;
	xui_button*				m_accept;
	xui_button*				m_cancel;
};

#endif//__onity_selector_h__