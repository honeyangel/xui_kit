#ifndef __cocos_preview_h__
#define __cocos_preview_h__

#include "xui_control.h"
#include "xui_action_ctrl.h"

namespace cocos2d
{
	class Sprite;
	class ParticleSystem;
	namespace ui
	{
		class TextBMFont;
		class SpineNode;
	}
}

class cocos_renderview;
class cocos_preview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_preview( void );

	/*
	//destructor
	*/
	virtual ~cocos_preview( void );

	/*
	//method
	*/
	cocos2d::Node*				get_drawnode		( void );
	cocos_renderview*			get_drawview		( void );
	void						set_toolshow		( bool flag );
	void						set_needshow		( void );
	void						set_viewprop		( xui_proproot* prop, bool play = true );
	void						set_drawrect		( const xui_rect2d<s32>& rect );

protected:
	/*
	//callback
	*/
	virtual void				on_updateself		( xui_method_update& args );

	/*
	//event
	*/
	void						on_buttonclick		( xui_component* sender, xui_method_args&	args );
	void						on_speedscroll		( xui_component* sender, xui_method_args&	args );
	void						on_headperform		( xui_component* sender, xui_method_args&	args );
	void						on_viewupdateself	( xui_component* sender, xui_method_update& args );
	void						on_viewrenderself	( xui_component* sender, xui_method_args&	args );
	void						on_viewmousewheel	( xui_component* sender, xui_method_mouse&	args );
	void						on_ctrltick			( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	f32							get_speed			( void );
	void						set_speed			( f32 speed );
	void						set_localtransform	( void );
	f32							get_incscale		( void );
	f32							get_decscale		( void );

	/*
	//member
	*/
	xui_panel*					m_head;
	xui_panel*					m_tool;
	xui_proproot*				m_prop;
	cocos_renderview*			m_view;
	xui_button*					m_play;
	xui_drawer*					m_text;
	xui_slider*					m_speed;
	xui_button*					m_large;
	xui_button*					m_small;
	xui_button*					m_reset;
	f32							m_scale;
	cocos2d::Node*				m_node;
	cocos2d::Sprite*			m_sprite;
	cocos2d::Node*				m_fxroot;
	cocos2d::ui::TextBMFont*	m_uitext;
	cocos2d::ui::SpineNode*		m_spanim;
	xui_rect2d<s32>				m_rect;
	xui_action_ctrl*			m_ctrl;
};

#endif//__cocos_preview_h__