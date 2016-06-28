#ifndef __onity_console_h__
#define __onity_console_h__

#include "xui_dockpage.h"

enum 
{
	LOGTYPE_ERROR,
	LOGTYPE_WARNING,
	LOGTYPE_MESSAGE,
	LOGTYPE_MAX,
};

class onity_console : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static void					game_print				( const std::string& log );

	/*
	//constructor
	*/
	onity_console( void );

	/*
	//destructor
	*/
	virtual ~onity_console( void );

	/*
	//method
	*/
	void						add_log					( u08 type, const std::wstring& file, const std::wstring& text );
	void						del_logall				( void );

protected:
	/*
	//event
	*/
	void						on_toggletextchanged	( xui_component* sender, xui_method_args& args );
	void						on_toggleclick			( xui_component* sender, xui_method_args& args );
	void						on_clearclick			( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	void						add_item				( u08 type, xui_listitem* item );
	void						add_item				( u08 type );
	void						del_item				( u08 type );

	/*
	//member
	*/
	xui_button*					m_clear;
	xui_toggle*					m_error;
	xui_toggle*					m_warning;
	xui_toggle*					m_message;
	xui_panel*					m_head;
	xui_listview*				m_list;
	std::vector<xui_listitem*>	m_logvec[LOGTYPE_MAX];
};

#endif//__onity_console_h__