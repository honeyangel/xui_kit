#ifndef __cocos_inspector_h__
#define __cocos_inspector_h__

#include "xui_dockpage.h"
#include "xui_propview.h"

namespace cocos2d
{
	class Ref;
	class Node;
}

class cocos_preview;
class cocos_renderview;
class cocos_inspector : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_inspector( void );

	/*
	//destructor
	*/
	virtual ~cocos_inspector( void );

	/*
	//method
	*/
	cocos_preview*		get_preview				( void );
	xui_propview*		get_propview			( void );
	void				set_proproot			( xui_proproot* proproot );
	void				set_proproot			( const xui_proproot_vec& proproot );

	/*
	//tips
	*/
	void				show_tips				( cocos2d::Ref* value, const xui_vector<s32>& pt );
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
	xui_propview*				m_propview;
	cocos_preview*				m_drawview;
	xui_control*				m_sizectrl;
	cocos_renderview*			m_tipsview;
	cocos2d::Node*				m_tipsnode;
};

#endif//__cocos_inspector_h__