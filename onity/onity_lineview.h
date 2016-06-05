#ifndef __onity_lineview_h__
#define __onity_lineview_h__

#include "xui_control.h"

class onity_propactor;
class onity_proplayer;
class onity_renderview;
class onity_lineview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const u32		tile_size = 80;
	static const u32		horz_grap = 10;
	static const u32		name_size = 20;

	/*
	//constructor
	*/
	onity_lineview( void );

	/*
	//method
	*/
	void					set_curframe				( s32 frame );
	onity_renderview*		get_drawview				( void );
	onity_propactor*		get_viewprop				( void );
	void					set_viewprop				( onity_propactor* viewprop );

protected:
	/*
	//event
	*/
	void					on_drawviewinvalid			( xui_component* sender, xui_method_args&  args );
	void					on_drawviewrenderself		( xui_component* sender, xui_method_args&  args );
	void					on_drawviewmousedown		( xui_component* sender, xui_method_mouse& args );
	void					on_drawviewmousewheel		( xui_component* sender, xui_method_mouse& args );
	void					on_drawviewmousedoubleclick	( xui_component* sender, xui_method_mouse& args );

	/*
	//method
	*/
	void					draw_layer					( u16 index, s32 x, s32 y,   onity_proplayer* proplayer );
	void					draw_tile					( const xui_rect2d<s32>& rt, onity_proplayer* proplayer );
	void					draw_name					( const xui_rect2d<s32>& rt, const std::string& name, u16 index );

	/*
	//member
	*/
	u16						m_selected;
	s32						m_curframe;
	onity_renderview*		m_drawview;
	xui_scroll*				m_viewroll;
	onity_propactor*		m_viewprop;
};

#endif//__onity_lineview_h__