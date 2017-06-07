#ifndef __onity_asset_h__
#define __onity_asset_h__

#include "xui_dockpage.h"

class onity_proproot;
class onity_gradpane;
class onity_boundbox;
class onity_renderview;
typedef std::vector<onity_boundbox*> onity_boundbox_vec;

class onity_asset : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_asset( void );

	/*
	//destructor
	*/
	virtual ~onity_asset( void );

	/*
	//method
	*/
	const xui_vector<s32>&		get_trans					( void ) const;
	f64							get_ratio					( void ) const;
	virtual void				set_trans					( const xui_vector<s32>& trans );
	virtual void				set_ratio					( f64 ratio );
	virtual void				set_nodevisible				( onity_boundbox* bbox );
	virtual void				set_toolupdate				( void );
	virtual onity_boundbox*		hit_propvisible				( const xui_vector<s32>& pt );
	virtual onity_boundbox*		get_operatorboundbox		( const xui_vector<s32>& pt, u08& result );
	virtual onity_boundbox_vec	get_selectedboundbox		( void );

protected:
	/*
	//method
	*/
	f64							get_incratio				( void );
	f64							get_decratio				( void );

	/*
	//event
	*/
	virtual void				on_buttonclick				( xui_component* sender, xui_method_args&		args );
	virtual void				on_fillpaneperform			( xui_component* sender, xui_method_args&		args );
	virtual void				on_fillpanerenderelse		( xui_component* sender, xui_method_args&		args );
	virtual void				on_fillpanekeybddown		( xui_component* sender, xui_method_keybd&		args );
	virtual void				on_drawviewnoncatch			( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewupdateself		( xui_component* sender, xui_method_update&		args );
	virtual void				on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewmouseenter		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmousedown		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmousemove		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmouserise		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&		args );
	void						on_animctrltick				( xui_component* sender, xui_method_args&		args );

	/*
	//virtual
	*/
	virtual void				on_keybdmoveimpl			( const xui_vector<s32>& delta );
	virtual void				on_mousepickimpl			( onity_boundbox* bbox, bool alt, bool ctrl, bool shift, u08 op );
	virtual void				on_mousedragimpl			( const xui_vector<s32>& delta );
	virtual void				on_mulselectimpl			( const xui_rect2d<s32>& rt, bool ctrl );

	/*
	//draw
	*/
	void						draw_locknode				( void );				
	void						draw_multisel				( void );

	/*
	//member
	*/
	xui_vector<s32>				m_trans;
	f64							m_ratio;
	xui_button*					m_reset;
	xui_button*					m_large;
	xui_button*					m_small;
	xui_button*					m_clear;
	xui_panel*					m_cubepane;
	xui_panel*					m_headpane;
	xui_toolbar*				m_linetool;
	xui_panel*					m_fillpane;
	xui_panel*					m_drawpane;
	xui_toggle*					m_showbbox;
	onity_renderview*			m_drawview;
	onity_gradpane*				m_horzgrad;
	onity_gradpane*				m_vertgrad;
	xui_action_ctrl*			m_animctrl;
	xui_action_ctrl*			m_lockctrl;
	bool						m_multisel;
	bool						m_dragview;
	u08							m_operator;
};

#endif//__onity_asset_h__