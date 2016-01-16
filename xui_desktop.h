#ifndef __xui_desktop_h__
#define __xui_desktop_h__

#include "xui_message.h"
#include "xui_panel.h"

class xui_desktop : public xui_panel
{
public:
	/*
	//constructor
	*/
	xui_desktop( const std::string& name, const xui_rect2d<s32>& rect );

	/*
	//destructor
	*/
	virtual ~xui_desktop( void );

	/*
	//ctrl
	*/
	xui_component*				get_catchctrl	( void );
	void						set_catchctrl	( xui_component* component );
	xui_component*				get_focusctrl	( void );
	void						set_focusctrl	( xui_component* component );
	xui_component*				get_hoverctrl	( void );
	void						set_hoverctrl	( xui_component* component );
	xui_component*				get_floatctrl	( void );
	void						set_floatctrl	( xui_component* component );

	/*
	//mouse
	*/
	const xui_vector<s32>&		get_mousedown	( void ) const;
	const xui_vector<s32>&		get_mouselast	( void ) const;
	const xui_vector<s32>&		get_mousecurr	( void ) const;

	/*
	//paste
	*/
	const std::wstring&			get_pastetext	( void ) const;
	void						set_pastetext	( const std::wstring& text );

	/*
	//modal
	*/
	xui_window*					get_modaltop	( void );
	void						add_modalwnd	( xui_window* window );
	void						del_modalwnd	( xui_window* window );

	/*
	//message
	*/
	void						move_recycle	( xui_component* component );
	void						send_message	( const xui_message& message );
	void						post_message	( const xui_message& message );

	/*
	//virtual
	*/
	virtual	void				update			( f32 delta );
	virtual void				render			( void );

	/*
	//os
	*/
	void						os_mousedown	( xui_method_mouse& args );
	void						os_mouserise	( xui_method_mouse& args );
	void						os_mousemove	( xui_method_mouse& args );
	void						os_keybddown	( xui_method_keybd& args );
	void						os_keybdrise	( xui_method_keybd& args );
	void						os_keybdchar	( u08 c );
	void						os_setcursor	( u32 cursor );

protected:
	/*
	//callback
	*/
	virtual void				on_addchild		( xui_method_args&  args );
	virtual void				on_delchild		( xui_method_args&  args );

	/*
	//proc
	*/
	void						proc_message	( void );
	void						proc_recycle	( void );
	void						proc_settext	( void );

protected:
	/*
	//member
	*/
	xui_component*				m_catchctrl;
	xui_component*				m_focusctrl;
	xui_component*				m_hoverctrl;
	xui_component*				m_floatctrl;
	u08							m_allowdrag;
	void*						m_catchdata;
	std::string					m_catchtype;
	std::string					m_inputtext;
	std::wstring				m_pastetext;

	xui_vector<s32>				m_mousedown;
	xui_vector<s32>				m_mouselast;
	xui_vector<s32>				m_mousecurr;
	std::vector<xui_window*>	m_modalpool;

	std::vector<xui_message>	m_messagevec;
	std::vector<xui_component*>	m_recyclebin;
};

extern xui_desktop* g_desktop;

#endif//__xui_desktop_h__