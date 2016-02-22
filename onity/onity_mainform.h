#ifndef __onity_mainform_h__
#define __onity_mainform_h__

#include "xui_window.h"

//transform operator anchor
enum
{
	TOA_PIVOT,
	TOA_CENTER,
};

//transform operator coordinate
enum 
{
	TOC_LOCAL,
	TOC_WORLD,
};

class onity_mainform : public xui_window
{
public:
	/*
	//constructor
	*/
	onity_mainform( void );

protected:
	/*
	//event
	*/
	void				on_clicktransform	( xui_component* sender, xui_method_args& args );
	void				on_clickanchor		( xui_component* sender, xui_method_args& args );
	void				on_clickcoordinate	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_toggle*			m_select;
	xui_toggle*			m_translate;
	xui_toggle*			m_scale;
	xui_toggle*			m_rotation;
	xui_toggle*			m_anchor;
	xui_toggle*			m_coordinate;
	xui_toggle*			m_run;
	xui_toggle*			m_pause;
	xui_toggle*			m_layout;
};

#endif//__onity_mainform_h__