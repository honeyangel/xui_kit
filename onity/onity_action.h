#ifndef __onity_action_h__
#define __onity_action_h__

#include "onity_asset.h"

class NP2DSActorRef;
class onity_alignbox;
class onity_boundbox;
class onity_propactor;
class onity_propframekey;
class onity_action : public onity_asset
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_action( void );

	/*
	//destructor
	*/
	virtual ~onity_action( void );

	/*
	//static
	*/
	static  onity_boundbox_vec	get_selectedvec			( void );

	/*
	//method
	*/
	virtual onity_boundbox*		hit_propvisible			( const xui_vector<s32>& pt );
	virtual onity_boundbox_vec	get_selectedboundbox	( void );
	onity_propframekey*			add_framekey			( xui_timeline* timeline, s32 curframe, bool moveafter = false );

protected:
	/*
	//event
	*/
	virtual void				on_fillpanekeybddown	( xui_component* sender, xui_method_keybd&		args );
	virtual void				on_drawviewrenderself	( xui_component* sender, xui_method_args&		args );
	virtual void				on_drawviewrenderelse	( xui_component* sender, xui_method_args&		args );
	void						on_drawviewmousedragover( xui_component* sender, xui_method_dragdrop&	args );
	void						on_drawviewmousedragdrop( xui_component* sender, xui_method_dragdrop&	args );

	/*
	//override
	*/
	virtual void				on_mousepickimpl		( onity_boundbox* bbox, bool alt, bool ctrl, bool shift, u08 op );
	virtual void				on_mulselectimpl		( const xui_rect2d<s32>& rt, bool ctrl );

	/*
	//member
	*/
	onity_alignbox*				m_alignbox;
	xui_toggle*					m_collider;
	NP2DSActorRef*				m_actorref;
};

#endif//__onity_action_h__