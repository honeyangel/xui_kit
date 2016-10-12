#ifndef __onity_scene_h__
#define __onity_scene_h__

#include "xui_dockpage.h"

class onity_renderview;
class onity_gradpane;
class onity_scene : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_scene( void );

	/*
	//destructor
	*/
	virtual ~onity_scene( void );

	/*
	//method
	*/
	const xui_vector<s32>&	get_trans					( void ) const;
	void					set_trans					( const xui_vector<s32>& trans );
	f64						get_ratio					( void ) const;
	void					set_ratio					( f64 ratio );
	void					set_nodevisible				( xui_treenode* node );

protected:
	/*
	//override
	*/
	virtual void			on_updateself				( xui_method_update& args );

	/*
	//event
	*/
	void					on_buttonclick				( xui_component* sender, xui_method_args&		args );
	void					on_fillpaneperform			( xui_component* sender, xui_method_args&		args );
	void					on_fillpanerenderelse		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewnoncatch			( xui_component* sender, xui_method_args&		args );
	void					on_drawviewupdateself		( xui_component* sender, xui_method_update&		args );
	void					on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewsetrendersz		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewkeybddown		( xui_component* sender, xui_method_keybd&		args );
	void					on_drawviewmousedown		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousemove		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmouserise		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousedragover	( xui_component* sender, xui_method_dragdrop&	args );
	void					on_drawviewmousedragdrop	( xui_component* sender, xui_method_dragdrop&	args );
	void					on_animctrltick				( xui_component* sender, xui_method_args&		args );

	/*
	//method
	*/
	f64						get_incratio				( void );
	f64						get_decratio				( void );
	xui_proproot_vec		get_propcand				( void );

	/*
	//member
	*/
	xui_vector<s32>			m_trans;
	f64						m_ratio;
	xui_button*				m_reset;
	xui_button*				m_large;
	xui_button*				m_small;
	xui_button*				m_clear;
	xui_panel*				m_cubepane;
	xui_panel*				m_headpane;
	xui_panel*				m_fillpane;
	onity_renderview*		m_drawview;
	onity_gradpane*			m_horzgrad;
	onity_gradpane*			m_vertgrad;
	xui_action_ctrl*		m_animctrl;
	bool					m_dragview;
};

#endif//__onity_scene_h__