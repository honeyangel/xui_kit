#ifndef __cocos_fileview_h__
#define __cocos_fileview_h__

#include "xui_control.h"

class cocos_tileview;
class cocos_fileview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_fileview( void );

	/*
	//destructor
	*/
	virtual ~cocos_fileview( void );

	/*
	//method
	*/
	void							set_locknode			( xui_treenode* locknode );
	xui_treeview*					get_lineview			( void );
	cocos_tileview*					get_tileview			( void );
	void							clear					( void );

	/*
	//method
	*/
	xui_method<xui_method_args >	xm_fileviewselection;
	xui_method<xui_method_mouse>	xm_fileviewdoubleclk;
	xui_method<xui_method_mouse>	xm_fileviewnodeclick;
	xui_method<xui_method_dragdrop>	xm_fileviewassetdrag;

protected:
	/*
	//callback
	*/
	virtual void					on_updateself			( xui_method_update& args );

	/*
	//event
	*/
	void							on_lineviewshow			( xui_component* sender, xui_method_args&	  args );
	void							on_lineviewrenderelse	( xui_component* sender, xui_method_args&     args );
	void							on_tileviewrenderelse	( xui_component* sender, xui_method_args&     args );
	void							on_fileviewselection	( xui_component* sender, xui_method_args&	  args );
	void							on_fileviewnodeclick	( xui_component* sender, xui_method_mouse&	  args );
	void							on_fileviewdoubleclk	( xui_component* sender, xui_method_mouse&	  args );
	void							on_fileviewassetdrag	( xui_component* sender, xui_method_dragdrop& args );

	/*
	//method
	*/
	void							draw_lockround			( const xui_rect2d<s32>& rt );

	/*
	//member
	*/
	xui_treeview*					m_lineview;
	cocos_tileview*					m_tileview;
	xui_treenode*					m_locknode;
	xui_action_ctrl*				m_lockctrl;
};

#endif//__cocos_fileview_h__