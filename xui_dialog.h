#ifndef __xui_dialog_h__
#define __xui_dialog_h__

#include "xui_window.h"

class xui_dialog : public xui_window
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_dialog( const std::wstring& text, s32 num_button );

	/*
	//method
	*/
	void			set_text	( u32 index, const std::wstring& text );

protected:
	/*
	//callback
	*/
	virtual void	on_accept	( xui_component* sender, xui_method_args& args );
	virtual void	on_cancel	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_drawer*		m_detail;
	xui_button*		m_accept;
	xui_button*		m_cancel;
};

#endif//__xui_dialog_h__