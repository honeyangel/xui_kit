#ifndef __onity_login_h__
#define __onity_login_h__

#include "xui_window.h"

class onity_config : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_config( void );

protected:
	/*
	//callback
	*/
	virtual void		on_perform			( xui_method_args& args );
	virtual void		on_accept			( xui_component* sender, xui_method_args& args );

	/*
	//event
	*/
	void				on_buttonclick		( xui_component* sender, xui_method_args& args );
	void				on_usernameselection( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void				load_config			( void );
	void				save_config			( void );

	/*
	//member
	*/
	xui_button*			m_accept;
	xui_drawer*			m_servtext;
	xui_dropbox*		m_servlist;
	xui_drawer*			m_account;
	xui_drawer*			m_qqstring;
	xui_drawer*			m_pwstring;
	xui_dropbox*		m_username;
	xui_textbox*		m_password;
	xui_drawer*			m_other;
	xui_drawer*			m_version;
	xui_textbox*		m_vernumb;
	xui_drawer*			m_patchtext;
	xui_drawer*			m_legaltext;
	xui_toggle*			m_skippatch;
	xui_toggle*			m_skiplegal;
};

#endif//__onity_config_h__