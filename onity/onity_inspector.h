#ifndef __onity_inspector_h__
#define __onity_inspector_h__

#include "xui_dockpage.h"
#include "xui_propview.h"

class NPObjectRef;
class NPNode;
class onity_preview;
class onity_renderview;
class onity_inspector : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_inspector( void );

	/*
	//destructor
	*/
	virtual ~onity_inspector( void );

	/*
	//method
	*/
	onity_preview*		get_preview				( void );
	xui_propview*		get_propview			( void );
	void				set_proproot			( xui_proproot* proproot );
	void				set_proproot			( const xui_proproot_vec& proproot );

	/*
	//tips
	*/
	void				show_tips				( NPObjectRef* value, const xui_vector<s32>& pt );
	void				hide_tips				( void );

protected:
	/*
	//callback
	*/
	virtual void		on_perform				( xui_method_args&   args );
	virtual void		on_updateself			( xui_method_update& args );

	/*
	//event
	*/
	void				on_sizectrlmousemove	( xui_component* sender, xui_method_mouse& args );
	void				on_tipsviewrenderself	( xui_component* sender, xui_method_args&  args );
	void				on_inspectorkeybddown	( xui_component* sender, xui_method_keybd& args );

	/*
	//method
	*/
	void				set_localtransform		( void );

	/*
	//member
	*/
	xui_propview*		m_propview;
	onity_preview*		m_drawview;
	xui_control*		m_sizectrl;
	onity_renderview*	m_tipsview;
	NPNode*				m_tipsnode;
};

#endif//__onity_inspector_h__