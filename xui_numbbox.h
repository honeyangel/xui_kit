#ifndef __xui_numbbox_h__
#define __xui_numbbox_h__

#include "xui_textbox.h"

class xui_numbbox : public xui_textbox
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_numbbox( const xui_vector<s32>& size, u08 numbtype, s32 interval );

	/*
	//override
	*/
	virtual xui_rect2d<s32>	get_renderrtins		( void ) const;

protected:
	/*
	//callback
	*/
	virtual void			on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void					on_arrowupdateself	( xui_component* sender, xui_method_update& args );
	void					on_arrowrenderself	( xui_component* sender, xui_method_args&	args );
	void					on_arrowclick		( xui_component* sender, xui_method_args&	args );

	/*
	//member
	*/
	s32						m_interval;
	xui_button*				m_incarrow;
	xui_button*				m_decarrow;
	f32						m_holdtime;
};

#endif//__xui_numbbox_h__