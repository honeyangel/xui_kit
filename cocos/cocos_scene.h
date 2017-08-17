#ifndef __cocos_scene_h__
#define __cocos_scene_h__

#include "xui_dockpage.h"

class cocos_proproot;
class cocos_gradpane;
class cocos_boundbox;
class cocos_alignbox;
class cocos_blankbox;
class cocos_snaptool;
class cocos_renderview;
class cocos_propcsd;
typedef std::vector<cocos_boundbox*> cocos_boundbox_vec;

class cocos_scene : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_scene( cocos_propcsd* prop );

	/*
	//destructor
	*/
	virtual ~cocos_scene( void );

	/*
	//static
	*/
	static cocos_boundbox_vec	get_selectedvec				( void );

	/*
	//method
	*/
	cocos_propcsd*				get_editprop				( void );
	void						set_editprop				( cocos_propcsd* prop );
	const xui_vector<s32>&		get_trans					( void ) const;
	f64							get_ratio					( void ) const;
	void						set_trans					( const xui_vector<s32>& trans );
	void						set_ratio					( f64 ratio );
	void						set_rootvisible				( void );
	void						set_nodevisible				( cocos_boundbox* bbox );
	void						set_toolupdate				( void );
	cocos_boundbox*				hit_propvisible				( const xui_vector<s32>& pt );
	cocos_boundbox*				get_operatorboundbox		( const xui_vector<s32>& pt, u08& result );
	cocos_boundbox_vec			get_selectedboundbox		( void );

protected:
	/*
	//method
	*/
	f64							get_incratio				( void );
	f64							get_decratio				( void );

	/*
	//event
	*/
	void						on_buttonclick				( xui_component* sender, xui_method_args&		args );
	void						on_fillpaneperform			( xui_component* sender, xui_method_args&		args );
	void						on_fillpanerenderelse		( xui_component* sender, xui_method_args&		args );
	void						on_fillpanekeybddown		( xui_component* sender, xui_method_keybd&		args );
	void						on_drawviewnoncatch			( xui_component* sender, xui_method_args&		args );
	void						on_drawviewupdateself		( xui_component* sender, xui_method_update&		args );
	void						on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	void						on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	void						on_drawviewmouseenter		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmousedown		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmousemove		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmouserise		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmousedragover	( xui_component* sender, xui_method_dragdrop&	args );
	void						on_drawviewmousedragdrop	( xui_component* sender, xui_method_dragdrop&	args );
	void						on_animctrltick				( xui_component* sender, xui_method_args&		args );

	/*
	//virtual
	*/
	void						on_keybdmoveimpl			( const xui_vector<s32>& delta );
	void						on_mousepickimpl			( cocos_boundbox* bbox, bool alt, bool ctrl, bool shift, u08 op );
	void						on_mousedragimpl			( const xui_vector<s32>& delta );
	void						on_mulselectimpl			( const xui_rect2d<s32>& rt, bool ctrl );

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
	cocos_renderview*			m_drawview;
	cocos_snaptool*				m_snaptool;
	cocos_alignbox*				m_alignbox;
	cocos_blankbox*				m_blankbox;
	cocos_gradpane*				m_horzgrad;
	cocos_gradpane*				m_vertgrad;
	xui_action_ctrl*			m_animctrl;
	xui_action_ctrl*			m_lockctrl;
	bool						m_multisel;
	bool						m_dragview;
	u08							m_operator;
	cocos_propcsd*				m_editprop;
	cocos2d::Node*				m_rootnode;
};

#endif//__cocos_scene_h__