#ifndef __xui_dialog_h__
#define __xui_dialog_h__

#include "xui_window.h"

enum
{
    k_accept_text,
    k_cancel_text,
    k_detail_text
};

class xui_dialog : public xui_window
{
	xui_declare_rtti

public:
	xui_dialog( const std::wstring& text, s32 num_button );

	void			set_text	( u08 index, const std::wstring& text );

protected:
	virtual void	on_accept	( xui_component* sender, xui_method_args& args );
	virtual void	on_cancel	( xui_component* sender, xui_method_args& args );

	xui_drawer*		m_detail;
	xui_button*		m_accept;
	xui_button*		m_cancel;
};

#endif//__xui_dialog_h__