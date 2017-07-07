#ifndef __onity_scene_h__
#define __onity_scene_h__

#include "onity_asset.h"

class onity_bounding;
class onity_alignbox;
class onity_blankbox;
class onity_pivotbox;
class onity_snaptool;

class onity_scene : public onity_asset
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
	//static
	*/
	static onity_boundbox_vec	get_selectedvec			( void );

	/*
	//method
	*/
	virtual void				set_trans					( const xui_vector<s32>& trans );
	virtual void				set_ratio					( f64 ratio );
	virtual void				set_nodevisible				( onity_boundbox* bbox );
	virtual void				set_toolupdate				( void );
	virtual onity_boundbox*		hit_propvisible				( const xui_vector<s32>& pt );
	virtual onity_boundbox_vec	get_selectedboundbox		( void );

protected:
	/*
	//event
	*/
	virtual void				on_buttonclick				( xui_component* sender, xui_method_args&		args );
	virtual void				on_fillpanekeybddown		( xui_component* sender, xui_method_keybd&		args );
	virtual void				on_drawviewupdateself		( xui_component* sender, xui_method_update&		args );
	virtual void				on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewmousedown		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmouserise		( xui_component* sender, xui_method_mouse&		args );
	virtual void				on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&		args );
	void						on_drawviewmousedragover	( xui_component* sender, xui_method_dragdrop&	args );
	void						on_drawviewmousedragdrop	( xui_component* sender, xui_method_dragdrop&	args );
	void						on_toggleclick				( xui_component* sender, xui_method_args&		args );

	/*
	//override
	*/
	virtual void				on_keybdmoveimpl			( const xui_vector<s32>& delta );
	virtual void				on_mousepickimpl			( onity_boundbox* pick, bool alt, bool ctrl, bool shift, u08 op );
	virtual void				on_mulselectimpl			( const xui_rect2d<s32>& rt, bool ctrl );

	/*
	//method
	*/
	xui_proproot_vec			get_propcand				( void );
	void						mir_propnode				( const onity_boundbox_vec& vec, const xui_vector<s32>& pivot, s32 style );
	void						rot_propnode				( const onity_boundbox_vec& vec, const xui_vector<s32>& pivot, s32 angle );

	/*
	//member
	*/
	onity_snaptool*				m_snaptool;
	onity_alignbox*				m_alignbox;
	onity_blankbox*				m_blankbox;
	onity_pivotbox*				m_pivotbox;
	xui_toggle*					m_showrect;
	xui_button*					m_horzflip;
	xui_button*					m_vertflip;
	xui_button*					m_cwrotate;
	xui_button*					m_ccrotate;
};

#endif//__onity_scene_h__