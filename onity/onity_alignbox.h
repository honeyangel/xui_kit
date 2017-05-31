#ifndef __onity_alignbox_h__
#define __onity_alignbox_h__

#include "xui_propdata.h"

class onity_bounding;
class onity_alignbox
{
public:
	/*
	//typedef
	*/
	typedef std::vector<onity_bounding*> (*get_func)( void );

	/*
	//constructor
	*/
	onity_alignbox( const xui_vector<s32>& button_size, get_func func );

	/*
	//method
	*/
	xui_linebox*	get_horzline		( void );
	xui_linebox*	get_vertline		( void );
	void			set_lineupdate		( void );

protected:
	/*
	//event
	*/
	void			on_horzbuttonclick	( xui_component* sender, xui_method_args& args );
	void			on_vertbuttonclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*		m_left;
	xui_button*		m_hcenter;
	xui_button*		m_right;
	xui_button*		m_top;
	xui_button*		m_vcenter;
	xui_button*		m_bottom;
	xui_linebox*	m_horzline;
	xui_linebox*	m_vertline;
	get_func		m_func;
};

#endif//__onity_alignbox_h__