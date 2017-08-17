#ifndef __cocos_blankbox_h__
#define __cocos_blankbox_h__

#include "xui_propdata.h"

class cocos_bounding;
class cocos_blankbox
{
public:
	/*
	//typedef
	*/
	typedef std::vector<cocos_boundbox*> (*get_func)( void );

	/*
	//constructor
	*/
	cocos_blankbox( const xui_vector<s32>& button_size, get_func func );

	/*
	//method
	*/
	xui_linebox*		get_horzline		( void );
	xui_linebox*		get_vertline		( void );
	void				set_lineupdate		( void );

protected:
	/*
	//event
	*/
	void				on_horzbuttonclick	( xui_component* sender, xui_method_args& args );
	void				on_vertbuttonclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*			m_horzinc;
	xui_button*			m_horzdec;
	xui_button*			m_horzcancel;
	xui_button*			m_horzequal;
	xui_button*			m_vertinc;
	xui_button*			m_vertdec;
	xui_button*			m_vertcancel;
	xui_button*			m_vertequal;
	xui_linebox*		m_horzline;
	xui_linebox*		m_vertline;
	get_func			m_func;
};

#endif//__cocos_blankbox_h__